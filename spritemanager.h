#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

#include "actionbase.h"
#include "spritebase.h"

// disable warning for unused __inline functions
#pragma warning( disable : 4514 )

#ifndef NULL
	#define NULL 0
#endif

class ActionNode;
class Action;

class SpriteNode;
class Sprite;

class SpriteTypeNode;
class SpriteType;

class SpriteGroupNode;
class SpriteGroup;

class SpriteManager;

class ActionNode : public ActionBase
{
	private:
	
	SpriteManager *master;
	ActionNode *next;
	ActionNode *prev;

	SpriteNode *source;
	ActionNode *sourceNext;
	ActionNode *sourcePrev;

	SpriteNode *target;
	ActionNode *targetNext;
	ActionNode *targetPrev;
	
	public:
	
	__inline ActionNode();
		
	__inline Action GetInterface() const;
	
	friend class Action;
	friend class SpriteManager;
};

class Action
{
	private:
	
	ActionNode *nodePtr;
	
	__inline Action( ActionNode *theNodePtr );

	public:
	
	__inline Action();

	__inline operator bool() const;
	__inline bool Valid() const;
	
	__inline bool operator == ( Action theAction ) const;
	__inline bool operator != ( Action theAction ) const;

	__inline ActionNode * operator *  () const;
	__inline ActionNode * operator -> () const;

	__inline Action & operator ++ ();
	__inline Action & operator -- ();
	
	__inline Action operator ++ (int);
	__inline Action operator -- (int);

	__inline Action Next() const;
	__inline Action Prev() const;

	__inline Sprite Source() const;
	__inline Action SourceNext() const;
	__inline Action SourcePrev() const;

	__inline Sprite Target() const;
	__inline Action TargetNext() const;
	__inline Action TargetPrev() const;

	friend class ActionNode;
	friend class Sprite;
	friend class SpriteNode;
	friend class SpriteManager;
};

class SpriteNode : public SpriteBase
{
	private:
	
	SpriteManager *master;
	SpriteNode *next;
	SpriteNode *prev;

	SpriteGroupNode *groupList;
	SpriteNode *groupNext;
	SpriteNode *groupPrev;

	SpriteTypeNode *typeList;
	SpriteNode *typeNext;
	SpriteNode *typePrev;

	int numSourceActions;

	ActionNode *firstSourceAction;
	ActionNode *lastSourceAction;

	int numTargetActions;

	ActionNode *firstTargetAction;
	ActionNode *lastTargetAction;
	
	public:
	
	SpriteNode();
	SpriteNode( string FileName );
	
	__inline Sprite GetInterface() const;
	
	friend class Sprite;
	friend class SpriteType;
	friend class SpriteGroup;
	friend class SpriteManager;
};

class Sprite
{
	private:
	
	SpriteNode *nodePtr;
	
	__inline Sprite( SpriteNode *theNodePtr );

	public:
	
	__inline Sprite();

	__inline operator bool() const;
	__inline bool Valid() const;
	
	__inline bool operator == ( Sprite theSprite ) const;
	__inline bool operator != ( Sprite theSprite ) const;

	__inline SpriteNode * operator *  () const;
	__inline SpriteNode * operator -> () const;

	__inline Sprite & operator ++ ();
	__inline Sprite & operator -- ();
	
	__inline Sprite operator ++ (int);
	__inline Sprite operator -- (int);

	__inline SpriteManager *Master() const;
	__inline Sprite Next() const;
	__inline Sprite Prev() const;

	__inline SpriteGroup Group() const;
	__inline Sprite GroupNext() const;
	__inline Sprite GroupPrev() const;

	__inline SpriteType Type() const;
	__inline Sprite TypeNext() const;
	__inline Sprite TypePrev() const;

	__inline int NumSourceActions() const;
	__inline Action FirstSourceAction() const;
	__inline Action LastSourceAction() const;
	
	__inline int NumTargetActions() const;
	__inline Action FirstTargetAction() const;
	__inline Action LastTargetAction() const;

	friend class SpriteNode;
	friend class Action;
	friend class SpriteType;
	friend class SpriteGroup;
	friend class SpriteManager;
};

class SpriteTypeNode
{
	private:

	SpriteTypeNode *next;
	SpriteTypeNode *prev;
		
	int typeID;

	int numSprites;

	SpriteNode *firstSprite;
	SpriteNode *lastSprite;
	
