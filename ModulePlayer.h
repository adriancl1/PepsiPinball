#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "Animation.h"

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
	void Addmultiply();
	void AddPoints(int add);
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	Object ball;

	SDL_Rect multipliers[6];
	SDL_Rect ballcount;

	Object leftkicker;
	PhysBody* leftwheel;

	Object rightkicker;
	PhysBody* rightwheel;

	Object rightkicker2;
	PhysBody* rightwheel2;

	SDL_Texture* underball;
	SDL_Texture* overball;
	SDL_Texture* graphics;

	Animation horse;

	uint death_fx;
	uint kicker_fx;
	uint horse_fx;

	int multiply;
	int lifes;
	int points;
	int score1;
	int ballx;
	int bally;
	int current_time;
	int last_time;
	int death_time;

public:

};