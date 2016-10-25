#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

class PhysBody;

#define HAT_REST 0.5
#define COWBOY_REST 0.2
#define BARRELS_REST 0.2


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
	void Draw();
	void RiseCowboys();
	p2List<PhysBody*> to_delete;//LIST TO DELETE BODIES OUTISDE WORLDSTEP

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> cowboys;
	uint deadcowboys;
	p2List<PhysBody*> hats;
	PhysBody* leftbarrels;
	PhysBody* rightbarrels;

	PhysBody* leftflag;
	SDL_Rect leftflagtex;
	bool lefton;
	PhysBody* midleflag;
	SDL_Rect midleflagtex;
	bool middleon;
	PhysBody* rightflag;
	SDL_Rect rightflagtex;
	bool righton;

	SDL_Rect texcoords;
	PhysBody* sensor;
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* graphics;
	SDL_Rect lights[11];

	Animation redwindows;

	uint bonus_fx;
	uint cowboy_fx;
	uint hat_fx;
	uint barrel_fx;
	uint deadcowboys_fx;

	int current_time;
	int last_time;

	p2Point<int> ray;
	bool ray_on;
};
