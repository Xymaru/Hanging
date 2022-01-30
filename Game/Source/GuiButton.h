#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

struct SDL_Texture;

class GuiButton : public GuiControl
{
private:
	SDL_Texture* texture;
public:

	GuiButton(uint32 id, SDL_Rect bounds, SDL_Texture* texture);
	virtual ~GuiButton();

	bool Update(float dt);
	bool Draw(App* app);

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;
};

#endif // __GUIBUTTON_H__