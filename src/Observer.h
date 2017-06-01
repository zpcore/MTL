//============================================================================
// Name        : Observer.h
// Author      : Pei Zhang
// Institution : Iowa State University
// Description : Class of different observers
//============================================================================

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
			if(child_observer_1->out_node.verdict==1){
				if(counter>=tau)//output true
					copy_en(out_node,-1,1,child_observer_1->out_node.time_stamp-tau);
				else//output waiting
					copy_en(out_node,-1,-1,-1);
				counter++;
			}else{//output false
				counter=0;
				copy_en(out_node,-1,0,child_observer_1->out_node.time_stamp);
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

/*
//version 1 (conservative)
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
		if(q1.empty()&&q2.empty()){
			copy_en(out_node,-1,-1,-1);//keep waiting
		}else if(q1.empty()&&!q2.empty()){//q2 has element
			copy_en(out_node,-1,-1,-1);//keep waiting
		}else if(!q1.empty()&&q2.empty()){//q1 has element
			copy_en(out_node,-1,-1,-1);//keep waiting
		}else{//q1,q2 both have element, search through the queue to find the match time stamp
			en a=q1.front();
			en b=q2.front();
			q1.pop_front();
			q2.pop_front();
			if(a.time_stamp>b.time_stamp){//dequeue q2
				while(a.time_stamp>b.time_stamp&&!q2.empty()){
					q2.pop_front();
					b=q2.front();
				}
				if(a.time_stamp==b.time_stamp){
					if(a.verdict==1&&b.verdict==1) copy_en(out_node,-1,1,a.time_stamp);
					else copy_en(out_node,-1,0,a.time_stamp);
				}else{//time stamp in q1 and q2 cannot match, keep waiting
					if(!q2.empty()) q2.push_back(b);//the last element in q2 maybe useful in the future
					else q1.push_back(a);//a maybe useful in the future. So push it back
					copy_en(out_node,-1,-1,-1);
				}
			}else if(a.time_stamp<b.time_stamp){//dequeue q1
				while(a.time_stamp<b.time_stamp&&!q1.empty()){
					q1.pop_front();
					a=q1.front();
				}
				if(a.time_stamp==b.time_stamp){
					if(a.verdict==1&&b.verdict==1) copy_en(out_node,-1,1,a.time_stamp);
					else copy_en(out_node,-1,0,a.time_stamp);
				}else{//time stamp in q1 and q2 cannot match, keep waiting
					if(!q1.empty()) q1.push_back(a);//the last element in q2 maybe useful in the future
					else q2.push_back(b);//a maybe useful in the future. So push it back
					copy_en(out_node,-1,-1,-1);
				}
			}else{
				if(a.verdict==1&&b.verdict==1) copy_en(out_node,-1,1,a.time_stamp);
				else copy_en(out_node,-1,0,a.time_stamp);
			}
		}
	}
private:
	list<en> q1,q2;
};
*/

/*
//version 2
//This version follows with the Andreas Hagmann's master thesis and she software in GitHub.
//Exactly the same bug as c based software.
class Observer_type_3 : public Observer{
public:
	~Observer_type_3(){}
	Observer_type_3(Observer *cob1, Observer *cob2):Observer(cob1,cob2){}
	void run(){
		//aggregation
		//cout<<""<<endl;
		if(is_new_event_1()){
			if(!q1.empty()&&child_observer_1->out_node.verdict==q1.back().verdict) q1.pop_back();
			//cout<<"child_observer1 "<<verdict_interprete(child_observer_1->out_node.verdict)<<child_observer_1->out_node.time_stamp<<endl;
			q1.push_back(child_observer_1->out_node);
		}
		if(is_new_event_2()){
			if(!q2.empty()&&child_observer_2->out_node.verdict==q2.back().verdict) q2.pop_back();
			//cout<<"child_observer2 "<<verdict_interprete(child_observer_2->out_node.verdict)<<child_observer_2->out_node.time_stamp<<endl;
			q2.push_back(child_observer_2->out_node);
		}
		copy_en(out_node,-1,-1,-1);
		if(!q1.empty()&&!q2.empty()){
			if(q1.front().verdict==1&&q2.front().verdict==1){
				copy_en(out_node,-1,1,min(q1.front().time_stamp,q2.front().time_stamp));
			}
			else if(q1.front().verdict==0&&q2.front().verdict==0){
				copy_en(out_node,-1,0,max(q1.front().time_stamp,q2.front().time_stamp));
			}
			else if(q1.front().verdict==1&&q2.front().verdict==0){
				copy_en(out_node,-1,0,q2.front().time_stamp);
			}else{//q1.front().verdict==0&&q2.front().verdict==1
				copy_en(out_node,-1,0,q1.front().time_stamp);
			}
		}
		else if(!q2.empty()){//!q2.empty()&&q1.empty()
			if(q2.front().verdict==0){
				copy_en(out_node,-1,0,q2.front().time_stamp);
			}
		}
		else{//!q1.empty()&&q2.empty()
			if(q1.front().verdict==0){
				copy_en(out_node,-1,0,q1.front().time_stamp);
			}
		}
		//deque operation
		while(!q1.empty() && out_node.time_stamp!=-1 && q1.front().time_stamp<=out_node.time_stamp) q1.pop_front();
		while(!q2.empty() && out_node.time_stamp!=-1 && q2.front().time_stamp<=out_node.time_stamp) q2.pop_front();
		cout<<"------debug------"<<endl;
		cout<<"------q1------"<<endl;
		debug_queue(q1);
		cout<<"------q2------"<<endl;
		debug_queue(q2);
		cout<<"--------end--------"<<endl;
	}
private:
	list<en> q1,q2;
	void debug_queue(list<en>& q){
		int size=q.size();
		for(int i=0;i<size;i++){
			en temp=q.front();
			cout<<verdict_interprete(temp.verdict)<<" "<<temp.time_stamp<<endl;
			q.pop_front();
			q.push_back(temp);
		}
	}
};
*/


//version 3
//Revised version of version 2
class Observer_type_3 : public Observer{
public:
	~Observer_type_3(){}
	Observer_type_3(Observer *cob1, Observer *cob2):Observer(cob1,cob2){bound_q1=-1;bound_q2=-1;}
	void run(){
		//aggregation
		if(is_new_event_1()){
			if(!q1.empty()&&child_observer_1->out_node.verdict==q1.back().verdict) q1.pop_back();
			q1.push_back(child_observer_1->out_node);
		}
		if(is_new_event_2()){
			if(!q2.empty()&&child_observer_2->out_node.verdict==q2.back().verdict) q2.pop_back();
			q2.push_back(child_observer_2->out_node);
		}
		copy_en(out_node,-1,-1,-1);
		if(!q1.empty()&&!q2.empty()){
			if(q1.front().verdict==1&&q2.front().verdict==1){
				if((q1.front().time_stamp<q2.front().time_stamp&&bound_q2<q1.front().time_stamp)||\
						(q2.front().time_stamp<=q1.front().time_stamp&&bound_q1<q2.front().time_stamp)){//additional condition
					copy_en(out_node,-1,1,min(q1.front().time_stamp,q2.front().time_stamp));
				}
			}
			else if(q1.front().verdict==0&&q2.front().verdict==0){
				copy_en(out_node,-1,0,max(q1.front().time_stamp,q2.front().time_stamp));
			}
			else if(q1.front().verdict==1&&q2.front().verdict==0){
				copy_en(out_node,-1,0,q2.front().time_stamp);
			}else{//q1.front().verdict==0&&q2.front().verdict==1
				copy_en(out_node,-1,0,q1.front().time_stamp);
			}
		}
		else if(!q2.empty()){//!q2.empty()&&q1.empty()
			if(q2.front().verdict==0){
				copy_en(out_node,-1,0,q2.front().time_stamp);
			}
		}
		else{//!q1.empty()&&q2.empty()
			if(q1.front().verdict==0){
				copy_en(out_node,-1,0,q1.front().time_stamp);
			}
		}
		//deque operation
		while(!q1.empty() && out_node.time_stamp!=-1 && q1.front().time_stamp<=out_node.time_stamp) {
			bound_q1=q1.front().time_stamp;
			q1.pop_front();
		}
		while(!q2.empty() && out_node.time_stamp!=-1 && q2.front().time_stamp<=out_node.time_stamp) {
			bound_q2=q2.front().time_stamp;
			q2.pop_front();
		}
	}
private:
	list<en> q1,q2;
	int bound_q1,bound_q2;
};


/*
//version 4 (use revised aggregation, undone)
class Observer_type_3 : public Observer{
public:
	~Observer_type_3(){}
	Observer_type_3(Observer *cob1, Observer *cob2):Observer(cob1,cob2){}
	void run(){
		if(is_new_event_1()){
			aggregation(q1,true);
		}
		if(is_new_event_2()){
			aggregation(q2,false);
		}
		if(q1.empty()||q2.empty()){
			copy_en(out_node,-1,-1,-1);
		}else{
			enen q1_enen = q1.back();
			enen q2_enen = q2.back();
			if(q1_enen.time_stamp_st==q1_enen.time_stamp_ed){
				search_queue(q1,q2,q1_enen,q2_enen);
			}else{
				search_queue(q2,q1,q2_enen,q1_enen);
			}
		}
	}
private:
	typedef	struct{
			int verdict;//0:false, 1:true, 2:maybe; start interval
			int time_stamp_st;
			int time_stamp_ed;
		}enen;
	list<enen> q1,q2;
	//revised aggregation function
	void aggregation(list<enen>& q, bool is_child_observer_1){
		en child_observer;
		if(is_child_observer_1) copy_en(child_observer,child_observer_1->out_node);
		else copy_en(child_observer,child_observer_2->out_node);
		enen interval_en;
		if(!q.empty()&&q.front().verdict==child_observer.verdict&&q.front().time_stamp_ed+1==child_observer.time_stamp){
			interval_en.verdict=q.front().verdict;
			interval_en.time_stamp_st=q.front().time_stamp_st;
			interval_en.time_stamp_ed=q.front().time_stamp_ed+1;
			q.pop_front();
		}
		else{
			interval_en.verdict=child_observer.verdict;
			interval_en.time_stamp_st=child_observer.time_stamp;
			interval_en.time_stamp_ed=child_observer.time_stamp;
		}
		q.push_front(interval_en);
		//cout<<interval_en.time_stamp_st<<" "<<interval_en.time_stamp_ed<<endl;
	}
	void search_queue(list<enen>& q1, list<enen>& q2, enen q1_enen, enen q2_enen){
		while(!q2.empty()&&q2.back().time_stamp_ed<q1_enen.time_stamp_st) q2.pop_back();
		if(q2.empty()) copy_en(out_node,-1,-1,-1);
		else if(q2.back().time_stamp_st<=q1_enen.time_stamp_st){
			copy_en(out_node,-1,q1_enen.verdict&q2.back().verdict,q1_enen.time_stamp_st);
			enen q2_back_enen = q2.back();
			q2.pop_back();
			q2_back_enen.time_stamp_st=q1_enen.time_stamp_st;
			q2.push_back(q2_back_enen);
		}else{
			q1.pop_back();
			if(q1_enen.verdict==0) copy_en(out_node,-1,0,q1_enen.time_stamp_st);//Pei add this command to give more output from AND observer
			else copy_en(out_node,-1,-1,-1);
		}
	}

};
*/

class Observer_type_4 : public Observer{
public:
	~Observer_type_4(){}
	Observer_type_4(Observer *cob, int tau1, int tau2):Observer(cob),tau1(tau1),tau2(tau2),counter(0){}
	void run(){
		if(is_new_event_1()){
			int tau=tau2-tau1;
			int new_time_stamp=0;
			if(child_observer_1->out_node.verdict==1){
				if(counter>=tau&&(new_time_stamp=child_observer_1->out_node.time_stamp-tau-tau1)>0)//output true
					copy_en(out_node,-1,1,new_time_stamp);
				else//output waiting
					copy_en(out_node,-1,-1,-1);
				counter++;
			}else{//output false
				counter=0;
				if((new_time_stamp=child_observer_1->out_node.time_stamp-tau1)<0)
					copy_en(out_node,-1,-1,-1);
				else
					copy_en(out_node,-1,0,new_time_stamp);
			}
			copy_new_event();
		}
		else{
			copy_en(out_node,0,-1,-1);
		}
	}
private:
	int tau1,tau2,counter;
};

//TODO until
class Observer_type_5 : public Observer{};

//TODO release
class Observer_type_6 : public Observer{};

