#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <iostream>
#include "event.h"


//constructor
event::event(string filename): filename(filename){
	std::ifstream infile(filename);
	std::string line;
	while (std::getline(infile, line)){
	    std::istringstream iss(line);
	    int time, verdict,time_stamp;
	    if (!(iss >> time >> verdict >> time_stamp)) { break; } // error	
	    // process pair (time, verdict, time_stamp)
	    event_node cur_node={.time=time, .verdict=verdict, time_stamp=time_stamp};
	    event_list.push_back(cur_node);
	}
}

bool event::is_new_event(int cur_time){
	return !event_list.empty() && event_list.front().time==cur_time
}

event_node event::read_new_event(){
	event_node cur_node=event_list.front();
	std::cout << "Reading " << filename << " @" << cur_node.time \
	<< "ver: " << verdict_interprete(cur_node.verdict) << '\n';
	event_list.pop_front();
	return cur_node;
}

string event::verdict_interprete(int num){
	switch(num){
		case 0: return "TRUE";
		case 1: return "FALSE";
		case 2: return "MAYBE";
		default: return "ERROR";
	}
}