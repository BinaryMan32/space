#ifndef QUEUE_H
#define QUEUE_H

template <class DataType>
class queue
{
	struct node
	{
		DataType data;
		node *next;
	};

	node *FrontElement;
	node *BackElement;
	
	public:

	int NumElements;
	
	queue();
	~queue();
	
	void EnQueue( DataType theData );
	
	DataType DeQueue();
	DataType Front();
};

#include "queue.cpp"

#endif
