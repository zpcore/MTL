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

private:
	list<en> event_list;

//class method
public:
	void check_new_event(int cur_time);
	Event(string filename);

private:


};