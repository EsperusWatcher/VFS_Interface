#include "../includes/ivfs.h"
#include "../includes/output.h"

#include <filesystem>

// Структура File Создается с указанным методом открытия файла
TestTask::File::File (const char *filePath, std::ios_base::openmode fileMode)
{
    this->fileHook.open(filePath, fileMode);
    this->fileMode = fileMode;

    this->filePath = new char(sizeof(filePath));
    strncpy(this->filePath, filePath, sizeof(filePath) + 1);

    std::cout << this->filePath << " | Opened\n";
}

// открывает файл в readonly режиме, Если такого файла нет или он открыт во writeonly режиме - возвращает nullptr
TestTask::File* TestTask::IVFS::Open ( const char *name )
{
    std::fstream fileHook(name);
    if (fileHook) // Файл найден
        return new TestTask::File(name, std::fstream::in);
    else // Такого файла нет
    {
        std::cout << "File doesnt exist or opened already\n";
        return nullptr;
    }
}

// Открыть или создать файл в writeonly режиме. Если нужно, то создать все нужные поддиректории, упомянутые в пути. 
// Вернуть nullptr, если этот файл уже открыт в readonly режиме.
TestTask::File* TestTask::IVFS::Create ( const char *name )
{
    return new TestTask::File(name, std::fstream::out);
}

size_t TestTask::IVFS::Read ( TestTask::File *f, char *buff, size_t len )
{
    return 0;
}

size_t TestTask::IVFS::Write (TestTask::File *f, char *buff, size_t len)
{
    return 0;
}

void TestTask::IVFS::Close ( TestTask::File *f )
{
    f->fileHook.close();

    std::cout << f->filePath << " | Closed\n";

    delete f->filePath;
    delete f;

    std::cout << "Memory Cleared\n";
}