#VPATH=source
vpath %.c ../source
vpath %.h ../source

#OBJS = $(wildcard *.o)
OBJS = game.o TcpSocket.o DoFrame.o Action.o PokerRate.o
CC = g++
CFLAGS = -g

../target/game:$(OBJS)
	$(CC) -lm -o $@ $(OBJS)
game.o:game.c Type.h TcpSocket.h DoFrame.h
	$(CC) $(CFLAGS) -c $< -o $@
TcpSocket.o:TcpSocket.c TcpSocket.h Type.h
	$(CC) $(CFLAGS) -c $< -o $@
DoFrame.o:DoFrame.c DoFrame.h Type.h
	$(CC) $(CFLAGS) -c $< -o $@
Action.o:Action.c Action.h Type.h PokerRate.h
	$(CC) $(CFLAGS) -c $< -o $@
PokerRate.o:PokerRate.c PokerRate.h
	$(CC) $(CFLAGS) -c $< -o $@
	

.PHONY:clean
clean:
	rm $(OBJS)
