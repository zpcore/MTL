//all the attributes of observers
class Observer{

public:
	struct event_node{
		int time;
		int verdict;//0:false, 1:true, 2:maybe
		int time_stamp;
	};
	void init();

protected:
	virtual event_node run(event_node e);//algorithm 1,2,4
	virtual event_node run(event_node e1, event_node e2); //algorithm 3



}