#ifndef FILESIM_H
#define FILESIM_H

#include <iostream>
#include "ivfs.h"

namespace FileSystemSim
{
    _IVFS::File **FindEmptySlot(_IVFS::IVFS *IVFS_Handler);
    _IVFS::File **ChooseFileByName(_IVFS::IVFS *IVFS_Handler, const char *name);
    void MainLoop(_IVFS::IVFS *IVFS_Handler);
}

#endif