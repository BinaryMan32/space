#ifndef WORLD_H
#define WORLD_H

#include "spritemanager.h"
#include "particles.h"

class World : public SpriteManager
{
	GLUquadricObj* QuadricObject;

	public:

	float MaxRadius;
	
	World();
	virtual ~World();
	
	Sprite GetClosest( Sprite theSprite );
	Sprite GetClosestType( Sprite theSprite );
	Sprite GetClosestTypeFront( Sprite theSprite );
		
	void SortSprites();
	void Collisions();
	void Actions();
	void Move();
	void RemoveDead();
	void Draw();
	void DrawMini();
	void PostDraw();
	void ShowFrameRate();
	
	void KeepInBox( float WorldSize );
	void DrawBox( float WorldSize );

	void Tick();
};

extern World GameWorld;

#endif
