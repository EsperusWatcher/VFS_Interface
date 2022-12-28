#include "../include/filesystemsim.h"
#include "../include/ivfs.h"
#include "../include/test.h"

// Тестирование функционала ivfs.h
// Без многопоточности

int main()
{
    _IVFS::IVFS *IVFS_Handler;
    IVFS_Handler = new _IVFS::IVFS();

    char buff[MAX_BUFFER];
    size_t buff_size = MAX_BUFFER;

    bool toExit = false;

    while (!toExit)
    {
        PrintMenu(IVFS_Handler->fileCounter);
        switch(ReadCommand())
        {
            // Открыть файл
            case OPEN:
                if (IVFS_Handler->fileCounter < MAX_FILES)
                {
                    FilePathPrompt();
                    *FileSystemSim::FindEmptySlot(IVFS_Handler) = IVFS_Handler->Open(ReadString().c_str());
                }
                else
                    std::cout << "Max amount of opened files reached\n";
                break;
            
            // Создать файл
            case CREATE:
                if (IVFS_Handler->fileCounter < MAX_FILES)
                {
                    FilePathPrompt();
                    *FileSystemSim::FindEmptySlot(IVFS_Handler) = IVFS_Handler->Create(ReadString().c_str());
                }
                else
                    std::cout << "Max amount of opened files reached\n";
                break;

            // Прочитать файл
            case READ:
                if (IVFS_Handler->fileCounter > 0)
                {
                    PrintFileList(IVFS_Handler);
                    std::cout << "Enter file name to read: ";
                    size_t readBytes = IVFS_Handler->Read(*FileSystemSim::GetFileByName(IVFS_Handler, ReadString().c_str()), buff, buff_size);
                    std::cout << "Read bytes: " << readBytes << "\n";
                }
                else
                    std::cout << "No opened files\n";
                
                break;

            // Записать в файл
            case WRITE:
                if (IVFS_Handler->fileCounter > 0)
                {
                    std::cout << "Enter the sentence to write: ";
                    std::string toWrite = ReadString().c_str();
                    
                    strcpy_s(buff, toWrite.c_str());
                    buff_size = toWrite.length();

                    PrintFileList(IVFS_Handler);
                    std::cout << "Enter destination name: ";
                    size_t wroteBytes = IVFS_Handler->Write(*FileSystemSim::GetFileByName(IVFS_Handler, ReadString().c_str()), buff, buff_size);
                    std::cout << "Wrote bytes: " << wroteBytes << "\n";
                }
                else
                    std::cout << "No opened files\n";

                break;

            // Закрыть файл
            case CLOSE:
                if (IVFS_Handler->fileCounter > 0)
                {
                    // Указатель на закрываемый файл передается в переменную
                    PrintFileList(IVFS_Handler);
                    std::cout << "Enter file name to close: ";
                    IVFS_Handler->Close(*FileSystemSim::GetFileByName(IVFS_Handler, ReadString().c_str()));
                }
                else
                    std::cout << "No opened files\n";

                break;

            case 6:
                PrintFileList(IVFS_Handler);
                break;

            // Выйти из программы
            case 8:
                toExit = true;
                break;

            default:
                continue;
        }
    }

    // Перед выходом все открытые файлы закрываются
    for (int i = 0; i < MAX_FILES; i++)
    {
        if (IVFS_Handler->openedFiles[i] != nullptr)
            IVFS_Handler->Close(IVFS_Handler->openedFiles[i]);
    }

    delete IVFS_Handler;
}

void PrintMenu(int fileCounter)
{
    std::cout << "-----------------------------\n";
    std::cout << "Currently opened files: " << fileCounter << " out of " << MAX_FILES << "\n";
    std::cout << "1 - Open   | File\n";
    std::cout << "2 - Create | File\n";
    std::cout << "3 - Read   | File\n";
    std::cout << "4 - Write  | File\n";
    std::cout << "5 - Close  | File\n";
    std::cout << "6 - Print  | Files\n";
    std::cout << "8 - Exit\n";
    std::cout << "-----------------------------\n";
    std::cout << "Choose: ";
}

void FilePathPrompt()
{
    std::cout << "---------------------------- \n";
    std::cout << "Format: 'a/b/c/.../file.txt' \n";
    std::cout << "Enter File Path: ";
}

void PrintFileList(_IVFS::IVFS *IVFS_Handler)
{
    for (int i = 0; i < MAX_FILES; i++)
        if (IVFS_Handler->openedFiles[i] != nullptr)
        {
            std::cout << "Mode: " << (IVFS_Handler->openedFiles[i]->fileMode == READONLY ? "Readonly" : "Writeonly");
            std::cout << " | Path: " << IVFS_Handler->openedFiles[i]->filePath << "\n";
        }
}

int ReadCommand()
{
    int ret = getchar() - '0'; 
    getchar();
    return ret;
}

std::string ReadString()
{
    std::string buff;
    getline(std::cin, buff);
    return buff;
}