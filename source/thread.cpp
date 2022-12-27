#include "../include/thread.h"
#include "../include/filesystem.h"

// Основной цикл взаимодействия с интерфейсом
// IVFS_Handler - доступ к структуре IVFS и ее методам
void Multithread::MainLoop(_IVFS::IVFS *IVFS_Handler)
{
    // Используется для работы с потоками
    // Ограничивает одновременный доступ к данным
    Multithread::Control threadControl;

    Multithread::Threads threads[NUM_THREADS];
    
    // Открытие файлов с командами для потоков
    for (int i = 0; i < NUM_THREADS; i++)
        threads[i].threadCommandsHook.open("thread_settings/thread_" + std::to_string(i) + ".txt");

    // Запуск потоков
    for (int i = 0; i < NUM_THREADS; i++)
        threads[i].thread = std::thread([&]{Multithread::ThreadCycle(IVFS_Handler, &threadControl);});

    // Синхронизация
    for (int i = 0; i < NUM_THREADS; i++)
        threads[i].thread.join();
}

// Цикл работы потока
// Считывает команды из соответствующего файла и выполняет их
void Multithread::ThreadCycle(_IVFS::IVFS *IVFS_handler, Multithread::Control *threadControl)
{
    // Поток не может закрыть файл если он его не открывал
    // Поток не может закрыть файл если в него идет запись/чтение данных другим потоком
    // Поток не
}
