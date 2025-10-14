all: driver logger encryptor

driver: Driver.cpp
	g++ Driver.cpp -o driver

logger: Logger.cpp
	g++ Logger.cpp -o logger

encryptor: Encryptor.cpp
	g++ Encryptor.cpp -o encryptor

clean:
	rm -f driver logger encryptor
