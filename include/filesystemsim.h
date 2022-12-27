#ifndef FILESIM_H
#define FILESIM_H

#include <iostream>
#include "ivfs.h"

// Для красоты
#define OPEN 1
#define CREATE 2
#define READ 3
#define WRITE 4
#define CLOSE 5

namespace FileSystemSim
{
    struct Control
    {
        std::fstream requestPool;

        std::shared_mutex openingLock;
        std::mutex operationLock;

        Control();
    };

    _IVFS::File **FindEmptySlot(_IVFS::IVFS *IVFS_Handler);
    _IVFS::File **GetFileByName(_IVFS::IVFS *IVFS_Handler, const char *name);
    bool CheckIfFileIsOpened(_IVFS::IVFS *IVFS_Handler, const char *name);
}

#endif