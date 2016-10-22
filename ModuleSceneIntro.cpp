#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png"); 
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	backgroundstage = App->textures->Load("pinball/UnderBall.png");
	

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);


	int stage[108] = {
		228, 542,
		248, 480,
		217, 453,
		194, 443,
		148, 419,
		132, 413,
		96, 378,
		75, 347,
		59, 315,
		132, 284,
		193, 296,
		151, 241,
		169, 218,
		205, 264,
		211, 211,
		475, 66,
		497, 66,
		521, 81,
		538, 87,
		592, 101,
		615, 122,
		643, 169,
		649, 194,
		656, 245,
		660, 286,
		658, 344,
		654, 417,
		623, 410,
		630, 332,
		630, 281,
		625, 218,
		606, 166,
		595, 144,
		576, 125,
		558, 119,
		549, 119,
		544, 123,
		538, 135,
		533, 158,
		556, 188,
		557, 236,
		567, 262,
		585, 296,
		596, 328,
		596, 357,
		591, 386,
		580, 400,
		560, 413,
		528, 418,
		494, 425,
		461, 439,
		438, 450,
		397, 481,
		427, 545
	};

	App->physics->CreateChain(0, 0, stage, 108, b2_staticBody);

	//rightisle
	int RightIsle[28] = {
		494, 362,
		510, 286,
		517, 262,
		521, 222,
		524, 243,
		531, 269,
		540, 296,
		556, 315,
		567, 334,
		570, 353,
		556, 373,
		533, 382,
		510, 386,
		494, 373
	};

	App->physics->CreateChain(0, 0, RightIsle, 28, b2_staticBody);


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 9, 0, b2_dynamicBody));
		circles.getLast()->data->listener = this;
	}



	

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	//DRAW MAP

	//App->renderer->Blit(backgroundstage, 0, 0, NULL);
	//App->renderer->Blit(leftkicker, 247, 450, NULL);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
