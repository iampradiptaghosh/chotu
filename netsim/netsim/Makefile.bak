TARGET=libnetsim.a
OBJECTS=Node.o FIFONode.o Scheduler.o PacketScheduler.o Timer.o Topology.o lex.yy.o parser.o Config.o Packet.o
SOURCES=Node.cpp FIFONode.cpp Scheduler.cpp PacketScheduler.cpp Timer.cpp Topology.cpp Packet.cpp
CCFLAGS=-g

all: $(TARGET)

$(TARGET): $(OBJECTS)
	ar cru $(TARGET) $(OBJECTS)

%.o: %.cpp %.h
	g++ -c $(CCFLAGS) $*.cpp

%.o: %.cpp 
	g++ -c $(CCFLAGS) $*.cpp

%.o: %.c
	gcc -c $(CCFLAGS) $*.c

depend: $(SOURCES)
	makedepend $(SOURCES)

parser.cpp: parser.y
	yacc -d parser.y -o parser.cpp

lex.yy.c: lexer.l
	flex lexer.l

clean:
	rm *.o libnetsim.a

# DO NOT DELETE

Node.o: common.h /usr/include/stdlib.h /usr/include/features.h
Node.o: /usr/include/alloca.h Timer.h Packet.h PacketScheduler.h Scheduler.h
Node.o: Topology.h Node.h
FIFONode.o: common.h /usr/include/stdlib.h /usr/include/features.h
FIFONode.o: /usr/include/alloca.h Timer.h Packet.h PacketScheduler.h
FIFONode.o: Scheduler.h Topology.h Node.h FIFONode.h
Scheduler.o: common.h /usr/include/stdlib.h /usr/include/features.h
Scheduler.o: /usr/include/alloca.h Node.h PacketScheduler.h Timer.h
Scheduler.o: Scheduler.h
PacketScheduler.o: common.h /usr/include/stdlib.h /usr/include/features.h
PacketScheduler.o: /usr/include/alloca.h Node.h Packet.h PacketScheduler.h
PacketScheduler.o: Timer.h Topology.h Scheduler.h
Timer.o: common.h /usr/include/stdlib.h /usr/include/features.h
Timer.o: /usr/include/alloca.h Timer.h Node.h PacketScheduler.h Topology.h
Timer.o: Scheduler.h
Topology.o: common.h /usr/include/stdlib.h /usr/include/features.h
Topology.o: /usr/include/alloca.h Packet.h Node.h PacketScheduler.h Timer.h
Topology.o: Scheduler.h Topology.h
Packet.o: common.h /usr/include/stdlib.h /usr/include/features.h
Packet.o: /usr/include/alloca.h Timer.h Packet.h PacketScheduler.h
Packet.o: Scheduler.h Topology.h Node.h
