#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cstring>
#include <algorithm>

std::string getCurrentTime();
int main(int argc, const char **argv)
{
    std::vector<std::string> history;
    std::string encryptCommand;
    std::string passwordNewline;
    std::string password;
    int p1[2], p2[2], p3[2]; // p3 for reading encryptor output
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

    if (pipe(p3) == -1)
    {
        std::cerr << "Unable to create encryptor output pipe" << std::endl;
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
        close(p2[0]);
        close(p2[1]);
        close(p3[0]);
        close(p3[1]);
        dup2(p1[0], 0);
        int fd = open(argv[1], O_WRONLY | O_CREAT | O_APPEND, 0644);
        dup2(fd, 1);
        close(fd);
        char *args[] = {const_cast<char *>("./logger"), const_cast<char *>(argv[1]), const_cast<char *>(curr_time.c_str()), NULL};
        execvp(args[0], args);
    }
    else
    {
        encrypt = fork();

        if (encrypt == 0)
        {
            close(p1[0]);
            close(p1[1]);
            dup2(p2[0], 0); // stdin from p2
            dup2(p3[1], 1); // stdout to p3
            close(p2[0]);
            close(p2[1]);
            close(p3[0]);
            close(p3[1]);
            char *args[] = {const_cast<char *>("./encryptor"), NULL};
            execvp(args[0], args);
        }
        else
        {
            close(p1[0]);
            close(p2[0]);
            close(p3[1]);

            // non-blocking for reading
            int flags = fcntl(p3[0], F_GETFL, 0);
            fcntl(p3[0], F_SETFL, flags | O_NONBLOCK);

            std::string userInput;
            do
            {
                std::cout << "---------------------------" << std::endl;
                std::cout << "Menu" << std::endl;
                std::cout << "---------------------------" << std::endl;
                std::cout << "password\nencrypt\ndecrypt\nhistory\nquit\n";
                std::cout << "Enter an option: ";

                std::cin >> userInput;
                std::string loggerInput = userInput + "\n";

                if (userInput == "password")
                {
                    if (history.empty())
                    {
                        std::cout << "Enter Password: ";
                        std::cin >> password;
                        encryptCommand = "PASS " + password + "\n";
                        write(p2[1], encryptCommand.c_str(), encryptCommand.size());
                        write(p1[1], loggerInput.c_str(), loggerInput.size());
                    }
                    else
                    {
                        std::cout << "Would you like to use history Y/N: ";
                        char input;
                        std::cin >> input;
                        input = std::tolower(input);
                        if (input == 'n')
                        {
                            std::cout << "Enter Password: ";
                            std::cin >> password;
                            encryptCommand = "PASS " + password + "\n";
                            write(p2[1], encryptCommand.c_str(), encryptCommand.size());
                            write(p1[1], loggerInput.c_str(), loggerInput.size());
                        }
                        else if (input == 'y')
                        {
                            for (int i = 1; i <= history.size(); i++)
                            {
                                std::cout << i << ". " << history[i - 1] << std::endl;
                            }
                            std::cout << "Pick which number to set as password: ";
                            int pass;
                            std::cin >> pass;
                            while (pass < 1 || pass > history.size())
                            {
                                std::cout << "Invalid Option, Try Again: ";
                                std::cin >> pass;
                            }
                            password = history[pass - 1];
                            encryptCommand = "PASS " + password + "\n";
                            write(p2[1], encryptCommand.c_str(), encryptCommand.size());
                            write(p1[1], loggerInput.c_str(), loggerInput.size());

                            char buffer[1024];
                            usleep(10000);
                            ssize_t n = read(p3[0], buffer, sizeof(buffer) - 1);
                            if (n > 0)
                            {
                                buffer[n] = '\0';
                            }
                        }
                    }
                }
                else if (userInput == "encrypt")
                {
                    std::string textToEncrypt;
                    if (history.empty())
                    {
                        std::cout << "Enter string to encrypt: ";
                        std::cin >> textToEncrypt;
                        history.push_back(textToEncrypt);
                    }
                    else
                    {
                        std::cout << "Would you like to use history Y/N: ";
                        char input;
                        std::cin >> input;
                        input = std::tolower(input);
                        if (input == 'n')
                        {
                            std::cout << "Enter string to encrypt: ";
                            std::cin >> textToEncrypt;
                            history.push_back(textToEncrypt);
                        }
                        else if (input == 'y')
                        {
                            for (int i = 1; i <= history.size(); i++)
                            {
                                std::cout << i << ". " << history[i - 1] << std::endl;
                            }
                            std::cout << "Pick which number to encrypt: ";
                            int choice;
                            std::cin >> choice;
                            while (choice < 1 || choice > history.size())
                            {
                                std::cout << "Invalid Option, Try Again: ";
                                std::cin >> choice;
                            }
                            textToEncrypt = history[choice - 1];
                        }
                    }

                    encryptCommand = "ENCRYPT " + textToEncrypt + "\n";
                    write(p2[1], encryptCommand.c_str(), encryptCommand.size());

                    write(p1[1], loggerInput.c_str(), loggerInput.size());
                    std::string originalText = textToEncrypt + "\n";
                    write(p1[1], originalText.c_str(), originalText.size());

                    // read from encryptor
                    char buffer[1024];
                    usleep(10000); // delay
                    ssize_t n = read(p3[0], buffer, sizeof(buffer) - 1);
                    if (n > 0)
                    {
                        buffer[n] = '\0';
                        std::string output(buffer);

                        // parse output
                        size_t resultPos = output.find("RESULT ");
                        if (resultPos != std::string::npos)
                        {
                            std::string result = output.substr(resultPos + 7); // Skip "RESULT "
                            // remove newlines using remove
                            result.erase(std::remove_if(result.begin(), result.end(),
                                                        [](char c)
                                                        { return c == '\n' || c == '\r'; }),
                                         result.end());

                            // print to terminal
                            std::cout << "RESULT " << result << std::endl;

                            // send message to logger
                            std::string logMsg = "Success: " + result + "\n";
                            write(p1[1], logMsg.c_str(), logMsg.size());
                        }
                    }
                }
                else if (userInput == "decrypt")
                {
                    std::string textToDecrypt;
                    if (history.empty())
                    {
                        std::cout << "Enter string to decrypt: ";
                        std::cin >> textToDecrypt;
                        history.push_back(textToDecrypt);
                    }
                    else
                    {
                        std::cout << "Would you like to use history Y/N: ";
                        char input;
                        std::cin >> input;
                        input = std::tolower(input);
                        if (input == 'n')
                        {
                            std::cout << "Enter string to decrypt: ";
                            std::cin >> textToDecrypt;
                            history.push_back(textToDecrypt);
                        }
                        else if (input == 'y')
                        {
                            for (int i = 1; i <= history.size(); i++)
                            {
                                std::cout << i << ". " << history[i - 1] << std::endl;
                            }
                            std::cout << "Pick which number to decrypt: ";
                            int choice;
                            std::cin >> choice;
                            while (choice < 1 || choice > history.size())
                            {
                                std::cout << "Invalid Option, Try Again: ";
                                std::cin >> choice;
                            }
                            textToDecrypt = history[choice - 1];
                        }
                    }

                    encryptCommand = "DECRYPT " + textToDecrypt + "\n";
                    write(p2[1], encryptCommand.c_str(), encryptCommand.size());

                    write(p1[1], loggerInput.c_str(), loggerInput.size());
                    std::string originalText = textToDecrypt + "\n";
                    write(p1[1], originalText.c_str(), originalText.size());

                    // same for decrypt
                    char buffer[1024];
                    usleep(10000); // delay
                    ssize_t n = read(p3[0], buffer, sizeof(buffer) - 1);
                    if (n > 0)
                    {
                        buffer[n] = '\0';
                        std::string output(buffer);

                        size_t resultPos = output.find("RESULT ");
                        if (resultPos != std::string::npos)
                        {
                            std::string result = output.substr(resultPos + 7);
                            result.erase(std::remove_if(result.begin(), result.end(),
                                                        [](char c)
                                                        { return c == '\n' || c == '\r'; }),
                                         result.end());

                            std::cout << "RESULT " << result << std::endl;

                            std::string logMsg = "Success: " + result + "\n";
                            write(p1[1], logMsg.c_str(), logMsg.size());
                        }
                    }
                }
                else if (userInput == "history")
                {
                    if (history.empty())
                    {
                        std::cout << "History is empty." << std::endl;
                    }
                    else
                    {
                        for (int i = 1; i <= history.size(); i++)
                        {
                            std::cout << i << ". " << history[i - 1] << std::endl;
                        }
                    }
                    write(p1[1], loggerInput.c_str(), loggerInput.size());
                }

            } while (userInput != "quit");

            std::string quitCommand = "QUIT\n";
            write(p2[1], quitCommand.c_str(), quitCommand.size());
            write(p1[1], quitCommand.c_str(), quitCommand.size());

            close(p1[1]);
            close(p2[1]);
            close(p3[0]);
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