#ifndef __SETTINGSCENE_H__
#define __SETTINGSCENE_H__

#include "Module.h"

struct SDL_Texture;
class GuiCheckbox;
class GuiSlider;

class SettingsScene : public Module
{
private:
	enum GuiID {
		FULLSCREEN,
		VSYNC,
		MUSIC,
		FX,
		SAVE,
		BACK
	};
public:

	SettingsScene();

	// Destructor
	virtual ~SettingsScene();

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

	bool fromGame = false;
private:
	SDL_Texture* background;

	GuiCheckbox *fscreen, *vsync;
	GuiSlider* music, * fx;
};

#endif // __SCENE_H__