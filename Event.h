#pragma once

#include <vector>
#include <list>
#include <string>
#include "Observer.h"
using namespace std;



class Event: public Observer{

//class variable
public:
	string filename;
	en out_node;
private:
	list<en> event_list;
	vector<int> event_list2;
//class method
public:
	void check_new_event(int cur_time);
	Event(string filename);

private:


};
