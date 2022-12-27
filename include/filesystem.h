#ifndef FILESIM_H
#define FILESIM_H

#include <iostream>
#include "ivfs.h"

namespace FileSystemSim
{
    _IVFS::File **FindEmptySlot(_IVFS::IVFS *IVFS_Handler);
    _IVFS::File **GetFileByName(_IVFS::IVFS *IVFS_Handler, const char *name);
    bool CheckIfFileIsOpened(_IVFS::IVFS *IVFS_Handler, const char *name);
}

#endif