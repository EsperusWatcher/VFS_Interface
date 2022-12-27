#include "../include/filesystem.h"
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
            case 1:
                if (IVFS_Handler->fileCounter < MAX_FILES)
                {
                    FilePathPrompt();
                    *FileSystemSim::FindEmptySlot(IVFS_Handler) = IVFS_Handler->Open(ReadString().c_str());
                }
                else
                    std::cout << "Max amount of opened files reached\n";
                break;
            
            // Создать файл
            case 2:
                if (IVFS_Handler->fileCounter < MAX_FILES)
                {
                    FilePathPrompt();
                    *FileSystemSim::FindEmptySlot(IVFS_Handler) = IVFS_Handler->Create(ReadString().c_str());
                }
                else
                    std::cout << "Max amount of opened files reached\n";
                break;

            // Прочитать файл
            case 3:
                if (IVFS_Handler->fileCounter > 0)
                {
                    PrintFileList(IVFS_Handler);
                    std::cout << "Enter file name to read: ";
                    size_t readBytes = IVFS_Handler->Read(*FileSystemSim::ChooseFileByName(IVFS_Handler, ReadString().c_str()), buff, buff_size);
                    std::cout << "Read bytes: " << readBytes << "\n";
                }
                else
                    std::cout << "No opened files\n";
                
                break;

            // Записать в файл
            case 4:
                if (IVFS_Handler->fileCounter > 0)
                {
                    std::cout << "Enter the sentence to write: ";
                    std::string toWrite = ReadString().c_str();
                    
                    strcpy_s(buff, toWrite.c_str());
                    buff_size = toWrite.length();

                    PrintFileList(IVFS_Handler);
                    std::cout << "Enter destination name: ";
                    size_t wroteBytes = IVFS_Handler->Write(*FileSystemSim::ChooseFileByName(IVFS_Handler, ReadString().c_str()), buff, buff_size);
                    std::cout << "Wrote bytes: " << wroteBytes << "\n";
                }
                else
                    std::cout << "No opened files\n";

                break;

            // Закрыть файл
            case 5:
                if (IVFS_Handler->fileCounter > 0)
                {
                    // Указатель на закрываемый файл передается в переменную
                    PrintFileList(IVFS_Handler);
                    std::cout << "Enter file name to close: ";
                    IVFS_Handler->Close(*FileSystemSim::ChooseFileByName(IVFS_Handler, ReadString().c_str()));
                }
                else
                    std::cout << "No opened files\n";

                break;

            case 6:
                PrintFileList(IVFS_Handler);
                break;

            // Выйти из программы
            case 7:
                UnitTests(IVFS_Handler);
                break;

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

    std::cout << "done\n";
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
    std::cout << "7 - Unit   | Tests\n";
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

