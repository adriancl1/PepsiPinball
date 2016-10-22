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
	void CreateStage();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	p2List<PhysBody*> to_delete;//LIST TO DELETE BODIES OUTISDE WORLDSTEP

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> cowboys;
	p2List<PhysBody*> hats;
	PhysBody* leftbarrels;
	PhysBody* rightbarrels;

	SDL_Rect texcoords;
	PhysBody* sensor;
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* graphics;
	SDL_Rect lights[11];
	uint bonus_fx;
	uint cowboy_fx;
	uint hat_fx;
	uint barrel_fx;
	p2Point<int> ray;
	bool ray_on;
};