	bool Enabled;
	
	public:
	
	__inline SpriteTypeNode( int theTypeID );

	friend class SpriteType;
	friend class SpriteManager;
};

class SpriteType
{
	private:

	SpriteTypeNode *nodePtr;

	__inline SpriteType( SpriteTypeNode *theNodePtr );

	__inline void AddSprite( Sprite theSprite );
	__inline void RemoveSprite( Sprite theSprite );
	__inline void Clear();

	public:

	__inline SpriteType();
	
	__inline operator bool() const;
	__inline bool Valid() const;
	
	__inline bool operator == ( SpriteType theSpriteType ) const;
	__inline bool operator != ( SpriteType theSpriteType ) const;

	__inline SpriteTypeNode * operator *  () const;
	__inline SpriteTypeNode * operator -> () const;

	__inline SpriteType & operator ++ ();
	__inline SpriteType & operator -- ();
	
	__inline SpriteType operator ++ (int);
	__inline SpriteType operator -- (int);

	__inline SpriteType Next() const;
	__inline SpriteType Prev() const;

	__inline int TypeID() const;

	__inline int NumSprites() const;
	__inline Sprite FirstSprite() const;
	__inline Sprite  LastSprite() const;
	
	__inline void Enable();
	__inline void Disable();
	__inline bool Enabled() const;

	friend class Sprite;
	friend class SpriteNode;
	friend class SpriteManager;
};

class SpriteGroupNode
{
	private:

	SpriteGroupNode *next;
	SpriteGroupNode *prev;
		
	int numSprites;

	SpriteNode *firstSprite;
	SpriteNode *lastSprite;
	
	public:
	
	__inline SpriteGroupNode();

	friend class SpriteGroup;
	friend class SpriteManager;
};

class SpriteGroup
{
	private:
	
	SpriteGroupNode *nodePtr;

	__inline SpriteGroup( SpriteGroupNode *theNodePtr );

	public:
	
	__inline SpriteGroup();
	
	__inline void AddSprite( Sprite theSprite );
	__inline void RemoveSprite( Sprite theSprite );
	__inline void Clear();

	__inline operator bool() const;
	__inline bool Valid() const;
	
	__inline bool operator == ( SpriteGroup theSpriteGroup ) const;
	__inline bool operator != ( SpriteGroup theSpriteGroup ) const;

	__inline SpriteGroupNode * operator *  () const;
	__inline SpriteGroupNode * operator -> () const;

	__inline SpriteGroup & operator ++ ();
	__inline SpriteGroup & operator -- ();
	
	__inline SpriteGroup operator ++ (int);
	__inline SpriteGroup operator -- (int);

	__inline SpriteGroup Next() const;
	__inline SpriteGroup Prev() const;

	__inline int NumSprites() const;
	__inline Sprite FirstSprite() const;
	__inline Sprite  LastSprite() const;

	friend class Sprite;
	friend class SpriteNode;
	friend class SpriteManager;
};

class SpriteManager
{
	private:
	
	int numActions;
	ActionNode *firstAction;
	ActionNode *lastAction;

	int numSprites;
	SpriteNode *firstSprite;
	SpriteNode *lastSprite;
	
	int numSpriteGroups;
	SpriteGroupNode *firstSpriteGroup;
	SpriteGroupNode *lastSpriteGroup;

	int numSpriteTypes;
	SpriteTypeNode *firstSpriteType;
	SpriteTypeNode *lastSpriteType;

	SpriteType AddSpriteType( SpriteTypeNode *theSpriteTypeNode );
	void RemoveSpriteType( SpriteType & theSpriteType );
	
	public:

	__inline int NumActions() const;
	__inline Action FirstAction() const;
	__inline Action  LastAction() const;

	__inline int NumSprites() const;
	__inline Sprite FirstSprite() const;
	__inline Sprite  LastSprite() const;
	
	__inline int NumSpriteGroups() const;
	__inline SpriteGroup FirstSpriteGroup() const;
	__inline SpriteGroup  LastSpriteGroup() const;

	__inline int NumSpriteTypes() const;
	__inline SpriteType FirstSpriteType() const;
	__inline SpriteType  LastSpriteType() const;

	__inline void EnableTypes() const;
	__inline void DisableTypes() const;

	__inline void EnableType( int theTypeID ) const;
	__inline void DisableType( int theTypeID ) const;
	__inline SpriteType GetSpriteType( int theTypeID ) const;
	
