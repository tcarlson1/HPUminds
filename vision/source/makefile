CC = g++
CFLAGS = -g -Wall
SRCS = pathFollower.cpp
PROG = pathFollower

OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)