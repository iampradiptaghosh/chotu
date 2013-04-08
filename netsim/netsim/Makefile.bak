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

Node.o: common.h /usr/include/sys/types.h /usr/include/sys/feature_tests.h
Node.o: /usr/include/sys/ccompile.h /usr/include/sys/isa_defs.h
Node.o: /usr/include/sys/machtypes.h /usr/include/sys/int_types.h
Node.o: /usr/include/sys/select.h /usr/include/sys/time_impl.h
Node.o: /usr/include/sys/time.h /usr/include/sys/time.h /usr/include/stdlib.h
Node.o: /usr/include/iso/stdlib_iso.h /usr/include/iso/stdlib_c99.h Timer.h
Node.o: Packet.h PacketScheduler.h Scheduler.h Topology.h Node.h
FIFONode.o: common.h /usr/include/sys/types.h
FIFONode.o: /usr/include/sys/feature_tests.h /usr/include/sys/ccompile.h
FIFONode.o: /usr/include/sys/isa_defs.h /usr/include/sys/machtypes.h
FIFONode.o: /usr/include/sys/int_types.h /usr/include/sys/select.h
FIFONode.o: /usr/include/sys/time_impl.h /usr/include/sys/time.h
FIFONode.o: /usr/include/sys/time.h /usr/include/stdlib.h
FIFONode.o: /usr/include/iso/stdlib_iso.h /usr/include/iso/stdlib_c99.h
FIFONode.o: Timer.h Packet.h PacketScheduler.h Scheduler.h Topology.h Node.h
FIFONode.o: FIFONode.h
Scheduler.o: common.h /usr/include/sys/types.h
Scheduler.o: /usr/include/sys/feature_tests.h /usr/include/sys/ccompile.h
Scheduler.o: /usr/include/sys/isa_defs.h /usr/include/sys/machtypes.h
Scheduler.o: /usr/include/sys/int_types.h /usr/include/sys/select.h
Scheduler.o: /usr/include/sys/time_impl.h /usr/include/sys/time.h
Scheduler.o: /usr/include/sys/time.h /usr/include/stdlib.h
Scheduler.o: /usr/include/iso/stdlib_iso.h /usr/include/iso/stdlib_c99.h
Scheduler.o: Node.h PacketScheduler.h Timer.h Scheduler.h
PacketScheduler.o: common.h /usr/include/sys/types.h
PacketScheduler.o: /usr/include/sys/feature_tests.h
PacketScheduler.o: /usr/include/sys/ccompile.h /usr/include/sys/isa_defs.h
PacketScheduler.o: /usr/include/sys/machtypes.h /usr/include/sys/int_types.h
PacketScheduler.o: /usr/include/sys/select.h /usr/include/sys/time_impl.h
PacketScheduler.o: /usr/include/sys/time.h /usr/include/sys/time.h
PacketScheduler.o: /usr/include/stdlib.h /usr/include/iso/stdlib_iso.h
PacketScheduler.o: /usr/include/iso/stdlib_c99.h Node.h Packet.h
PacketScheduler.o: PacketScheduler.h Timer.h Topology.h Scheduler.h
Timer.o: common.h /usr/include/sys/types.h /usr/include/sys/feature_tests.h
Timer.o: /usr/include/sys/ccompile.h /usr/include/sys/isa_defs.h
Timer.o: /usr/include/sys/machtypes.h /usr/include/sys/int_types.h
Timer.o: /usr/include/sys/select.h /usr/include/sys/time_impl.h
Timer.o: /usr/include/sys/time.h /usr/include/sys/time.h
Timer.o: /usr/include/stdlib.h /usr/include/iso/stdlib_iso.h
Timer.o: /usr/include/iso/stdlib_c99.h Timer.h Node.h PacketScheduler.h
Timer.o: Topology.h Scheduler.h
Topology.o: common.h /usr/include/sys/types.h
Topology.o: /usr/include/sys/feature_tests.h /usr/include/sys/ccompile.h
Topology.o: /usr/include/sys/isa_defs.h /usr/include/sys/machtypes.h
Topology.o: /usr/include/sys/int_types.h /usr/include/sys/select.h
Topology.o: /usr/include/sys/time_impl.h /usr/include/sys/time.h
Topology.o: /usr/include/sys/time.h /usr/include/stdlib.h
Topology.o: /usr/include/iso/stdlib_iso.h /usr/include/iso/stdlib_c99.h
Topology.o: Packet.h Node.h PacketScheduler.h Timer.h Scheduler.h Topology.h
Packet.o: common.h /usr/include/sys/types.h /usr/include/sys/feature_tests.h
Packet.o: /usr/include/sys/ccompile.h /usr/include/sys/isa_defs.h
Packet.o: /usr/include/sys/machtypes.h /usr/include/sys/int_types.h
Packet.o: /usr/include/sys/select.h /usr/include/sys/time_impl.h
Packet.o: /usr/include/sys/time.h /usr/include/sys/time.h
Packet.o: /usr/include/stdlib.h /usr/include/iso/stdlib_iso.h
Packet.o: /usr/include/iso/stdlib_c99.h Timer.h Packet.h PacketScheduler.h
Packet.o: Scheduler.h Topology.h Node.h
