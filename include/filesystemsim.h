#ifndef FILESIM_H
#define FILESIM_H

#include <iostream>
#include "ivfs.h"

// Для красоты (Соответствующие команды в файле с командами задаются этими числами)
#define OPEN 1
#define CREATE 2
#define READ 3
#define WRITE 4
#define CLOSE 5

namespace FileSystemSim
{
    // Открывает файл с командами для потоков
    struct Control
    {
        std::fstream requestPool;

        std::mutex printLock;
        std::mutex operationLock;
        std::shared_mutex openingLock;

        Control();
    };

    _IVFS::File **FindEmptySlot(_IVFS::IVFS *IVFS_Handler);
    _IVFS::File **GetFileByName(_IVFS::IVFS *IVFS_Handler, const char *name);
    bool CheckIfFileIsOpened(_IVFS::IVFS *IVFS_Handler, const char *name);
}

#endif