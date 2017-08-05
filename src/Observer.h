//============================================================================
// Name        : Observer.h
// Author      : Pei Zhang
// Institution : Iowa State University
// Description : Class of different observers
//============================================================================

#pragma once

#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include <string>
#include "CircularBuffer.h"
#include "common.h"

using namespace std;
//all the attributes of observers

string verdict_interprete(int);
void copy_en(en&,int,int);
void copy_en(en &,en &);//a=b

class Observer{

public:
	Observer();
	Observer(Observer *cob1, Observer *cob2);
	Observer(Observer *cob);
	//Observer(Observer *cob, int);
	//Observer(Observer *cob1, Observer *cob2, int);
	bool is_new_event_1();
	bool is_new_event_2();
	void copy_new_event();
	virtual void run(){};
	virtual void dprint1(){};
	virtual void dprint2(){};
	virtual bool read_buffer(){return false;};
	virtual void check_new_event(int){};
	virtual ~Observer(){};
	CircularBuffer *cb=new CircularBuffer();
protected:
	bool is_positive_edge_occur(en pre,en pos);
	bool is_negative_edge_occur(en pre,en pos);


public:
	en out_node;//output of this observer
	en last_node_1;//store child_observer_1's output
	en last_node_2;//store child_observer_2's output
protected:
	Observer *child_observer_1=0;
	Observer *child_observer_2=0;
	bool hasPrint=false;
	int rdPtr=0,rdPtr2=0;
	en read_data={0,0};
	int last_tau=-1;
	en result={0,0};
};


class Event: public Observer{
public:
	Event(string filename){
		cb->isBottom=true;
		ifstream infile(filename.c_str());
		if(!infile){cout<<"Cannot find file!"<<endl;}
		else{
			string line;
			while (getline(infile, line)){
				istringstream iss(line);
				int verdict,time_stamp;
				if (!(iss >> verdict>> time_stamp )) { break; } // error
				en cur_node={verdict,time_stamp};
				cb->write(cur_node);
			}
		}
	}
};

//END (for assembly only)
class Observer_type_end : public Observer{
public:
	int pc=0;
	~Observer_type_end(){}
	Observer_type_end(Observer *cob):Observer(cob){}
	Observer_type_end(Observer *cob, int pc):Observer(cob),pc(pc){}
	bool read_buffer(){
		read_data=child_observer_1->cb->read(rdPtr);
		return read_data.time_stamp>last_tau;
	}
	void dprint1(){
		if(result.time_stamp!=-1){
			hasPrint=true;
			cout<<"END PC:"<<pc<<" = ("<<read_data.verdict<<","<<read_data.time_stamp<<")"<<endl;
		}
	}
	void dprint2(){
		if(!hasPrint) cout<<"END PC:"<<pc<<" = (-,-)"<<endl;
	}
	void run(){
		hasPrint=false;
		result={0,-1};
		while(!child_observer_1->cb->isEmpty(rdPtr)){
			if(read_buffer()){
				result=read_data;
				cb->write(result);
			}
			dprint1();
			rdPtr++;
			last_tau=result.time_stamp==-1?last_tau:result.time_stamp;
		}
		dprint2();
		if(child_observer_1->cb->recedPtr(rdPtr)) rdPtr--;
	}
};

//LOAD (for assembly only)
class Observer_type_0 : public Observer{
public:
	int pc=0;
	~Observer_type_0(){}
//constructor
	Observer_type_0(Observer *cob):Observer(cob){}
	Observer_type_0(Observer *cob, int pc):Observer(cob),pc(pc){}
//override
	void dprint1(){
		cout<<"LOAD PC:"<<pc<<" = ("<<result.verdict<<","<<result.time_stamp<<")"<<endl;
	}
	void run(){//child_node_1.out_node
		result=child_observer_1->cb->read(rdPtr);;
		cb->write(result);
		dprint1();
	}
};

//NOT
class Observer_type_1 : public Observer{
public:
	int pc=0;
	~Observer_type_1(){}
//constructor
	Observer_type_1(Observer *cob):Observer(cob){}
	Observer_type_1(Observer *cob, int pc):Observer(cob),pc(pc){}
//override
	bool read_buffer(){
		read_data=child_observer_1->cb->read(rdPtr);
		return read_data.time_stamp>last_tau;
	}
	void dprint1(){
		if(result.time_stamp!=-1){
			hasPrint=true;
			cout<<"NOT PC:"<<pc<<" = ("<<result.verdict<<","<<result.time_stamp<<")"<<endl;
		}
	}
	void dprint2(){
		if(!hasPrint) cout<<"NOT PC:"<<pc<<" = (-,-)"<<endl;
	}
	void run(){//child_node_1.out_node
		hasPrint=false;
		result={0,-1};
		while(!child_observer_1->cb->isEmpty(rdPtr)){
			if(read_buffer()){
				result={!read_data.verdict,read_data.time_stamp};
				cb->write(result);
			}
			dprint1();
			last_tau=result.time_stamp==-1?last_tau:result.time_stamp;
			rdPtr++;
		}
		dprint2();
		if(child_observer_1->cb->recedPtr(rdPtr)) rdPtr--;
	}
};

