#ifndef THREAD_H
#define THREAD_H

#include <thread>
#include <mutex>
#include "ivfs.h"

// Увеличение количества используемых потоков
// Потребует создания новых файлов с командами
// в files/thread_settings/
#define NUM_THREADS 2

namespace Multithread
{
    std::mutex printLock;

    struct Threads
    {
        std::thread thread;

        // Для чтения команд из соотв. потоку файла
        std::fstream threadCommandsHook;
    };
    
    void ThreadCycle(_IVFS::IVFS *IVFS_handler);
    void MainLoop(_IVFS::IVFS *IVFS_Handler);
    void threadReport(const char *filename, const char *action, bool ioOperation = false, int result = 0);
}

#endif