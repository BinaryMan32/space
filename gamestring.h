///////////////////////////////////////////////////////////////////////////////
//
//  gamestring.h
//
//  Written December 1999 by Fred Fetinger
//
//  Implements a string class with many operations:
//
//	All functions work on char, char *, and string data types
//
//	All six boolean operators are implemented on char, char *, and string data types
//		( ==, !=, <, <=, >, >= )
//
//	Add two strings using + ( or += )
//	
//	Member Functions:
//		<data> can be char, char*, or string
//
//	Find( data )					- finds first occurence of <data> in the string
//										and returns the position ( -1 if not found )
//	IncreaseSize( n )				- adds <n> spaces to the end of the string
//	Insert( i, data )				- inserts <data> into the string at position <i>
//	Length()						- returns the length of the string
//	LoadFile()						- loads an entire file into the string
//	MakeLowerCase()					- changes all upper case characters to lower case
//	MakeUpperCase()					- changes all lower case characters to upper case
//	Remove( data )					- removes first occurence of <data> from the string
//										and returns the position ( -1 if not found )
//	Remove( start, length )			- removes <length> characters beginning at position <start>
//										and returns the removed string
//	RemoveAll( data )				- removes all occurences of <data> from the string
//	Replace( oldData, newData )		- replaces first occurence of <oldData> with <newData>
//										and returns the position ( -1 if not found )
//	ReplaceAll( oldData, newData )	- replaces all occurences of <oldData> with <newData>
//	Reverse()						- reverses the string in place
//	string( data )					- creates a new string from <data>
//	string( length )				- creates a new string of <length> spaces
//	string( length, c )				- creates a new string of <length> characters filled with char <c>
//	SubString( start, length )		- returns the string <length> characters long beginning at position <start>

#ifndef GAMESTRING_H
#define GAMESTRING_H

#include <iosfwd>
#include "queue.h"

class string
{
	private:
	
	// Writes a set of characters to a memory location
	//	- theString:	memory location to write to
	//	- theValue:		character to write
	//	- theLength:	number of characters to write
	void MemWrite( char *theString, char theValue, int theLength );

	// Copies a string of bytes from one location to another
	//	- Source:		pointer to source of memory copy
	//	- Destination:	pointer to destination of memory copy
	//	- theLength:	number of bytes to copy
	void MemCopy( char *Source, char *Destination, int theLength );

	// Finds the length of a string
	//	- theString:	pointer to string data
	int GetLength( char *theString );
	
	// Finds a character in a string
	//	- theString:	pointer to string data
	//	- theLength:	length of the string
	//	- theChar:		character to search for
	int FindChar( char *theString, int theLength, char theChar );

	// Finds a string in a string
	//	- theString:	pointer to string data
	//	- theLength:	length of the string
	//	- FindChar:		pointer to string data to search for
	//	- FindLength:	length of the string to search for
	int FindString( char *theData, int theLength, char *FindData, int FindLength );
	
	// Compares two strings
	//	- LeftString:	pointer to data in left string
	//	- LeftLength:	length of left string
	//	- RightString:	pointer to data in right string
	//	- RightLength:	length of right string
	//	- returns:		-1 if l <  r
	//					 0 if l == r
	//					 1 if l >  r
	int StringCompare( char *LeftString, int LeftLength, char *RightString, int RightLength );

	// private parameter constructor
	//	- use only for temporary storage of old strings
	//	- frees theData when the string goes out of scope
	string( char *theData, int theLength );
	
	char *data; // string data
	int length; // length of string ( actual memory usage will be length + 1 )

	public:

	// default constructor
	//	- creates an empty string
	string();

	// parameter constructor
	//	- creates a new string
	//	- theData:	character used to construct the string
	string( char theData );

	// parameter constructor
	//	- creates a new string
	//	- theData:	pointer to character data used to construct the string
	string( char *theData );

	// parameter constructor
	//	- creates a new string
	//	- theLength:	length of new string
	string( int theLength );

	// parameter constructor
	//	- creates a new string
	//	- theLength:	length of new string
	//	- theValue:		character to fill the new string with
	string( int theLength, char theValue );

	// copy constructor
	//	- copies a string
	string( const string &rhs );

	// assigns an int to a string
	string & operator = ( int theInteger );

	// assigns a string to a string
	string & operator = ( const string &theString );

	// assigns a character pointer to a string
	string & operator = ( char *theData );

	// assigns a character to a string
	string & operator = ( char theData );

	// destructor
	//	- frees dynamic memory
	~string();

	// operator char *
	//	- handles conversion to a char *
	//	- allows a string to be used with existing functions
	operator char * ();

	// operator bool
	//    - handles type cast to bool
	//    - allows programmer to use if ( StringName )
	//      to determine if the string contains data
	operator bool ();
	
	// Returns the length of the string
	int Length();

	// Array index operator
	//    - returns the char at <index>
	char & operator[] ( int index );
	
	// operator +
	//    - appends <c> to end of string
	string operator + ( char c );

	// operator +
	//    - appends <rhs> to end of string
	string operator + ( char *rhs );

	// operator +
	//    - appends <rhs> to end of string
	string operator + ( const string & rhs );
	
	// operator +=
	//    - appends <c> to end of string
	string & operator += ( char c );

