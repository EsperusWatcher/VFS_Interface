#include "../includes/ui.h"
#include "../includes/ivfs.h"

void Interface::PrintMenu(int fileCounter)
{
    std::cout << "Currently used files: " << fileCounter << " / " << MAX_FILES << "\n";
    std::cout << "1 - Open   | File\n";
    std::cout << "2 - Create | File\n";
    std::cout << "3 - Read   | File\n";
    std::cout << "4 - Write  | File\n";
    std::cout << "5 - Close  | File\n";
    std::cout << "6 - Switch | Thread\n";
    std::cout << "7 - Exit\n";
    std::cout << "Choose: ";
}

int Interface::ReadCommand()
{
    int ret = getchar() - '0'; 
    getchar();
    return ret;
}

const char *Interface::ReadString()
{
    std::string buff;
    
    std::cout << "File Path: ";
    getline(std::cin, buff);

    // TODO: For some reason the last character of the string is lost
    for (int i = 0; i < sizeof(buff.c_str()); i++)
        std::cout << buff.c_str()[i] << "|\n";
    std::cout << "Opening: " << buff.c_str() << "\n";

    return buff.c_str();
}

