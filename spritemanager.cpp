#include "spritemanager.h"

//	SpriteNode	///////////////////////////////////////////////////////////////////////////////

SpriteNode::SpriteNode()
: SpriteBase()
{
	master = NULL;
	next = prev = NULL;
	
	groupList = NULL;
	groupNext = groupPrev = NULL;

	typeList = NULL;
	typeNext = typePrev = NULL;

	numSourceActions = 0;
	firstSourceAction = lastSourceAction = NULL;
	
	numTargetActions = 0;
	firstTargetAction = lastTargetAction = NULL;
}

SpriteNode::SpriteNode( string FileName )
: SpriteBase( FileName )
{
	master = NULL;
	next = prev = NULL;
	
	groupList = NULL;
	groupNext = groupPrev = NULL;

	typeList = NULL;
	typeNext = typePrev = NULL;

	numSourceActions = 0;
	firstSourceAction = lastSourceAction = NULL;
	
	numTargetActions = 0;
	firstTargetAction = lastTargetAction = NULL;
}

//	SpriteManager	///////////////////////////////////////////////////////////////////////////

SpriteType SpriteManager::AddSpriteType( SpriteTypeNode *theSpriteTypeNode )
{
	// check if sprite type is valid
	if ( theSpriteTypeNode == NULL ) return SpriteType();
	
	// insert into sprite type list
	theSpriteTypeNode->prev = lastSpriteType;

	( lastSpriteType != NULL )
		? ( lastSpriteType->next = theSpriteTypeNode )
		: ( firstSpriteType = theSpriteTypeNode );

	lastSpriteType = theSpriteTypeNode;

	numSpriteTypes++;

	// return the new sprite type
	return SpriteType( theSpriteTypeNode );
}

void SpriteManager::RemoveSpriteType( SpriteType & theSpriteType )
{
	// check if sprite type is valid
	if ( !theSpriteType.Valid() ) return;
	
	// cache sprite type node pointer
	SpriteTypeNode *theSpriteTypeNode = theSpriteType.nodePtr;

	// remove from sprite type list
	( theSpriteTypeNode->prev != NULL )
		? ( theSpriteTypeNode->prev->next = theSpriteTypeNode->next )
		: ( firstSpriteType = theSpriteTypeNode->next );

	( theSpriteTypeNode->next != NULL )
		? ( theSpriteTypeNode->next->prev = theSpriteTypeNode->prev )
		: ( lastSpriteType = theSpriteTypeNode->prev );

	numSpriteTypes--;

	// advance to next node
	theSpriteType.nodePtr = theSpriteTypeNode->next;
	
	// release memory
	delete theSpriteTypeNode;
}

SpriteManager::SpriteManager()
{
	numActions = 0;
	firstAction = lastAction = NULL;

	numSprites = 0;
	firstSprite = lastSprite = NULL;

	numSpriteGroups = 0;
	firstSpriteGroup = lastSpriteGroup = NULL;

	numSpriteTypes = 0;
	firstSpriteType = lastSpriteType = NULL;
}

SpriteManager::~SpriteManager()
{
	Clear();
}

SpriteGroup SpriteManager::AddSpriteGroup( SpriteGroupNode *theSpriteGroupNode )
{
	// check if sprite group is valid
	if ( theSpriteGroupNode == NULL ) return SpriteGroup();
	
	// insert into sprite group list
	theSpriteGroupNode->prev = lastSpriteGroup;

	( lastSpriteGroup != NULL )
		? ( lastSpriteGroup->next = theSpriteGroupNode )
		: ( firstSpriteGroup = theSpriteGroupNode );

	lastSpriteGroup = theSpriteGroupNode;

	numSpriteGroups++;

	// return the new sprite group
	return SpriteGroup( theSpriteGroupNode );
}

void SpriteManager::RemoveSpriteGroup( SpriteGroup & theSpriteGroup )
{
	// check if sprite group is valid
	if ( !theSpriteGroup.Valid() ) return;
	
	// cache sprite group node pointer
	SpriteGroupNode *theSpriteGroupNode = theSpriteGroup.nodePtr;
	
	// remove from sprite group list
	( theSpriteGroupNode->prev != NULL )
		? ( theSpriteGroupNode->prev->next = theSpriteGroupNode->next )
		: ( firstSpriteGroup = theSpriteGroupNode->next );

	( theSpriteGroupNode->next != NULL )
		? ( theSpriteGroupNode->next->prev = theSpriteGroupNode->prev )
		: ( lastSpriteGroup = theSpriteGroupNode->prev );

	numSpriteGroups--;

	// advance to next node
	theSpriteGroup.nodePtr = theSpriteGroupNode->next;

	// release memory
	delete theSpriteGroupNode;
}

