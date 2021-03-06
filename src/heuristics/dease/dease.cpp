#include "dease.h"

#include <vector>

using namespace std;

#include "graphGroup.h"
#include "journeyInfo.h"
#include "basicStructures.h"
#include "FWGroup.h"
#include "ioFunctions.h"
#include "debug.h"
#include "options.h"

//returns the index of (the first instance of)"key" in v,
// or -1 if it's not there
template<typename t>
int find_in_vector(const vector<t>& v, t key){
    for(unsigned i = 0; i < v.size(); i++){
        if(v[i] == key)
            return i;
    }
    // if we get here, it's not there
    return -1;
}

/*
 * Dease Algorithm
 *
 * Finds edges that are commonly shared and deletes them. This is to encourage
 * sharing and try to find a nash equilibrium.
 */
graphGroup runDeaseHeuristic(graphGroup mainGraph, const vector<journeyInfo>& listOfJourneys) {

    bool coalition_one = false;
    bool outsider_one = false;

    vector< vector< floatWInf > > minSavings;
    vector< vector< floatWInf > > maxSavings;
    vector< vector< floatWInf > > averageSavings;
    string fileName;

    FWGroup FloydPaths;
    vector<int> journeysNum;

    for(unsigned j = 0; j < listOfJourneys.size(); j++)
        journeysNum.push_back(listOfJourneys[j].journeyNum());

    FloydPaths.set(journeysNum, mainGraph);

    if(debug)
        output("Running tests...");

    //route each journey by it's FW path
    for(unsigned j = 0; j < listOfJourneys.size(); j++){
        mainGraph.addJourney(listOfJourneys[j].journeyNum(),
                             FloydPaths.returnPath(listOfJourneys[j].source(),
                                                   listOfJourneys[j].destination()));
    }

    for(unsigned cur_pass = 0; cur_pass < num_passes; cur_pass++){


        //for each edge...
        for(unsigned j = 0; j < mainGraph.returnN(); j++) {
            for(unsigned k = 0; k < mainGraph.returnN(); k++) {
                // if edge (j,k) has > 1 journey on it
                if (mainGraph.numJourneysUsing(j,k) >= journey_threshold &&
                    mainGraph.totalEdgeCost(j,k) / mainGraph.numJourneysUsing(j,k) >= weight_threshold
                    ){
                    //then delete the edge- first remove the journeys using it
                    if(debug){
                        output("removing journeys from edge ("
                                + str(j) + "," + str(k) + ")");
                    }

                    vector<int> journeys_rerouted; // list of journeys we rip up
                    vector<path> saved_paths; // paths of the journeys we rip up
                    vector<floatWInf> saved_costs; // costs of the paths we rip up
                    saved_paths = mainGraph.returnSharedPaths();
                    saved_costs = mainGraph.returnSharedCosts();
                    for(unsigned cur_journey = 0;
                        cur_journey < mainGraph.numJourneys();
                        cur_journey++){
                        if(mainGraph.isJourneyIn(j,k,cur_journey)){ // then rip it up

                            mainGraph.removeJourney(listOfJourneys[cur_journey].journeyNum());
                            journeys_rerouted.push_back(listOfJourneys[cur_journey].journeyNum());
                            if(debug)
                                output("Ripping up " + str(cur_journey));
                        }
                    }
                    //remove the edge from the graph
                    floatWInf saved_edge_cost = mainGraph.totalEdgeCost(j,k);
                    mainGraph.update_edge_cost(j,k,infinity);

                    //now reroute everyone that was displaced
                    mainGraph.addJourneysSP(journeys_rerouted);
                    bool displaced_improvement = false; // do any dispalced journeys get better?
                    unsigned displaced_improvement_num = 0;
                    bool outside_improvement = false;    //do any outsiders get better?
                    unsigned outsider_improvement_num = 0;

                    float sad_rat_num = 0;
                    float sad_rat_den = 0;
                    //now see if anyone else wants to move
                    for(unsigned cur_journey = 0; cur_journey < mainGraph.numJourneys();
                        cur_journey++){
                        int my_journey_num = listOfJourneys[cur_journey].journeyNum();
                        if(find_in_vector(journeys_rerouted,
                                            listOfJourneys[cur_journey].journeyNum())
                            == -1){
                            //then it's not there

                            mainGraph.removeJourney(my_journey_num);
                            mainGraph.addJourneySP(my_journey_num);
                            if(mainGraph.returnSharedCost(my_journey_num) < saved_costs[my_journey_num])
                                // an outsider got better
                                outsider_improvement_num++;
                        }
                        else{ // then this is a displaced journey
                            if (mainGraph.returnSharedCost(my_journey_num)
                                < saved_costs[my_journey_num])
                                // a displaced journey got better
                                displaced_improvement_num++ ;
                        }
                        floatWInf sad_difference = saved_costs[cur_journey] -
                            mainGraph.returnSharedCost(my_journey_num);
                        if(sad_difference > 0) // cost got better
                            sad_rat_den += sad_difference.value();
                        else  //cost got worse
                            sad_rat_num += -1* sad_difference.value();
                    }  // end loop through all journeys to update the outsiders
                    //and check for improvement

                    //put the edge back
                    mainGraph.update_edge_cost(j,k,saved_edge_cost);
                    // did enough displaced journeys improve?
                    if(coalition_one && displaced_improvement_num >0)
                        displaced_improvement = true;
                    if(!coalition_one && displaced_improvement_num > journeys_rerouted.size()/2)
                        displaced_improvement = true;

                    //did enough outsider journeys improve?
                    if(outsider_one && outsider_improvement_num > 0)
                        outside_improvement = true;
                    if(!outsider_one && outsider_improvement_num > (mainGraph.numJourneys() - journeys_rerouted.size())/2)
                        outside_improvement = true;
                    if(displaced_improvement && outside_improvement){
                        // then we keep the new journeys
                        if(debug){
                            output("Keeping the new paths");
                            output("New sad rat : " + str(sad_rat_num*1.0/sad_rat_den));
                            output("New paths:");

                            for(unsigned cur_journey = 0; cur_journey < mainGraph.numJourneys();
                                cur_journey++)
                                printJourney(mainGraph.getJourney(listOfJourneys[cur_journey].journeyNum()));
                        }
                    }
                    else{ // put the old paths back
                        if(debug){
                            output("No improvement- replacing old paths");
                        }
                        mainGraph.removeAllJourneys();
                        mainGraph.addJourneys(saved_paths);
                    }
                } // end "if >1 journey on this edge"
            }
        } // end "for all edges"
    } // end # of passes

    return mainGraph;
}
