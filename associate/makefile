main: main.cpp associate.o
	g++ -g main.cpp associate.o associaterq.o associateac.o ../dul/dulassociaterq.o ../dul/dulassociateac.o ../socket/socket.o ../pdatatf/dimse/dimse.o ../pdatatf/dimse/dimserq.o ../pdatatf/dimse/dimsersp.o ../pdatatf/dimse/cfind/cfindrq.o ../pdatatf/dimse/cfind/cfindrsp.o ../pdatatf/dimse/cmove/cmoverq.o ../pdatatf/dimse/cmove/cmoversp.o ../pdatatf/pdatatf.o ../dul/dulpdatatfrq.o ../dul/dulpdatatfrsp.o -o main
associate.o: associate.h associate.cpp associaterq.o associateac.o
	g++ -g -c associate.cpp
associaterq.o: associaterq.h associaterq.cpp 
	g++ -g -c associaterq.cpp
associateac.o: associateac.h associateac.cpp 
	g++ -g -c associateac.cpp
clean:
	rm main *.o
