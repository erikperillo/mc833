DEPS = group.h message.h user.h chat.h
OBJS = group.o message.o user.o chat.o
OBJSDIR = ./bin
CC = g++
CCFLAGS = -std=c++14 -Wall

all: $(OBJS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $(OBJSDIR)/$@ $< $(CCFLAGS)
