#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

struct SDL_Texture;

class GuiSlider : public GuiControl
{
private:
	SDL_Texture* texture;
	SDL_Rect slider;
public:

	GuiSlider(uint32 id, SDL_Rect slider, SDL_Rect slider_btn, SDL_Texture* texture);
	virtual ~GuiSlider();

	bool Update(float dt);
	bool Draw(App* render);

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

	float GetValue() { return float(slider.x) / (bounds.x + bounds.w); }
};

#endif // __GUIBUTTON_H__