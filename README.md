# VFS_Interface
Virtual File System interface

Папка data - хранит файлы, с которыми взаимодействует программа

В папке settings - пул команд для многопоточной работы

Многопоточность реализована через два потока

Можно протестировать функционал интерфейса вручную (без потоков) - через make debug и ivfs_test.exe

Формат записи команд в файл settings:
-
(номер команды)!(Путь к файлу)!(Если команда на запись в файл, то тут данные для записи)

Символ ! используется как разделитель, но его можно использовать в данных для записи в файл
