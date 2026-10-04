#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>

#include "gamestring.h"
	
int string::BytesCopied = 0;
int string::BytesScanned = 0;

// Writes a set of characters to a memory location
//	- theString:	memory location to write to
//	- theValue:		character to write
//	- theLength:	number of characters to write
void string::MemWrite( char *theString, char theValue, int theLength )
{
	if ( theString == NULL || theLength == 0 ) return;
	memset( theString, theValue, theLength );

	BytesCopied += theLength;
}
	
// Copies a string of bytes from one location to another
//	- Source:		pointer to source of memory copy
//	- Destination:	pointer to destination of memory copy
//	- theLength:	number of bytes to copy
void string::MemCopy( char *Source, char *Destination, int theLength )
{
	if ( Source == NULL || Destination == NULL || theLength == 0 ) return;
	memcpy( Destination, Source, theLength );

	BytesCopied += theLength;
}


// Finds the length of a string
//	- theString:	pointer to string data
int string::GetLength( char *theString )
{
	if ( theString == NULL ) return 0;
	int theLength = int( strlen( theString ) );
	BytesScanned += theLength;
	return theLength;
}


// Finds a character in a string
//	- theString:	pointer to string data
//	- theLength:	length of the string
//	- theChar:		character to search for
int string::FindChar( char *theString, int theLength, char theChar )
{
	if ( theString == NULL ) return -1;
	if ( theLength <= 0 ) return -1;

	BytesScanned += theLength;

	for ( int index = 0; index < theLength; index++ )
	{
		if ( theString[ index ] == theChar ) return index;
	}

	return -1;
}

// Finds a string in a string
//	- theString:	pointer to string data
//	- theLength:	length of the string
//	- FindChar:		pointer to string data to search for
//	- FindLength:	length of the string to search for
int string::FindString( char *theData, int theLength, char *FindData, int FindLength )
{
	if ( FindData == NULL ) return -1;
	if ( FindLength <= 0 ) return -1;
	if ( FindLength > theLength ) return -1;
		
	BytesScanned += theLength;
		
	int i;
	int delta;
		
	char *tempData = theData;
	int tempLength = theLength;
		
	while ( ( delta = FindChar( tempData, tempLength, *FindData  ) ) >= 0 )
	{
		tempData += delta;
		tempLength -= delta;

		if ( FindLength > tempLength ) return -1;
			
		for ( i=0; i<FindLength; i++ )
		{
			if ( FindData[ i ] != tempData[ i ] ) break;
		}

		if ( i == FindLength ) return ( theLength - tempLength );
		
		tempData++;
		tempLength--;
	}

	return -1;
}
	
// Compares two strings
//	- LeftString:	pointer to data in left string
//	- LeftLength:	length of left string
//	- RightString:	pointer to data in right string
//	- RightLength:	length of right string
//	- returns:		-1 if l <  r
//					 0 if l == r
//					 1 if l >  r
int string::StringCompare( char *LeftString, int LeftLength, char *RightString, int RightLength )
{
	if ( LeftString  == NULL || LeftLength  == 0 ) return 1;
	if ( RightString == NULL || RightLength == 0 ) return -1;

	if ( RightLength < LeftLength ) LeftLength = RightLength;
	LeftLength++;

	BytesScanned += LeftLength;

	int result = memcmp( LeftString, RightString, LeftLength );

	if ( result < 0 ) return -1;
	if ( result > 0 ) return 1;
	return 0;
}

// private parameter constructor
//	- use only for temporary storage of old strings
//	- frees theData when the string goes out of scope
string::string( char *theData, int theLength )
{
	data = theData;
	length = theLength;
}
	
// default constructor
//	- creates an empty string
string::string()
{
	length = 0;
	data = new char [ length + 1 ];
	data[ length ] = 0;
}

// parameter constructor
//	- creates a new string
//	- theData:	character used to construct the string
string::string( char theData )
{
	length = 1;
	data = new char [ length + 1 ];
	data[ 0 ] = theData;

	data[ length ] = 0;
}
	
