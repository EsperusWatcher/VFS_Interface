#include "../includes/ui.h"

void Interface::PrintMenu(int fileCounter)
{
    std::cout << "-----------------------------\n";
    std::cout << "Currently opened files: " << fileCounter << " out of " << MAX_FILES << "\n";
    std::cout << "1 - Open   | File\n";
    std::cout << "2 - Create | File\n";
    std::cout << "3 - Read   | File\n";
    std::cout << "4 - Write  | File\n";
    std::cout << "5 - Close  | File\n";
    std::cout << "6 - Switch | Thread\n";
    std::cout << "7 - Exit\n";
    std::cout << "-----------------------------\n";
    std::cout << "Choose: ";
}

void Interface::FilePathPrompt()
{
    std::cout << "---------------------------- \n";
    std::cout << "Format: 'a/b/c/.../file.txt' \n";
    std::cout << "Enter File Path: ";
}

int Interface::ReadCommand()
{
    int ret = getchar() - '0'; 
    getchar();
    return ret;
}

std::string Interface::ReadString()
{
    std::string buff;
    getline(std::cin, buff);
    return buff;
}

// Файл закрылся, необходимо переставить оставшиеся по порядку
// Это все только для интерфейса
void Interface::RearrangeFileDisplay(TestTask::File **openedFiles, int fileCounter)
{
    for (int i = 0; i < fileCounter; i++)
    {
        // Найден пустой слот
        if (openedFiles[i] == nullptr)
        {
            // Пустой слот уезжает в конец массива
            TestTask::File *temp = openedFiles[i];
            openedFiles[i] = openedFiles[i + 1];
            openedFiles[i + 1] = temp;
            temp = nullptr;
        }
    }
}

