OCT 13 4:55 PM
Project 1 involes using processes, there will be three separate programs, a logger which logs all activity, an encryption program that encrypts strings using Vigenere cypher and a driver program that is responsible for starting both the encryption and logger program. Since the Driver is basically the starting point for the other two programs i might start building that first. I plan to use C++ to build this because i've been wanting to go more indepth with C++ and this will provide me with the necessary practice.

What i plan to achive this session is as follows:
Establish menu for Driver process
Establish Communcation between Logger and Logger File
Establish Communcation between Logger process and Driver Process
Stretch goals:
Establish communication betwwen Driver and Encryption Program

based on past experiences with C/C++ i know i need to use pipes for communcation between two processes so in total this project will need 2 pipes, one for the logger and the other for the encryption process.
I will create the menu first and build from there
I have created the menu and added process id variables for the logger and the encryption process. what i need to do now is to create both processes but i will start with the logger because i want to establish comuncation between the logger and the driver first

I have been able to create the logger file and also pipe its stdin to the driver file so it will receive input from the driver

The next thing would be to create a file and set the input of the file to the output of logger so when i std::cout in logger it writes to the file instead of the terminal

I have been able to create the file however it is not reading input from the terminal, more research needs to be done to figure out why. I started doing research into how to make the logger start with the YYY:MM:DD:HH:MM format and stumbled upon an article that uses chrono and system_clock to get the current time. I will use the article as a reference to implement it.

I have been able to implement the clock, now i am able to get the current time. I will use both functions in driver and logger because i need to know when the driver started so that can be included in the log.

Right now what im able to do is create a file, get the time from driver and send it to logger which writes to the file. Which means i have achieved 2 of my goals. The last goal of today is to establish communcation between logger and logger file (make the file read from logger input)

Upon further research, i realized the std::getline() function will be best for this, i am already appending a new line character to the userInput so the std::getline() function should be able to differenciate between options. For asthethic purposes i will make each option the user selects uppercase and enclosed in brackets.

I have implemented the std::getline() function and it is able to read the options that the user picked so i have been able to achieve all my goals for this session. Now it is time to work on the stretch goal which is to establish communication between driver and encryption process

i have created the encryption file, now i just need to establish communication between the encryption process and the driver. This will be similar to the logger so i will reuse code

OCT 14 06:12 AM
I didnt complete the stretch goals for yesterday but i was able to complete the planned goals. This session i plan to finish the driver program and implement all the options so that it easier for me to connect it to the encryption program when i get to writing that. Right now i have completed the password option and part of the history option, all that is left is to complete the history, decrypt, and encrypt options.