// parameter constructor
//	- creates a new string
//	- theData:	pointer to character data used to construct the string
string::string( char *theData )
{
	length = GetLength( theData );
	data = new char [ length + 1 ];
	MemCopy( theData, data, length );

	data[ length ] = 0;
}
	
// parameter constructor
//	- creates a new string
//	- theLength:	length of new string
string::string( int theLength )
{
	theLength > 0 ? length = theLength : length = 0;
		
	data = new char [ length + 1 ];
	data[ length ] = 0;
}

// parameter constructor
//	- creates a new string
//	- theLength:	length of new string
//	- theValue:		character to fill the new string with
string::string( int theLength, char theValue )
{
	theLength > 0 ? length = theLength : length = 0;

	data = new char [ length + 1 ];
	MemWrite( data, theValue, length );
		
	data[ length ] = 0;
}

// copy constructor
//	- copies a string
string::string( const string &rhs )
{
	length = rhs.length;

	data = new char [ length + 1 ];
	MemCopy( rhs.data, data, length );

	data[ length ] = 0;
}
	
// assigns an int to a string
string & string::operator = ( int theInteger )
{
	int PlaceValue, i, temp = theInteger;
		
	length = 1;
	while ( ( temp /= 10 ) != 0 ) length++;

	PlaceValue = 1;
	for ( i=1; i<length; i++, PlaceValue*=10 );
		
	delete [] data;

	if ( theInteger < 0 )
	{
		length++;
		data = new char [ length + 1 ];

		data[ 0 ] = '-';
		theInteger *= -1;
		i=1;

		for ( i=1; i<length; i++ )
		{
			temp = theInteger / PlaceValue;
			theInteger -= ( temp * PlaceValue );
			
			data[ i ] = char( '0' + temp );
			
			PlaceValue /= 10;
		}
	}
	else
	{
		data = new char [ length + 1 ];

		for ( i=0; i<length; i++ )
		{
			temp = theInteger / PlaceValue;
			theInteger -= ( temp * PlaceValue );
			
			data[ i ] = char( '0' + temp );
			
			PlaceValue /= 10;
		}
	}

	data[ length ] = 0;

	return *this;
}

// assigns a string to a string
string & string::operator = ( const string &theString )
{
	if ( this != &theString )
	{
		length = theString.length;
			
		delete [] data;
		data = new char [ length + 1 ];

		MemCopy( theString.data, data, length );

		data[ length ] = 0;
	}

	return *this;
}

// assigns a character pointer to a string
string & string::operator = ( char *theData )
{
	length = GetLength( theData );
	if ( length < 0 ) length = 0;
		
	delete [] data;
	data = new char [ length + 1 ];

	MemCopy( theData, data, length );

	data[ length ] = 0;

	return *this;
}

// assigns a character to a string
string & string::operator = ( char theData )
{
	length = 1;

	delete [] data;
	data = new char [ length + 1 ];

	data[ 0 ] = theData;
		
	data[ length ] = 0;
	
	return *this;
}
	
// destructor
//	- frees dynamic memory
string::~string()
{
	if ( data ) delete [] data;
	length = 0;
}

// operator char *
//	- handles conversion to a char *
//	- allows a string to be used with existing functions
string::operator char * ()
{
	return data;
}

// operator bool
//    - handles type cast to bool
//    - allows programmer to use if ( StringName )
//      to determine if the string contains data
string::operator bool ()
{
	return ( length > 0 );
}
	
// Returns the length of the string
int string::Length()
{
	return length;
}

// Array index operator
//    - returns the char at <index>
char & string::operator[] ( int index )
{
	if ( index < 0 ) 
		return data[ 0 ];
		
	if ( index > length ) 
		return data[ length ];
		
	return data[ index ];
}
	
// operator +
//    - appends <c> to end of string
string string::operator + ( char c )
{
	string temp;

	temp.length = length + 1;
	temp.data = new char [ length + 2 ];

	MemCopy( data, temp.data, length );
		
	temp.data[ length ] = c;
	temp.data[ length + 1 ] = 0;

	return temp;
}

