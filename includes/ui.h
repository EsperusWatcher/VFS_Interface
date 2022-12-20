#ifndef UI_H
#define UI_H

#include <iostream>

namespace Interface
{
    void PrintMenu(int fileCounter);
    int ReadCommand();
    const char *ReadString();
}

#endif