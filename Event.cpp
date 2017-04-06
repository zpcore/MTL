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
	if(!infile){cout<<"Cannot find file!"<<endl;}
	else{
		string line;
		while (getline(infile, line)){
			istringstream iss(line);
			int time, verdict,time_stamp;
			if (!(iss >> time >> verdict >> time_stamp)) { break; } // error
			en cur_node={time,verdict,time_stamp};
			event_list.push_back(cur_node);
		}
	}
}

void Event::check_new_event(int cur_time){
	if(!event_list.empty() && event_list.front().time==cur_time){
		en front=event_list.front();
		copy_en(out_node,front);
/*		cout << "Reading " << Event::filename << " @time " << out_node.time \
			<< ", verdict: " << verdict_interprete(out_node.verdict)\
			<< ", time stamp: "<<out_node.time_stamp<<endl;*/
		event_list.pop_front();
	}
}