// operator +
//    - appends <rhs> to end of string
string string::operator + ( char *rhs )
{
	string temp;
	int rhsLength = GetLength( rhs );

	temp.length = length + rhsLength;
	temp.data = new char [ temp.length + 1 ];

	MemCopy( data, temp.data, length );
	MemCopy( rhs, temp.data+length, rhsLength+1 );
		
	return temp;
}

// operator +
//    - appends <rhs> to end of string
string string::operator + ( const string & rhs )
{
	string temp;

	temp.length = length + rhs.length;
	temp.data = new char [ temp.length + 1 ];

	MemCopy( data, temp.data, length );
	MemCopy( rhs.data, temp.data+length, rhs.length+1 );
		
	return temp;
}
	
// operator +=
//    - appends <c> to end of string
string & string::operator += ( char c )
{
	char *OldData = data;

	length++;
	data = new char [ length + 1 ];

	MemCopy( OldData, data, length );
		
	data[ length - 1 ] = c;
	data[ length ] = 0;

	delete [] OldData;

	return *this;
}

// operator +=
//    - appends <rhs> to end of string
string & string::operator += ( char *rhs )
{
	char *OldData = data;
	int OldLength = length;
	int rhsLength = GetLength( rhs );

	length += rhsLength;
	data = new char [ length + 1 ];

	MemCopy( OldData, data, OldLength );
	MemCopy( rhs, data+OldLength, rhsLength+1 );
		
	delete [] OldData;

	return *this;
}

// operator +=
//    - appends <rhs> to end of string
string & string::operator += ( const string & rhs )
{
	char *OldData = data;
	int OldLength = length;	

	length += rhs.length;
	data = new char [ length + 1 ];

	if ( this != &rhs )
	{
		MemCopy( OldData, data, OldLength );
		MemCopy( rhs.data, data+OldLength, rhs.length+1 );
	}
	else
	{
		MemCopy( OldData, data, OldLength );
		MemCopy( OldData, data+OldLength, OldLength+1 );
	}		
		
	return *this;
}

// compare string to another string
	
bool string::operator == ( const string & rhs )
	{ return ( StringCompare( data, length, rhs.data, rhs.length ) == 0 ); }

bool string::operator != ( const string & rhs )
	{ return ( StringCompare( data, length, rhs.data, rhs.length ) != 0 ); }

bool string::operator <= ( const string & rhs )
	{ return ( StringCompare( data, length, rhs.data, rhs.length ) <= 0 ); }

bool string::operator >= ( const string & rhs )
	{ return ( StringCompare( data, length, rhs.data, rhs.length ) >= 0 ); }

bool string::operator <  ( const string & rhs )
	{ return ( StringCompare( data, length, rhs.data, rhs.length ) < 0 ); }

bool string::operator >  ( const string & rhs )
	{ return ( StringCompare( data, length, rhs.data, rhs.length ) > 0 ); }

// compare string to a char *
	
bool string::operator == ( char * rhs )
	{ return ( StringCompare( data, length, rhs, GetLength(rhs) ) == 0 ); }

bool string::operator != ( char * rhs )
	{ return ( StringCompare( data, length, rhs, GetLength(rhs) ) != 0 ); }

bool string::operator <= ( char * rhs )
	{ return ( StringCompare( data, length, rhs, GetLength(rhs) ) <= 0 ); }

bool string::operator >= ( char * rhs )
	{ return ( StringCompare( data, length, rhs, GetLength(rhs) ) >= 0 ); }

bool string::operator <  ( char * rhs )
	{ return ( StringCompare( data, length, rhs, GetLength(rhs) ) < 0 ); }

bool string::operator >  ( char * rhs )
	{ return ( StringCompare( data, length, rhs, GetLength(rhs) ) > 0 ); }

// Find
//    - finds the first occurence of <c>
//    - returns position if found, -1 if not in string
int string::Find( char c )
{
	return FindChar( data, length, c );
}
	
