#include "../include/ivfs.h"

// Используется для инициализации массива открытых файлов
_IVFS::IVFS::IVFS()
{
    this->openedFiles = new _IVFS::File*[MAX_FILES]();

    for (int i = 0; i < MAX_FILES; i++)
        this->openedFiles[i] = nullptr;
}

_IVFS::IVFS::~IVFS()
{
    for (int i = 0; i < MAX_FILES; i++)
        if (this->openedFiles[i] != nullptr)
            this->Close(this->openedFiles[i]);

    delete this->openedFiles;
}

// Конструктор файла для хранения данных
_IVFS::File::File(const char *name, std::ios_base::openmode openMode)
{
    this->fileMode = (openMode == std::ios_base::in ? READONLY : WRITEONLY);
    this->filePath = new char[strlen(name)]();
    this->fileHook = new std::fstream;

    strcpy(this->filePath, name);

    this->fileHook->open(this->filePath, openMode);
}

_IVFS::File::~File()
{
    delete this->filePath;
    delete this->fileHook;
}

// Открыть файл в readonly режиме. 
// Если нет такого файла или же он открыт во writeonly режиме - вернуть nullptr
_IVFS::File *_IVFS::IVFS::Open( const char *name )
{
    for (int i = 0; i < MAX_FILES; i++)
    {
        if (this->openedFiles[i] == nullptr)
            continue;

        if (strcmp(this->openedFiles[i]->filePath, name) == 0)
            if (this->openedFiles[i]->fileMode == WRITEONLY)
                return nullptr;
    }

    std::fstream checkHook(name);

    // Файл существует
    if (checkHook)
    {
        // Открыт новый файл
        this->fileCounter++;

        return new _IVFS::File(name, std::ios_base::in);
    }
    else
        return nullptr;
} 

// Открыть или создать файл в writeonly режиме. Если нужно, то создать все нужные поддиректории, упомянутые в пути. 
// Вернуть nullptr, если этот файл уже открыт в readonly режиме.
_IVFS::File *_IVFS::IVFS::Create( const char *name )
{
    // Проверяет, не открыт ли уже такой файл в readonly режиме
    for (int i = 0; i < MAX_FILES; i++)
    {
        if (this->openedFiles[i] == nullptr)
            continue;

        if (strcmp(this->openedFiles[i]->filePath, name) == 0)
            if (this->openedFiles[i]->fileMode == READONLY)
                return nullptr;
    }

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

    this->fileCounter++;

    return new _IVFS::File(name, std::ios_base::out);
}

// Прочитать данные из файла. Возвращаемое значение - сколько реально байт удалось прочитать
// На самом деле ничего не читает, для этого нет функционала, просто считает их
size_t _IVFS::IVFS::Read( _IVFS::File *f, char *buff, size_t len )
{
    if (f->fileMode == WRITEONLY)
        return 0;

    std::size_t totalBytes = 0;

    // Данные читаются (считаются) кусками под размер буфера
    do
    {
        f->fileHook->read(buff, len);
        totalBytes += f->fileHook->gcount();
    }
    while (!f->fileHook->eof());

    // Возвращение потока в начало файля если потребуется прочитать еще раз (не закрывая файл)
    f->fileHook->clear();
    f->fileHook->seekg(0);

    return totalBytes;
}

// Записать данные в файл. Возвращаемое значение - сколько реально байт удалось записать
// Буфер с данными и его размер определяются перед вызовом функции
size_t _IVFS::IVFS::Write( File *f, char *buff, size_t len )
{
    if (f->fileMode == READONLY)
        return 0;

    f->fileHook->write(buff, len);

    return len;
}

// Закрытие файла
void _IVFS::IVFS::Close( File *f )
{
    f->fileHook->close();
    
    delete f;

    // Помечает слот для открытия файлов как свободный
    for (int i = 0; i < MAX_FILES; i++)
        if (this->openedFiles[i] == f)
            this->openedFiles[i] = nullptr;

    this->fileCounter--;
}