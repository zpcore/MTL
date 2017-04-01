#include <list>

class event{

//class method
public:
	event(string filename);
	bool is_new_event(int cur_time);
	event_node read_new_event(int cur_time);
private:
	string verdict_interprete(int num);

//class variable
public:
	struct event_node{
		int time;
		int verdict;//0:false, 1:true, 2:maybe
		int time_stamp;
	};
	string filename;
private:
	std::list<event_node> event_list;
}