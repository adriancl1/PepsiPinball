#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"

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
	cowboy_fx = App->audio->LoadFx("pinball/cowboyhit.wav");
	hat_fx = App->audio->LoadFx("pinball/hatfx.wav");
	barrel_fx = App->audio->LoadFx("pinball/barrelfx.wav");
	deadcowboys_fx = App->audio->LoadFx("pinball/deadcowboysfx.wav");
	cart_fx = App->audio->LoadFx("pinball/cartfx.wav");
	tunnel_fx = App->audio->LoadFx("pinball/tunnelfx.wav");
	graphics = App->textures->Load("pinball/SpriteSheet.png");
	deadcowboys = 0;
	lefton = false;
	middleon = false;
	righton = false;

	redwindows.PushBack({ 0,0,312,162 });
	redwindows.PushBack({ 0,0,0,0 });
	redwindows.PushBack({ 0,0,312,162 });
	redwindows.PushBack({ 0,0,0,0 });
	redwindows.PushBack({ 0,0,312,162 });
	redwindows.PushBack({ 0,0,0,0 });
	redwindows.PushBack({ 0,0,312,162 });
	redwindows.PushBack({ 0,0,0,0 });
	redwindows.loop = true;
	redwindows.speed = 0.15f;

	PepsiTitle.PushBack({ 0, 285, 255, 121 });
	PepsiTitle.PushBack({ 0,0,0,0 });
	PepsiTitle.loop = true;
	PepsiTitle.speed = 0.1f;

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	CreateStage();
	
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
	current_time = SDL_GetTicks();

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 9, 0,false, b2_dynamicBody));
		circles.getLast()->data->listener = this;
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	//COWBOYS DEATHS AND RESURRECTION
	if (deadcowboys == 11 && last_time==0) {
		App->audio->PlayFx(deadcowboys_fx);
		last_time = SDL_GetTicks();
		App->player->AddPoints(25000);
	}
	if ((current_time > last_time + 6000) && deadcowboys==11) {
		RiseCowboys();
		deadcowboys = 0;
		last_time = 0;
	}
	else if (deadcowboys == 11 && (current_time <= last_time + 6000)) {
		App->renderer->Blit(graphics, 63, 61, &redwindows.GetCurrentFrame());
		App->renderer->Blit(graphics, 86, 12, &PepsiTitle.GetCurrentFrame());
	}

	//FLAGS CHECK
	if (lefton == true && middleon == true && righton == true && flag_time == 0) {
		leftflagtex.y = 0;
		midleflagtex.y = 74;
		rightflagtex.y = 158;
		flag_time = SDL_GetTicks();
		App->player->Addmultiply();
		App->player->AddPoints(1000);
	}
	if (lefton == true && middleon == true && righton == true && (current_time > flag_time + 2000)) {
		lefton = false;
		righton = false;
		middleon = false;
		flag_time = 0;
	}

	//SENSOR CHECKS TO AWAKE THEM
	if ((tunnelsensor->body->IsAwake()==false && (current_time > sensor_time+2000))) {
		tunnelsensor->body->SetAwake(true);
	}
	if ((cartsensor->body->IsAwake() == false && (current_time > sensor_time + 2000))) {
		cartsensor->body->SetAwake(true);
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


	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{

	if (cowboys.find(bodyA) != -1) {
		p2List_item<PhysBody*>* cowboysbody = cowboys.getFirst();
		int i = 0;
		while (cowboysbody != NULL) {
			if (cowboysbody->data == bodyA && cowboysbody->data->body->GetFixtureList()->IsSensor()==false) {
				App->audio->PlayFx(cowboy_fx);
				cowboysbody->data->body->GetFixtureList()->SetSensor(true) ;
				lights[i].x = 291;
				lights[i].y = 175;
				lights[i].w = 17;
				lights[i].h = 11;
				deadcowboys++;
				App->player->AddPoints(250);
				return;
			}
			i++;
			cowboysbody = cowboysbody->next;
		}
	}
	if (hats.find(bodyA) != -1) {
		App->audio->PlayFx(hat_fx);
		App->player->AddPoints(150);
	}

	if (bodyA == leftbarrels|| bodyA == rightbarrels) {
		App->audio->PlayFx(barrel_fx);
		App->player->AddPoints(100);
	}

	if (bodyA == leftflag && lefton==false) {
		leftflagtex.y = 37;
		lefton = true;
	}
	if (bodyA == midleflag && middleon==false) {
		midleflagtex.y = 116;
		middleon = true;
	}
	if (bodyA == rightflag && righton==false) {
		rightflagtex.y = 204;
		righton = true;
	}
	if (bodyA == cartsensor && cartsensor->body->IsAwake()) {
		App->audio->PlayFx(cart_fx);
		cartsensor->body->SetAwake(false);
		App->player->AddPoints(25000);
		sensor_time = SDL_GetTicks();
	}
	if (bodyA == tunnelsensor && tunnelsensor->body->IsAwake()) {
		App->audio->PlayFx(tunnel_fx);
		tunnelsensor->body->SetAwake(false);
		App->player->AddPoints(50000);
		sensor_time = SDL_GetTicks();
	}
}

void ModuleSceneIntro::Draw() {
	int x, y;

	int upperrowx = 244, upperrowy = 240;
	for (int i = 0; i <= 5; i++) {
		App->renderer->Blit(graphics, upperrowx, upperrowy, &lights[i], 1.0f);
		upperrowx += 18;
		upperrowy -= 11;
	}
	int bottomrowx = 284, bottomrowy = 255;
	for (int i = 6; i <= 10; i++) {
		App->renderer->Blit(graphics, bottomrowx, bottomrowy, &lights[i], 1.0f);
		bottomrowx += 18;
		bottomrowy -= 11;
	}

	//DRAW COWBOYS UP OR DOWN
	p2List_item<PhysBody*>* cowboysdraw = cowboys.getFirst();
	while (cowboysdraw != NULL)
	{
		if (cowboysdraw->data->body->GetFixtureList()->IsSensor() == false) {
			texcoords.x = 256;
			texcoords.y = 246;
			texcoords.w = 23;
			texcoords.h = 47;
			cowboysdraw->data->GetPosition(x, y);
			App->renderer->Blit(graphics, x, y, &texcoords, 1.0f);
			cowboysdraw = cowboysdraw->next;
		}
		else {
			texcoords.x = 280;
			texcoords.y = 246;
			texcoords.w = 23;
			texcoords.h = 47;
			cowboysdraw->data->GetPosition(x, y);
			App->renderer->Blit(graphics, x, y, &texcoords, 1.0f);
			cowboysdraw = cowboysdraw->next;
		}
	}



	//DRAW MEXICAN HATS
	p2List_item<PhysBody*>* hatsdraw = hats.getFirst();
	texcoords.x = 256;
	texcoords.y = 294;
	texcoords.w = 38;
	texcoords.h = 32;
	while (hatsdraw != NULL)
	{
		hatsdraw->data->GetPosition(x, y);
		App->renderer->Blit(graphics, x, y, &texcoords, 1.0f);
		hatsdraw = hatsdraw->next;
	}

	//DRAW FLAGS
	leftflag->GetPosition(x, y);
	App->renderer->Blit(graphics, x-45, y-5, &leftflagtex);
	midleflag->GetPosition(x, y);
	App->renderer->Blit(graphics, x-20, y-3, &midleflagtex);
	rightflag->GetPosition(x, y);
	App->renderer->Blit(graphics, x-9, y-4, &rightflagtex);

	
}



void ModuleSceneIntro::RiseCowboys() {
	p2List_item<PhysBody*>* cowboysbody = cowboys.getFirst();
	int i = 0;
	while (cowboysbody != NULL) {
		cowboysbody->data->body->GetFixtureList()->SetSensor(false);
		lights[i].x = 291;
		lights[i].y = 163;
		lights[i].w = 17;
		lights[i].h = 11;
		deadcowboys = 0;
		i++;
		cowboysbody = cowboysbody->next;
	}
}

void ModuleSceneIntro::CreateStage() {
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

	App->physics->CreateChain(0, 0, stage, 108, b2_staticBody, 0, false);

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

	App->physics->CreateChain(0, 0, RightIsle, 28, b2_staticBody, 0, false);

	//cowboy figurines
	int cowboy[18] = {
		3, 43,
		3, 15,
		4, 9,
		8, 5,
		13, 3,
		19, 6,
		21, 11,
		21, 36,
		8, 44
	};

	int upperrowx = 220, upperrowy = 200;
	for (int i = 0; i <= 5; i++) {
		cowboys.add(App->physics->CreateChain(upperrowx, upperrowy, cowboy, 18, b2_staticBody, COWBOY_REST, false));
		upperrowx += 21;
		upperrowy -= 11;
		cowboys.getLast()->data->listener = this;
	}
	int bottomrowx = 260, bottomrowy = 215;
	for (int i = 0; i <= 4; i++) {
		cowboys.add(App->physics->CreateChain(bottomrowx, bottomrowy, cowboy, 18, b2_staticBody, COWBOY_REST, false));
		bottomrowx += 21;
		bottomrowy -= 11;
		cowboys.getLast()->data->listener = this;
	}

	//lights underneath cowboys
	for (int i = 0; i <= 10; i++) {
		lights[i].x = 291;
		lights[i].y = 163;
		lights[i].w = 17;
		lights[i].h = 11;
	}

	//mexican hats

	int hat[24] = {
		3, 18,
		5, 23,
		11, 26,
		20, 27,
		29, 26,
		35, 21,
		35, 16,
		32, 13,
		26, 8,
		18, 7,
		10, 8,
		5, 13
	};

	hats.add(App->physics->CreateChain(401, 184, hat, 24, b2_staticBody, HAT_REST, false));
	hats.getLast()->data->listener = this;
	hats.add(App->physics->CreateChain(396, 142, hat, 24, b2_staticBody, HAT_REST, false));
	hats.getLast()->data->listener = this;
	hats.add(App->physics->CreateChain(455, 160, hat, 24, b2_staticBody, HAT_REST, false));
	hats.getLast()->data->listener = this;

	//leftbarrels

	int leftbarrelcoords[12] = {
		149, 371,
		232, 410,
		232, 386,
		214, 378,
		208, 348,
		151, 357
	};

	leftbarrels = App->physics->CreateChain(0, 0, leftbarrelcoords, 12, b2_staticBody, BARRELS_REST, false);
	leftbarrels->listener = this;

	int rightbarrelcoords[16] = {
		414, 405,
		494, 371,
		510, 343,
		470, 352,
		463, 362,
		444, 365,
		428, 378,
		413, 383
	};
	rightbarrels = App->physics->CreateChain(0, 0, rightbarrelcoords, 16, b2_staticBody, BARRELS_REST, false);
	rightbarrels->listener = this;

	//FLAGS

	int leftflagcoords[8]{
		0, 0,
		-10, -2,
		-34, 23,
		9, 3
	};
	leftflag = App->physics->CreateChain(453, 114, leftflagcoords, 8, b2_staticBody, 0, true);
	leftflag->listener = this;
	leftflagtex.x = 313;
	leftflagtex.y = 0;
	leftflagtex.w = 64;
	leftflagtex.h = 36;
	int middleflagcoords[6] = {
		0, 0,
		-12, 28,
		22, 6
	};
	midleflag = App->physics->CreateChain(475, 120, middleflagcoords, 6, b2_staticBody, 0, true);
	midleflag->listener = this;
	midleflagtex.x = 313;
	midleflagtex.y = 74;
	midleflagtex.w = 50;
	midleflagtex.h = 41;
	int rightflagcoords[6] = {
		0, 0,
		-4, 32,
		23, 6
	};
	rightflag = App->physics->CreateChain(509, 130, rightflagcoords, 6, b2_staticBody, 0, true);
	rightflag->listener = this;
	rightflagtex.x = 331;
	rightflagtex.y = 158;
	rightflagtex.w = 37;
	rightflagtex.h = 45;

	//SENSORS
	int cartsensorcoords[8]{
		0,0,
		25,-15,
		25,-10,
		0,5
	};
	cartsensor = App->physics->CreateChain(170, 259, cartsensorcoords, 8, b2_staticBody, 0, true);
	cartsensor->listener = this;

	int tunnelsensorcoords[8]{
		0,0,
		35,0,
		35,3,
		0,3
	};
	tunnelsensor = App->physics->CreateChain(525, 239, tunnelsensorcoords, 8, b2_staticBody, 0, true);
	tunnelsensor->listener = this;
	
}