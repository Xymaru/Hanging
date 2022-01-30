#ifndef __CONTROLSCENE_H__
#define __CONTROLSCENE_H__

#include "Module.h"

struct SDL_Texture;

class ControlsScene : public Module
{
public:

	ControlsScene();

	// Destructor
	virtual ~ControlsScene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	bool OnGuiMouseClickEvent(GuiControl* control) override;
private:
	SDL_Texture* background;
};

#endif // __SCENE_H__