#include "../includes/output.h"
#include "../includes/ivfs.h"
#include "../includes/ui.h"

int main()
{
    TestTask::IVFS* IVFS_handler = new TestTask::IVFS;

    TestTask::File *openedFiles[MAX_FILES];
    int fileCounter = 0; // Считает открытые / закрытые файлы
    bool toExit = false;

    openedFiles[fileCounter] = IVFS_handler->Open("init.txt");
    fileCounter++;

    try
    {
        while (!toExit)
        {
            Interface::PrintMenu(fileCounter);
            switch(Interface::ReadCommand())
            {
                case 1:
                    if (fileCounter < MAX_FILES)
                    {
                        openedFiles[fileCounter] = IVFS_handler->Open(Interface::ReadString());
                        if (openedFiles[fileCounter] != nullptr)
                            fileCounter++;
                    }
                    else
                        throw ("Max amount of opened files reached");
                    break;

                case 7:
                    toExit = true;
                    break;

                default:
                    continue;
            }
        }
    }
    catch(const char* error)
    {
        std::cout << error;
    }

    delete IVFS_handler;
    return 0;
}