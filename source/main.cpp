#include "../includes/ivfs.h"
#include "../includes/ui.h"

int main()
{
    TestTask::IVFS* IVFS_handler = new TestTask::IVFS;

    Interface::mainCycle(&IVFS_handler);

    delete IVFS_handler;
    std::cout << "IVFS Handler deleted\n";
    
    return 0;
}