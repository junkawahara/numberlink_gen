OPT = -O3 -DB_64
OBJS = Graph.o State.o Mate.o Hint.o
SBDDDIR = SAPPOROBDD
SBDDOBJS = $(SBDDDIR)/bddc.o $(SBDDDIR)/BDD.o $(SBDDDIR)/ZBDD.o
HEADERS = TimeCount.h Edge.h Graph.h State.h Mate.h Hint.h ZDDNode.h Hash.h ZDDNodeSet.h

all: nlgen makegrid

nlgen: nlgen.o $(HEADERS) $(OBJS) $(SBDDOBJS)
	g++ $(OPT) -o nlgen nlgen.o $(OBJS) $(SBDDOBJS) $(OPT)

makegrid: makegrid.c
	gcc -o makegrid makegrid.c -O3

.c.o:
	gcc -o $*.o -c $*.c $(OPT)
.cpp.o:
	g++ -o $*.o -c $*.cpp $(OPT)
.cc.o:
	g++ -o $*.o -c $*.cc $(OPT)

clean:
	rm -f *.o
	rm -f $(SBDDOBJS)
	rm -f nlgen.exe
	rm -f nlgen
	rm -f makegrid.exe
	rm -f makegrid
