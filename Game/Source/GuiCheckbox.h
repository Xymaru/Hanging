#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

struct SDL_Texture;

class GuiCheckbox : public GuiControl
{
private:
	SDL_Texture* texture;
public:

	GuiCheckbox(uint32 id, SDL_Rect bounds, SDL_Texture* texture);
	virtual ~GuiCheckbox();

	bool Update(float dt);
	bool Draw(App* app);

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

	bool activated;
};

#endif // __GUIBUTTON_H__