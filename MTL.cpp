//============================================================================
// Name        : MTL.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : MTL formula verification
//============================================================================

#include <iostream>
#include "Event.h"
#include "Observer.h"

using namespace std;

int main() {
	int num_sensor=2;
	int num_observer=4;
	int tot_IMU=31;//time length to do the simulation

	Observer** sensor=new Observer*[num_sensor];
	sensor[0]=new Event("alt.log");
	sensor[1]=new Event("pitch.log");
	Observer** observer=new Observer*[num_observer];
	observer[0] = new Observer_type_1(sensor[0]);
	observer[1] = new Observer_type_1(sensor[1]);
	observer[2] = new Observer_type_2(observer[0],5);
	//observer[3] = new Observer_type_3(observer[1],observer[2]);
	Observer* ROOT=observer[2];//pointer to root observer
	for(int i=0;i<tot_IMU;i++){//simulate at each IMU i, the event happens
	//MUST follow the update sequence from bottom layer to top layer
		/*EVENT UPDATE*/
		sensor[0]->check_new_event(i);
		sensor[1]->check_new_event(i);
		/*OBSERVER UPDATE*/
		observer[0]->run();
		observer[1]->run();
		observer[2]->run();
		/*ROOT OUTPUT UPDATE*/
		cout<<"MTL RESULT:@time " <<i<<"	ver: " << verdict_interprete(ROOT->out_node.verdict)\
			<<"	time stamp:"<<ROOT->out_node.time_stamp<<endl;
	}

	delete[] sensor;
	delete[] observer;
	delete ROOT;
	return 0;
}
