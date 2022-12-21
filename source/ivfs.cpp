#include "../includes/ivfs.h"
#include "../includes/output.h"
#include "../includes/ui.h"

// Структура File Создается с указанным методом открытия файла
TestTask::File::File (const char *filePath, std::ios_base::openmode fileMode)
{
    this->fileHook.open(filePath, fileMode);
    this->fileMode = fileMode == std::fstream::in ? READONLY : WRITEONLY;
    this->filePath = new char(strlen(filePath));

    strcpy(this->filePath, filePath);

    std::cout << this->filePath << " | Opened\n";
}

// Открывает файл в readonly режиме, Если такого файла нет или он открыт во writeonly режиме - возвращает nullptr 
TestTask::File* TestTask::IVFS::Open ( const char *name )
{
    std::fstream fileHook(name);

    // Файл найден
    if (fileHook) 
        return new TestTask::File(name, std::fstream::in);
    // Такого файла нет
    else 
    {
        std::cout << "File doesnt exist\n";
        return nullptr;
    }

    // Файл открыт во writeonly режиме
    // TODO: Добавить проверку на writeonly
}

// Открыть или создать файл в writeonly режиме. Если нужно, то создать все нужные поддиректории, упомянутые в пути. 
// Вернуть nullptr, если этот файл уже открыт в readonly режиме.
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

    // Если поддиректории есть в пути, но не существуют, то создает их
    if (complexPath)
    {
        // Из конца строки с поддиректориями убирается имя самого файла
        char *pathToCreate = new char(strlen(name) - counter);
        strncpy(pathToCreate, name, strlen(name) - counter);

        std::filesystem::create_directories(pathToCreate);
        
        delete pathToCreate;
    }

    return new TestTask::File(name, std::fstream::out);

    // TODO: Добавить проверку на readonly
    //return nullptr;
}

// Читает данные из файла, возвращает сколько байт удалось прочитать
size_t TestTask::IVFS::Read ( TestTask::File *f, char *buff, size_t len )
{
    if (f->fileMode != READONLY)
    {
        std::cout << "File cant be read\n";
        return 0;
    }

    std::cout << "Reading: " << f->filePath << " |\n";
    return 0;
}

// Записывает данные в файл, возвращает сколько байт удалось записать
size_t TestTask::IVFS::Write (TestTask::File *f, char *buff, size_t len)
{
    if (f->fileMode != WRITEONLY)
    {
        std::cout << "File cant be written\n";
        return 0;
    }

    std::cout << "Writing: " << f->filePath << " |\n";
    return 0;
}

// Закрывает файл, очищает память
void TestTask::IVFS::Close ( TestTask::File *f )
{
    f->fileHook.close();

    std::cout << f->filePath << " | Closed\n";

    delete f->filePath;
    delete f;
}

TestTask::File *TestTask::ChooseFile(TestTask::File **openedFiles, int fileCounter)
{
    for (int i = 0; i < fileCounter; i++)
    {
        std::cout << i + 1 << " | mode: " << ((*openedFiles[i]).fileMode == WRITEONLY ? "Writeonly" : "Readonly ");
        std::cout << " |" << (*openedFiles[i]).filePath << "\n";
    }

    bool validInput = false;
    int choice;

    while (!validInput)
    {
        std::cout << "Choose: ";
        choice = Interface::ReadCommand() - 1;
        
        if (choice < 0 || choice > fileCounter - 1)
            std::cout << "Not a valid choice\n";
        else
            validInput = true;
    }

    return &(*openedFiles[choice]);    
}
