#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"

class App;
class PhysBody;
class GuiControl;

class Module
{
public:

	Module() : active(false), isEnabled(false)
	{}

	void Init()
	{
		active = true;
	}

	// Called before render is available
	// L01: DONE 5: Sending config file to all modules
	virtual bool Awake(pugi::xml_node& config)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate(float dt)
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	// Called to activate module
	virtual void Activate() {
		active = true;
	}

	// Called to deactivate module
	virtual void Deactivate() {
		active = false;
	}

	// Called on two bodies collision
	virtual void OnCollision(PhysBody* bodyA, PhysBody* bodyB)
	{
	}

	virtual bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}

public:

	SString name;
	bool active;
	bool isEnabled;

	void Enable() {
		if (!isEnabled)
		{
			isEnabled = true;
			Start();
		}
	}

	// Switches isEnabled and calls CleanUp() method
	void Disable() {
		if (isEnabled)
		{
			isEnabled = false;
			CleanUp();
		}
	}

	inline bool IsEnabled() const { return isEnabled; }

	void EnableAndActivate() {
		Enable();
		Activate();
	}

	void DisableAndDeactivate() {
		Disable();
		Deactivate();
	}

private:
};

#endif // __MODULE_H__