#include "../includes/ivfs.h"
#include "../includes/ui.h"

// Структура File Создается с указанным методом открытия файла
// filePath - путь к файлу
// fileMode - режим открытия файла
TestTask::File::File (const char *filePath, std::ios_base::openmode fileMode)
{
    this->fileHook.open(filePath, fileMode);
    this->fileMode = fileMode == std::ios::in ? READONLY : WRITEONLY;
    this->filePath = new char(strlen(filePath));

    strcpy(this->filePath, filePath);

    std::cout << this->filePath << " | Opened\n";
}

// Открывает файл в readonly режиме, Если такого файла нет или он открыт во writeonly режиме - возвращает nullptr 
// name - путь к файлу
TestTask::File* TestTask::IVFS::Open ( const char *name )
{
    std::fstream testHook(name);
    // Файл найден
    if (testHook) 
        return new TestTask::File(name, std::ios::in);
    // Такого файла нет
    else 
    {
        std::cout << "File doesnt exist\n";
        return nullptr;
    }

    // Файл открыт во writeonly режиме
    // TODO: Добавить проверку на writeonly
    // ???
}

// Открыть или создать файл в writeonly режиме. Если нужно, то создать все нужные поддиректории, упомянутые в пути. 
// Вернуть nullptr, если этот файл уже открыт в readonly режиме.
// name - путь к файлу
TestTask::File* TestTask::IVFS::Create ( const char *name )
{
    // Определяет, есть ли в пути поддиректории
    bool complexPath = false;

    // Считает символы в строке, определяющие имя файла
    int counter = 0;

    for (int i = 0; i < strlen(name); i++)
    {
        if (name[i] == '/')
        {
            complexPath = true;
            counter = 1;
        }
        else
            counter++;
    }

    // Если поддиректории есть в пути
    if (complexPath)
    {
        // Из конца строки с поддиректориями убирается имя самого файла
        // Файл создается позже в конструкторе File, тут только директории
        // Если директории уже существуют то ничего не происходит
        char *pathToCreate = new char(strlen(name) - counter);
        strncpy(pathToCreate, name, strlen(name) - counter);
        pathToCreate[strlen(name) - counter] = '\0';

        std::filesystem::create_directories(pathToCreate);
        
        delete pathToCreate;
    }

    // TODO: Добавить проверку на readonly
    //return nullptr;

    return new TestTask::File(name, std::ios::out);

}

// Читает данные из файла, возвращает сколько байт удалось прочитать
// f - структура File
// buff - ???
// len - ???
size_t TestTask::IVFS::Read ( TestTask::File *f, char *buff, size_t len )
{
    if (f->fileMode != READONLY)
    {
        std::cout << "File cant be read\n";
        return 0;
    }

    std::cout << "Reading: " << f->filePath << " |\n";

    size_t readData = 0;
    
    f->fileHook.get(buff, len, EOF);

    // Возвращение потока в начало файла для будущего доступа
    f->fileHook.clear();
    f->fileHook.seekg(0);

    return strlen(buff);
}

// Записывает данные в файл, возвращает сколько байт удалось записать
// buff - ???
// len - ???
size_t TestTask::IVFS::Write (TestTask::File *f, char *buff, size_t len)
{
    if (f->fileMode != WRITEONLY)
    {
        std::cout << "File cant be written\n";
        return 0;
    }
    
    std::cout << "Input: "; 
    strcpy(buff, Interface::ReadString().c_str());

    f->fileHook << buff;

    return strlen(buff);
}

// Закрывает файл, очищает память
// f - структура File
void TestTask::IVFS::Close ( TestTask::File *f )
{
    f->fileHook.close();

    std::cout << f->filePath << " | Closed\n";

    delete f->filePath;
    delete f;
}