	SpriteManager();
	~SpriteManager();

	SpriteGroup AddSpriteGroup( SpriteGroupNode *theSpriteGroupNode );
	void RemoveSpriteGroup( SpriteGroup & theSpriteGroup );

	Sprite AddSprite( SpriteNode *theSpriteNode, int theTypeID = -1 );
	void RemoveSprite( Sprite & theSprite );

	Sprite MoveBeforeSprite( Sprite Source, Sprite Target );
	Sprite MoveAfterSprite( Sprite Source, Sprite Target );

	Action AddAction( Sprite theSource, Sprite theTarget, ActionNode *theActionNode );
	void RemoveAction( Action & theAction );

	void Clear();
};

///////////////////////////////////////////////////////////////////////////////////////////////
//	Inline Function definitions	///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

//	ActionNode	///////////////////////////////////////////////////////////////////////////////

ActionNode::ActionNode()
{
	master = NULL;
	next = prev = NULL;

	source = NULL;
	sourceNext = sourcePrev = NULL;

	target = NULL;
	targetNext = targetPrev = NULL;
}

Action ActionNode::GetInterface() const
{
	return Action( (ActionNode *)this );
}

//	Action	///////////////////////////////////////////////////////////////////////////////////

Action::Action( ActionNode *theNodePtr )
{
	nodePtr = theNodePtr;
}

Action::Action()
{
	nodePtr = NULL;
}

Action::operator bool() const
{
	return ( nodePtr != NULL );
}

bool Action::Valid() const
{
	return ( nodePtr != NULL );
}

bool Action::operator == ( Action theAction ) const
{
	return ( nodePtr == theAction.nodePtr );
}

bool Action::operator != ( Action theAction ) const
{
	return ( nodePtr != theAction.nodePtr );
}

ActionNode * Action::operator * () const
{
	return nodePtr;
}

ActionNode * Action::operator -> () const
{
	return nodePtr;
}

Action & Action::operator ++ ()
{
	nodePtr = nodePtr->next;
	return *this;
}

Action & Action::operator -- ()
{
	nodePtr = nodePtr->prev;
	return *this;
}

Action Action::operator ++ (int)
{
	ActionNode *temp = nodePtr;
	nodePtr = nodePtr->next;
	return Action( temp );
}

Action Action::operator -- (int)
{
	ActionNode *temp = nodePtr;
	nodePtr = nodePtr->prev;
	return Action( temp );
}

Action Action::Next() const
{
	return Action( nodePtr->next );
}

Action Action::Prev() const
{
	return Action( nodePtr->prev );
}

Sprite Action::Source() const
{
	return Sprite( nodePtr->source );
}

Action Action::SourceNext() const
{
	return Action( nodePtr->sourceNext );
}

Action Action::SourcePrev() const
{
	return Action( nodePtr->sourcePrev );
}

Sprite Action::Target() const
{
	return Sprite( nodePtr->target );
}

Action Action::TargetNext() const
{
	return Action( nodePtr->targetNext );
}

Action Action::TargetPrev() const
{
	return Action( nodePtr->targetPrev );
}

//	SpriteNode	///////////////////////////////////////////////////////////////////////////////

Sprite SpriteNode::GetInterface() const
{
	return Sprite( (SpriteNode *)this );
}

//	Sprite	///////////////////////////////////////////////////////////////////////////////////

Sprite::Sprite( SpriteNode *theNodePtr )
{
	nodePtr = theNodePtr;
}

Sprite::Sprite()
{
	nodePtr = NULL;
}

Sprite::operator bool() const
{
	return ( nodePtr != NULL );
}

bool Sprite::Valid() const
{
	return ( nodePtr != NULL );
}

bool Sprite::operator == ( Sprite theSprite ) const
{
	return ( nodePtr == theSprite.nodePtr );
}

bool Sprite::operator != ( Sprite theSprite ) const
{
	return ( nodePtr != theSprite.nodePtr );
}

SpriteNode * Sprite::operator * () const
{
	return nodePtr;
}

SpriteNode * Sprite::operator -> () const
{
	return nodePtr;
}

Sprite & Sprite::operator ++ ()
{
	nodePtr = nodePtr->next;
	return *this;
}

Sprite & Sprite::operator -- ()
{
	nodePtr = nodePtr->prev;
	return *this;
}

