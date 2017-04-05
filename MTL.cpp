//============================================================================
// Name        : MTL.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Event.h"
#include "Observer.h"

using namespace std;



int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	Observer* e1 = new Event("e1.log");
	Observer* Ob1 = new Observer_type_1(e1);
	Observer* ROOT=Ob1;//point to root observer
	cout << Ob1->last_node_1.time_stamp<<endl;
	for(int i=0;i<10;i++){//simulate at each IMU i, the event happens
	//MUST follow the update sequence from bottom layer to top layer

		/*EVENT UPDATE*/
		e1->check_new_event(i);
		/*OBSERVER UPDATE*/

		Ob1->run();

		/*ROOT OUTPUT UPDATE*/
		if(Ob1->is_new_event()){
		cout<<"MTL RESULT:"<<"ver: " << verdict_interprete(ROOT->out_node.verdict)\
			<<";	time stamp:"<<ROOT->out_node.time_stamp<<endl;
		}
	}


	delete e1;
	delete Ob1;
	return 0;
}
