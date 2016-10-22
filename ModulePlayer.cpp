#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	
	ball.body = App->physics->CreateCircle(632, 400, 9, 0.5, b2_dynamicBody);
	//ball.texture = App->textures->Load("pinball/ball.png");

	//leftkicker
	b2Vec2 upperleftvertices[5];
	upperleftvertices[0].Set(0, 0);
	upperleftvertices[1].Set(0.029, -0.15);
	upperleftvertices[2].Set(0.104, -0.3);
	upperleftvertices[3].Set(0.894, -0.16);
	upperleftvertices[4].Set(0.934, 0);
	b2Vec2 bottomleftvertices[5];
	bottomleftvertices[0].Set(0, 0);
	bottomleftvertices[1].Set(0.029, 0.15);
	bottomleftvertices[2].Set(0.104, 0.3);
	bottomleftvertices[3].Set(0.894, 0.1);
	bottomleftvertices[4].Set(0.934, 0);


	leftkicker.body = App->physics->CreatePolygons(upperleftvertices, bottomleftvertices, 5, 5, b2_dynamicBody);
	leftwheel=App->physics->CreateCircle(263, 470, 9, 0, b2_staticBody);
	App->physics->CreateRevoluteJoint(leftkicker.body->body, leftwheel->body, 0, -30);

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
	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN ) {
		leftkicker.body->Turn(-360);
	}
	//Check if Ball died
	if (bally >= 490) {
		App->physics->DestroyBody(ball.body->body);
		ball.body = nullptr;
	}
	
	//App->renderer->Blit(balltexture, ballx, bally, NULL);

	return UPDATE_CONTINUE;
}



