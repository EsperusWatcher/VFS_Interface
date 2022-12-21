#include "../includes/output.h"
#include "../includes/ivfs.h"
#include "../includes/ui.h"

int main()
{
    TestTask::IVFS* IVFS_handler = new TestTask::IVFS;

    TestTask::File *openedFiles[MAX_FILES];

    // Считает открытые / закрытые файлы 
    int fileCounter = 0; 
    bool toExit = false;

    openedFiles[fileCounter] = IVFS_handler->Open("init.txt");
    fileCounter++;
    openedFiles[fileCounter] = IVFS_handler->Open("one/two/three/lmao.txt");
    fileCounter++;
    openedFiles[fileCounter] = IVFS_handler->Create("one/two/three/lmao2.txt");
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
                        openedFiles[fileCounter] = IVFS_handler->Open(Interface::ReadString().c_str());
                        if (openedFiles[fileCounter] != nullptr)
                            fileCounter++;
                    }
                    else
                        throw ("Max amount of opened files reached");
                    break;
                
                case 2:
                    if (fileCounter < MAX_FILES)
                    {
                        openedFiles[fileCounter] = IVFS_handler->Create(Interface::ReadString().c_str());
                        if (openedFiles[fileCounter] != nullptr)
                            fileCounter++;
                        std::cout << "done\n";
                    }
                    else
                        throw("Max amount of opened files reached");
                    break;

                case 3:
                    if (fileCounter > 0)
                    {
                        char *buff = new char(255);
                        size_t readBytes;
                        IVFS_handler->Read(TestTask::ChooseFile(openedFiles, fileCounter), buff, readBytes);
                    }
                    else
                        std::cout << "No opened files\n";
                    
                    break;

                case 4:
                    if (fileCounter > 0)
                    {
                        char *buff = new char(255);
                        size_t readBytes;
                        IVFS_handler->Write(TestTask::ChooseFile(openedFiles, fileCounter), buff, readBytes);
                    }
                    else
                        std::cout << "No opened files\n";

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

    for (int i = fileCounter - 1; i >= 0; i--)
        IVFS_handler->Close(openedFiles[i]);

    delete IVFS_handler;
    std::cout << "IVFS Handler deleted\n";
    return 0;
}