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

main.o: ../netsim/common.h /usr/include/sys/types.h
main.o: /usr/include/sys/feature_tests.h /usr/include/sys/ccompile.h
main.o: /usr/include/sys/isa_defs.h /usr/include/sys/machtypes.h
main.o: /usr/include/sys/int_types.h /usr/include/sys/select.h
main.o: /usr/include/sys/time_impl.h /usr/include/sys/time.h
main.o: /usr/include/sys/time.h /usr/include/stdlib.h
main.o: /usr/include/iso/stdlib_iso.h /usr/include/iso/stdlib_c99.h
main.o: ../netsim/Config.h ../netsim/Packet.h ../netsim/Timer.h
main.o: ../netsim/PacketScheduler.h ../netsim/Scheduler.h
main.o: ../netsim/Topology.h /usr/include/unistd.h /usr/include/sys/unistd.h
FDTPFlow.o: ../netsim/common.h /usr/include/sys/types.h
FDTPFlow.o: /usr/include/sys/feature_tests.h /usr/include/sys/ccompile.h
FDTPFlow.o: /usr/include/sys/isa_defs.h /usr/include/sys/machtypes.h
FDTPFlow.o: /usr/include/sys/int_types.h /usr/include/sys/select.h
FDTPFlow.o: /usr/include/sys/time_impl.h /usr/include/sys/time.h
FDTPFlow.o: /usr/include/sys/time.h /usr/include/stdlib.h
FDTPFlow.o: /usr/include/iso/stdlib_iso.h /usr/include/iso/stdlib_c99.h
FDTPFlow.o: ../netsim/Node.h ../netsim/FIFONode.h ../netsim/Packet.h
FDTPFlow.o: DTPPacket.h ../netsim/Timer.h ../netsim/PacketScheduler.h
FDTPFlow.o: ../netsim/Scheduler.h FDTPFlow.h Host.h ../netsim/Topology.h
FDTPFlow.o: Router.h
Router.o: ../netsim/common.h /usr/include/sys/types.h
Router.o: /usr/include/sys/feature_tests.h /usr/include/sys/ccompile.h
Router.o: /usr/include/sys/isa_defs.h /usr/include/sys/machtypes.h
Router.o: /usr/include/sys/int_types.h /usr/include/sys/select.h
Router.o: /usr/include/sys/time_impl.h /usr/include/sys/time.h
Router.o: /usr/include/sys/time.h /usr/include/stdlib.h
Router.o: /usr/include/iso/stdlib_iso.h /usr/include/iso/stdlib_c99.h
Router.o: ../netsim/Node.h ../netsim/FIFONode.h ../netsim/PacketScheduler.h
Router.o: ../netsim/Timer.h ../netsim/Scheduler.h Router.h FDTPFlow.h Host.h
Host.o: ../netsim/common.h /usr/include/sys/types.h
Host.o: /usr/include/sys/feature_tests.h /usr/include/sys/ccompile.h
Host.o: /usr/include/sys/isa_defs.h /usr/include/sys/machtypes.h
Host.o: /usr/include/sys/int_types.h /usr/include/sys/select.h
Host.o: /usr/include/sys/time_impl.h /usr/include/sys/time.h
Host.o: /usr/include/sys/time.h /usr/include/stdlib.h
Host.o: /usr/include/iso/stdlib_iso.h /usr/include/iso/stdlib_c99.h
Host.o: ../netsim/Node.h ../netsim/FIFONode.h ../netsim/Packet.h DTPPacket.h
Host.o: ../netsim/PacketScheduler.h ../netsim/Timer.h ../netsim/Scheduler.h
Host.o: Host.h Router.h
DTPPacket.o: ../netsim/common.h /usr/include/sys/types.h
DTPPacket.o: /usr/include/sys/feature_tests.h /usr/include/sys/ccompile.h
DTPPacket.o: /usr/include/sys/isa_defs.h /usr/include/sys/machtypes.h
DTPPacket.o: /usr/include/sys/int_types.h /usr/include/sys/select.h
DTPPacket.o: /usr/include/sys/time_impl.h /usr/include/sys/time.h
DTPPacket.o: /usr/include/sys/time.h /usr/include/stdlib.h
DTPPacket.o: /usr/include/iso/stdlib_iso.h /usr/include/iso/stdlib_c99.h
DTPPacket.o: ../netsim/Node.h ../netsim/FIFONode.h ../netsim/Packet.h
DTPPacket.o: DTPPacket.h ../netsim/Timer.h ../netsim/PacketScheduler.h
DTPPacket.o: ../netsim/Scheduler.h
app_config.o: /usr/include/string.h /usr/include/iso/string_iso.h
app_config.o: /usr/include/sys/feature_tests.h /usr/include/sys/ccompile.h
app_config.o: /usr/include/sys/isa_defs.h ../netsim/common.h
app_config.o: /usr/include/sys/types.h /usr/include/sys/machtypes.h
app_config.o: /usr/include/sys/int_types.h /usr/include/sys/select.h
app_config.o: /usr/include/sys/time_impl.h /usr/include/sys/time.h
app_config.o: /usr/include/sys/time.h /usr/include/stdlib.h
app_config.o: /usr/include/iso/stdlib_iso.h /usr/include/iso/stdlib_c99.h
app_config.o: ../netsim/Config.h ../netsim/Node.h ../netsim/FIFONode.h
app_config.o: ../netsim/Packet.h FDTPFlow.h Host.h Router.h
