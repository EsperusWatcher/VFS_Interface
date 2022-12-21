#ifndef IVFS_H
#define IVFS_H

#include <fstream>
#include <filesystem>
#include <string.h>

#define MAX_FILES 10 // Одновременно открытых
#define WRITEONLY 0
#define READONLY 1

namespace TestTask
{
    // Вы имеете право как угодно задать содержимое этой структуры 
    struct File
    {
        std::fstream fileHook; // Доступ к файлу
        int fileMode; // Режим доступа к файлу 

        char* filePath;

        File(){};
        File(const char *filePath, std::ios_base::openmode openMode);
    };

    struct IVFS
    {
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
    };

    File *ChooseFile(File **openedFiles, int fileCounter);
}


#endif