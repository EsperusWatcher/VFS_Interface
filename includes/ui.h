#ifndef UI_H
#define UI_H

#include <iostream>
#include "ivfs.h"

namespace Interface
{
    void PrintMenu(int fileCounter);
    void FilePathPrompt();
    int ReadCommand();
    std::string ReadString();
    void RearrangeFileDisplay(TestTask::File **openedFiles, int fileCounter);
    void mainCycle(TestTask::IVFS **IVFS_handler);
    TestTask::File *ChooseFile(TestTask::File **openedFiles, int fileCounter);
}


#endif