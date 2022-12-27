#include "../include/thread.h"
#include "../include/filesystem.h"

// Основной цикл взаимодействия с интерфейсом
// IVFS_Handler - доступ к структуре IVFS и ее методам
void Multithread::MainLoop(_IVFS::IVFS *IVFS_Handler)
{
    // Используется для работы с потоками
    // Ограничивает одновременный доступ к данным
    Multithread::Threads threads[NUM_THREADS];
    
    // Открытие файлов с командами для потоков
    for (int i = 0; i < NUM_THREADS; i++)
        threads[i].threadCommandsHook.open("thread_settings/thread_" + std::to_string(i) + ".txt");

    // Запуск потоков
    for (int i = 0; i < NUM_THREADS; i++)
        threads[i].thread = std::thread(Multithread::ThreadCycle, IVFS_Handler);

    // Синхронизация
    for (int i = 0; i < NUM_THREADS; i++)
        threads[i].thread.join();

    std::cout << "threads synced\n";
}

// Это только для примера
#define OPEN 1
#define CREATE 2
#define READ 3
#define WRITE 4
#define CLOSE 5

// Цикл работы потока
// Считывает команды из соответствующего файла и выполняет их
void Multithread::ThreadCycle(_IVFS::IVFS *IVFS_Handler)
{
    Multithread::threadReport("", "started");

    // Пока ничего не делают, только чтобы расписать принцип работы
    int threadCommand;
    const char *filename;
    std::string writeBuff = "example";

    switch (threadCommand)
    {
        case OPEN:
            {
                Multithread::threadReport(filename, "opening");

                *FileSystemSim::FindEmptySlot(IVFS_Handler) = IVFS_Handler->Open(filename);
            }
            break;

        case CREATE:
            {
                Multithread::threadReport(filename, "creating");

                *FileSystemSim::FindEmptySlot(IVFS_Handler) = IVFS_Handler->Create(filename);
            }
            break;

        case READ:
            if (FileSystemSim::CheckIfFileIsOpened(IVFS_Handler, filename))
            {
                char buff[MAX_BUFFER];

                // Получает ссылку на файл для чтения (хранится во временной переменной)
                _IVFS::File **readingFile = FileSystemSim::GetFileByName(IVFS_Handler, filename);

                // Блокирует файл от закрытия
                // Все еще разрешает другим потокам чтение из файла
                std::shared_lock<std::shared_mutex> reading((*readingFile)->fileLock);

                size_t readBytes;
                readBytes = IVFS_Handler->Read((*readingFile), buff, MAX_BUFFER);

                reading.unlock();

                Multithread::threadReport(filename, "reading", true, readBytes);

                readingFile = nullptr;
            }
            break;

        case WRITE:
            if (FileSystemSim::CheckIfFileIsOpened(IVFS_Handler, filename))
            {
                _IVFS::File **writingFile = FileSystemSim::GetFileByName(IVFS_Handler, filename);

                char *buff = new char[writeBuff.length()];
                size_t len = writeBuff.length();
                strcpy(buff, writeBuff.c_str());

                // Запрещает другим потокам доступ к файлу на время записи
                std::unique_lock<std::shared_mutex> writing((*writingFile)->fileLock);

                size_t writeBytes;
                writeBytes = IVFS_Handler->Write(*writingFile, buff, len);

                writing.unlock();

                Multithread::threadReport(filename, "writing", true, writeBytes);

                writingFile = nullptr;
            }
            break;

        case CLOSE:
            if (FileSystemSim::CheckIfFileIsOpened(IVFS_Handler, filename))
            {
                _IVFS::File **closingFile = FileSystemSim::GetFileByName(IVFS_Handler, filename);

                // Если кто-то читает или пишет файл, то ждет
                // Если нет, запрещает читать/писать до удаления
                std::unique_lock<std::shared_mutex> closing((*closingFile)->fileLock);

                Multithread::threadReport(filename, "closing");

                IVFS_Handler->Close(*closingFile);

                closing.unlock();
                
                closingFile = nullptr;
            }
            break;

        default:
            break;
    }
}

// Выведение различной информации на экран
void Multithread::threadReport(const char *filename, const char *action, bool ioOperation = false, int result = 0)
{
    Multithread::printLock.lock();
    std::cout << "Thread |" << std::this_thread::get_id() << "| " << action << " " << filename << "\n";
    if (ioOperation)
        std::cout << "Result: " << result << " bytes\n";
    Multithread::printLock.unlock();
}
