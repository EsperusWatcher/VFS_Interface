#ifndef THREAD_H
#define THREAD_H

#include <thread>
#include <mutex>
#include "ivfs.h"
#include "filesystemsim.h"

// Изменение количества используемых потоков (но лучше не надо)
#define NUM_THREADS 2

namespace Multithread
{
    struct Request
    {
        int command;
        const char *filename;
        std::string writeData;

        std::mutex requestLock;
    };

    void ThreadCycle(_IVFS::IVFS *IVFS_Handler, FileSystemSim::Control *fileRequestControl);
    void TranslateRequest(Request *request, std::string parser, std::string rawCommand);
    void MainLoop(_IVFS::IVFS *IVFS_Handler);
    void threadReport(const char *filename, const char *action, bool ioOperation = false, int result = 0);
}

#endif