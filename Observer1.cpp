class Observer1 : public Observer{

//constructor
	Observer1(Observer cob, Observer pob){
		init(cob, pob);
	}

//override
	void run(event_node e){//child_node_1.out_node
		if(is_new_event(e)){
			out_node.time_stamp=e.time_stamp;
			out_node.verdict=e.verdict==1?0:1;
		}
	}
}