#include "GuiButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, SDL_Texture* texture) : GuiControl(GuiControlType::BUTTON, id)
{
	this->texture = texture;
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;
}

GuiButton::~GuiButton()
{

}

bool GuiButton::Update(float dt)
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

bool GuiButton::Draw(App* app)
{
	/*Font& fontobj = app->fonts->GetFont(font);
	int textXOffset = bounds.w / 2 - fontobj.char_w * text.Length() / 2;
	int textYOffset = bounds.h / 2 - fontobj.char_h / 2;*/

	// Draw the right button depending on state
	//switch (state)
	//{

	//case GuiControlState::DISABLED: 
	//{
	//	int avg = color.r * 0.299 + color.g * 0.587 + color.b * 0.114;
	//	app->render->DrawRectangle(bounds, avg, avg, avg, color.a);

	//} break;

	//case GuiControlState::NORMAL:
	//{
	//	app->render->DrawRectangle(bounds, color.r, color.g, color.b, color.a);
	//} break;

	////L14: TODO 4: Draw the button according the GuiControl State
	//case GuiControlState::FOCUSED:
	//{
	//	app->render->DrawRectangle(bounds, color.r, color.g, color.b, 160);
	//} break;
	//case GuiControlState::PRESSED:
	//{
	//	app->render->DrawRectangle(bounds, color.r / 2, color.g / 2, color.b / 2, 255);
	//} break;

	///******/

	//case GuiControlState::SELECTED: app->render->DrawRectangle(bounds, 0, 255, 0, 255);
	//	break;

	//default:
	//	break;
	//}
	//
	//app->render->DrawTexture(texture, bounds.x, bounds.y, &section);
	//app->fonts->BlitText(bounds.x + textXOffset, bounds.y + textYOffset, font, text.GetString());

	SDL_Rect section = { 0,bounds.h * (int)state,bounds.w,bounds.h };
	app->render->DrawTexture(texture, bounds.x, bounds.y, &section);

	return false;
}