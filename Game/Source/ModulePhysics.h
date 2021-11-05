#pragma once
#include "Module.h"
#include "Box2D/Box2D/Box2D.h"
#include "App.h"

#define DEFAULT_GRAVITY 9.8f

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * (m)))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * (p))

enum class PhysBodyType {
	PLAYER,
	GROUND,
	SPIKES,
	END,
	PLATFORM,
	NEUTRAL
};
// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
	PhysBodyType bodyType;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics();
	~ModulePhysics();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, bool dynamic);
	PhysBody* CreateRectangle(int x, int y, int width, int height, bool dynamic);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, bool dynamic);
	PhysBody* CreateChain(int x, int y, int* points, int size, bool dynamic);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

	void ReStart();

private:
	float gravity;

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
};