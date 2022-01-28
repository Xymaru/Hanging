#include "GuiSlider.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	bar = {
		bounds.x,
		bounds.y + (bounds.h / 2) - bounds.h / 6,
		bounds.w,
		bounds.h / 3
	};

	slider = {
		bounds.x,
		bounds.y,
		4,
		bounds.h
	};
}

GuiSlider::~GuiSlider()
{

}

bool GuiSlider::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// L14: TODO 3: Update the state of the GUiButton according to the mouse position
		int mouseX, mouseY;
		app->input->GetMousePosition(mouseX, mouseY);

		if ((mouseX > bounds.x ) && (mouseX < (bounds.x + bounds.w )) &&
			(mouseY > bounds.y ) && (mouseY < (bounds.y + bounds.h )))
		{
			state = GuiControlState::FOCUSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
				slider.x = mouseX;
			}

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				NotifyObserver();
			}
		}
		else state = GuiControlState::NORMAL;
	}

	return false;
}

bool GuiSlider::Draw(App* app)
{
	Font& fontobj = app->fonts->GetFont(font);
	int textXOffset = bounds.w / 2 - fontobj.char_w * text.Length() / 2;
	int textYOffset = bounds.h / 2 - fontobj.char_h / 2;

	// Draw the right button depending on state
	switch (state)
	{

	case GuiControlState::DISABLED:
	{
		int avg = color.r * 0.299 + color.g * 0.587 + color.b * 0.114;
		app->render->DrawRectangle(bar, avg, avg, avg, color.a);
		app->render->DrawRectangle(slider, avg, avg, avg, color.a);
	} break;

	case GuiControlState::NORMAL:
	{
		app->render->DrawRectangle(bar, color.r, color.g, color.b, color.a);
		app->render->DrawRectangle(slider, color.r, color.g, color.b, color.a);
	} break;

	//L14: TODO 4: Draw the button according the GuiControl State
	case GuiControlState::FOCUSED:
	{
		app->render->DrawRectangle(bar, color.r, color.g, color.b, 160);
		app->render->DrawRectangle(slider, color.r, color.g, color.b, 160);
	} break;
	case GuiControlState::PRESSED:
	{
		app->render->DrawRectangle(bar, color.r / 2, color.g / 2, color.b / 2, 255);
		app->render->DrawRectangle(slider, color.r / 2, color.g / 2, color.b / 2, 255);
	} break;

	/******/

	case GuiControlState::SELECTED: app->render->DrawRectangle(bar, 0, 255, 0, 255);
		break;

	default:
		break;
	}

	app->fonts->BlitText(bounds.x + textXOffset, bounds.y + textYOffset, font, text.GetString());

	return false;
}