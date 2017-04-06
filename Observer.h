#pragma once

#include <iostream>
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
	bool is_new_event();
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
		if(is_new_event()){
			//update out_node
			int new_ver=(child_observer_1->out_node.verdict)==1?0:1;//type maybe not included
			copy_en(out_node,child_observer_1->out_node.time,new_ver,child_observer_1->out_node.time_stamp);
			copy_new_event();
		}
	}
};

class Observer_type_2 : public Observer{
public:
	~Observer_type_2(){}
	Observer_type_2(Observer *cob, int tau):Observer(cob),tau(tau){
		a=0;
		b=0;
		copy_en(temp_en,-1,0,0);
	}
	void run(){
		if(is_new_event()){
			copy_en(temp_en,child_observer_1->out_node);
			a=is_positive_edge_occur(last_node_1,child_observer_1->out_node)?b:a;
			b=child_observer_1->out_node.time_stamp+1;
			//cout<<a<<","<<b<<endl;
			if(temp_en.verdict==0){//if holds
				if(a<=temp_en.time_stamp-tau)
					temp_en.time_stamp-=tau;
			}
			copy_en(out_node,temp_en);
			copy_new_event();
		}
	}
private:
	int a,b,tau;
	en temp_en;
};

//TODO
class Observer_type_3 : public Observer{
public:
	Observer_type_3(Observer *cob1, Observer *cob2):Observer(cob1,cob2){}
	void run(){

	}
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

