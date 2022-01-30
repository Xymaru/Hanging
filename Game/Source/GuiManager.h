#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GuiControl.h"

#include "List.h"

struct SDL_Texture;

class GuiButton;
class GuiCheckbox;
class GuiSlider;

class GuiManager : public Module
{
public:

	// Constructor
	GuiManager();

	// Destructor
	virtual ~GuiManager();

	// Called before the first frame
	 bool Start();

	 bool Update(float dt);

	bool UpdateAll(float dt,bool logic);

	bool Draw();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Texture* texture=NULL, SDL_Rect sliderBounds = { 0,0,0,0 });
	GuiButton* CreateButton(int id, SDL_Rect bounds, SDL_Texture* texture, Module* observer);
	GuiCheckbox* CreateCheckbox(int id, SDL_Rect bounds, SDL_Texture* texture, Module* observer);
	GuiSlider* CreateSlider(int id, SDL_Rect bounds, SDL_Rect slider_btn, SDL_Texture* texture, Module* observer);

	void DestroyGuiControl(GuiControl* entity);
	void AddGuiControl(GuiControl* entity);

public:

	List<GuiControl*> controls;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;

	SDL_Texture* texture;
	bool clear = false;
};

#endif // __GUIMANAGER_H__
