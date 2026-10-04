#include "List.h"

template <class DataType>
List<DataType>::List()
{
	head = new node;
	tail = new node;
	length = 0;

	head->prev = head;
	head->next = tail;

	tail->prev = head;
	tail->next = tail;
}

template <class DataType>
List<DataType>::List( List & rhs )
{
	head = new node;
	tail = new node;

	head->prev = head;
	tail->next = tail;

	length = rhs.length;

	if ( rhs.length == 0 )
	{
		head->next = tail;
		tail->prev = head;
	}
	else
	{
		node *RhsCurrent = rhs.head->next;
		
		node *current = head;
		node *previous;

		while ( RhsCurrent != rhs.tail )
		{
			previous = current;

			current = new node( RhsCurrent->data );
			RhsCurrent = RhsCurrent->next;
			
			previous->next = current;
			current->prev = previous;
		}

		current->next = tail;
		tail->prev = current;
	}
}

template <class DataType>
List<DataType>::~List()
{
	Clear();

	delete head;
	delete tail;
}

template <class DataType>
List<DataType> & List<DataType>::operator = ( List<DataType> & rhs )
{
	if ( this != &rhs )
	{
		Clear();
		
		length = rhs.length;

		if ( rhs.length > 0 )
		{
			node *RhsCurrent = rhs.head->next;
			
			node *current = head;
			node *previous;

			while ( RhsCurrent != rhs.tail )
			{
				previous = current;

				current = new node( RhsCurrent->data );
				RhsCurrent = RhsCurrent->next;
				
				previous->next = current;
				current->prev = previous;
			}

			current->next = tail;
			tail->prev = current;
		}
	}

	return *this;
}

template <class DataType>
typename List<DataType>::Iterator List<DataType>::Start()
{
	return Iterator( head );
}

template <class DataType>
typename List<DataType>::Iterator List<DataType>::End()
{
	return Iterator( tail );
}

template <class DataType>
int List<DataType>::Length()
{
	return length;
}

template <class DataType>
void List<DataType>::Insert( const DataType & theData )
{
	node *temp = new node( theData );
	
	temp->prev = tail->prev;
	temp->next = tail;

	temp->prev->next = temp;
	temp->next->prev = temp;

	length++;
}		

template <class DataType>
void List<DataType>::Insert( Iterator & InsertIterator, const DataType & theData )
{
	if ( InsertIterator.NodePtr == head ) return;

	node *temp = new node( theData );
	
	temp->prev = InsertIterator.NodePtr->prev;
	temp->next = InsertIterator.NodePtr;

	temp->prev->next = temp;
	temp->next->prev = temp;

	length++;
}

template <class DataType>
void List<DataType>::Remove( Iterator & RemoveIterator )
{
	if ( RemoveIterator.NodePtr == head || RemoveIterator.NodePtr == tail ) return;

	node *temp = RemoveIterator.NodePtr;
	RemoveIterator.NodePtr = RemoveIterator.NodePtr->next;

	temp->prev->next = temp->next;
	temp->next->prev = temp->prev;

	delete temp;

	length--;
}

template <class DataType>
void List<DataType>::Clear()
{
	node *current = head->next;
	node *temp;

	while ( current != tail )
	{
		temp = current->next;
		delete current;
		current = temp;
	}

	head->next = tail;
	tail->prev = head;

	length = 0;
}
