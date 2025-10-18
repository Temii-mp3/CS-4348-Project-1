#include <iostream>

int main()
{
    std::string passkey;
    std::cout << "Hello im a donkey" << std::endl;
    while (std::getline(std::cin, passkey))
    {

        for (char &c : passkey)
            c = std::toupper(c);

        std::cout << passkey << std::endl;
    }
}