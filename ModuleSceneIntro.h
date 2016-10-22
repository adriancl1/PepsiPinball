#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* backgroundstage;
	SDL_Texture* leftkicker;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
