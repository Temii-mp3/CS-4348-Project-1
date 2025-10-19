#include <iostream>
#include <sstream>
#include <string>

std::string vigenereDecrypt(std::string, std::string);
std::string vigenereEncrypt(std::string, std::string);
char cesearCypherEncrypt(char, char);
char cesearCypherDecrypt(char, char);
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
            std::cout << "Password has been set" << std::endl;
        }
        else if (command == "ENCRYPT")
        {
            std::cout << argument << std::endl;
            if (currentPassword.empty())
            {
                std::cout << "ERROR Password not set" << std::endl;
            }
            else
            {
                std::string encrypted = vigenereEncrypt(argument, currentPassword);
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
                std::string decrypted = vigenereDecrypt(argument, currentPassword);
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

std::string vigenereDecrypt(std::string text, std::string key)
{
    std::string result;
    for (int i = 0; i < text.length(); i++)
    {
        result.append(1, cesearCypherDecrypt(text[i], key[i % key.length()]));
    }

    return result;
}

std::string vigenereEncrypt(std::string text, std::string key)
{
    std::string result;
    for (int i = 0; i < text.length(); i++)
    {
        result.append(1, cesearCypherEncrypt(text[i], key[i % key.length()]));
    }

    return result;
}

char cesearCypherEncrypt(char letter, char letter2)
{
    int textIndex = letter - 'a';
    int keyIndex = letter2 - 'a';
    int resultIndex = (textIndex + keyIndex) % 26;
    return resultIndex + 'A'; // returns ascii value
}

char cesearCypherDecrypt(char letter, char letter2)
{
    int textIndex = letter - 'a';
    int keyIndex = letter2 - 'a';
    int resultIndex = ((textIndex - keyIndex) + 26) % 26;
    return resultIndex + 'A'; // returns ascii value
}