// Find
//    - finds the first occurence of <theString>
//    - returns position if found, -1 if not in string
int string::Find( char *theString )
{
	return FindString( data, length, theString, GetLength( theString ) );
}

// Find
//    - finds the first occurence of <theString>
//    - returns position if found, -1 if not in string
int string::Find( const string & theString )
{
	return FindString( data, length, theString.data, theString.length );
}

// Find
//    - finds the first occurence of <c> after position <index>
//    - returns position if found, -1 if not in string
int string::Find( char c, int index )
{
	int delta = FindChar( data + index, length - index, c );
	if ( delta < 0 ) return -1;
	return index + delta;
}
	
// Find
//    - finds the first occurence of <theString> after position <index>
//    - returns position if found, -1 if not in string
int string::Find( char *theString, int index )
{
	int delta = FindString( data + index, length - index, theString, GetLength( theString ) );
	if ( delta < 0 ) return -1;
	return index + delta;
}

// Find
//    - finds the first occurence of <theString> after position <index>
//    - returns position if found, -1 if not in string
int string::Find( const string & theString, int index )
{
	int delta = FindString( data + index, length - index, theString.data, theString.length );
	if ( delta < 0 ) return -1;
	return index + delta;
}

// Remove
//    - removes the first occurence of <c>
//    - returns position if removed, -1 if not in string
int string::Remove( char c )
{
	int index = FindChar( data, length, c );
	if ( index < 0 ) return -1;
		
	char *OldData = data;

	length--;
	data = new char [ length + 1 ];

	MemCopy( OldData, data, index );
	MemCopy( OldData+index+1, data+index, length-index+1 );

	delete [] OldData;

	return index;
}

// Remove
//    rhs - pointer to character data to remove
//    - removes the first occurence of <rhs>
//    - returns position if removed, -1 if not in string
int string::Remove( char *rhs )
{
	int index = Find( rhs );
	if ( index < 0 ) return -1;

	char *OldData = data;
	int RhsLength = GetLength( rhs );

	length -= RhsLength;
	data = new char [ length + 1 ];

	MemCopy( OldData, data, index );
	MemCopy( OldData+index+RhsLength, data+index, length-index+1 );

	delete [] OldData;

	return index;
}

// Remove
//    - removes the first occurence of <rhs>
//    - returns position if removed, -1 if not in string
int string::Remove( const string & rhs )
{
	int index = Find( rhs );
	if ( index < 0 ) return -1;

	char *OldData = data;

	length -= rhs.length;
	data = new char [ length + 1 ];

	MemCopy( OldData, data, index );
	MemCopy( OldData+index+rhs.length, data+index, length-index+1 );

	delete [] OldData;

	return index;
}

// Remove
//    start - beginning position of string to remove
//    theLength - length of string to remove
//    returns the string that is removed
string string::Remove( int start, int theLength )
{
	if ( theLength <= 0 ) return string();
		
	if ( start < 0 )
	{
		if ( start + theLength < 0 ) return string();

		theLength += start;
		start = 0;
	}
			
	if ( start + theLength >= length )
	{
		if ( start >= length ) return string();

		theLength = length - start;
	}

	string removed( theLength );

	char *OldData = data;
	length -= theLength;
	data = new char [ length + 1 ];

	MemCopy( OldData, data, start );
	MemCopy( OldData+start, removed.data, theLength );
	MemCopy( OldData+start+theLength, data+start, length-start+1 );

	delete [] OldData;

	return removed;
}

