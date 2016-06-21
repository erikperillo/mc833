DEPS = group.h message.h user.h elementnotfound.h chat.h chatview.h chatcontroller.h peer.h
OBJS = group.o message.o user.o elementnotfound.o chat.o chatview.o chatcontroller.o peer.o
OBJSDIR = .
CC = g++
CCFLAGS = -std=c++14 -Wall

all: $(OBJS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $(OBJSDIR)/$@ $< $(CCFLAGS)

test: $(OBJS) test.cpp
	$(CC) -o test test.cpp $(OBJS) $(CLIFLAGS) $(CCFLAGS)
