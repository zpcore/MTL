//all the attributes of observers
#include "Observer.h"
#include <iostream>
using namespace std;
//constructor
Observer::Observer(){};

Observer::Observer(Observer *cob, Observer *pob):child_observer_1(cob),parent_observer(pob){
	last_node_1.time=-1;
	last_node_1.verdict=0;
	last_node_1.time_stamp=0;
}

/*constructor for bottom layer observer*/
Observer::Observer(Observer *cob):child_observer_1(cob){
	last_node_1.time=-1;
	last_node_1.verdict=0;
	last_node_1.time_stamp=-1;
	cout<<"Observer Initiated"<<endl;
}


bool Observer::is_new_event(){//check new input
	if(last_node_1.verdict!=child_observer_1->out_node.verdict || \
			last_node_1.time_stamp!=child_observer_1->out_node.time_stamp){
		//copy the new event to last_node
		last_node_1.time=child_observer_1->out_node.time;
		last_node_1.verdict=child_observer_1->out_node.verdict;
		last_node_1.time_stamp=child_observer_1->out_node.time_stamp;
		return true;
	}
	return false;
}

string verdict_interprete(int num){
	switch(num){
		case 0: return "TRUE";
		case 1: return "FALSE";
		case 2: return "MAYBE";
		default: return "ERROR";
	}
}
/*
bool Observer::is_new_event(en e){//check new input
	if(last_node_1.time!=e.time){
		last_node_1.time=e.time;
		last_node_1.verdict=e.verdict;
		last_node_1.time_stamp=e.time_stamp;
		return true;
	}
	return false;
}
*/
//TODO
/*
bool Observer::is_new_event(event_node e1, event_node e2){//check new input
	return last_node_1.time!=e.time||last_node_2.time!=e2.time;
}
*/
