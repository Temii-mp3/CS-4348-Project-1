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