// ну не совсем
void UnitTests(_IVFS::IVFS *IVFS_Handler)
{
    char buff2[MAX_BUFFER];
    size_t len2 = MAX_BUFFER;
    *FileSystemSim::FindEmptySlot(IVFS_Handler) = IVFS_Handler->Open("data/more/more_2.txt");
    std::cout << IVFS_Handler->Read(*FileSystemSim::ChooseFileByName(IVFS_Handler, "data/more/more_2.txt"), buff2, len2) << " bytes\n";
    std::cout << IVFS_Handler->Read(*FileSystemSim::ChooseFileByName(IVFS_Handler, "data/more/more_2.txt"), buff2, len2) << " bytes\n";

    // Тест на открытие файла
    std::cout << "---------------------1-----------------------\n";
    std::cout << "Opening: data/data_1.txt | readonly\n";
    std::cout << "Expecting: opened\n";
    std::cout << "Result: ";
    *FileSystemSim::FindEmptySlot(IVFS_Handler) = IVFS_Handler->Open("data/data_1.txt");

    // Тест на попытку открыть файл во writeonly пока он открыт в readonly
    std::cout << "---------------------2-----------------------\n";
    std::cout << "Creating: data/data_1.txt | writeonly\n";
    std::cout << "Expecting: already opened in readonly\n";
    std::cout << "Result: ";
    *FileSystemSim::FindEmptySlot(IVFS_Handler) = IVFS_Handler->Create("data/data_1.txt");

    // Тест на попытку записать в readonly файл что-нибудь
    std::cout << "---------------------3-----------------------\n";
    std::cout << "Writing 'hello' to data/data_1.txt\n";
    std::cout << "Expecting: 0 bytes (file in readonly)\n";
    std::cout << "Result: " << (IVFS_Handler)->Write(*FileSystemSim::ChooseFileByName(IVFS_Handler, "data/data_1.txt"), "hello", 5) << " bytes\n";
    
    // Тест на создание директорий, открытие файла во writeonly
    std::cout << "---------------------4-----------------------\n";
    std::cout << "Creating: data/more/more_1.txt | writeonly\n";
    std::cout << "Expecting: path created (if not already), opened\n";
    std::cout << "Result: ";
    *FileSystemSim::FindEmptySlot(IVFS_Handler) = IVFS_Handler->Create("data/more/more_1.txt");

    // Тест на закрытие файла
    std::cout << "---------------------5-----------------------\n";
    std::cout << "Closing: data/data_1.txt\n";
    std::cout << "Expecting: closed\n";
    std::cout << "Result: ";
    IVFS_Handler->Close(*FileSystemSim::ChooseFileByName(IVFS_Handler, "data/data_1.txt"));

    // Тест на запись чего-нибудь в файл (а также на функцию сортировки открытых файлов)
    std::cout << "---------------------6-----------------------\n";
    std::cout << "Writing 'hello' to data/more/more_1.txt\n";
    std::cout << "Expecting: 5 bytes\n";
    std::cout << "Result: " << IVFS_Handler->Write(*FileSystemSim::ChooseFileByName(IVFS_Handler, "data/more/more_1.txt"), "hello", 5) << " bytes\n";

    // Еще один тест на закрытие
    std::cout << "---------------------7-----------------------\n";
    std::cout << "Closing data/more/more_1.txt\n";
    std::cout << "Expecting: closed\n";
    std::cout << "Result: ";
    IVFS_Handler->Close(*FileSystemSim::ChooseFileByName(IVFS_Handler, "data/more/more_1.txt"));

    std::cout << "---------------------8-----------------------\n";
    std::cout << "Opening data/more/more_1.txt\n";
    std::cout << "Expecting: opened\n";
    std::cout << "Result: ";
    *FileSystemSim::FindEmptySlot(IVFS_Handler) = IVFS_Handler->Open("data/more/more_1.txt");

    char buff [MAX_BUFFER];
    // Тест на чтение чего-нибудь из файла
    std::cout << "---------------------9-----------------------\n";
    std::cout << "Reading from data/more/more_1.txt\n";
    std::cout << "Expecting: 5 bytes\n";
    std::cout << "Result: " << IVFS_Handler->Read(*FileSystemSim::ChooseFileByName(IVFS_Handler, "data/more/more_1.txt"), buff, MAX_BUFFER) << " bytes\n";

    // Тест на попытку открыть readonly файл во writeonly 
    std::cout << "---------------------10----------------------\n";
    std::cout << "Creating data/more/more_1.txt\n";
    std::cout << "Expecting: already opened in readonly\n";
    std::cout << "Result: ";
    *FileSystemSim::FindEmptySlot(IVFS_Handler) = IVFS_Handler->Create("data/more/more_1.txt");
    std::cout << "---------------------------------------------\n";

    for (int i = 0; i < MAX_FILES; i++)
    {
        if (IVFS_Handler->openedFiles[i] != nullptr)
            IVFS_Handler->Close(IVFS_Handler->openedFiles[i]);
    }
}