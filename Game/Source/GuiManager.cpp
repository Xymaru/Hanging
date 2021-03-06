#include "GuiManager.h"
#include "App.h"
#include "Textures.h"

#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckbox.h"
#include "Audio.h"

GuiManager::GuiManager() :Module()
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Texture* texture, SDL_Rect sliderBounds)
{
	// L14: TODO1: Create a GUI control and add it to the list of controls

	GuiControl* control = nullptr;

	//Call the constructor according to the GuiControlType
	switch (type)
	{
	case GuiControlType::BUTTON:
		control = new GuiButton(id, bounds, texture);
		break;
	case GuiControlType::SLIDER:
		control = new GuiSlider(id, bounds, sliderBounds, texture);
		break;
	case GuiControlType::CHECKBOX:
		control = new GuiCheckbox(id, bounds, texture);
		break;
	default:
		break;
	}

	//Set the observer

	control->SetObserver(observer);
	//control->SetTexture(texture);

	// Created GuiControls are added to the list of controls
	if (control != nullptr) controls.Add(control);

	return control;
}

GuiButton* GuiManager::CreateButton(int id, SDL_Rect bounds, SDL_Texture* texture, Module* observer)
{
	GuiButton* btn = new GuiButton(id, bounds, texture);
	btn->SetObserver(observer);

	controls.Add(btn);

	return btn;
}

GuiCheckbox* GuiManager::CreateCheckbox(int id, SDL_Rect bounds, SDL_Texture* texture, Module* observer)
{
	GuiCheckbox* btn = new GuiCheckbox(id, bounds, texture);
	btn->SetObserver(observer);

	controls.Add(btn);

	return btn;
}

GuiSlider* GuiManager::CreateSlider(int id, SDL_Rect bounds, SDL_Rect slider_btn, SDL_Texture* texture, Module* observer)
{
	GuiSlider* btn = new GuiSlider(id, bounds, slider_btn, texture);
	btn->SetObserver(observer);

	controls.Add(btn);

	return btn;
}

bool GuiManager::Update(float dt)
{	
	if (clear) {
		clear = false;
		CleanUp();
	}

	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	UpdateAll(dt,doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}
	
	return true;
}

bool GuiManager::UpdateAll(float dt, bool doLogic) {

	if (doLogic) {

		ListItem<GuiControl*>* control = controls.start;

		while (control != nullptr)
		{
			control->data->Update(dt);
			control = control->next;
		}

	}
	return true; 

}

bool GuiManager::Draw() {

	ListItem<GuiControl*>* control = controls.start;

	while (control != nullptr)
	{
		control->data->Draw(app);
		control = control->next;
	}

	return true;

}

bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* control = controls.start;

	while (control != nullptr)
	{
		RELEASE(control->data);
		control = control->next;
	}

	controls.Clear();

	return true;
}



