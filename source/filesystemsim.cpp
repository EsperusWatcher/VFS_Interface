#include "../include/filesystemsim.h"

// Вызывается при открытии/создании файла, находит в массиве для открытых файлов
// свободный слот и передает его для получения файла
_IVFS::File **FileSystemSim::FindEmptySlot(_IVFS::IVFS *IVFS_Handler)
{
    int emptySlotIndex;
    for (int i = 0; i < MAX_FILES; i++)
        if(IVFS_Handler->openedFiles[i] == nullptr)
        {
            emptySlotIndex = i;
            break;
        }

    return &IVFS_Handler->openedFiles[emptySlotIndex];
}

// Используется для нахождения файла по его имени (для чтения/записи)
// Поиск осуществляется в массиве открытых файлов
_IVFS::File **FileSystemSim::GetFileByName(_IVFS::IVFS *IVFS_Handler, const char *name)
{
    bool fileChosen = false;
    int chosenIndex;

    for (int i = 0; i < MAX_FILES; i++)
    {
        if (IVFS_Handler->openedFiles[i] != nullptr)
            if(strcmp(IVFS_Handler->openedFiles[i]->filePath, name) == 0)
            {
                fileChosen = true;
                chosenIndex = i;
                break;
            }
    }

    if (fileChosen)
        return &IVFS_Handler->openedFiles[chosenIndex];
    else
    {
        std::cout << "file not found\n";
        return nullptr;
    }
}

// Проверяет, открыт ли файл
bool FileSystemSim::CheckIfFileIsOpened(_IVFS::IVFS *IVFS_Handler, const char *name)
{
    for (int i = 0; i < MAX_FILES; i++)
    {
        if (IVFS_Handler->openedFiles[i] != nullptr)
            if(strcmp(IVFS_Handler->openedFiles[i]->filePath, name) == 0)
                return true;
    }

    return false;
}

FileSystemSim::Control::Control()
{
    this->requestPool.open("settings/commands.txt", std::ios::in);
}