//KEP
class Observer_type_2 : public Observer{
public:
	~Observer_type_2(){}
	Observer_type_2(Observer *cob, int tau):Observer(cob),tau(tau){}
	Observer_type_2(Observer *cob, int tau, int pc):Observer(cob),pc(pc),tau(tau),counter(0){}
	bool read_buffer(){
		read_data=child_observer_1->cb->read(rdPtr);
		return read_data.time_stamp>last_tau;
	}
	void dprint(){
		if(result.time_stamp==-1){
			if(!hasPrint&&read_buffer()) cout<<"G["<<tau<<"] PC:"<<pc<<" = (-,-)"<<endl;
		}else{
			hasPrint=true;
			cout<<"G["<<tau<<"] PC:"<<pc<<" = ("<<result.verdict<<","<<result.time_stamp<<")"<<endl;
		}
	}
	void run(){
		hasPrint=false;
		while(!child_observer_1->cb->isEmpty(rdPtr)){
			result={0,-1};
			if(read_buffer()){
				if(read_data.verdict==1){
					if(counter>=tau){
						result={1,read_data.time_stamp-tau};
						cb->write(result);
					}
					counter++;
				}else{
					counter=0;
					result={0,read_data.time_stamp};
					cb->write(result);
				}
			}
			dprint();
			last_tau=read_data.time_stamp;
			rdPtr++;
		}
		if(child_observer_1->cb->recedPtr(rdPtr)) rdPtr--;
	}
//	void run(){
//		if(is_new_event_1()){
//			if(child_observer_1->out_node.verdict==1){
//				if(counter>=tau)//output true
//					copy_en(out_node,1,child_observer_1->out_node.time_stamp-tau);
//				else//output waiting
//					copy_en(out_node,-1,-1);
//				counter++;
//			}else{//output false
//				counter=0;
//				copy_en(out_node,0,child_observer_1->out_node.time_stamp);
//			}
//			copy_new_event();
//		}
//		else{
//			copy_en(out_node,-1,-1);
//		}
//	}
private:
	int pc=0,tau,counter=0;
};

//AND
class Observer_type_3 : public Observer{
public:
	int pc=0;
	en read_data2={0,0};
	~Observer_type_3(){}
	Observer_type_3(Observer *cob1, Observer *cob2):Observer(cob1,cob2){}
	Observer_type_3(Observer *cob1, Observer *cob2, int pc):Observer(cob1,cob2),pc(pc){}
	bool read_buffer(){
		if(child_observer_1->cb->isEmpty(rdPtr)) return false;
		read_data=child_observer_1->cb->read(rdPtr);
		return read_data.time_stamp>last_tau;
	}
	bool read_buffer2(){
		if(child_observer_2->cb->isEmpty(rdPtr2)) return false;
		read_data=child_observer_2->cb->read(rdPtr2);
		return read_data.time_stamp>last_tau;
	}
	void dprint1(){
		if(result.time_stamp!=-1){
			hasPrint=true;
			cout<<"AND PC:"<<pc<<" = ("<<result.verdict<<","<<result.time_stamp<<")"<<endl;
		}
	}
	void dprint2(){
		if(!hasPrint) cout<<"AND PC:"<<pc<<" = (-,-)"<<endl;
	}

