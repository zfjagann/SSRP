#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <limits.h>
#include <utility>
#include <FlexLexer.h>
#include <time.h>

using namespace std;

#include "colorString.h"
#include "options.h"
#include "basicEdgeGroup.h"
#include "graphGroup.h"
#include "ioFunctions.h"
#include "heuristics.h"
#include "rand.h"

#include "messages.h"

int yyparse();

template <typename T1, typename T2, typename T3>
struct triple {
    T1 first;
    T2 second;
    T3 third;
    triple(T1 initFirst, T2 initSecond, T3 initThird)
        : first(initFirst), second(initSecond), third(initThird) {
        // Do Nothing
    }
};

void doStats() {

    cout << "Using stat mode.\n";
    cout << "Vertices:   " << STATvertices << endl;
    cout << "Journeys:   " << STATjourneys << endl;
    cout << "Min Weight: " << STATminWeight << endl;
    cout << "Max Weight: " << STATmaxWeight << endl;
    cout << "Count:      " << STATcount << endl;
    cout << "Directed:   " << boolalpha << STATdirected << endl;

    // Turn off graph dumping
    //dumpGraphToFile = false;

    // Output formatting
    pad(" ", 10); output("|", "");
    for(int i = 0; i < heuristics.size(); i++){
        outputCenter(heuristics[i].name, heuristics[i].name.size()+2);
        output("|", "");
    }
    output(" Time (s)");

    vector<triple<int,double,double> > numberCorrect(heuristics.size(), triple<int,double,double>(0, 0.0, 0.0));

    // Run the loop
    for(int caseNumber = 0; caseNumber < STATcount; caseNumber++) {
        int graphNum = getGraphNumber();
        output("Case:", "");
        outputRight(str(graphNum),5);
        output("|", "");

        basicEdgeGroup g;
        graphGroup mainGraph;
        vector<journeyInfo> listOfJourneys;

        generateSparseGraph(g, STATvertices, STATdirected, STATminWeight, STATmaxWeight);

        listOfJourneys.resize(STATjourneys);
        for(int i = 0; i < STATjourneys; i++)
            listOfJourneys[i].setJourneyNum(i);
        generateJourneys(listOfJourneys, STATvertices);
        mainGraph.set(g, listOfJourneys);

        vector<floatWInf> results(heuristics.size());

        vector<double> times(heuristics.size(), 0.0);

        for(int i = 0; i < heuristics.size(); i++) {
            //timer start
            timespec sTime;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &sTime);
            
            graphGroup g = heuristics[i].func(mainGraph, listOfJourneys);

            //timer end
            timespec eTime;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &eTime);
	    
            double startTime = sTime.tv_sec + ((double)sTime.tv_nsec/ 1000000000.0);
	    double endTime = eTime.tv_sec + ((double)eTime.tv_nsec/ 1000000000.0);
	    
            dumpGraph(g,heuristics[i].name);
            results[i] = 0;
            for(int n = 0; n < listOfJourneys.size(); n++)
            {
                results[i] += g.returnSharedCost(n).value();
            }
            times[i] = endTime - startTime;
            numberCorrect[i].third += times[i];
        }

        floatWInf best = floatWInf(true, 0);
        int numBest = 0;
        for(int i = 0; i < heuristics.size(); i++) {
            if(results[i] < best) {
                best = results[i];
                numBest = 1;
            }
            else if(results[i] == best) {
                numBest++;
            }
        }

        double totalTime = 0;
        for(int i = 0; i < heuristics.size(); i++) {
            totalTime += times[i];
            if(results[i] == best) {
                numberCorrect[i].first++;
                numberCorrect[i].second += 1.0 / double(numBest);
                outputGreenRight(str(results[i]), heuristics[i].name.size()+2);
                output("|", "");
            }
            else {
                outputRight(str(results[i]), heuristics[i].name.size()+2);
                output("|", "");
            }
        }
        
        output(string(" ") + str(totalTime));

        incrementGraphNumber();
    }

    outputLeft("Totals:", 10);
    output("|", "");
    for(int i = 0; i < heuristics.size(); i++) {
        outputCenter(heuristics[i].name, heuristics[i].name.size()+2);
        output("|", "");
    }
    output("");

    pad(" ", 10);
    output("|", "");
    for(int i = 0; i < heuristics.size(); i++) {
        output(str(numberCorrect[i].first), "");
        outputRight(str(numberCorrect[i].second, 4),
                    heuristics[i].name.size()+2
                    -str(numberCorrect[i].first).size());
        output("|", "");
    }
    output("");

    outputLeft("Time (s):", 10);
    output("|", "");
    double totalTime = 0;
    for(int i = 0; i < heuristics.size(); i++) {
        outputRight(str(double(numberCorrect[i].third)),
                    heuristics[i].name.size()+2);
        output("|", "");
        totalTime += double(numberCorrect[i].third);
    }
    output(" " + str(totalTime, 3));

    outputLeft("Time (%):", 10);
    output("|", "");
    for(int i = 0; i < heuristics.size(); i++) {
        outputRight(str(((double(numberCorrect[i].third)) * 100.0)  / totalTime),
                    heuristics[i].name.size()+2);
        output("|", "");
    }

    output("");
}

int main(int argc, char* argv[]) {

    output(welcomeHeader);

    ostringstream temp;
    for(int i = 1; i < argc; i++) {
        temp << argv[i] << " ";
    }
    istringstream* yyin = new istringstream(temp.str());
    scanner = new yyFlexLexer(yyin);
    yyparse();

    if(printHeuristicsInfo) {
        output(heuristicsInfo);
        return 0;
    }

    if(numErrors > 0) {
        output("");
        output(usage);
        return 1;
    }

    else {

        setupHeuristics();

        if(programMode == STAT) {

            doStats();

        }

        else {
            // programMode == DEMO

            if(false) {
                inFile = new ifstream("input");
            }
            else {
                inFile = &cin;
            }

            graphGroup mainGraph;
            basicEdgeGroup basicGraph;
            vector<journeyInfo> listOfJourneys;

            // Generate the graph and journeys
            readGraph(basicGraph);
            readJourneys(listOfJourneys, basicGraph);

            for(int i = 0; i < listOfJourneys.size(); i++) {
                output("Journey " +str(i)+ ": ", "");
                output(str(listOfJourneys[i].source()) + " -> " + str(listOfJourneys[i].destination()));
            }

            // Set the journey numbers for each journey:
            // Journey i's journey number is i
            for(int i = 0; i < listOfJourneys.size(); i++)
                listOfJourneys[i].setJourneyNum(i);
            mainGraph.set(basicGraph, listOfJourneys);

            int shortestPathCost = 0;
            for(int i = 0; i < heuristics.size(); i++) {
                graphGroup g = heuristics[i].func(mainGraph, listOfJourneys);
                dumpGraph(g,heuristics[i].name);
                int result = 0;
                for(int n = 0; n < listOfJourneys.size(); n++)
                    result += g.returnSharedCost(n).value();
                if(i == 0)
                    shortestPathCost = result;
                output("Running heuristic: " + heuristics[i].name);
                output("\tHas total cost of: " + str(result));
                if(i > 0)
                    output("\tImprovement over shortest path: " + str(shortestPathCost - result));
                output("");
            }

            if(inFile != &cin)
                delete inFile;

        }

        return 0;
    }
}