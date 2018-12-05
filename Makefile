CC = g++
CFLAGS = -ggdb3 -std=c++11 -Wall
TARGET = a.out
OBJECTS = main.o lrt.o pst.o wl.o util.o

all : $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

main.o : main.cpp
	g++ -g -std=c++11 -c -o main.o main.cpp

lrt.o : lrt.cpp
	g++ -g -std=c++11 -c -o lrt.o lrt.cpp

pst.o : pst.cpp
	g++ -g -std=c++11 -c -o pst.o pst.cpp

wl.o : wl.cpp
	g++ -g -std=c++11 -c -o wl.o wl.cpp

util.o : util.cpp
	g++ -g -std=c++11 -c -o util.o util.cpp


clean :
	rm *.o a.out
