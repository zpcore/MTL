class Observer1 : public Observer{
	event_node run(event_node e){
		e.verdict=e.verdict==1?0:1;
		return e;
	}

}