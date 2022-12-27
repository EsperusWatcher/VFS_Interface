#include "../include/thread.h"
#include "../include/filesystemsim.h"

// Основной цикл взаимодействия с интерфейсом
// IVFS_Handler - доступ к структуре IVFS и ее методам
void Multithread::MainLoop(_IVFS::IVFS *IVFS_Handler)
{
    // Массив с потоками, для демонстрации функционала используется 2 потока
    std::thread threads[NUM_THREADS];
    
    FileSystemSim::Control *fileRequestControl = new FileSystemSim::Control();

    // Запуск потоков
    for (int i = 0; i < NUM_THREADS; i++)
        threads[i] = std::thread([&]{Multithread::ThreadCycle(IVFS_Handler, fileRequestControl);});

    // Синхронизация
    for (int i = 0; i < NUM_THREADS; i++)
        threads[i].join();

    std::cout << "threads synced\n";

    fileRequestControl->requestPool.close();  
    delete fileRequestControl;
}

// Цикл работы потока
// Считывает построчно команды из файла settings/commands.txt и выполняет их
void Multithread::ThreadCycle(_IVFS::IVFS *IVFS_Handler, FileSystemSim::Control *fileRequestControl)
{
    Multithread::threadReport("", "started");

    // Строка из файла с командами
    std::string rawCommandData;
    // Используется для переработки строки выше в структуру ниже
    std::string parser;

    Multithread::Request *request = new Multithread::Request();

    do
    {
        getline(fileRequestControl->requestPool, rawCommandData);
        Multithread::TranslateRequest(request, parser, rawCommandData);

        // Для избежания выполнения команды до открытия файла (при одновременном получении команд потоками)
        // Если поток не нахожит открытый файл, то зацикливается пока тот не откроется
        // Не оптимально, но работает....
        bool requestCompleted = false;

        while (!requestCompleted)
        {
            switch (request->command)
            {
                case OPEN:
                {
                    // Файлы могут открываться одновременно, но на это время приостанавливаются команды над ними
                    std::shared_lock<std::shared_mutex> reading(fileRequestControl->openingLock);
                    Multithread::threadReport(request->filename, "opening");

                    *FileSystemSim::FindEmptySlot(IVFS_Handler) = IVFS_Handler->Open(request->filename);
                    requestCompleted = true;
                }
                break;

                case CREATE:
                {
                    std::shared_lock<std::shared_mutex> reading(fileRequestControl->openingLock);
                    Multithread::threadReport(request->filename, "creating");

                    *FileSystemSim::FindEmptySlot(IVFS_Handler) = IVFS_Handler->Create(request->filename);
                    requestCompleted = true;
                }
                break;

                case READ:
                {
                    // Если файл открывается, то ждет
                    std::shared_lock<std::shared_mutex> reading(fileRequestControl->openingLock);
                    if (FileSystemSim::CheckIfFileIsOpened(IVFS_Handler, request->filename))
                    {

                        char buff[MAX_BUFFER];

                        // Получает ссылку на файл для чтения (хранится во временной переменной)
                        _IVFS::File **readingFile = FileSystemSim::GetFileByName(IVFS_Handler, request->filename);

                        size_t readBytes;
                        readBytes = IVFS_Handler->Read((*readingFile), buff, MAX_BUFFER);

                        Multithread::threadReport(request->filename, "reading", true, readBytes);

                        readingFile = nullptr;
                        requestCompleted = true;
                    }
                    // Если другой поток еще не успел открыть файл
                    else
                        continue;
                }
                break;

                case WRITE:
                {
                    // Если файл открывается, то ждет
                    std::unique_lock<std::shared_mutex> writing(fileRequestControl->openingLock);
                    if (FileSystemSim::CheckIfFileIsOpened(IVFS_Handler, request->filename))
                    {

                        _IVFS::File **writingFile = FileSystemSim::GetFileByName(IVFS_Handler, request->filename);

                        char *buff = new char[request->writeData.length()];
                        size_t len = request->writeData.length();
                        strcpy(buff, request->writeData.c_str());

                        size_t writeBytes;
                        writeBytes = IVFS_Handler->Write(*writingFile, buff, len);

                        delete buff;

                        Multithread::threadReport(request->filename, "writing", true, writeBytes);

                        writingFile = nullptr;
                        requestCompleted = true;
                    }
                    else
                        continue;
                }
                break;

                case CLOSE:
                {
                    // Если файл открывается, то ждет
                    std::unique_lock<std::shared_mutex> closing(fileRequestControl->openingLock);
                    if (FileSystemSim::CheckIfFileIsOpened(IVFS_Handler, request->filename))
                    {
                        fileRequestControl->openingLock.unlock();
                        _IVFS::File **closingFile = FileSystemSim::GetFileByName(IVFS_Handler, request->filename);

                        Multithread::threadReport(request->filename, "closing");

                        IVFS_Handler->Close(*closingFile);

                        closingFile = nullptr;
                        requestCompleted = true;
                    }
                    else
                        continue;
                }
                break;

                default:
                    break;
            }
        }
    } while (!fileRequestControl->requestPool.eof());

    delete request;
}

// Выведение различной информации о действиях потоков на экран
void Multithread::threadReport(const char *filename, const char *action, bool ioOperation, int result)
{
    // Не позволяет потокам выводить данные одновременно
    printLock.lock();
    std::cout << "Thread |" << std::this_thread::get_id() << "| " << action << " " << filename;
    // Для вывода в консоль при чтении/записи в файл
    if (ioOperation)
        std::cout << " | Result: " << result << " bytes\n";
    else
        std::cout << "\n";
    printLock.unlock();
}

// Переводит считанную строку из файла к виду структуры request
// В качестве разделителя используется символ !
void Multithread::TranslateRequest(Multithread::Request *request, std::string parser, std::string rawCommand)
{
    parser = rawCommand.substr(0, rawCommand.find("!"));
    request->command = (int)parser[0] - '0';
    
    rawCommand.erase(0, rawCommand.find("!") + 1);
    parser = rawCommand.substr(0, rawCommand.find("!"));

    request->filename = parser.c_str();

    rawCommand.erase(0, rawCommand.find("!") + 1);

    if (request->command == WRITE)
        request->writeData = rawCommand;
}
