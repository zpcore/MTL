#pragma once

#include "Event.h"
#include <iostream>
using namespace std;
//all the attributes of observers
class Observer{

public:
	Observer();
	Observer(Observer *cob, Observer *pob);
	Observer(Event *ce1);
//	Observer(Observer *cob1, Observer *cob2, Observer *pob);
	bool is_new_event();
	bool is_new_event(en e);
	virtual void run(){};//algorithm 1,2,4
	virtual void run(en e){};//algorithm 1,2,4
	virtual void run(en e1, en e2){}; //algorithm 3
	virtual ~Observer(){};
protected:


public:
	en out_node;//output of this observer
	en last_node_1;//store child_observer_1's output
	en last_node_2;//store child_observer_2's output
protected:
	Observer *child_observer_1=NULL;
	Observer *child_observer_2=NULL;
	Observer *parent_observer=NULL;
	Event *child_event_1=NULL;
	Event *child_event_2=NULL;

};

class Observer_type_1 : public Observer{
public:
//constructor
	Observer_type_1(Observer *cob, Observer *pob):Observer(cob,pob){}
	Observer_type_1(Event *ce1):Observer(ce1){}
//override
	void run(){//child_node_1.out_node
		cout << "!!!Hello" << endl;
		if(is_new_event()){

			out_node.verdict=(child_event_1->out_node.verdict)==1?0:1;
			out_node.time_stamp=child_event_1->out_node.time_stamp;

		}
	}
	~Observer_type_1(){};
};

//TODO
class Observer_type_2 : public Observer{};
class Observer_type_3 : public Observer{};
class Observer_type_4 : public Observer{};
class Observer_type_5 : public Observer{};

