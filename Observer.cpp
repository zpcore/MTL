//all the attributes of observers
#include "Observer.h"

void Observer::init(Observer cob, Observer pob):\
child_observer_1(cob),parent_observer(pob){
	last_node_1={.time=-1, .verdict=0, time_stamp=0};
}

void Observer::init(Observer cob1, Observer cob2, Observer pob):\
child_observer_1(cob1),child_observer_2(ob2),parent_observer(pob){
	last_node_1={.time=-1, .verdict=0, time_stamp=0};
	last_node_2={.time=-1, .verdict=0, time_stamp=0};
}

bool Observer::is_new_event(event_node e){//check new input
	if(last_node.time!=e.time){
		last_node.time=e.time;
		last_node.verdict=e.verdict;
		last_node.time_stamp=e.time_stamp;
		return true;
	}
	return false;
}

bool Observer::is_new_event(event_node e1, event_node e2){//check new input
//TODO
	return last_node_1.time!=e.time||last_node_2.time!=e2.time;
}



