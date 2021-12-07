#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "LogoScene.h"
#include "GameScene.h"
#include "PlayerModule.h"
#include "Map.h"
#include "ModulePhysics.h"
#include "MainMenu.h"
#include "StageSelect.h"
#include "Fonts.h"
#include "EndScene.h"
#include "ModuleFadeToBlack.h"
#include "Pathfinding.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	win = new Window();
	input = new Input();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	logoScene = new LogoScene();
	gameScene = new GameScene();
	playerModule = new PlayerModule();
	map = new Map();
	physics = new ModulePhysics();
	menu = new MainMenu();
	stage = new StageSelect();
	fonts = new Fonts();
	fade = new ModuleFadeToBlack();
	endScene = new EndScene();
	pathfinding = new PathFinding();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win, true);
	AddModule(input, true);
	AddModule(tex, true);
	AddModule(audio, true);
	AddModule(logoScene, true);
	AddModule(gameScene, false);
	AddModule(map, false);
	AddModule(pathfinding, true);
	AddModule(playerModule, false);
	AddModule(physics, false);
	AddModule(menu, false);
	AddModule(stage, false);
	AddModule(fonts, true);
	AddModule(endScene, false);
	AddModule(fade, true);

	// Render last to swap buffer
	AddModule(render, true);

	ptimer = new PerfTimer();
	frameDuration = new PerfTimer();
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module, bool active)
{
	module->Init();
	modules.Add(module);
	module->active = active;
}

// Called before render is available
bool App::Awake()
{
	// TODO 3: Load config from XML
	bool ret = LoadConfig();

	if(ret == true)
	{
		// TODO 4: Read the title from the config file
		title.Create(configApp.child("title").child_value());
		win->SetTitle(title.GetString());

		ListItem<Module*>* item;
		item = modules.start;

		while(item != NULL && ret == true)
		{
			// TODO 5: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
		maxFrameRate = configApp.child("frcap").attribute("value").as_int();
	}

	return ret;
}

// Called before the first frame
bool App::Start()
{
	startupTime.Start();
	lastSecFrameTime.Start();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	//lastTime = SDL_GetTicks();
	//msFrame = 1.0f / FPS;
	//dt = msFrame;

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// Load config from XML file
bool App::LoadConfig()
{
	bool ret = true;

	// TODO 3: Load config.xml file using load_file() method from the xml_document class
	pugi::xml_parse_result result = configFile.load_file("config.xml");

	// TODO 3: Check result for loading errors
	if(result == NULL)
	{
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		config = configFile.child("config");
		configApp = config.child("app");
	}

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	frameCount++;
	lastSecFrameCount++;

	// Calculate the dt: differential time since last frame
	dt = frameDuration->ReadMs();
	frameDuration->Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	// This is a good place to call Load / Save functions

	// FPS Control
	/*currentTime = SDL_GetTicks();

	dt = (currentTime - lastTime) / 1000.0f;

	if (dt < msFrame) {
		SDL_Delay(msFrame - dt);
	}


	lastTime = SDL_GetTicks();*/

	// Now calculate:
	// Amount of frames since startup
	// Amount of time since game start (use a low resolution timer)
	// Amount of ms took the last update
	// Amount of frames during the last second
	// Average FPS for the whole game life
	float secondsSinceStartup = startupTime.ReadSec();

	if (lastSecFrameTime.Read() > 1000) {
		lastSecFrameTime.Start();
		framesPerSecond = lastSecFrameCount;
		lastSecFrameCount = 0;
		averageFps = (averageFps + framesPerSecond) / 2;
	}

	static char title[256];
	bool is_vsync = config.child("vsync").attribute("value").as_bool(true);
	if (is_vsync == true) {
		sprintf_s(title, 256, "FPS: %i Av.FPS: %.2f Last-frame MS: %I64u Vsync: on",
			framesPerSecond, averageFps, frameCount);
	}
	else {
		sprintf_s(title, 256, "FPS: %i Av.FPS: %.2f Last-frame MS: %I64u Vsync: off",
			framesPerSecond, averageFps, frameCount);
	}

	//Use SDL_Delay to make sure you get your capped framerate
	float delay = float(maxFrameRate) - frameDuration->ReadMs();
	//LOG("F: %f Delay:%f", frameDuration->ReadMs(), delay);

	//Measure accurately the amount of time SDL_Delay() actually waits compared to what was expected
	PerfTimer* delayt = new PerfTimer();
	delayt->Start();
	if (maxFrameRate > 0 && delay > 0) SDL_Delay(delay);
	LOG("Expected %f milliseconds and the real delay is % f", delay, delayt->ReadMs());

	app->win->SetTitle(title);
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;

	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate(dt);
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate(dt);
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}