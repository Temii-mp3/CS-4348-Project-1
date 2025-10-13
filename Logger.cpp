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
        std::cout << "File does not exist. Creating it...\n";
        std::ofstream outfile(filename);
        if (!outfile)
        {
            std::cerr << "Error creating file\n";
            return 1;
        }
    }
    else
    {
        std::cout << argv[2] << " " << "[START] Driver Started\n"
                  << getCurrentTime() << " " << "[START] Logger Started\n";
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
