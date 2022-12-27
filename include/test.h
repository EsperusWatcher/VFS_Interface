#ifndef TEST_H
#define TEST_H

#include "ivfs.h"
#include <iostream>

void PrintMenu(int fileCounter);
void FilePathPrompt();
int ReadCommand();
std::string ReadString();
void PrintFileList(_IVFS::IVFS *IVFS_Handler);

#endif