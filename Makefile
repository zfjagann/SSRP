# Christopher Earl
# Makefile for graphGroup

# Rewritten by Zeal Jagannatha May2011 #

.PHONY: clean realclean git

SrcDir = src

Includes	= \
	-I$(SrcDir)/FWGroup \
	-I$(SrcDir)/basicEdgeGroup \
	-I$(SrcDir)/edge \
	-I$(SrcDir)/journeyGroup \
	-I$(SrcDir)/match \
	-I$(SrcDir)/basicStructures \
	-I$(SrcDir)/journeyInfo \
	-I$(SrcDir)/path \
	-I$(SrcDir)/vertex \
	-I$(SrcDir)/edgeGroup \
	-I$(SrcDir)/graphGroup \
	-I$(SrcDir)/journey \
	-I$(SrcDir)/sHeuristics \
	-I$(SrcDir)/rand \
	-I$(SrcDir)/ioFunctions

CXX		= g++
CXXFLAGS	= -g $(Includes)

OBJS =	$(SrcDir)/basicEdgeGroup/basicEdgeGroup.o \
	$(SrcDir)/edge/edge.o \
	$(SrcDir)/journeyGroup/journeyGroup.o \
	$(SrcDir)/match/match.o \
	$(SrcDir)/test.o \
	$(SrcDir)/basicStructures/basicStructures.o \
	$(SrcDir)/FWGroup/FWGroup.o \
	$(SrcDir)/journeyInfo/journeyInfo.o \
	$(SrcDir)/path/path.o \
	$(SrcDir)/vertex/vertex.o \
	$(SrcDir)/edgeGroup/edgeGroup.o \
	$(SrcDir)/graphGroup/graphGroup.o \
	$(SrcDir)/journey/journey.o \
	$(SrcDir)/sHeuristics/sHeuristics.o \
	$(SrcDir)/rand/rand.o \
	$(SrcDir)/ioFunctions/ioFunctions.o

EXE =	graph

graph: $(OBJS)
	g++ -o $(EXE) $(OBJS)

clean:
	rm -f $(OBJS)

realclean: clean
	rm -f $(EXE)
	rm -f `find . | grep "\.*~"`

$(SrcDir)/test.o: \
	$(SrcDir)/test.cpp

$(SrcDir)/test.cpp:

$(SrcDir)/sHeuristics/sHeuristics.o: \
	$(SrcDir)/sHeuristics/sHeuristics.cpp \
	$(SrcDir)/sHeuristics/sHeuristics.h \
	$(SrcDir)/graphGroup/graphGroup.h \
	$(SrcDir)/journeyInfo/journeyInfo.h

$(SrcDir)/sHeuristics/sHeuristics.h:

$(SrcDir)/sHeuristics/sHeuristics.cpp:

$(SrcDir)/FWGroup/FWGroup.o: \
	$(SrcDir)/FWGroup/FWGroup.cpp \
	$(SrcDir)/FWGroup/FWGroup.h \
	$(SrcDir)/basicStructures/basicStructures.h \
	$(SrcDir)/vertex/vertex.h \
	$(SrcDir)/path/path.h \
	$(SrcDir)/journeyInfo/journeyInfo.h \
	$(SrcDir)/graphGroup/graphGroup.h

$(SrcDir)/FWGroup/FWGroup.cpp:

$(SrcDir)/FWGroup/FWGroup.h:

$(SrcDir)/graphGroup/graphGroup.o: \
	$(SrcDir)/graphGroup/graphGroup.cpp \
	$(SrcDir)/graphGroup/graphGroup.h \
	$(SrcDir)/basicStructures/basicStructures.h \
	$(SrcDir)/path/path.h \
	$(SrcDir)/basicEdgeGroup/basicEdgeGroup.h \
	$(SrcDir)/journeyInfo/journeyInfo.h \
	$(SrcDir)/journeyGroup/journeyGroup.h \
	$(SrcDir)/edgeGroup/edgeGroup.h \
	$(SrcDir)/match/match.h

$(SrcDir)/graphGroup/graphGroup.cpp:

$(SrcDir)/graphGroup/graphGroup.h:

