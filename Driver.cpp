#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <chrono>
#include <iomanip>

std::string getCurrentTime();
int main(int argc, const char **argv)
{
    int p1[2], p2[2];
    pid_t logger;
    pid_t encrypt;

    if (pipe(p1) == -1)
    {
        std::cerr << "Unable to create logger pipe" << std::endl;
        exit(-1);
    }

    logger = fork();

    if (logger == -1)
    {
        std::cerr << "Error creating c1" << std::endl;
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
        std::string userInput;
        do
        {
            std::cout << "---------------------------" << std::endl;
            std::cout << "Menu" << std::endl;
            std::cout << "---------------------------" << std::endl;
            std::cout << "password\nencrypt\ndecrypt\nhistory\nquit\n";
            std::cout << "Enter an option: ";

            std::cin >> userInput;
            userInput = userInput + "\n";

            write(p1[1], userInput.c_str(), userInput.size());

        } while (userInput != "quit");
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
