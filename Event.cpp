#include "Event.h"

#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <iostream>

using namespace std;

//constructor
Event::Event(string filename): filename(filename){
	ifstream infile(filename.c_str());
	string line;
	while (getline(infile, line)){
	    istringstream iss(line);
	    int time, verdict,time_stamp;
	    if (!(iss >> time >> verdict >> time_stamp)) { break; } // error	
	    // process pair (time, verdict, time_stamp)
	    en cur_node={.time=time, .verdict=verdict, .time_stamp=time_stamp};
	    //event_node cur_node={.time=1, .verdict=2, .time_stamp=3};
	    event_list.push_back(cur_node);
	}
}

bool Event::is_new_event(int cur_time){
	if(!event_list.empty() && event_list.front().time==cur_time){
		en front=event_list.front();
		out_node.time=front.time;
		out_node.verdict=front.verdict;
		out_node.time_stamp=front.time_stamp;
		cout << "Reading " << Event::filename << " @" << out_node.time \
			<< "ver: " << verdict_interprete(out_node.verdict) << '\n';
		event_list.pop_front();
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