$(SrcDir)/edgeGroup/edgeGroup.o: \
	$(SrcDir)/edgeGroup/edgeGroup.h \
	$(SrcDir)/edgeGroup/edgeGroup.cpp \
	$(SrcDir)/basicStructures/basicStructures.h \
	$(SrcDir)/vertex/vertex.h \
	$(SrcDir)/path/path.h \
	$(SrcDir)/edge/edge.h \
	$(SrcDir)/basicEdgeGroup/basicEdgeGroup.h \
	$(SrcDir)/journeyInfo/journeyInfo.h

$(SrcDir)/edgeGroup/edgeGroup.h:

$(SrcDir)/edgeGroup/edgeGroup.cpp:

$(SrcDir)/journeyGroup/journeyGroup.o: \
	$(SrcDir)/journeyGroup/journeyGroup.cpp \
	$(SrcDir)/journeyGroup/journeyGroup.h \
	$(SrcDir)/basicStructures/basicStructures.h \
	$(SrcDir)/path/path.h \
	$(SrcDir)/journey/journey.h \
	$(SrcDir)/journeyInfo/journeyInfo.h

$(SrcDir)/journeyGroup/journeyGroup.cpp:

$(SrcDir)/journeyGroup/journeyGroup.h:

$(SrcDir)/journey/journey.o: \
	$(SrcDir)/journey/journey.cpp \
	$(SrcDir)/journey/journey.h \
	$(SrcDir)/basicStructures/basicStructures.h \
	$(SrcDir)/path/path.h

$(SrcDir)/journey/journey.cpp:

$(SrcDir)/journey/journey.h:

$(SrcDir)/journeyInfo/journeyInfo.o: \
	$(SrcDir)/journeyInfo/journeyInfo.cpp \
	$(SrcDir)/journeyInfo/journeyInfo.h

$(SrcDir)/journeyInfo/journeyInfo.h:

$(SrcDir)/journeyInfo/journeyInfo.cpp:

$(SrcDir)/basicEdgeGroup/basicEdgeGroup.o: \
	$(SrcDir)/basicEdgeGroup/basicEdgeGroup.cpp \
	$(SrcDir)/basicEdgeGroup/basicEdgeGroup.h \
	$(SrcDir)/basicStructures/basicStructures.h

$(SrcDir)/basicEdgeGroup/basicEdgeGroup.h:

$(SrcDir)/basicEdgeGroup/basicEdgeGroup.cpp:

$(SrcDir)/edge/edge.o: \
	$(SrcDir)/edge/edge.cpp \
	$(SrcDir)/edge/edge.h \
	$(SrcDir)/basicStructures/basicStructures.h

$(SrcDir)/edge/edge.cpp:

$(SrcDir)/edge/edge.h:

$(SrcDir)/path/path.o: \
	$(SrcDir)/path/path.cpp \
	$(SrcDir)/path/path.h \
	$(SrcDir)/basicStructures/basicStructures.h

$(SrcDir)/path/path.cpp:

$(SrcDir)/path/path.h:

$(SrcDir)/vertex/vertex.o: \
	$(SrcDir)/vertex/vertex.cpp \
	$(SrcDir)/vertex/vertex.h \
	$(SrcDir)/basicStructures/basicStructures.h

$(SrcDir)/vertex/vertex.cpp:

$(SrcDir)/vertex/vertex.h:

$(SrcDir)/basicStructures/basicStructures.o: \
	$(SrcDir)/basicStructures/basicStructures.cpp \
	$(SrcDir)/basicStructures/basicStructures.h

$(SrcDir)/basicStructures/basicStructures.h:

$(SrcDir)/basicStructures/basicStructures.cpp:

$(SrcDir)/match/match.o: \
	$(SrcDir)/path/path.h \
	$(SrcDir)/match/match.cpp \
	$(SrcDir)/match/match.h

$(SrcDir)/match/match.cpp:

$(SrcDir)/match/match.h:

$(SrcDir)/ioFunctions/ioFunctions.o: \
	$(SrcDir)/ioFunctions/ioFunctions.cpp \
	$(SrcDir)/ioFunctions/ioFunctions.h

$(SrcDir)/ioFunctions/ioFunctions.h:

$(SrcDir)/ioFunctions/ioFunctions.cpp:

$(SrcDir)/rand/rand.o: \
	$(SrcDir)/rand/rand.h \
	$(SrcDir)/rand/rand.cpp

$(SrcDir)/rand/rand.h:

$(SrcDir)/rand/rand.cpp: