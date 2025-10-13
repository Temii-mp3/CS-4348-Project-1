#include <iostream>
int main(int argc, const char **argv)
{
    int p1[2], p2[2];
    pid_t logger;
    pid_t encrypt;

    std::string userInput;
    do
    {
        std::cout << "---------------------------" << std::endl;
        std::cout << "Menu" << std::endl;
        std::cout << "---------------------------" << std::endl;
        std::cout << "password\nencrypt\ndecrypt\nhistory\nquit\n";
        std::cout << "Enter an option: ";

        std::cin >> userInput;

    } while (userInput != "quit");

    return 0;
}