Sprite SpriteManager::AddSprite( SpriteNode *theSpriteNode, int theTypeID )
{
	if ( theSpriteNode == NULL ) return Sprite();

	// Insert into main list
	theSpriteNode->master = this;
	theSpriteNode->prev = lastSprite;

	( lastSprite != NULL ) ? ( lastSprite->next = theSpriteNode )
						   : ( firstSprite = theSpriteNode );
	
	lastSprite = theSpriteNode;

	numSprites++;

	// Find the type list
	SpriteType theSpriteType = FirstSpriteType();
	while ( theSpriteType )
	{
		if ( theSpriteType.TypeID() == theTypeID ) break;
		theSpriteType++;
	}

	// Create a new type list if not found
	if ( !theSpriteType.Valid() ) theSpriteType = AddSpriteType( new SpriteTypeNode( theTypeID ) );

	// Insert sprite into type list
	theSpriteType.AddSprite( theSpriteNode );
	
	// return the new Sprite
	return Sprite( theSpriteNode );
}

void SpriteManager::RemoveSprite( Sprite & theSprite )
{
	// check if sprite is valid
	if ( !theSprite.Valid() ) return;

	// remove from group list
	if ( theSprite.Group().Valid() ) theSprite.Group().RemoveSprite( theSprite );
	
	// remove from type list
	theSprite.Type().RemoveSprite( theSprite );

	// cache sprite node pointer
	SpriteNode *theSpriteNode = theSprite.nodePtr;

	// remove from sprite list
	( theSpriteNode->prev != NULL )
		? ( theSpriteNode->prev->next = theSpriteNode->next )
		: ( firstSprite = theSpriteNode->next );
	
	( theSpriteNode->next != NULL )
		? ( theSpriteNode->next->prev = theSpriteNode->prev )
		: ( lastSprite = theSpriteNode->prev );

	numSprites--;

	Action theAction;

	// remove source edges
	while ( ( theAction = theSprite.FirstSourceAction() ) ) RemoveAction( theAction );

	// remove target edges
	while ( ( theAction = theSprite.FirstTargetAction() ) ) RemoveAction( theAction );
	
	// advance to next node
	theSprite.nodePtr = theSpriteNode->next;
	
	// release memory
	delete theSpriteNode;
}

Sprite SpriteManager::MoveBeforeSprite( Sprite Source, Sprite Target )
{
	// make sure source vertex is valid
	if ( !Source.Valid() ) return Sprite();
	
	// cache sprite node pointers
	SpriteNode *sourcePtr = Source.nodePtr;
	SpriteNode *targetPtr = Target.nodePtr;
	
	// save address of next sprite
	SpriteNode *temp = sourcePtr->next;

	// remove old connections
	( sourcePtr->prev != NULL ) ? ( sourcePtr->prev->next = sourcePtr->next )
								: ( firstSprite = sourcePtr->next );

	( sourcePtr->next != NULL ) ? ( sourcePtr->next->prev = sourcePtr->prev )
								: ( lastSprite = sourcePtr->prev );

	// make new connections
	if ( targetPtr != NULL )
	{
		sourcePtr->prev = targetPtr->prev;
		sourcePtr->next = targetPtr;

		( sourcePtr->prev != NULL ) ? ( sourcePtr->prev->next = sourcePtr )
									: ( firstSprite = sourcePtr );

		targetPtr->prev = sourcePtr;
	}
	else
	{
		sourcePtr->prev = lastSprite;
		sourcePtr->next = NULL;

		( lastSprite != NULL ) ? ( lastSprite->next = sourcePtr )
							   : ( firstSprite = sourcePtr );
		
		lastSprite = sourcePtr;
	}

	// return next sprite
	return Sprite( temp );
}

Sprite SpriteManager::MoveAfterSprite( Sprite Source, Sprite Target )
{
	// make sure source vertex is valid
	if ( !Source.Valid() ) return Sprite();
	
	// cache sprite node pointers
	SpriteNode *sourcePtr = Source.nodePtr;
	SpriteNode *targetPtr = Target.nodePtr;
	
	// save address of next sprite
	SpriteNode *temp = sourcePtr->next;

	// remove old connections
	( sourcePtr->prev != NULL ) ? ( sourcePtr->prev->next = sourcePtr->next )
								: ( firstSprite = sourcePtr->next );

	( sourcePtr->next != NULL ) ? ( sourcePtr->next->prev = sourcePtr->prev )
								: ( lastSprite = sourcePtr->prev );

	// make new connections
	if ( targetPtr != NULL )
	{
		sourcePtr->prev = targetPtr;
		sourcePtr->next = targetPtr->next;

		( sourcePtr->next != NULL ) ? ( sourcePtr->next->prev = sourcePtr )
									: ( lastSprite = sourcePtr );

		targetPtr->next = sourcePtr;
	}
	else
	{
		sourcePtr->prev = NULL;
		sourcePtr->next = firstSprite;

		( firstSprite != NULL ) ? ( firstSprite->prev = sourcePtr )
								: ( lastSprite = sourcePtr );
		
		firstSprite = sourcePtr;
	}

	// return next sprite
	return Sprite( temp );
}

