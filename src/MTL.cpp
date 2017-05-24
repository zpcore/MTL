//============================================================================
// Name        : MTL.cpp
// Author      : Pei Zhang
// Version     : 1.0.1
// Copyright   : Your copyright notice
// Description : Main function for MTL formula verification
//============================================================================
//============================================================================
// Description :
// 1. Assign the sensor number, simulation time stamps;
// 2. Link the sensor to the .log file;
// 3. Write your MTL formula as a string. For MTL format, see Formula.h;
// 4. Run.
//============================================================================

#include <iostream>
#include "Event.h"
#include "Observer.h"
#include "Formula.h"

using namespace std;

int main() {
////MTL setup
	//string formula="ALW[5,10]{S[0]}";
	string formula="AND{S[1],KEP[5]{S[0]}}";
	//string formula="KEP[5]{NOT{NOT{S[0]}}}";
	//string formula="AND{S[0],S[1]}";
	//string formula="KEP[5]{S[1]}";
	int num_sensor=2;
	int tot_IMU=31;//time length to do the simulation
	Observer** sensor=new Observer*[num_sensor];
	sensor[0]=new Event("./src/alt.log");
	sensor[1]=new Event("./src/pitch.log");
//************************************************************

//Don't touch the following code
	int num_observer=0;
	for(const char& c:formula) num_observer=c=='{'?num_observer+1:num_observer;//auto compute the total number of observers required
	cout<<"Number of Observer: "<<num_observer<<endl;
	Observer** observer=new Observer*[num_observer];
	Formula f=Formula(formula,sensor,observer);
	Observer* ROOT=num_observer==0?sensor[0]:observer[0];//pointer to root observer/sensor
	for(int i=0;i<tot_IMU;i++){//simulate at each IMU i, the event happens
	//MUST follow the update sequence from bottom layer to top layer (no need to care)
		/*EVENT UPDATE*/
		for(int n=0;n<num_sensor;n++) sensor[n]->check_new_event(i);
		/*OBSERVER UPDATE*/
		for(int n=0;n<num_observer;n++) observer[num_observer-n-1]->run();
		/*ROOT OUTPUT UPDATE*/
		cout<<"MTL RESULT:@time " <<i+1<<"	ver: " << verdict_interprete(ROOT->out_node.verdict)\
					<<"	time stamp:"<<ROOT->out_node.time_stamp+1<<endl;
	}
	delete[] sensor;
	delete[] observer;
	delete ROOT;
	return 0;
}