Sprite Sprite::operator ++ (int)
{
	SpriteNode *temp = nodePtr;
	nodePtr = nodePtr->next;
	return Sprite( temp );
}

Sprite Sprite::operator -- (int)
{
	SpriteNode *temp = nodePtr;
	nodePtr = nodePtr->prev;
	return Sprite( temp );
}

SpriteManager *Sprite::Master() const
{
	return ( nodePtr->master );
}

Sprite Sprite::Next() const
{
	return Sprite( nodePtr->next );
}

Sprite Sprite::Prev() const
{
	return Sprite( nodePtr->prev );
}

SpriteGroup Sprite::Group() const
{
	return SpriteGroup( nodePtr->groupList );
}

Sprite Sprite::GroupNext() const
{
	return Sprite( nodePtr->groupNext );
}

Sprite Sprite::GroupPrev() const
{
	return Sprite( nodePtr->groupPrev );
}

SpriteType Sprite::Type() const
{
	return SpriteType( nodePtr->typeList );
}

Sprite Sprite::TypeNext() const
{
	return Sprite( nodePtr->typeNext );
}

Sprite Sprite::TypePrev() const
{
	return Sprite( nodePtr->typePrev );
}

int Sprite::NumSourceActions() const
{
	return ( nodePtr->numSourceActions );
}

Action Sprite::FirstSourceAction() const
{
	return Action( nodePtr->firstSourceAction );
}

Action Sprite::LastSourceAction() const
{
	return Action( nodePtr->lastSourceAction );
}

int Sprite::NumTargetActions() const
{
	return ( nodePtr->numTargetActions );
}

Action Sprite::FirstTargetAction() const
{
	return Action( nodePtr->firstTargetAction );
}

Action Sprite::LastTargetAction() const
{
	return Action( nodePtr->lastTargetAction );
}

//	SpriteTypeNode	///////////////////////////////////////////////////////////////////////////

SpriteTypeNode::SpriteTypeNode( int theTypeID )
{
	typeID = theTypeID;

	prev = next = NULL;

	numSprites = 0;
	firstSprite = lastSprite = NULL;

	Enabled = true;
}

//	SpriteType	///////////////////////////////////////////////////////////////////////////////

SpriteType::SpriteType( SpriteTypeNode *theNodePtr )
{
	nodePtr = theNodePtr;
}

void SpriteType::AddSprite( Sprite theSprite )
{
	// check if sprite is valid
	if ( !theSprite.Valid() ) return;

	// cache sprite node pointer
	SpriteNode *theSpriteNode = theSprite.nodePtr;
	
	// pointer to list
	theSpriteNode->typeList = nodePtr;

	// insert into sprite list
	theSpriteNode->typePrev = nodePtr->lastSprite;

	( nodePtr->lastSprite != NULL ) ? ( nodePtr->lastSprite->typeNext = theSpriteNode )
									: ( nodePtr->firstSprite = theSpriteNode );
	
	nodePtr->lastSprite = theSpriteNode;

	nodePtr->numSprites++;
}

void SpriteType::RemoveSprite( Sprite theSprite )
{
	// check if sprite is valid
	if ( !theSprite.Valid() ) return;
	
	// cache sprite node pointer
	SpriteNode *theSpriteNode = theSprite.nodePtr;
	
	// remove from sprite list
	( theSpriteNode->typePrev != NULL )
		? ( theSpriteNode->typePrev->typeNext = theSpriteNode->typeNext )
		: ( nodePtr->firstSprite = theSpriteNode->typeNext );

	( theSpriteNode->typeNext != NULL )
		? ( theSpriteNode->typeNext->typePrev = theSpriteNode->typePrev )
		: ( nodePtr->lastSprite = theSpriteNode->typePrev );
	
	theSpriteNode->typeList = NULL;
	
	nodePtr->numSprites--;
}

void SpriteType::Clear()
{
	Sprite SpriteIterator = FirstSprite();
	Sprite Temp;
	
	while ( SpriteIterator )
	{
		Temp = SpriteIterator.TypeNext();
		RemoveSprite( SpriteIterator );
		SpriteIterator = Temp;
	}
}

SpriteType::SpriteType()
{
	nodePtr = NULL;
}

SpriteType::operator bool() const
{
	return ( nodePtr != NULL );
}

bool SpriteType::Valid() const
{
	return ( nodePtr != NULL );
}