// RemoveAll
//    - theString : string to remove
void string::RemoveAll( const string & theString )
{
	char *TempData = data;
	int TempLength = length;
	int delta;
	queue <int> Position;

	while ( ( delta = FindString( TempData, TempLength, theString.data, theString.length ) ) >= 0 )
	{
		TempData += delta;
		TempLength -= delta;
	
		Position.EnQueue( delta );

		TempData += theString.length;
		TempLength -= theString.length;
	}
	
	if ( Position.NumElements == 0 ) return;
		
	char *OldPtr = data;
	char *OldData = data;

	length -= Position.NumElements * theString.length;
	data = new char [ length + 1 ];

	TempData = data;
	TempLength = length;

	while ( Position.NumElements )
	{
		delta = Position.DeQueue();
		
		MemCopy( OldData, TempData, delta );
		OldData += delta + theString.length;
		TempData += delta;
		TempLength -= delta;
	}

	MemCopy( OldData, TempData, TempLength+1 );

	delete [] OldPtr;
}
	
// Insert
//    index - position to insert at
//    c - character to insert
void string::Insert( int index, char c )
{
	if ( index < 0 || index >= length ) return;
	char *OldData = data;

	length++;
	data = new char [ length + 1 ];

	MemCopy( OldData, data, index );
	data[ index ] = c;
	MemCopy( OldData+index, data+index+1, length-index );

	delete [] OldData;
}

// Insert
//    index - position to insert at
//    rhs - string to insert
void string::Insert( int index, char *rhs )
{
	if ( index < 0 || index >= length ) return;

	char *OldData = data;
	int RhsLength = GetLength( rhs );

	length += RhsLength;
	data = new char [ length + 1 ];

	MemCopy( OldData, data, index );
	MemCopy( rhs, data+index, RhsLength );
	MemCopy( OldData+index, data+index+RhsLength, length-index-RhsLength+1 );

	delete [] OldData;
}

// Insert
//    index - position to insert at
//    rhs - string to insert
void string::Insert( int index, const string & rhs )
{
	if ( index < 0 || index >= length ) return;

	char *OldData = data;

	length += rhs.length;
	data = new char [ length + 1 ];

	MemCopy( OldData, data, index );
	MemCopy( rhs.data, data+index, rhs.length );
	MemCopy( OldData+index, data+index+rhs.length, length-index-rhs.length+1 );

	delete [] OldData;
}

// Replace
//    - lhs : character to find
//    - rhs : character to insert
//    - returns position if replaced, -1 if not
int string::Replace( char lhs, char rhs )
{
	int index = Find( lhs );
	
	if ( index >= 0 )
	{
		data[ index ] = rhs;
	}

	return index;
}
	
// Replace
//    - lhs : character to find
//    - rhs : character data to insert
//    - returns position if replaced, -1 if not
int string::Replace( char lhs, char *rhs )
{
	int index = Find( lhs );

	if ( index >= 0 )
	{
		string old( data, length );
		int RhsLength = GetLength( rhs );
			
		length += RhsLength - 1;
		data = new char [ length + 1 ];
			
		MemCopy( old.data, data, index );
		MemCopy( rhs, data+index, RhsLength );
		MemCopy( old.data+index+1, data+index+RhsLength, old.length-index );
	}

	return index;
}

// Replace
//    - lhs : character to find
//    - rhs : string to insert
//    - returns position if replaced, -1 if not
int string::Replace( char lhs, const string & rhs )
{
	int index = Find( lhs );

	if ( index >= 0 )
	{
		string old( data, length );
			
		length += rhs.length - 1;
		data = new char [ length + 1 ];
			
		MemCopy( old.data, data, index );
		MemCopy( rhs.data, data+index, rhs.length );
		MemCopy( old.data+index+1, data+index+rhs.length, old.length-index );
	}

	return index;
}

// Replace
//    - lhs : character data to find
//    - rhs : character to insert
//    - returns position if replaced, -1 if not
int string::Replace( char *lhs, char rhs )
{
	int LhsLength = GetLength( lhs );
	int index = FindString( data, length, lhs, LhsLength );
		
	if ( index >= 0 )
	{
		string old( data, length );

		length += 1 - LhsLength;
		data = new char [ length + 1 ];

		MemCopy( old.data,	data,	index );
		data[ index ] = rhs;
		MemCopy( old.data+index+LhsLength, data+index+1, old.length-index );
	}

	return index;
}

