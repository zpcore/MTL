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


//TODO version 1 need check!!

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

/*
//TODO version 2 need check!!!
//This version follows with my hardware implementation
class Observer_type_3 : public Observer{
public:
	~Observer_type_3(){}
	Observer_type_3(Observer *cob1, Observer *cob2):Observer(cob1,cob2){}
	void run(){
		if(status.qfor==empty){
			if(is_new_event_1()&&is_new_event_2()){
				op1 = child_observer_1->out_node;
				op2 = child_observer_1->out_node;
				if(op1.time_stamp==op2.time_stamp){
					copy_en(out_node,-1,op1.verdict&op2.verdict,op1.time_stamp);
				}else if(op1.time_stamp>op2.time_stamp) {
					q.push_back(op1);
					status.qfor=Q1;
					//status.numOfTrueRemain=op1.verdict==1?status.numOfTrueRemain+1:status.numOfTrueRemain;
					if(op2.verdict==0) copy_en(out_node,-1,0,op2.time_stamp);
					else copy_en(out_node,-1,-1,-1);
				}else{
					q.push_back(op2);
					status.qfor=Q2;
					//status.numOfTrueRemain=op2.verdict==1?status.numOfTrueRemain+1:status.numOfTrueRemain;
					if(op1.verdict==0) copy_en(out_node,-1,0,op1.time_stamp);
					else copy_en(out_node,-1,-1,-1);
				}
			}else if(is_new_event_1()){
				op1 = child_observer_1->out_node;
				if(op1.verdict==1) {
					q.push_back(op1);
					status.qfor=Q1;
					//status.numOfTrueRemain=op1.verdict==1?status.numOfTrueRemain+1:status.numOfTrueRemain;
					copy_en(out_node,-1,-1,-1);
				}
				else{
					copy_en(out_node,-1,0,op1.time_stamp);
				}
			}else if(is_new_event_2()){
				op2 = child_observer_2->out_node;
				if(op2.verdict==1) {
					q.push_back(op2);
					status.qfor=Q2;
					//status.numOfTrueRemain=op2.verdict==1?status.numOfTrueRemain+1:status.numOfTrueRemain;
					copy_en(out_node,-1,-1,-1);
				}
				else{
					copy_en(out_node,-1,0,op2.time_stamp);
				}
			}
		}else if(status.qfor==Q1){
			if(is_new_event_2()){
				if(is_new_event_1()) q.push_back(child_observer_1->out_node);
				op2 = child_observer_2->out_node;
				while(!q.empty()){
					if(q.front().time_stamp<op2.time_stamp){
						q.pop_front();
					}
				}
				if(!q.empty()){
					if(q.front().time_stamp==op2.time_stamp){
						copy_en(out_node,-1,q.front().verdict&op2.verdict,op2.time_stamp);
					}else{//q.front().time_stamp > op2.time_stamp
						while(!q.empty()&&q.front().verdict==0){//dequeue
							lastEn=q.front();
							q.pop_front();
						}
						if(q.empty()){
							if(lastEn.verdict==1) q.push_back(lastEn);
							else{
								copy_en(out_node,lastEn);
								status.qfor=empty;
							}
						}
					}
				}else{//all en in q with time_stamp < op2
					if(op2.verdict==1){
						q.push_back(op2);
						status.qfor=Q2;
					}else{
						copy_en(out_node,op2);
						status.qfor=empty;
					}
				}
			}else if(is_new_event_1()){
				op1 = child_observer_1->out_node;
				q.push_back(op1);
				while(!q.empty()&&q.front().verdict==0){
					lastEn=q.front();
					q.pop_front();
				}
				if(q.empty()){
					if(lastEn.verdict==0){
						copy_en(out_node,lastEn);
						status.qfor=empty;
					}else{//dequeue process blocked by a true verdict
						copy_en(out_node,-1,-1,-1);//should I output the node before lastEn??
					}
				}
			}
		}else{//status.qfor==Q2
			if(is_new_event_1()){
				if(is_new_event_2()) q.push_back(child_observer_2->out_node);
				op1 = child_observer_1->out_node;
				while(!q.empty()){
					if(q.front().time_stamp<op1.time_stamp){
						q.pop_front();
					}
				}
				if(!q.empty()){
					if(q.front().time_stamp==op1.time_stamp){
						copy_en(out_node,-1,q.front().verdict&op1.verdict,op1.time_stamp);
					}else{//q.front().time_stamp > op2.time_stamp
						while(!q.empty()&&q.front().verdict==0){//dequeue
							lastEn=q.front();
							q.pop_front();
						}
						if(q.empty()){
							if(lastEn.verdict==1) q.push_back(lastEn);
							else{
								copy_en(out_node,lastEn);
								status.qfor=empty;
							}
						}
					}
				}else{//all en in q with time_stamp < op2
					if(op1.verdict==1){
						q.push_back(op1);
						status.qfor=Q1;
					}else{
						copy_en(out_node,op1);
						status.qfor=empty;
					}
				}
			}else if(is_new_event_2()){
				op2 = child_observer_2->out_node;
				q.push_back(op2);
				while(!q.empty()&&q.front().verdict==0){
					lastEn=q.front();
					q.pop_front();
				}
				if(q.empty()){
					if(lastEn.verdict==0){
						copy_en(out_node,lastEn);
						status.qfor=empty;
					}else{//dequeue process blocked by a true verdict
						copy_en(out_node,-1,-1,-1);//should I output the node before lastEn??
					}
				}
			}
		}
	}
private:
	en op1,op2,lastEn;
	list<en> q;
	enum qFor {Q1,Q2,empty};
	struct{
		int numOfTrueRemain=0;
		qFor qfor=empty;
	}status;
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

