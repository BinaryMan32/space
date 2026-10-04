#include "queue.h"

template <class DataType>
queue<DataType>::queue()
{
	FrontElement = NULL;
	BackElement = NULL;
	NumElements = 0;
}

template <class DataType>
queue<DataType>::~queue()
{
	node *temp;

	while ( NumElements > 0 )
	{
		temp = FrontElement;
		FrontElement = FrontElement->next;
		delete temp;
		NumElements--;
	}
}

template <class DataType>
inline void queue<DataType>::EnQueue( DataType theData )
{
	if ( NumElements == 0 )
	{
		FrontElement = BackElement = new node;
	}
	else
	{
		BackElement->next = new node;
		BackElement = BackElement->next;
	}

	BackElement->data = theData;
	BackElement->next = NULL;

	NumElements++;
}

template <class DataType>
DataType queue<DataType>::DeQueue()
{
	if ( NumElements == 0 ) return DataType();

	DataType FrontValue = FrontElement->data;

	node *temp = FrontElement;

	FrontElement = FrontElement->next;

	delete temp;

	NumElements--;

	return FrontValue;
}

template <class DataType>
DataType queue<DataType>::Front()
{
	if ( NumElements == 0 ) return DataType();

	return FrontElement->data;
}