	void run(){
		result={0,-1};
		hasPrint=false;

		while(!child_observer_1->cb->isEmpty(rdPtr)||!child_observer_2->cb->isEmpty(rdPtr2)){

			while(!child_observer_1->cb->isEmpty(rdPtr)&&!read_buffer()) rdPtr++;
			while(!child_observer_2->cb->isEmpty(rdPtr2)&&!read_buffer2()) rdPtr2++;
			cout<<"ptr1"<<rdPtr<<endl;
						cout<<"ptr2"<<rdPtr2<<endl;
			if(!child_observer_1->cb->isEmpty(rdPtr)&&!child_observer_2->cb->isEmpty(rdPtr2)){
				if(read_data.verdict==1&&read_data2.verdict==1){
					if((read_data.time_stamp<read_data2.time_stamp&&read_buffer())||\
							(read_data2.time_stamp<=read_data.time_stamp&&read_buffer2()))
						result={1,min(read_data.time_stamp,read_data2.time_stamp)};
				}else if(read_data.verdict==0&&read_data2.verdict==0){
					result={0,max(read_data.time_stamp,read_data2.time_stamp)};
				}else if(read_data.verdict==1&&read_data2.verdict==0){
					result={0,read_data2.time_stamp};
				}else{
					result={0,read_data.time_stamp};
				}
			}
			else if(!child_observer_2->cb->isEmpty(rdPtr2)){//!q2.empty()&&q1.empty()
				if(read_data2.verdict==0){
					result={0,read_data2.time_stamp};
				}
			}
			else if(!child_observer_1->cb->isEmpty(rdPtr)&&child_observer_2->cb->isEmpty(rdPtr2)){
				if(read_data.verdict==0){
					result={0,read_data.time_stamp};
				}
			}
			dprint1();
			if(result.time_stamp!=-1){
				last_tau=result.time_stamp;
				cb->write(result);
			}
		}
		cout<<"tau"<<last_tau<<endl;
		dprint2();
		if(child_observer_1->cb->recedPtr(rdPtr)) rdPtr--;
		if(child_observer_2->cb->recedPtr(rdPtr2)) rdPtr2--;
	}
};
//class Observer_type_3 : public Observer{
//public:
//	~Observer_type_3(){}
//	Observer_type_3(Observer *cob1, Observer *cob2):Observer(cob1,cob2){tau=-1;}
//	void run(){
//		//aggregation
//		if(is_new_event_1()){
//			if(!q1.empty()&&child_observer_1->out_node.verdict==q1.back().verdict) q1.pop_back();
//			q1.push_back(child_observer_1->out_node);
//		}
//		if(is_new_event_2()){
//			if(!q2.empty()&&child_observer_2->out_node.verdict==q2.back().verdict) q2.pop_back();
//			q2.push_back(child_observer_2->out_node);
//		}
//
//		//deque operation
//		while(!q1.empty() && q1.front().time_stamp<=tau) q1.pop_front();
//		while(!q2.empty() && q2.front().time_stamp<=tau) q2.pop_front();
//
//		copy_en(out_node,-1,-1);
//		if(!q1.empty()&&!q2.empty()){
//			if(q1.front().verdict==1&&q2.front().verdict==1){
//				if((q1.front().time_stamp<q2.front().time_stamp&&tau<q1.front().time_stamp)||\
//						(q2.front().time_stamp<=q1.front().time_stamp&&tau<q2.front().time_stamp)){//additional condition
//					copy_en(out_node,1,min(q1.front().time_stamp,q2.front().time_stamp));
//				}
//			}
//			else if(q1.front().verdict==0&&q2.front().verdict==0){
//				copy_en(out_node,0,max(q1.front().time_stamp,q2.front().time_stamp));
//			}
//			else if(q1.front().verdict==1&&q2.front().verdict==0){
//				copy_en(out_node,0,q2.front().time_stamp);
//			}else{//q1.front().verdict==0&&q2.front().verdict==1
//				copy_en(out_node,0,q1.front().time_stamp);
//			}
//		}
//		else if(!q2.empty()){//!q2.empty()&&q1.empty()
//			if(q2.front().verdict==0){
//				copy_en(out_node,0,q2.front().time_stamp);
//			}
//		}
//		else if(!q1.empty()&&q2.empty()){
//			if(q1.front().verdict==0){
//				copy_en(out_node,0,q1.front().time_stamp);
//			}
//		}
//		if(out_node.time_stamp!=-1)	tau=out_node.time_stamp;
//	}
//private:
//	list<en> q1,q2;
//	int tau;
//	void debug_queue(list<en>& q){
//		int size=q.size();
//		for(int i=0;i<size;i++){
//			en temp=q.front();
//			cout<<verdict_interprete(temp.verdict)<<" "<<temp.time_stamp<<endl;
//			q.pop_front();
//			q.push_back(temp);
//		}
//	}
//};

//ALW
class Observer_type_4 : public Observer{
public:
	int pc=0;
	~Observer_type_4(){}
	Observer_type_4(Observer *cob, int tau1, int tau2):Observer(cob),tau1(tau1),tau2(tau2),counter(0){}
	Observer_type_4(Observer *cob, int tau1, int tau2, int pc):Observer(cob),tau1(tau1),tau2(tau2),counter(0),pc(pc){}
	void run(){
		if(is_new_event_1()){
			int tau=tau2-tau1;
			int new_time_stamp=0;
			if(child_observer_1->out_node.verdict==1){
				if(counter>=tau&&(new_time_stamp=child_observer_1->out_node.time_stamp-tau2)>=0)//output true
					copy_en(out_node,1,new_time_stamp);
				else//output waiting
					copy_en(out_node,-1,-1);
				counter++;
			}else{//output false
				counter=0;
				if((new_time_stamp=child_observer_1->out_node.time_stamp-tau1)<0)
					copy_en(out_node,-1,-1);
				else
					copy_en(out_node,0,new_time_stamp);
			}
			copy_new_event();
		}
		else{
			copy_en(out_node,-1,-1);
		}
	}
private:
	int tau1,tau2,counter;
};

//TODO until
class Observer_type_5 : public Observer{};

//TODO release
class Observer_type_6 : public Observer{};

