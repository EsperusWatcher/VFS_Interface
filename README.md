# VFS_Interface
Virtual File System interface

Папка data - хранит файлы, с которыми взаимодействует программа

В папке settings - пул команд для многопоточной работы

Многопоточность реализована через два потока

Можно протестировать функционал интерфейса вручную (без потоков) - через make debug и ivfs_test.exe