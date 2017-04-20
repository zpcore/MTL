/*
--This file is used to interpret the MTL formula written as a string
--Sensor: @S[0],@S[1]...
--Operator:
	not->NOT{} //algorithm 1
	keep->KEEP[lb]{} //algorithm 2 !!!check the interval!!!
	and->AND{,} //algorithm 3
	always->ALW[lb,up]{} //algorithm 4
	until->UNT[lb,ub]{,} //algorithm 5
	release->REL[lb,ub]{,} //algorithm ??
--MTL Example:
	NOT{S[0]}
	{S[0]}AND{S[1]}
	ALW[5,10]{{NOT{S[0]}}AND{NOT{S[1]}}}AND{S[2]}
*/
#pragma once

#include <iostream>
using namespace std;

typedef struct{
	int st;
	int ed;
}interval_t;

enum op_t{ALW,NOT,UNT,KEP,REL,AND,SEN,ERR};

typedef	struct{
	op_t op;
	int label;
	interval_t interval;
}seg_t;

class Formula{
public:
	Formula(string,Observer**,Observer**);
	~Formula(){};
private:
	op_t get_type(int pos);
	interval_t get_interval(int pos);
	string delSpaces(string &);
	int read_sensor_num(int pos);
public:
	string s;
};
