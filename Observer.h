#pragma once

#include <iostream>
using namespace std;
//all the attributes of observers
typedef	struct{
		int time;
		int verdict;//0:false, 1:true, 2:maybe
		int time_stamp;
	}en;
string verdict_interprete(int num);

class Observer{

public:
	Observer();
	Observer(Observer *cob, Observer *pob);
	Observer(Observer *cob);
//	Observer(Observer *cob1, Observer *cob2, Observer *pob);
	bool is_new_event();
	bool is_new_event(en e);
	virtual void run(){};//algorithm 1,2,4
	virtual void run(en e){};//algorithm 1,2,4
	virtual void run(en e1, en e2){}; //algorithm 3
	virtual void check_new_event(int){};
	virtual ~Observer(){};
protected:


public:
	en out_node;//output of this observer
	en last_node_1;//store child_observer_1's output
	en last_node_2;//store child_observer_2's output
protected:
	Observer *child_observer_1;
	Observer *child_observer_2;
	Observer *parent_observer;

};

class Observer_type_1 : public Observer{
public:
//constructor
	Observer_type_1(Observer *cob, Observer *pob):Observer(cob,pob){}
	Observer_type_1(Observer *cob):Observer(cob){}
//override
	void run(){//child_node_1.out_node
		if(is_new_event()){
			out_node.verdict=(child_observer_1->out_node.verdict)==1?0:1;
			out_node.time_stamp=child_observer_1->out_node.time_stamp;

		}
	}
	~Observer_type_1(){};
};

//TODO
class Observer_type_2 : public Observer{};
class Observer_type_3 : public Observer{};
class Observer_type_4 : public Observer{};
class Observer_type_5 : public Observer{};