bool SpriteType::operator == ( SpriteType theSpriteType ) const
{
	return ( nodePtr == theSpriteType.nodePtr );
}

bool SpriteType::operator != ( SpriteType theSpriteType ) const
{
	return ( nodePtr != theSpriteType.nodePtr );
}

SpriteTypeNode * SpriteType::operator * () const
{
	return nodePtr;
}

SpriteTypeNode * SpriteType::operator -> () const
{
	return nodePtr;
}

SpriteType & SpriteType::operator ++ ()
{
	nodePtr = nodePtr->next;
	return *this;
}

SpriteType & SpriteType::operator -- ()
{
	nodePtr = nodePtr->prev;
	return *this;
}

SpriteType SpriteType::operator ++ (int)
{
	SpriteTypeNode *temp = nodePtr;
	nodePtr = nodePtr->next;
	return SpriteType( temp );
}

SpriteType SpriteType::operator -- (int)
{
	SpriteTypeNode *temp = nodePtr;
	nodePtr = nodePtr->prev;
	return SpriteType( temp );
}

SpriteType SpriteType::Next() const
{
	return SpriteType( nodePtr->next );
}

SpriteType SpriteType::Prev() const
{
	return SpriteType( nodePtr->prev );
}

int SpriteType::TypeID() const
{
	return ( nodePtr->typeID );
}

int SpriteType::NumSprites() const
{
	return ( nodePtr->numSprites );
}

Sprite SpriteType::FirstSprite() const
{
	return Sprite( nodePtr->firstSprite );
}

Sprite SpriteType::LastSprite() const
{
	return Sprite( nodePtr->lastSprite );
}

void SpriteType::Enable()
{
	nodePtr->Enabled = true;
}

void SpriteType::Disable()
{
	nodePtr->Enabled = false;
}

bool SpriteType::Enabled() const
{
	return ( nodePtr->Enabled );
}

//	SpriteGroupNode	///////////////////////////////////////////////////////////////////////////

SpriteGroupNode::SpriteGroupNode()
{
	prev = next = NULL;
	
	numSprites = 0;
	firstSprite = lastSprite = NULL;
}

//	SpriteGroup	///////////////////////////////////////////////////////////////////////////////

SpriteGroup::SpriteGroup( SpriteGroupNode *theNodePtr )
{
	nodePtr = theNodePtr;
}

SpriteGroup::SpriteGroup()
{
	nodePtr = NULL;
}

void SpriteGroup::AddSprite( Sprite theSprite )
{
	// check if sprite is valid
	if ( !theSprite.Valid() ) return;
	
	// cache sprite node pointer
	SpriteNode *theSpriteNode = theSprite.nodePtr;

	// pointer to list
	theSpriteNode->groupList = nodePtr;

	// insert into sprite list
	theSpriteNode->groupPrev = nodePtr->lastSprite;

	( nodePtr->lastSprite != NULL )
		? ( nodePtr->lastSprite->groupNext = theSpriteNode )
		: ( nodePtr->firstSprite = theSpriteNode );
	
	nodePtr->lastSprite = theSpriteNode;

	nodePtr->numSprites++;
}

void SpriteGroup::RemoveSprite( Sprite theSprite )
{
	// check if sprite is valid
	if ( !theSprite.Valid() ) return;
	
	// cache sprite node pointer
	SpriteNode *theSpriteNode = theSprite.nodePtr;
	
	// remove from sprite list
	( theSpriteNode->groupPrev != NULL )
		? ( theSpriteNode->groupPrev->groupNext = theSpriteNode->groupNext )
		: ( nodePtr->firstSprite = theSpriteNode->groupNext );

	( theSpriteNode->groupNext != NULL )
		? ( theSpriteNode->groupNext->groupPrev = theSpriteNode->groupPrev )
		: ( nodePtr->lastSprite = theSpriteNode->groupPrev );
	
	theSpriteNode->groupList = NULL;

	nodePtr->numSprites--;
}

void SpriteGroup::Clear()
{
	Sprite SpriteIterator = FirstSprite();
	Sprite Temp;
	
	while ( SpriteIterator )
	{
		Temp = SpriteIterator.GroupNext();
		RemoveSprite( SpriteIterator );
		SpriteIterator = Temp;
	}
}

SpriteGroup::operator bool() const
{
	return ( nodePtr != NULL );
}

