#ifndef LIST_H
#define LIST_H

template <class DataType>
class List
{
	private:
	
	class node
	{
		public:
		
		node *next;
		node *prev;

		DataType data;

		node() : data()
		{
			prev = next = NULL;
		}
	
		node( DataType & theData ) 
		{
			data = theData;
			prev = next = NULL;
		}
	};

	node *head;
	node *tail;
	int length;

	public:

	class Iterator
	{
		private:
		
		node *NodePtr;

		public:
		
		Iterator() { NodePtr = NULL; }
		Iterator( node *theNodePtr ) { NodePtr = theNodePtr; }
		
		inline DataType& operator *() { return NodePtr->data; }
		inline DataType& operator->() { return NodePtr->data; }
		
		inline bool operator == ( Iterator & rhs )
		{
			return ( NodePtr == rhs.NodePtr );
		}
		
		inline bool operator != ( Iterator & rhs )
		{
			return ( NodePtr != rhs.NodePtr );
		}
		
		inline operator bool ()
		{
			return ( ( NodePtr != NULL ) && ( NodePtr != NodePtr->next ) && ( NodePtr != NodePtr->prev ) );
		}

		inline Iterator & operator ++ ()
		{
			NodePtr = NodePtr->next;
			return *this;
		}
		
		inline Iterator & operator -- ()
		{
			NodePtr = NodePtr->prev;
			return *this;
		}

		inline Iterator operator ++ (int)
		{
			Iterator temp = *this;
			NodePtr = NodePtr->next;
			return temp;
		}
		
		inline Iterator operator -- (int)
		{
			Iterator temp = *this;
			NodePtr = NodePtr->prev;
			return temp;
		}
		
		friend class List<DataType>;
	};
	
	List();
	List( List & rhs );
	~List();
	
	List & operator = ( List & rhs );
	
	Iterator Start();
	Iterator End();
	int Length();
	
	void Insert( DataType & theData );
	void Insert( Iterator & InsertIterator, DataType & theData );
	
	void Remove( Iterator & RemoveIterator );
	void Clear();
};

#include "List.cpp"

#endif
