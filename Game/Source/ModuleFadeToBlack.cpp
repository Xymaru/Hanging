#include "ModuleFadeToBlack.h"

#include "App.h"
#include "Render.h"
#include "Window.h"

#include "SDL/include/SDL_render.h"

#include <iostream>

ModuleFadeToBlack::ModuleFadeToBlack() : Module()
{
	screenRect = { 0, 0, 640 , 612 };
}

ModuleFadeToBlack::~ModuleFadeToBlack()
{

}

bool ModuleFadeToBlack::Start()
{
	LOG("Preparing Fade Screen");

	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool ModuleFadeToBlack::Update(float dt)
{
	bool ret = true;
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return ret;

	if (currentStep == Fade_Step::TO_BLACK)
	{
		timer += dt;
		if (timer >= time)
		{
			moduleToDisable->Deactivate();
			moduleToDisable->Disable();

			moduleToEnable->Enable();
			moduleToEnable->Activate();
			currentStep = Fade_Step::FROM_BLACK;
		}
	}
	else
	{
		timer -= dt;
		if (timer <= 0.0f)
		{
			currentStep = Fade_Step::NONE;
		}
	}

	return ret;
}

bool ModuleFadeToBlack::PostUpdate(float dt)
{
	bool ret = true;
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return ret;

	float fadeRatio = timer / time;

	fadeRatio = CAP(fadeRatio, 0.0f, 1.0f);

	// Render the black square with alpha on the screen
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screenRect);

	return ret;
}

bool ModuleFadeToBlack::FadeToBlack(Module* moduleToDisable, Module* moduleToEnable, float fadeTime, bool instant_out)
{
	bool ret = false;

	// If we are already in a fade process, ignore this call
	if(currentStep == Fade_Step::NONE)
	{
		currentStep = Fade_Step::TO_BLACK;
		timer = 0;
		time = fadeTime;

		this->moduleToDisable = moduleToDisable;
		this->moduleToEnable = moduleToEnable;

		if (instant_out) {
			timer = fadeTime;

			moduleToDisable->Disable();
			moduleToEnable->Enable();

			moduleToDisable->Deactivate();
			moduleToEnable->Activate();
			
			currentStep = Fade_Step::FROM_BLACK;
		}

		ret = true;
	}

	return ret;
}