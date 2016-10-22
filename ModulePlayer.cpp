#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"


ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	death_fx =App->audio->LoadFx("pinball/deathfx.wav");
	kicker_fx = App->audio->LoadFx("pinball/kickerfx.wav");
	//ball
	ball.body = App->physics->CreateCircle(632, 400, 9, 0.5, b2_dynamicBody);
	ball.section.x = 256;
	ball.section.y = 163;
	ball.section.w = 34;
	ball.section.h = 32;



	//leftkicker
	b2Vec2 upperleftvertices[5];
	upperleftvertices[0].Set(0, 0);
	upperleftvertices[1].Set(0.029, -0.15);
	upperleftvertices[2].Set(0.104, -0.25);
	upperleftvertices[3].Set(1.3, -0.12);
	upperleftvertices[4].Set(1.344, 0);
	b2Vec2 bottomleftvertices[5];
	bottomleftvertices[0].Set(0, 0);
	bottomleftvertices[1].Set(0.029, 0.15);
	bottomleftvertices[2].Set(0.104, 0.22);
	bottomleftvertices[3].Set(1.3, 0.08);
	bottomleftvertices[4].Set(1.344, 0);

	leftkicker.body = App->physics->CreatePolygons(upperleftvertices, bottomleftvertices, 5, 5, b2_dynamicBody, 300, 470);
	leftwheel=App->physics->CreateCircle(257, 500, 9, 0, b2_staticBody);
	App->physics->CreateRevoluteJoint(leftkicker.body->body, leftwheel->body, 0, -23, 13, 0);

	leftkicker.section.x = 256;
	leftkicker.section.y = 221;
	leftkicker.section.w = 76;
	leftkicker.section.h = 26;

	//rightkicker
	b2Vec2 upperrightvertices[5];
	upperrightvertices[0].Set(0, 0);
	upperrightvertices[1].Set(-0.029, -0.15);
	upperrightvertices[2].Set(-0.104, -0.25);
	upperrightvertices[3].Set(-1.3, -0.12);
	upperrightvertices[4].Set(-1.344, 0);
	b2Vec2 bottomrightvertices[5];
	bottomrightvertices[0].Set(0, 0);
	bottomrightvertices[1].Set(-0.029, 0.15);
	bottomrightvertices[2].Set(-0.104, 0.25);
	bottomrightvertices[3].Set(-1.3, 0.08);
	bottomrightvertices[4].Set(-1.344, 0);

	rightkicker.body = App->physics->CreatePolygons(upperrightvertices, bottomrightvertices, 5, 5, b2_dynamicBody, 400, 500);
	rightwheel = App->physics->CreateCircle(390, 500, 9, 0, b2_staticBody);
	App->physics->CreateRevoluteJoint(rightkicker.body->body,rightwheel->body, 23, -10,-13,0);

	rightkicker.section.x = 256;
	rightkicker.section.y = 196;
	rightkicker.section.w = 76;
	rightkicker.section.h = 26;

	//rightkicker2

	b2Vec2 upperrightvertices2[5];
	upperrightvertices2[0].Set(0, 0);
	upperrightvertices2[1].Set(-0.029, -0.15);
	upperrightvertices2[2].Set(-0.104, -0.25);
	upperrightvertices2[3].Set(-1.3, -0.12);
	upperrightvertices2[4].Set(-1.344, 0);
	b2Vec2 bottomrightvertices2[5];
	bottomrightvertices2[0].Set(0, 0);
	bottomrightvertices2[1].Set(-0.029, 0.15);
	bottomrightvertices2[2].Set(-0.104, 0.22);
	bottomrightvertices2[3].Set(-1.3, 0.08);
	bottomrightvertices2[4].Set(-1.344, 0);

	rightkicker2.body = App->physics->CreatePolygons(upperrightvertices2, bottomrightvertices2, 5, 5, b2_dynamicBody, 490, 300);
	rightwheel2 = App->physics->CreateCircle(490, 300, 9, 0, b2_staticBody);
	App->physics->CreateRevoluteJoint(rightkicker2.body->body, rightwheel2->body, 23, -10, -13, 0);

	rightkicker2.section.x = 256;
	rightkicker2.section.y = 196;
	rightkicker2.section.w = 76;
	rightkicker2.section.h = 26;

	//TEXTURES LOAD
	underball = App->textures->Load("pinball/UnderBall.png");
	overball = App->textures->Load("pinball/OverBall.png");
	graphics = App->textures->Load("pinball/SpriteSheet.png");

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");


	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	if (ball.body == nullptr) {
		ball.body = App->physics->CreateCircle(632, 400, 9, 0.5, b2_dynamicBody);
	}

	ball.body->GetPosition(ballx, bally);

	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN && ballx >= 628 && bally >= 390) {
		b2Vec2 force(0, -80);
		ball.body->body->ApplyForceToCenter(force, true);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN ) {
		leftkicker.body->Turn(-360);
		App->audio->PlayFx(kicker_fx);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
		rightkicker.body->Turn(360);
		rightkicker2.body->Turn(360);
		App->audio->PlayFx(kicker_fx);
	}
	//Check if Ball died
	if (bally >= 520) {
		App->physics->DestroyBody(ball.body->body);
		ball.body = nullptr;
		App->audio->PlayFx(death_fx);
	}

	int x, y;
	
	App->renderer->Blit(underball, 0, 0, NULL);

	App->renderer->Blit(graphics, ballx-4.5, bally-4.5, &ball.section, 1.0f);
	
	leftkicker.body->GetPosition(x, y);
	App->renderer->Blit(graphics, x, y-15, &leftkicker.section, 1.0f, leftkicker.body->GetAngle(), 0, 0);

	rightkicker.body->GetPosition(x, y);
	App->renderer->Blit(graphics, x-68, y+15, &rightkicker.section, 1.0f, rightkicker.body->GetAngle(), 0, 0);

	rightkicker2.body->GetPosition(x, y);
	App->renderer->Blit(graphics, x - 68, y + 15, &rightkicker2.section, 1.0f, rightkicker2.body->GetAngle(), 0, 0);

	App->renderer->Blit(overball, 0, 0, NULL);

	return UPDATE_CONTINUE;
}



