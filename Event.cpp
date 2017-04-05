#include "Event.h"
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <iostream>

using namespace std;

//constructor
Event::Event(string filename): filename(filename){
	ifstream infile(filename);
	if(!infile){cout<<"Cannot find file!"<<endl;}
	else{
		string line;
		cout<<filename<<endl;
		int j=0;
		while (getline(infile, line)){
			cout<<line<<endl;
			istringstream iss(line);
			int time, verdict,time_stamp;
			if (!(iss >> time >> verdict >> time_stamp)) { break; } // error
			// process pair (time, verdict, time_stamp)
			en cur_node;
			cur_node.time=time;
			event_list.push_back(cur_node);
			event_list2.push_back(j++);
		}
		for(int i=0;i<event_list.size();i++)
			cout<<event_list2.back()<<endl;
			event_list2.erase();
	}
}
void Event::check_new_event(int cur_time){
	if(!event_list.empty() && event_list.front().time==cur_time){
		en front=event_list.front();
		out_node.time=front.time;
		out_node.verdict=front.verdict;
		out_node.time_stamp=front.time_stamp;
		cout << "Reading " << Event::filename << " @time " << out_node.time \
			<< ",	verdict: " << verdict_interprete(out_node.verdict) << '\n';
		event_list.pop_front();
	}
}