// Replace
//    - lhs : character data to find
//    - rhs : character data to insert
//    - returns position if replaced, -1 if not
int string::Replace( char *lhs, char *rhs )
{
	int LhsLength = GetLength( lhs );
	int index = FindString( data, length, lhs, LhsLength );

	if ( index >= 0 )
	{
		string old( data, length );
		int RhsLength = GetLength( rhs );

		length += RhsLength - LhsLength;
		data = new char [ length + 1 ];

		MemCopy( old.data, data, index );
		MemCopy( rhs, data+index, RhsLength );
		MemCopy( old.data+index+LhsLength, data+index+RhsLength, old.length-index );
	}

	return index;
}

// Replace
//    - lhs : character data to find
//    - rhs : string to insert
//    - returns position if replaced, -1 if not
int string::Replace( char *lhs, const string & rhs )
{
	int LhsLength = GetLength( lhs );
	int index = FindString( data, length, lhs, LhsLength );

	if ( index >= 0 )
	{
		string old( data, length );

		length += rhs.length - LhsLength;
		data = new char [ length + 1 ];

		MemCopy( old.data, data, index );
		MemCopy( rhs.data, data+index, rhs.length );
		MemCopy( old.data+index+LhsLength, data+index+rhs.length, old.length-index );
	}

	return index;
}

// Replace
//    - lhs : string to find
//    - rhs : character to insert
//    - returns position if replaced, -1 if not
int string::Replace( const string & lhs, char rhs )
{
	int index = Find( lhs );

	if ( index >= 0 )
	{
		string old( data, length );

		length += 1 - lhs.length;
		data = new char [ length + 1 ];

		MemCopy( old.data,	data,	index );
		data[ index ] = rhs;
		MemCopy( old.data+index+lhs.length, data+index+1, old.length-index );
	}

	return index;
}
	
// Replace
//    - lhs : string to find
//    - rhs : character data to insert
//    - returns position if replaced, -1 if not
int string::Replace( const string & lhs, char *rhs )
{
	int index = Find( lhs );

	if ( index >= 0 )
	{
		string old( data, length );
		int RhsLength = GetLength( rhs );

		length += RhsLength - lhs.length;
		data = new char [ length + 1 ];

		MemCopy( old.data, data, index );
		MemCopy( rhs, data+index, RhsLength );
		MemCopy( old.data+index+lhs.length, data+index+RhsLength, old.length-index );
	}

	return index;
}

// Replace
//    - lhs : string to find
//    - rhs : string to insert
//    - returns position if replaced, -1 if not
int string::Replace( const string & lhs, const string & rhs )
{
	int index = Find( lhs );

	if ( index >= 0 )
	{
		string old( data, length );

		length += rhs.length - lhs.length;
		data = new char [ length + 1 ];

		MemCopy( old.data, data, index );
		MemCopy( rhs.data, data+index, rhs.length );
		MemCopy( old.data+index+lhs.length, data+index+rhs.length, old.length-index );
	}

	return index;
}

// ReplaceAll
//    - lhs : string to find
//    - rhs : string to insert
void string::ReplaceAll( const string & lhs, const string & rhs )
{
	char *TempData = data;
	int TempLength = length;
	int delta;
	queue <int> Position;

	while ( ( delta = FindString( TempData, TempLength, lhs.data, lhs.length ) ) >= 0 )
	{
		TempData += delta;
		TempLength -= delta;
		
		Position.EnQueue( delta );

		TempData += lhs.length;
		TempLength -= lhs.length;
	}
	
	if ( Position.NumElements == 0 ) return;
		
	char *OldPtr = data;
	char *OldData = data;

	length += Position.NumElements * rhs.length;
	data = new char [ length + 1 ];

	TempData = data;
	TempLength = length;

	while ( Position.NumElements )
	{
		delta = Position.DeQueue();
	
		MemCopy( OldData, TempData, delta );
		OldData += delta + lhs.length;
		TempData += delta;
		TempLength -= delta;

		MemCopy( rhs.data, TempData, rhs.length );
		TempData += rhs.length;
		TempLength -= rhs.length;
	}

	MemCopy( OldData, TempData, TempLength+1 );

	delete [] OldPtr;
}

