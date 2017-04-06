//all the attributes of observers
#include "Observer.h"
#include <iostream>
using namespace std;
//constructor
Observer::Observer():child_observer_1(NULL),child_observer_2(NULL){};

Observer::Observer(Observer *cob1, Observer *cob2):child_observer_1(cob1),child_observer_2(cob2){
	last_node_1.time=-1;
	last_node_1.verdict=2;
	last_node_1.time_stamp=-1;
}

/*constructor for bottom layer observer*/
Observer::Observer(Observer *cob):child_observer_1(cob),child_observer_2(NULL){
	child_observer_1=cob;
	//last_node_1={-1,0,-1};//not support
	last_node_1.time=-1;
	last_node_1.verdict=2;
	last_node_1.time_stamp=-1;
}


bool Observer::is_new_event(){//check new input
	return last_node_1.verdict!=child_observer_1->out_node.verdict || \
			last_node_1.time_stamp!=child_observer_1->out_node.time_stamp;
}

void Observer::copy_new_event(){//update last_node
	copy_en(last_node_1,child_observer_1->out_node);
	/*
	last_node_2.time=child_observer_2->out_node.time;
	last_node_2.verdict=child_observer_2->out_node.verdict;
	last_node_2.time_stamp=child_observer_2->out_node.time_stamp;
	*/
}

bool Observer::is_positive_edge_occur(en pre, en pos){
	return pos.time_stamp==1&&pre.time_stamp==0;
}

bool Observer::is_negative_edge_occur(en pre, en pos){
	return pre.time_stamp==1&&pos.time_stamp==0;
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
		case 0: return "TRUE";
		case 1: return "FALSE";
		case 2: return "MAYBE";
		default: return "ERROR";
	}
}

