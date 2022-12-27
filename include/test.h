#ifndef TEST_H
#define TEST_H

#include "ivfs.h"
#include <iostream>

void PrintMenu(int fileCounter);
void FilePathPrompt();
int ReadCommand();
std::string ReadString();
int ChooseFileIndex(_IVFS::File **openedFiles, int fileCounter);
void PrintFileList(_IVFS::IVFS *IVFS_Handler);
void UnitTests(_IVFS::IVFS *IVFS_Handler);

#endif