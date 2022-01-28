#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiSlider : public GuiControl
{
public:

	GuiSlider(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GuiSlider();

	bool Update(float dt);
	bool Draw(App* render);

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

	SDL_Rect bar;
	SDL_Rect slider;

	float GetValue() { return float(slider.x) / (bounds.x + bounds.w); }
};

#endif // __GUIBUTTON_H__