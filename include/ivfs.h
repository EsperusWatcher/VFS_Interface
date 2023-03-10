#ifndef IVFS_H
#define IVFS_H

#include <fstream>
#include <filesystem>
#include <string.h>
#include <mutex>
#include <shared_mutex>

// Ограничение на макс. количество открытых файлов
#define MAX_FILES 20

// Для чтения/записи данных 
#define MAX_BUFFER 255 

#define WRITEONLY 0
#define READONLY 1

namespace _IVFS
{
    // Описывает структуру открытого файла
    struct File
    {
        // Поток доступа к файлу
        std::fstream *fileHook;
        
        // Режим доступа к файлу (readonly/writeonly)
        int fileMode; 

        // Путь к файлу
        char *filePath;

        File(const char *name, std::ios_base::openmode openMode);
        ~File();
    };

    // Интерфейс для работы с файлами
    struct IVFS
    {
        // Используется для отслеживания открытых файлов и доступа к ним
        File **openedFiles;
        int fileCounter = 0;

        // Открыть файл в readonly режиме. Если нет такого файла или же он открыт во writeonly режиме - вернуть nullptr
        File *Open( const char *name ); 
        
        // Открыть или создать файл в writeonly режиме. Если нужно, то создать все нужные поддиректории, упомянутые в пути. 
        // Вернуть nullptr, если этот файл уже открыт в readonly режиме.
        File *Create( const char *name ); 
        
        // Прочитать данные из файла. Возвращаемое значение - сколько реально байт удалось прочитать
        size_t Read( File *f, char *buff, size_t len ); 
        
        // Записать данные в файл. Возвращаемое значение - сколько реально байт удалось записать
        size_t Write( File *f, char *buff, size_t len ); 
        
        // Закрыть файл
        void Close( File *f ); 

        IVFS();
        ~IVFS();
    };
}

#endif