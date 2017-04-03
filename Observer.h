//all the attributes of observers
class Observer{

public:
	void init(Observer cob, Observer pob);
	void init(Observer cob1, Observer cob2, Observer pob);

protected:
	virtual void run(event_node e);//algorithm 1,2,4
	virtual void run(event_node e1, event_node e2); //algorithm 3


public:
	struct event_node{
		int time;//the time is only used for checking new input and debuging
		int verdict;//0:false, 1:true, 2:maybe
		int time_stamp;
	};
	int cur_time;
private:
	Observer child_observer_1;
	Observer child_observer_2;
	Observer parent_observer;

	event_node last_node_1;//store child_observer_1's output
	event_node last_node_2;//store child_observer_2's output
	event_node out_node;//output of this observer

}