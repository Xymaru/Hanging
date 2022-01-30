#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "PerfTimer.h"
#include "Timer.h"

#include "PugiXml/src/pugixml.hpp"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class LogoScene;
class GameScene;
class PlayerModule;
class Map;
class ModulePhysics;
class MainMenu;
class StageSelect;
class Fonts;
class ModuleFadeToBlack;
class EndScene;
class PathFinding;
class EntityModule;
class GuiManager;
class ControlsScene;
class CreditsScene;
class SettingsScene;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module, bool active);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	bool IsDebugMode() { return debug; }

private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	LogoScene* logoScene;
	GameScene* gameScene;
	PlayerModule* playerModule;
	Map* map;
	ModulePhysics* physics;
	MainMenu* menu;
	Fonts* fonts;
	ModuleFadeToBlack* fade;
	EndScene* endScene;
	StageSelect* stage;
	PathFinding* pathfinding;
	EntityModule* enemies;
	GuiManager* guiManager;
	ControlsScene* controlScene;
	CreditsScene* creditsScene;
	SettingsScene* settingsScene;

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module *> modules;

	// Create new variables from pugui namespace:
	// a xml_document to store the config file and
	// two xml_node to read specific branches of the xml
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	/*unsigned int
		lastTime,
		currentTime;
	float msFrame;

	float dt;*/
	PerfTimer* ptimer;
	PerfTimer* frameDuration;

	Timer startupTime;
	Timer frameTime;
	Timer lastSecFrameTime;

	uint64 frameCount = 0;
	uint32 framesPerSecond = 0;
	uint32 lastSecFrameCount = 0;

	float averageFps = 0.0f;
	float dt = 0.0f;

	bool debug;

	uint32 maxFrameRate = 0;
};

extern App* app;

#endif	// __APP_H__