Action SpriteManager::AddAction( Sprite theSource, Sprite theTarget, ActionNode *theActionNode )
{
	// check if action is valid
	if ( theActionNode == NULL ) return Action();
	
	// check if source and target sprites are valid
	if ( !theSource.Valid() || !theTarget.Valid() )
	{
		delete theActionNode;
		return Action();
	}
	
	// insert into action list
	theActionNode->master = this;
	theActionNode->prev = lastAction;

	( lastAction != NULL ) ? ( lastAction->next = theActionNode )
						   : ( firstAction = theActionNode );
	
	lastAction = theActionNode;

	numActions++;

	// connect to source sprite
	SpriteNode *sourcePtr = theSource.nodePtr;

	theActionNode->source = sourcePtr;

	theActionNode->sourcePrev = sourcePtr->lastSourceAction;

	( sourcePtr->lastSourceAction != NULL )
		? ( sourcePtr->lastSourceAction->sourceNext = theActionNode )
		: ( sourcePtr->firstSourceAction = theActionNode );

	sourcePtr->lastSourceAction = theActionNode;

	sourcePtr->numSourceActions++;

	// connect to target sprite
	SpriteNode *targetPtr = theTarget.nodePtr;

	theActionNode->target = targetPtr;

	theActionNode->targetPrev = targetPtr->lastTargetAction;

	( targetPtr->lastTargetAction != NULL )
		? ( targetPtr->lastTargetAction->targetNext = theActionNode )
		: ( targetPtr->firstTargetAction = theActionNode );

	targetPtr->lastTargetAction = theActionNode;

	targetPtr->numTargetActions++;

	// return the new action
	return Action( theActionNode );
}

void SpriteManager::RemoveAction( Action & theAction )
{
	// check if action is valid
	if ( !theAction.Valid() ) return;

	// cache action node pointer
	ActionNode *theActionNode = theAction.nodePtr;
	
	// remove from action list
	( theActionNode->prev != NULL )
		? ( theActionNode->prev->next = theActionNode->next )
		: ( firstAction = theActionNode->next );
	
	( theActionNode->next != NULL )
		? ( theActionNode->next->prev = theActionNode->prev )
		: ( lastAction = theActionNode->prev );

	numActions--;

	// remove from source list
	( theActionNode->sourcePrev != NULL )
		? ( theActionNode->sourcePrev->sourceNext = theActionNode->sourceNext )
		: ( theActionNode->source->firstSourceAction = theActionNode->sourceNext );

	( theActionNode->sourceNext != NULL )
		? ( theActionNode->sourceNext->sourcePrev = theActionNode->sourcePrev )
		: ( theActionNode->source->lastSourceAction = theActionNode->sourcePrev );

	theActionNode->source->numSourceActions--;

	// remove from target list
	( theActionNode->targetPrev != NULL )
		? ( theActionNode->targetPrev->targetNext = theActionNode->targetNext )
		: ( theActionNode->target->firstTargetAction = theActionNode->targetNext );

	( theActionNode->targetNext != NULL )
		? ( theActionNode->targetNext->targetPrev = theActionNode->targetPrev )
		: ( theActionNode->target->lastTargetAction = theActionNode->targetPrev );

	theActionNode->target->numTargetActions--;

	// advance to next node
	theAction.nodePtr = theActionNode->next;
	
	// release memory
	delete theActionNode;
}

void SpriteManager::Clear()
{
	Action ActionIterator = FirstAction();
	while ( ActionIterator ) RemoveAction( ActionIterator );

	Sprite SpriteIterator = FirstSprite();
	while ( SpriteIterator ) RemoveSprite( SpriteIterator );

	SpriteType TypeIterator = FirstSpriteType();
	while ( TypeIterator ) RemoveSpriteType( TypeIterator );

	SpriteGroup GroupIterator = FirstSpriteGroup();
	while ( GroupIterator ) RemoveSpriteGroup( GroupIterator );
}