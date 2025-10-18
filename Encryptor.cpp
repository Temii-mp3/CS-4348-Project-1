#include <iostream>
#include <sstream>
#include <string>

int main()
{
    std::string currentPassword = "";
    std::string line;

    while (std::getline(std::cin, line))
    {
        std::istringstream iss(line);
        std::string command;
        iss >> command;

        std::string argument;
        std::getline(iss, argument);

        if (!argument.empty() && argument[0] == ' ')
            argument = argument.substr(1);

        if (command == "PASS")
        {
            currentPassword = argument;
            std::cout << "RESULT" << std::endl;
        }
        else if (command == "ENCRYPT")
        {
            if (currentPassword.empty())
            {
                std::cout << "ERROR Password not set" << std::endl;
            }
            else
            {
                std::string encrypted = vigenereEncrypt(argument);
                std::cout << "RESULT " << encrypted << std::endl;
            }
        }
        else if (command == "DECRYPT")
        {
            if (currentPassword.empty())
            {
                std::cout << "ERROR Password not set" << std::endl;
            }
            else
            {
                std::string decrypted = vigenereDecrypt(argument);
                std::cout << "RESULT " << decrypted << std::endl;
            }
        }
        else if (command == "QUIT")
        {
            break;
        }
    }

    return 0;
}

std::string vigenereDecrypt(std::string arg, std::string pass)
{
}

std::string vigenereEncrypt(std::string arg, std::string pass)
{
}