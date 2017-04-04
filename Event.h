#pragma once

#include <list>
#include <string>

using namespace std;

typedef	struct{
		int time;
		int verdict;//0:false, 1:true, 2:maybe
		int time_stamp;
	}en;
string verdict_interprete(int num);

class Event{

//class variable
public:
	string filename;
	en out_node;
private:
	list<en> event_list;

//class method
public:
	Event(string filename);
	bool is_new_event(int cur_time);

private:


};