	// operator +=
	//    - appends <rhs> to end of string
	string & operator += ( char *rhs );

	// operator +=
	//    - appends <rhs> to end of string
	string & operator += ( const string & rhs );

	// compare string to another string
	
	bool operator == ( const string & rhs );
	bool operator != ( const string & rhs );
	bool operator <= ( const string & rhs );
	bool operator >= ( const string & rhs );
	bool operator <  ( const string & rhs );
	bool operator >  ( const string & rhs );

	// compare string to a char *
	
	bool operator == ( char * rhs );
	bool operator != ( char * rhs );
	bool operator <= ( char * rhs );
	bool operator >= ( char * rhs );
	bool operator <  ( char * rhs );
	bool operator >  ( char * rhs );

	// Find
	//    - finds the first occurence of <c>
	//    - returns position if found, -1 if not in string
	int Find( char c );

	// Find
	//    - finds the first occurence of <theString>
	//    - returns position if found, -1 if not in string
	int Find( char *theString );

	// Find
	//    - finds the first occurence of <theString>
	//    - returns position if found, -1 if not in string
	int Find( const string & theString );

	// Find
	//    - finds the first occurence of <c> after position <index>
	//    - returns position if found, -1 if not in string
	int Find( char c, int index );
	
	// Find
	//    - finds the first occurence of <theString> after position <index>
	//    - returns position if found, -1 if not in string
	int Find( char *theString, int index );

	// Find
	//    - finds the first occurence of <theString> after position <index>
	//    - returns position if found, -1 if not in string
	int Find( const string & theString, int index );

	// Remove
	//    - removes the first occurence of <c>
	//    - returns position if removed, -1 if not in string
	int Remove( char c );
	
	// Remove
	//    rhs - pointer to character data to remove
	//    - removes the first occurence of <rhs>
	//    - returns position if removed, -1 if not in string
	int Remove( char *rhs );

	// Remove
	//    - removes the first occurence of <rhs>
	//    - returns position if removed, -1 if not in string
	int Remove( const string & rhs );

	// Remove
	//    start - beginning position of string to remove
	//    theLength - length of string to remove
	//    returns the string that is removed
	string Remove( int start, int theLength );

	// RemoveAll
	//    - theString : string to remove
	void RemoveAll( const string & theString );
	
	// Insert
	//    index - position to insert at
	//    c - character to insert
	void Insert( int index, char c );

	// Insert
	//    index - position to insert at
	//    rhs - string to insert
	void Insert( int index, char *rhs );

	// Insert
	//    index - position to insert at
	//    rhs - string to insert
	void Insert( int index, const string & rhs );
	
	// Replace
	//    - lhs : character to find
	//    - rhs : character to insert
	//    - returns position if replaced, -1 if not
	int Replace( char lhs, char rhs );
	
	// Replace
	//    - lhs : character to find
	//    - rhs : character data to insert
	//    - returns position if replaced, -1 if not
	int Replace( char lhs, char *rhs );

	// Replace
	//    - lhs : character to find
	//    - rhs : string to insert
	//    - returns position if replaced, -1 if not
	int Replace( char lhs, const string & rhs );

	// Replace
	//    - lhs : character data to find
	//    - rhs : character to insert
	//    - returns position if replaced, -1 if not
	int Replace( char *lhs, char rhs );

	// Replace
	//    - lhs : character data to find
	//    - rhs : character data to insert
	//    - returns position if replaced, -1 if not
	int Replace( char *lhs, char *rhs );

	// Replace
	//    - lhs : character data to find
	//    - rhs : string to insert
	//    - returns position if replaced, -1 if not
	int Replace( char *lhs, const string & rhs );

	// Replace
	//    - lhs : string to find
	//    - rhs : character to insert
	//    - returns position if replaced, -1 if not
	int Replace( const string & lhs, char rhs );

	// Replace
	//    - lhs : string to find
	//    - rhs : character data to insert
	//    - returns position if replaced, -1 if not
	int Replace( const string & lhs, char *rhs );

	// Replace
	//    - lhs : string to find
	//    - rhs : string to insert
	//    - returns position if replaced, -1 if not
	int Replace( const string & lhs, const string & rhs );
	
	// ReplaceAll
	//    - lhs : string to find
	//    - rhs : string to insert
	void ReplaceAll( const string & lhs, const string & rhs );

	// IncreaseSize
	//    - theLength - number of spaces to append to end of string
	void IncreaseSize( int theLength );

	// Reverse
	//    - reverses the string in place
	void Reverse();

	// MakeUpperCase
	void MakeUpperCase();

	// MakeLowerCase
	void MakeLowerCase();

	// SubString
	//    start - beginning position of substring
	//    theLength - length of substring
	string SubString( int start, int theLength );

	// LoadFile
	//    Loads a complete file from disk into the string
	void LoadFile( char *FileName );

	// LoadFile
	//    Loads a complete file from disk into the string
	void LoadFile( char *FileName, int BufferLength, int MaxBuffers );

	friend std::ostream & operator << ( std::ostream & stream, const string & theString );
	friend std::istream & operator >> ( std::istream & stream, string & theString );

	static int BytesCopied;
	static int BytesScanned;
};

std::ostream & operator << ( std::ostream & stream, const string & theString );
std::istream & operator >> ( std::istream & stream, string & theString );

#endif