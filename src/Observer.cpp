//all the attributes of observers
#include "Observer.h"
#include <iostream>
using namespace std;
//constructor
Observer::Observer():child_observer_1(NULL),child_observer_2(NULL){};

Observer::Observer(Observer *cob1, Observer *cob2):child_observer_1(cob1),child_observer_2(cob2){
	copy_en(last_node_1,-1,2,-1);
	copy_en(out_node,-1,2,-1);
}

/*constructor for bottom layer observer*/
Observer::Observer(Observer *cob):child_observer_1(cob),child_observer_2(NULL){
	child_observer_1=cob;
	copy_en(last_node_1,-1,2,-1);
	copy_en(out_node,-1,2,-1);
}


bool Observer::is_new_event_1(){//check new input
	return child_observer_1->out_node.verdict!=-1;
}
bool Observer::is_new_event_2(){//check new input
	return child_observer_2->out_node.verdict!=-1;
}

void Observer::copy_new_event(){//update last_node
	copy_en(last_node_1,child_observer_1->out_node);
	//copy_en(last_node_2,child_observer_2->out_node);
}

bool Observer::is_positive_edge_occur(en pre, en pos){
	return pos.verdict==1&&pre.verdict==0;
}

bool Observer::is_negative_edge_occur(en pre, en pos){
	return pre.verdict==1&&pos.verdict==0;
}


//support functions
void copy_en(en &a,int time,int verdict,int time_stamp){
	a.time=time;
	a.verdict=verdict;
	a.time_stamp=time_stamp;
}

void copy_en(en &a,en &b){
	a.time=b.time;
	a.verdict=b.verdict;
	a.time_stamp=b.time_stamp;
}

string verdict_interprete(int num){
	switch(num){
		case -1: return "_____";
 		case 0: return "FALSE";
		case 1: return "TRUE ";
		case 2: return "MAYBE";
		default: return "ERROR";
	}
}

