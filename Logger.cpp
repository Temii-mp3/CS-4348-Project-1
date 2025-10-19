#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <chrono>
std::string getCurrentTime();
int main(int argc, const char **argv)
{
    std::string filename = argv[1];
    std::string nextAction;
    std::ifstream infile(filename);
    if (!infile)
    {
        std::cout << "File does not exist. Creating it...\n"
                  << std::flush;
        std::ofstream outfile(filename);
        if (!outfile)
        {
            std::cerr << "Error creating file\n";
            return 1;
        }
    }
    std::cout << argv[2] << " " << "[START] Driver Started\n"
              << getCurrentTime() << " " << "[START] Logger Started\n"
              << std::flush;
    while (std::getline(std::cin, nextAction))
    {
        if (nextAction == "quit")
            break;
        for (char &c : nextAction)
            c = std::toupper(c);
        if (nextAction == "PASSWORD")
        {
            std::cout << getCurrentTime() << " [PASSWORD] password was set" << std::endl;
        }
        else if (nextAction == "ENCRYPT")
        {
            std::string textEncrypted;
            std::getline(std::cin, textEncrypted);
            std::cout << getCurrentTime() << " [ENCRYPT] " << textEncrypted << std::endl;

            // Read the success line
            std::string successLine;
            std::getline(std::cin, successLine);
            std::cout << getCurrentTime() << " [ENCRYPT] " << successLine << std::endl;
        }
        else if (nextAction == "DECRYPT")
        {
            std::string textDecrypted;
            std::getline(std::cin, textDecrypted);
            std::cout << getCurrentTime() << " [DECRYPT] " << textDecrypted << std::endl;

            // Read the success line
            std::string successLine;
            std::getline(std::cin, successLine);
            std::cout << getCurrentTime() << " [DECRYPT] " << successLine << std::endl;
        }
        else if (nextAction == "HISTORY")
        {
            std::cout << getCurrentTime() << " [HISTORY] History Checked" << std::endl;
        }
        else
        {
            std::cout << getCurrentTime() << " [" << nextAction << "]" << std::endl;
        }
    }
    std::cout << getCurrentTime() << " [STOPPED] Logger Stopped" << std::endl;
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