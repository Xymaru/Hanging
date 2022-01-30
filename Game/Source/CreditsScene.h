#ifndef __CREDITSSCENE_H__
#define __CREDITSSCENE_H__

#include "Module.h"

struct SDL_Texture;

class CreditsScene : public Module
{
public:

	CreditsScene();

	// Destructor
	virtual ~CreditsScene();

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