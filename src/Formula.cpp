/*
 Stack Data format:
 {SEN,sensor_label,{-1,-1}}
 {AND/NOT,observer_label,{-1,-1}}
 {REL/UNT/ALW/KEP,observer_label,{start,end}}
*/

#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
#include "Event.h"
#include "Observer.h"
#include "Formula.h"

using namespace std;

//constructor
Formula::Formula(string str,Observer** sensor,Observer** observer):s(str){
	delSpaces(s);
	for (auto & c: s) c = toupper(c);
	stack<seg_t> s1;
	unordered_map<int, vector<seg_t> > hm;//an operator at most has two children, use the hashmap to find the children; positive means observer, negative means sensor
	int observer_label=0;
	op_t op=ERR;
	interval_t interval;
	int i=0;
	while(i<s.length()){
		//char c=s.at(i);
		if(s.at(i)==','){
			i++;
			continue;
		}
	//check operator first
		if(get_type(i)==SEN){
			seg_t seg;
			seg.op=SEN;
			seg.label=read_sensor_num(i);
			seg.interval.st=-1;
			seg.interval.ed=-1;
			s1.push(seg);
			while(s.at(i)!=']'){i++;};//skip the sensor segment
		}
		else if(get_type(i)!=ERR){
			seg_t seg;
			op=get_type(i);
			seg.op=op;
			seg.label=observer_label;
			i+=3;
			if(!(op==NOT||op==AND)){//have interval
				interval=get_interval(i);
				while(s.at(i++)!=']'){};//skip the interval segment
				seg.interval=interval;
			}else{//no interval
				seg.interval.st=-1;//this operation does not have interval
				seg.interval.ed=-1;
			}
			observer_label++;//every operator has an observer label. The label is counting from 0
			s1.push(seg);
			i++;//skip '{'
		}
	//pop the operator
		if(s.at(i)=='}'||s.at(i)==']'){//']'for pop sensor op
			i++;
			seg_t ob;
			ob=s1.top();
			hm[s1.size()].push_back(ob);
			switch(ob.op){
			case SEN:
				break;
			case NOT:
				if(hm[s1.size()+1].at(0).op==SEN) {observer[ob.label]=new Observer_type_1(sensor[hm[s1.size()+1].at(0).label]);}
				else observer[ob.label]=new Observer_type_1(observer[hm[s1.size()+1].at(0).label]);
				break;
			case KEP:
				if(hm[s1.size()+1].at(0).op==SEN) observer[ob.label]=new Observer_type_2(sensor[hm[s1.size()+1].at(0).label],ob.interval.ed);
				else observer[ob.label]=new Observer_type_2(observer[hm[s1.size()+1].at(0).label],ob.interval.ed);
				break;
			case AND:
				if(hm[s1.size()+1].at(0).op==SEN){
					if(hm[s1.size()+1].at(1).op==SEN) observer[ob.label]=new Observer_type_3(sensor[hm[s1.size()+1].at(0).label],sensor[hm[s1.size()+1].at(1).label]);
					else observer[ob.label]=new Observer_type_3(sensor[hm[s1.size()+1].at(0).label],observer[hm[s1.size()+1].at(1).label]);
				}else{
					if(hm[s1.size()+1].at(1).op==SEN) observer[ob.label]=new Observer_type_3(observer[hm[s1.size()+1].at(0).label],sensor[hm[s1.size()+1].at(1).label]);
					else observer[ob.label]=new Observer_type_3(observer[hm[s1.size()+1].at(0).label],observer[hm[s1.size()+1].at(1).label]);
				}
				break;
			case ALW:
				if(hm[s1.size()+1].at(0).op==SEN) observer[ob.label]=new Observer_type_4(sensor[hm[s1.size()+1].at(0).label],ob.interval.st,ob.interval.ed);
				else observer[ob.label]=new Observer_type_4(observer[hm[s1.size()+1].at(0).label],ob.interval.st,ob.interval.ed);
				break;
			//TODO
			case UNT:
				break;
			//TODO
			case REL:
				break;
			//TODO
			case ERR:
				break;
			default:
				break;
			}
			//clear hashmap pair vector whose key value above the stack level
			hm[s1.size()+1].clear();//need further test
			s1.pop();//remove that operator
		}
	}
}

int Formula::read_sensor_num(int pos){
	int start_pos=pos+2;
	while(s.at(pos++)!=']'){}
	return stoi(s.substr(start_pos,pos-start_pos),nullptr);
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
	case 'N': return NOT;
	case 'U': return UNT;
	case 'K': return KEP;
	case 'R': return REL;
	case 'S': return SEN;
	default: return ERR;
	}
}

interval_t Formula::get_interval(int pos){//pos is the position of '['
	pos++;//skip '['
	int loc_left_bracket=pos;
	int loc_right_bracket=pos;
	interval_t interval;
	interval.st=0;
	while(s.at(pos)!=']'){
		pos++;
		if(s.at(pos)==','){
			loc_right_bracket=pos+1;
			interval.st = stoi(s.substr(loc_left_bracket,pos-loc_left_bracket),nullptr);
		}
	}
	interval.ed =stoi(s.substr(loc_right_bracket,pos-loc_right_bracket),nullptr);
	//cout<<interval.st<<","<<interval.ed<<endl;
	return interval;
}