// IncreaseSize
//    - theLength - number of spaces to append to end of string
void string::IncreaseSize( int theLength )
{
	char *OldData = data;
	data = new char [ length + theLength + 1 ];
		
	MemCopy( OldData, data, length );
	MemWrite( OldData + length, ' ', theLength );
	length += theLength;
		
	data[ length ] = 0;
}
	
// Reverse
//    - reverses the string in place
void string::Reverse()
{
	if ( length <= 1 ) return;
		
	std::reverse( data, data + length );
}


// MakeUpperCase
void string::MakeUpperCase()
{
	char *tempData = data;
	int tempLength = length;

	while ( tempLength > 0 )
	{
		if ( *tempData >= 'a' && *tempData <= 'z' ) *tempData &= 0xDF;
		tempData++;
		tempLength--;
	}
}

// MakeLowerCase
void string::MakeLowerCase()
{
	char *tempData = data;
	int tempLength = length;

	while ( tempLength > 0 )
	{
		if ( *tempData >= 'A' && *tempData <= 'Z' ) *tempData |= 0x20;
		tempData++;
		tempLength--;
	}
}

// SubString
//    start - beginning position of substring
//    theLength - length of substring
string string::SubString( int start, int theLength )
{
	if ( theLength <= 0 ) return string();
		
	if ( start < 0 )
	{
		if ( start + theLength < 0 ) return string();

		theLength += start;
		start = 0;
	}
			
	if ( start + theLength >= length )
	{
		if ( start >= length ) return string();

		theLength = length - start;
	}

	string temp( theLength );
	MemCopy( data+start, temp.data, theLength );

	return temp;
}

// LoadFile
//    Loads a complete file from disk into the string
void string::LoadFile( char *FileName )
{
	LoadFile( FileName, 1024, 32 * 1024 );
}

// LoadFile
//    Loads a complete file from disk into the string
void string::LoadFile( char *FileName, int BufferLength, int MaxBuffers )
{
	int index;
	int NumBuffers = -1;

	char **Buffers;
	Buffers = new char * [ MaxBuffers ];
	
	std::ifstream infile( FileName );
	infile.unsetf( std::ios::skipws );

	length = 0;

	while ( 1 )
	{
		if ( ++NumBuffers >= MaxBuffers ) // quit if out of buffers
		{
			NumBuffers--;
			Buffers[ NumBuffers ][ BufferLength ] = 0;
			break;
		}

		Buffers[ NumBuffers ] = new char [ BufferLength + 1 ];
		if ( Buffers[ NumBuffers ] == NULL ) // quit if out of memory
		{
			NumBuffers--;
			Buffers[ NumBuffers ][ BufferLength ] = 0;
			break;
		}

		infile.get( Buffers[ NumBuffers ], BufferLength + 1, '\f' );
	
		if ( infile.eof() ) // quit if end of file reached
		{
			length+=GetLength( Buffers[ NumBuffers ] );
			Buffers[ NumBuffers ][ length % BufferLength ] = 0;
			break;
		}
		else
		{
			length+=BufferLength;
		}
	}

	delete [] data;
	data = new char [ length + 1 ];

	for ( index=0; index<NumBuffers; index++ )
	{
		MemCopy( Buffers[ index ], data + index * BufferLength, BufferLength );
		delete [] Buffers[ index ];
	}

	MemCopy( Buffers[ NumBuffers ], data + index * BufferLength, length-(BufferLength*NumBuffers) + 1 );
	delete [] Buffers[ NumBuffers ];
		
	infile.close();
}

std::ostream & operator << ( std::ostream & stream, const string & theString )
{
	stream << theString.data;
	
	return stream;
}

std::istream & operator >> ( std::istream & stream, string & theString )
{
	char *temp;
	temp = new char [ 2048 ];

	stream.getline( temp, 2048 );
	
	theString = string( temp );
	
	return stream;
}
