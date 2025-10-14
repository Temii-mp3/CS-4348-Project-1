#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <chrono>
#include <iomanip>
#include <vector>

std::string getCurrentTime();
int main(int argc, const char **argv)
{
    std::vector<std::string> history;
    std::string password;
    int p1[2], p2[2];
    pid_t logger;
    pid_t encrypt;

    if (pipe(p1) == -1)
    {
        std::cerr << "Unable to create logger pipe" << std::endl;
        exit(-1);
    }

    if (pipe(p2) == -1)
    {
        std::cerr << "Unable to create encryptor pipe" << std::endl;
        exit(-1);
    }

    logger = fork();

    if (logger == -1)
    {
        std::cerr << "Error creating Child Process" << std::endl;
        exit(-1);
    }

    if (logger == 0)
    {
        std::string curr_time = getCurrentTime();
        close(p1[1]);
        dup2(p1[0], 0);
        int fd = open(argv[1], O_WRONLY | O_CREAT | O_APPEND, 0644);
        dup2(fd, 1);
        close(fd);
        char *args[] = {"./logger", (char *)argv[1], (char *)curr_time.c_str(), NULL};
        execvp(args[0], args);
    }
    else
    {
        encrypt = fork();

        if (encrypt == 0)
        {
            dup2(p2[0], 0);
            close(p2[1]);
            char *args[] = {"./encryptor", NULL};
            execvp(args[0], args);
        }
        else
        {
            std::string userInput;
            do
            {
                std::cout << "---------------------------" << std::endl;
                std::cout << "Menu" << std::endl;
                std::cout << "---------------------------" << std::endl;
                std::cout << "password\nencrypt\ndecrypt\nhistory\nquit\n";
                std::cout << "Enter an option: ";

                std::cin >> userInput;
                std::cout << history.size() << std::endl;
                // userInput = userInput + "\n";

                if (userInput == "password" && history.empty())
                {
                    std::cout << "Enter Password: ";
                    std::cin >> password;
                    history.push_back(password);
                    write(p2[1], password.c_str(), password.size());
                }
                else if (userInput == "password" && !history.empty())
                {
                    std::cout << "Would you like to use history Y/N: ";
                    char input;
                    std::cin >> input;
                    std::tolower(input);
                    switch (input)
                    {
                    case 'n':
                        std::cout << "Enter Password: ";
                        std::cin >> password;
                        history.push_back(password);
                        write(p2[1], password.c_str(), password.size());
                    case 'y':
                        for (int i = 1; i <= history.size(); i++)
                        {
                            std::cout << i << ". " << history[i - 1] << std::endl;
                        }
                        std::cout << "Pick which number to set as password";
                        int pass;
                        std::cin >> pass;
                        while (pass > history.size())
                        {
                            std::cout << "Invalid Option, Try Again: ";
                            std::cin >> pass;
                        }
                        password = pass;
                        write(p2[1], password.c_str(), password.size());
                    }
                }

                write(p1[1], userInput.c_str(), userInput.size());

            } while (userInput != "quit");
        }
    }

    return 0;
}

std::string getCurrentTime()
{
    using std::chrono::system_clock;

    auto timeNow = system_clock::now();

    std::time_t t = system_clock::to_time_t(timeNow);
    std::tm local_tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%Y-%m-%d %H:%M");

    return oss.str();
}
