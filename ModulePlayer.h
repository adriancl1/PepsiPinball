#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"


class PhysBody;
class SDL_Texture;

struct Object {
	SDL_Texture* texture;
	PhysBody* body;
	uint fx;

	Object(): texture(NULL), body(NULL){}
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

	//SDL_Texture* balltexture;

	int ballx;
	int bally;

public:

};