bool SpriteGroup::Valid() const
{
	return ( nodePtr != NULL );
}

bool SpriteGroup::operator == ( SpriteGroup theSpriteGroup ) const
{
	return ( nodePtr == theSpriteGroup.nodePtr );
}

bool SpriteGroup::operator != ( SpriteGroup theSpriteGroup ) const
{
	return ( nodePtr != theSpriteGroup.nodePtr );
}

SpriteGroupNode * SpriteGroup::operator * () const
{
	return nodePtr;
}

SpriteGroupNode * SpriteGroup::operator -> () const
{
	return nodePtr;
}

SpriteGroup & SpriteGroup::operator ++ ()
{
	nodePtr = nodePtr->next;
	return *this;
}

SpriteGroup & SpriteGroup::operator -- ()
{
	nodePtr = nodePtr->prev;
	return *this;
}

SpriteGroup SpriteGroup::operator ++ (int)
{
	SpriteGroupNode *temp = nodePtr;
	nodePtr = nodePtr->next;
	return SpriteGroup( temp );
}

SpriteGroup SpriteGroup::operator -- (int)
{
	SpriteGroupNode *temp = nodePtr;
	nodePtr = nodePtr->prev;
	return SpriteGroup( temp );
}

SpriteGroup SpriteGroup::Next() const
{
	return SpriteGroup( nodePtr->next );
}

SpriteGroup SpriteGroup::Prev() const
{
	return SpriteGroup( nodePtr->prev );
}

int SpriteGroup::NumSprites() const
{
	return ( nodePtr->numSprites );
}

Sprite SpriteGroup::FirstSprite() const
{
	return Sprite( nodePtr->firstSprite );
}

Sprite SpriteGroup::LastSprite() const
{
	return Sprite( nodePtr->lastSprite );
}

//	SpriteManager	///////////////////////////////////////////////////////////////////////////

int SpriteManager::NumActions() const
{
	return numActions;
}

Action SpriteManager::FirstAction() const
{
	return Action( firstAction );
}

Action SpriteManager::LastAction() const
{
	return Action(  lastAction );
}

int SpriteManager::NumSprites() const
{
	return numSprites;
}

Sprite SpriteManager::FirstSprite() const
{
	return Sprite( firstSprite );
}

Sprite SpriteManager::LastSprite() const
{
	return Sprite(  lastSprite );
}

int SpriteManager::NumSpriteGroups() const
{
	return numSpriteGroups;
}

SpriteGroup SpriteManager::FirstSpriteGroup() const
{
	return SpriteGroup( firstSpriteGroup );
}

SpriteGroup SpriteManager::LastSpriteGroup() const
{
	return SpriteGroup(  lastSpriteGroup );
}

int SpriteManager::NumSpriteTypes() const
{
	return numSpriteTypes;
}

SpriteType SpriteManager::FirstSpriteType() const
{
	return SpriteType( firstSpriteType );
}

SpriteType SpriteManager::LastSpriteType() const
{
	return SpriteType(  lastSpriteType );
}

void SpriteManager::EnableTypes() const
{
	SpriteType CurrentType = FirstSpriteType();

	while ( CurrentType )
	{
		CurrentType.Enable();
		CurrentType++;
	}
}

void SpriteManager::DisableTypes() const
{
	SpriteType CurrentType = FirstSpriteType();

	while ( CurrentType )
	{
		CurrentType.Disable();
		CurrentType++;
	}
}

void SpriteManager::EnableType( int theTypeID ) const
{
	SpriteType CurrentType = FirstSpriteType();

	while ( CurrentType )
	{
		if ( CurrentType.TypeID() == theTypeID )
		{
			CurrentType.Enable();
			break;
		}

		CurrentType++;
	}
}

void SpriteManager::DisableType( int theTypeID ) const
{
	SpriteType CurrentType = FirstSpriteType();

	while ( CurrentType )
	{
		if ( CurrentType.TypeID() == theTypeID )
		{
			CurrentType.Disable();
			break;
		}

		CurrentType++;
	}
}

SpriteType SpriteManager::GetSpriteType( int theTypeID ) const
{
	SpriteType CurrentType = FirstSpriteType();

	while ( CurrentType )
	{
		if ( CurrentType.TypeID() == theTypeID ) break;
		
		CurrentType++;
	}

	return CurrentType;
}

#endif
