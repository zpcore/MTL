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
	int num_sensor=1;
	int num_observer=2;
	int tot_IMU=20;//time length to do the simulation

	Observer** sensor=new Observer*[num_sensor];
	sensor[0]=new Event("e1.log");
	//sensor[1]=new Event("e2.log");
	Observer** observer=new Observer*[num_observer];
	observer[0] = new Observer_type_1(sensor[0]);
	observer[1] = new Observer_type_2(observer[0],4);
	Observer* ROOT=observer[1];//pointer to root observer
	for(int i=0;i<tot_IMU;i++){//simulate at each IMU i, the event happens
	//MUST follow the update sequence from bottom layer to top layer
		/*EVENT UPDATE*/
		sensor[0]->check_new_event(i);
		/*OBSERVER UPDATE*/
		observer[0]->run();
//		cout<<"MTL RESULT:@time " <<i<<"	ver: " << verdict_interprete(observer[0]->out_node.verdict)\
//					<<"	time stamp:"<<observer[0]->out_node.time_stamp<<endl;
		observer[1]->run();

		/*ROOT OUTPUT UPDATE*/
		cout<<"MTL RESULT:@time " <<i<<"	ver: " << verdict_interprete(ROOT->out_node.verdict)\
			<<"	time stamp:"<<ROOT->out_node.time_stamp<<endl;
	}

	delete[] sensor;
	delete[] observer;
	delete ROOT;
	return 0;
}
