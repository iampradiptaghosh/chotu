TARGET=netsim_app
OBJECTS=main.o FDTPFlow.o Router.o Host.o DTPPacket.o app_config.o 
LIBRARY=../netsim/libnetsim.a
SOURCES=main.cpp FDTPFlow.cpp Router.cpp Host.cpp DTPPacket.cpp app_config.cpp
CCFLAGS=-g

all: netsim $(TARGET)

netsim:
	(cd ../netsim; make)

$(TARGET): $(OBJECTS) $(LIBRARY)
	g++ $(CCFLAGS) -o $(TARGET) $(OBJECTS) $(LIBRARY)

%.o: %.cpp
	g++ -c $(CCFLAGS) $*.cpp

%.o: %.c
	gcc -c $(CCFLAGS) $*.c

depend: $(SOURCES)
	(cd ../netsim; make depend)
	makedepend $(SOURCES)

clean:
	(cd ../netsim; make clean)
	rm *.o

# DO NOT DELETE

main.o: ../netsim/common.h /usr/include/stdlib.h /usr/include/features.h
main.o: /usr/include/alloca.h ../netsim/Config.h ../netsim/Packet.h
main.o: ../netsim/Timer.h ../netsim/PacketScheduler.h ../netsim/Scheduler.h
main.o: ../netsim/Topology.h /usr/include/unistd.h /usr/include/getopt.h
FDTPFlow.o: ../netsim/common.h /usr/include/stdlib.h /usr/include/features.h
FDTPFlow.o: /usr/include/alloca.h ../netsim/Node.h ../netsim/FIFONode.h
FDTPFlow.o: ../netsim/Packet.h DTPPacket.h ../netsim/Timer.h
FDTPFlow.o: ../netsim/PacketScheduler.h ../netsim/Scheduler.h FDTPFlow.h
FDTPFlow.o: Host.h ../netsim/Topology.h Router.h
Router.o: ../netsim/common.h /usr/include/stdlib.h /usr/include/features.h
Router.o: /usr/include/alloca.h ../netsim/Node.h ../netsim/FIFONode.h
Router.o: ../netsim/PacketScheduler.h ../netsim/Timer.h ../netsim/Scheduler.h
Router.o: Router.h FDTPFlow.h Host.h
Host.o: ../netsim/common.h /usr/include/stdlib.h /usr/include/features.h
Host.o: /usr/include/alloca.h ../netsim/Node.h ../netsim/FIFONode.h
Host.o: ../netsim/Packet.h DTPPacket.h ../netsim/PacketScheduler.h
Host.o: ../netsim/Timer.h ../netsim/Scheduler.h Host.h Router.h
DTPPacket.o: ../netsim/common.h /usr/include/stdlib.h /usr/include/features.h
DTPPacket.o: /usr/include/alloca.h ../netsim/Node.h ../netsim/FIFONode.h
DTPPacket.o: ../netsim/Packet.h DTPPacket.h ../netsim/Timer.h
DTPPacket.o: ../netsim/PacketScheduler.h ../netsim/Scheduler.h
app_config.o: /usr/include/string.h /usr/include/features.h
app_config.o: /usr/include/xlocale.h ../netsim/common.h /usr/include/stdlib.h
app_config.o: /usr/include/alloca.h ../netsim/Config.h ../netsim/Node.h
app_config.o: ../netsim/FIFONode.h ../netsim/Packet.h FDTPFlow.h Host.h
app_config.o: Router.h
