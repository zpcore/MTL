#include "Formula.h"
#include <iostream>
#include <regex>
#include <stack>
#include "Event.h"
#include "Observer.h"
#include <string>
#include <unordered_map>

using namespace std;
/*
 Stack Data format:
 {SEN,sensor_label,{-1,-1}}
 {AND/NOT,observer_label,{-1,-1}}
 {REL/UNT/ALW/KEP,observer_label,{start,end}}
 */

Formula::Formula(string s,Observer** sensor,Observer** observer):s(s){
	delSpaces(s);//correct?? s=delSpaces(s);??
	int num=0;
	stack<seg_t> s1;
	unordered_map<int,seg_t[2]> hm;//an operator at most has two children, use the hashmap to find the children; positive means observer, negative means sensor
	int totSensor=0;
	int sensor_num;
	int observer_label=0;
	op_t op=ERR;
	interval_t interval;
	int i=0;
	while(i<s.length()){
		char c=s.at(i);
		if(c==','){
			i++;
			continue;
		}
	//check operator first
		if(get_type(i)==SEN){
			seg_t seg;
			seg.label=read_sensor_num(i);
			seg.interval.st=-1;
			seg.interval.ed=-1;
			s1.push(seg);
			while(s.at(i++)!=']'){};//skip the sensor segment
		}
		else if(get_type(i)!=ERR){
			observer_label++;//every operator has an observer label. The label is counting from 0
			seg_t seg;
			op=get_type(i);
			seg.op=op;
			seg.label=observer_label;
			i+=3;
			if(op!=NOT||op!=AND){//have interval
				interval=get_interval(i);
				while(s.at(i++)!=']'){};//skip the interval segment
				seg.interval=interval;
			}else{//no interval
				seg.interval.st=-1;//this operation does not have interval
				seg.interval.ed=-1;
			}
			s1.push(seg);
			i++;//skip '{'
		}
	//pop the operator
		if(c=='}'){
			seg_t ob=s1.top();
			if(hm[s1.size()][0].interval.st!=0) hm.at(s1.size())[0]=ob;//first time use this map key
			else hm.at(s1.size())[1]=ob;
			s1.pop();//remove that operator
			switch(ob.op){
			case SEN:
				break;
			case NOT:
				if(hm[s1.size()+1][0].op==SEN) observer[ob.label]=new Observer_type_1(sensor[hm[s1.size()+1][0].label]);
				else observer[ob.label]=new Observer_type_1(observer[hm[s1.size()+1][0].label]);
				break;
			case KEP:
				if(hm[s1.size()+1][0].op==SEN) observer[ob.label]=new Observer_type_2(sensor[hm[s1.size()+1][0].label],ob.interval.st);
				else observer[ob.label]=new Observer_type_2(observer[hm[s1.size()+1][0].label],ob.interval.st);
				break;
			case AND:
				if(hm[s1.size()+1][0].op==SEN){
					if(hm[s1.size()+1][1].op==SEN) observer[ob.label]=new Observer_type_3(sensor[hm[s1.size()+1][0].label],sensor[hm[s1.size()+1][1].label]);
					else observer[ob.label]=new Observer_type_3(sensor[hm[s1.size()+1][0].label],observer[hm[s1.size()+1][1].label]);
				}else{
					if(hm[s1.size()+1][1].op==SEN) observer[ob.label]=new Observer_type_3(observer[hm[s1.size()+1][0].label],sensor[hm[s1.size()+1][1].label]);
					else observer[ob.label]=new Observer_type_3(observer[hm[s1.size()+1][0].label],observer[hm[s1.size()+1][1].label]);
				}
				break;
			case ALW:
				if(hm[s1.size()+1][0].op==SEN) observer[ob.label]=new Observer_type_4(sensor[hm[s1.size()+1][0].label],ob.interval.st,ob.interval.ed);
				else observer[ob.label]=new Observer_type_4(observer[hm[s1.size()+1][0].label],ob.interval.st,ob.interval.ed);
				break;
			//TODO
			case UNT:
				break;
			//TODO
			case REL:
			}
		}
	}
}

int Formula::read_sensor_num(int pos){
	int start_pos=pos+1;
	while(s.at(pos++)!=']'){}
	return stoi(s.substr(start_pos,pos),nullptr);
}

//remove all the space in the string
string Formula::delSpaces(string &str){
	str.erase(remove(str.begin(), str.end(), ' '), str.end());
	return str;
}

op_t Formula::get_type(int pos){
	switch(s.at(pos)){
	case 'A':
		if(s.at(pos+1)=='N') return AND;
		else return ALW;
	case 'N':
		return NOT;
	case 'U':
		return UNT;
	case 'K':
		return KEP;
	case 'R':
		return REL;
	case 'S':
		return SEN;
	default:
		//cout<<"Wrong Operator"<<endl;
		return ERR;
	}
}

interval_t Formula::get_interval(int pos){//pos is the position of '['
	int loc_left_bracket=pos;
	int loc_right_bracket;
	bool hasComma=false;
	interval_t interval;
	while(s.at(pos++)!=']'){
		if(s.at(pos)==','){
			hasComma=true;
			loc_right_bracket=pos-1;
			interval.st = stoi(s.substr(loc_left_bracket,pos),nullptr);
		}
	}
	interval.ed = hasComma?stoi(s.substr(loc_right_bracket,pos),nullptr):-1;
	return interval;
}
