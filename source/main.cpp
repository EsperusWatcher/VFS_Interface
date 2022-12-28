#include "../include/ivfs.h"
#include "../include/thread.h"

int main()
{
    _IVFS::IVFS *IVFS_Handler = new _IVFS::IVFS();

    Multithread::MainLoop(IVFS_Handler);

    std::cout << "done\n";
    return 0;
}