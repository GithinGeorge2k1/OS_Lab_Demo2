main: main.cpp util.cpp
	g++ main.cpp util.cpp -lncurses -o main

clean:
	rm main
