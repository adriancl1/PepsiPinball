#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

#include "SDL\include\SDL.h"

class PhysBody;

#define KICKER_REST 0.05
struct Object {

	SDL_Rect section;
	PhysBody* body;
	uint fx;

	Object(): body(NULL){}
};

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	Object ball;

	Object leftkicker;
	PhysBody* leftwheel;

	Object rightkicker;
	PhysBody* rightwheel;

	Object rightkicker2;
	PhysBody* rightwheel2;

	SDL_Texture* underball;
	SDL_Texture* overball;
	SDL_Texture* graphics;

	uint death_fx;
	uint kicker_fx;

	int ballx;
	int bally;

public:

};