// Основной цикл взаимодействия с пользователем
// Обрабатывает интерфейс работы с файлами
// Выход через команду
// IVFS_handler - доступ к структуре IVFS и ее методам
void Interface::mainCycle(TestTask::IVFS **IVFS_handler)
{
    TestTask::File *openedFiles[MAX_FILES];

    // TODO: Костыль, убрать потом
    // Обращает все неактивные 'слоты' для файлов в null
    // Сделано чтобы работала Interface::RearrangeFileDisplay(...)
    for (int i = 0; i < MAX_FILES; i++)
        openedFiles[i] = nullptr;

    // Считает открытые / закрытые файлы 
    int fileCounter = 0; 
    bool toExit = false;

    // TODO: Только для тестирования
    openedFiles[fileCounter] = (*IVFS_handler)->Open("test/test.txt");
    fileCounter++;
    openedFiles[fileCounter] = (*IVFS_handler)->Open("test/one/test.txt");
    fileCounter++;
    openedFiles[fileCounter] = (*IVFS_handler)->Create("test/two/test.txt");
    fileCounter++;
    // TODO: ----------------------

    while (!toExit)
    {
        Interface::PrintMenu(fileCounter);
        switch(Interface::ReadCommand())
        {
            // Открыть файл
            case 1:
                if (fileCounter < MAX_FILES)
                {

                    bool checkPassed = true;

                    Interface::FilePathPrompt();
                    std::string fileToOpen = Interface::ReadString();

                    // Проверяет, открыт ли уже этот файл во writeonly режиме
                    for (int i = 0; i < fileCounter; i++)
                    {
                        std::string toCompare(openedFiles[i]->filePath);
                        if (fileToOpen.compare(toCompare) == 0 && openedFiles[i]->fileMode == WRITEONLY)
                        {
                            std::cout << "File already opened in writeonly mode\n";
                            checkPassed = false;
                            break;
                        }
                    }

                    if (checkPassed)
                    {
                        openedFiles[fileCounter] = (*IVFS_handler)->Open(fileToOpen.c_str());

                        if (openedFiles[fileCounter] != nullptr)
                            fileCounter++;
                    }
                }
                else
                    std::cout << "Max amount of opened files reached\n";
                break;
            
            // Создать файл
            case 2:
                if (fileCounter < MAX_FILES)
                {
                    bool checkPassed = true;

                    Interface::FilePathPrompt();
                    std::string fileToCreate = Interface::ReadString();
                    
                    // Проверяет, открыт ли уже этот файл в readonly режиме
                    for (int i = 0; i < fileCounter; i++)
                    {
                        std::string toCompare(openedFiles[i]->filePath);
                        if (fileToCreate.compare(toCompare) == 0 && openedFiles[i]->fileMode == READONLY)
                        {
                            std::cout << "File already opened in readonly mode\n";
                            checkPassed = false;
                            break;
                        }
                    }

                    if (checkPassed)
                    {
                        openedFiles[fileCounter] = (*IVFS_handler)->Create(fileToCreate.c_str());

                        if (openedFiles[fileCounter] != nullptr)
                            fileCounter++;
                    }
                }
                else
                    std::cout << "Max amount of opened files reached\n";
                break;

            // Прочитать файл
            case 3:
                if (fileCounter > 0)
                {
                    char buff[MAX_BUFFER];
                    size_t readBytes = MAX_BUFFER;
                    readBytes = (*IVFS_handler)->Read(Interface::ChooseFile(openedFiles, fileCounter), buff, readBytes);
                    std::cout << "Bytes read: " << readBytes << "\n";
                }
                else
                    std::cout << "No opened files\n";
                
                break;

            // Записать в файл
            case 4:
                if (fileCounter > 0)
                {
                    char buff[MAX_BUFFER];
                    size_t writeBytes;
                    writeBytes = (*IVFS_handler)->Write(Interface::ChooseFile(openedFiles, fileCounter), buff, writeBytes);
                    std::cout << "Wrote bytes: " << writeBytes << "\n";
                }
                else
                    std::cout << "No opened files\n";

                break;

            // Закрыть файл
            case 5:
                if (fileCounter > 0)
                {
                    // Указатель на закрываемый файл передается в переменную
                    TestTask::File *fileToClose = Interface::ChooseFile(openedFiles, fileCounter);

                    // Отвратительно (ли?)
                    // Указатель на файл в массиве обращается в null
                    for (int i = 0; i < fileCounter; i++)
                        if (openedFiles[i] == fileToClose)
                        {
                            openedFiles[i] = nullptr;
                            break;
                        }

                    // Файл закрывается, уменьшается число открытых файлов
                    (*IVFS_handler)->Close(fileToClose);
                    fileCounter--;

                    // Ссылка (уже null) на удаленный файл перемещается в конец массива
                    Interface::RearrangeFileDisplay(openedFiles, fileCounter);
                }
                else
                    std::cout << "No opened files\n";

                break;

            // Выйти из программы
            case 7:
                toExit = true;
                break;

            default:
                continue;
        }
    }

    // Перед выходом все открытые файлы закрываются
    for (int i = fileCounter - 1; i >= 0; i--)
        (*IVFS_handler)->Close(openedFiles[i]);
}

// Выбор файля для чтения/записи из числа открытых
// openedFiles - Массив открытых файлов
// fileCounter - Количество открытых файлов
TestTask::File *Interface::ChooseFile(TestTask::File **openedFiles, int fileCounter)
{
    for (int i = 0; i < fileCounter; i++)
    {
        std::cout << i + 1 << " | mode: " << ((*openedFiles[i]).fileMode == WRITEONLY ? "Writeonly" : "Readonly ");
        std::cout << " | " << (*openedFiles[i]).filePath << "\n";
    }

    bool validInput = false;
    int choice;

    while (!validInput)
    {
        std::cout << "Choose: ";
        choice = Interface::ReadCommand() - 1;
        
        if (choice < 0 || choice > fileCounter - 1)
            std::cout << "Not a valid choice\n";
        else
            validInput = true;
    }

    return &(*openedFiles[choice]);    
}