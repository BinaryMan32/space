#ifndef OBJECTS_H
#define OBJECTS_H

#include "World.h"
#include "StarField.h"

#define CameraID 1
class Camera : public SpriteNode
{
	StarField Stars;

	public:

	Camera();

	virtual void OnTick();
};

#define AsteroidID	100
class Asteroid : public SpriteNode
{
	static float MassArray[4];
	static float RadiusArray[4];
	static char *ImageArray[4];

	int size;

	public:
	
	Asteroid( int theSize = 3 );

	virtual void OnStateChange();
	virtual void OnTick();
};

#define PlayerShipID 101
class PlayerShip : public SpriteNode
{
	GLUquadricObj *QuadricObject;
	float MaxGunEnergy, GunEnergy;
	float MaxFireDelay, FireDelay;

	public:

	PlayerShip();
	~PlayerShip();

	virtual void OnStateChange();
	virtual void OnTick();
	virtual bool OnCollide( SpriteBase *otherSprite );
	virtual void OnDraw();
	virtual void PostDraw();
};

#define HealthID 102
class Health : public SpriteNode
{
	public:

	Health();

	virtual void OnTick();
	virtual bool OnCollide( SpriteBase *otherSprite );
};

#define RedLaserID 200
class RedLaser : public SpriteNode
{
	float TimeToLive;
	float TrailTime;

	public:

	RedLaser();

	virtual void OnStateChange();
	virtual void OnTick();
};

#define RedExplosionID 300
class RedExplosion : public SpriteNode
{
	public:
	
	RedExplosion();

	virtual void OnStateChange();
	virtual void OnTick();
};

#define SmallExplosionID 301
class SmallExplosion : public SpriteNode
{
	public:

	SmallExplosion();

	virtual void OnStateChange();
	virtual void OnTick();
};

#define LargeExplosionID 302
class LargeExplosion : public SpriteNode
{
	public:

	LargeExplosion();

	virtual void OnStateChange();
	virtual void OnTick();
};

#define FlareID 303
class Flare : public SpriteNode
{
	public:

	Flare();

	virtual void OnTick();
};

//	Actions	///////////////////////////////////////////////////////////////////////////////////

class CameraLock : public ActionNode
{
	public:

	void OnTick();
};

class FollowSprite : public ActionNode
{
	public:

	void OnTick();
};

#endif