#pragma once

#include <iostream>
#include <list>
using namespace std;
//all the attributes of observers
typedef	struct{
		int time;
		int verdict;//0:false, 1:true, 2:maybe
		int time_stamp;
	}en;
string verdict_interprete(int);
void copy_en(en&,int,int,int);
void copy_en(en &a,en &b);//a=b

class Observer{

public:
	Observer();
	Observer(Observer *cob1, Observer *cob2);
	Observer(Observer *cob);
	bool is_new_event_1();
	bool is_new_event_2();
	void copy_new_event();
	virtual void run(){};
	virtual void check_new_event(int){};
	virtual ~Observer(){};
protected:
	bool is_positive_edge_occur(en pre,en pos);
	bool is_negative_edge_occur(en pre,en pos);

public:
	en out_node;//output of this observer
	en last_node_1;//store child_observer_1's output
	en last_node_2;//store child_observer_2's output
protected:
	Observer *child_observer_1;
	Observer *child_observer_2;
};

class Observer_type_1 : public Observer{
public:
	~Observer_type_1(){}
//constructor
	Observer_type_1(Observer *cob):Observer(cob){}
//override
	void run(){//child_node_1.out_node
		if(is_new_event_1()){
			//update out_node
			int new_ver=(child_observer_1->out_node.verdict)==1?0:1;//type maybe not included
			copy_en(out_node,child_observer_1->out_node.time,new_ver,child_observer_1->out_node.time_stamp);
			copy_new_event();
		}else{
			copy_en(out_node,0,-1,-1);
		}
	}
};

class Observer_type_2 : public Observer{
public:
	~Observer_type_2(){}
	Observer_type_2(Observer *cob, int tau):Observer(cob),tau(tau),counter(0){}
	void run(){
		if(is_new_event_1()){
			if(child_observer_1->out_node.verdict==0){
				if(counter>=tau)//output true
					copy_en(out_node,-1,0,child_observer_1->out_node.time_stamp-tau);
				else//output waiting
					copy_en(out_node,-1,-1,-1);
				counter++;
			}else{//output false
				counter=0;
				copy_en(out_node,-1,1,child_observer_1->out_node.time_stamp);
			}
			copy_new_event();
		}
		else{
			copy_en(out_node,0,-1,-1);
		}
	}
private:
	int tau,counter;
};

//TODO
class Observer_type_3 : public Observer{
public:
	~Observer_type_3(){}
	Observer_type_3(Observer *cob1, Observer *cob2):Observer(cob1,cob2){}
	void run(){
		if(is_new_event_1()){
			q1.push_back(child_observer_1->out_node);
		}
		if(is_new_event_2()){
			q2.push_back(child_observer_2->out_node);
		}

		if(q1.empty()||q2.empty())
		{
			copy_en(out_node,0,-1,-1);
		}else{
			if(!q1.empty()&&!q2.empty()){
				en a=q1.front();
				en b=q2.front();
				q1.pop_front();
				q2.pop_front();
				if(a.verdict==0&&b.verdict==0) copy_en(out_node,0,0,a.time_stamp);
				else copy_en(out_node,0,1,a.time_stamp);
				if(a.time_stamp<=b.time_stamp){
					q1.clear();
				}else if(a.time_stamp>=b.time_stamp){
					q2.clear();
				}
			}else{
				copy_en(out_node,-1,-1,-1);
			}
		}
	}
private:
	list<en> q1,q2;
};

//TODO
class Observer_type_4 : public Observer{
public:
	Observer_type_4(Observer *cob):Observer(cob){}
	void run(){

	}
};
//TODO
class Observer_type_5 : public Observer{};

