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

	ball = App->physics->CreateCircle(632, 400, 9, 0.5, b2_dynamicBody);
	//balltexture = App->textures->Load("pinball/ball.png");
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
	if (ball == nullptr) {
		ball = App->physics->CreateCircle(632, 400, 9, 0.5, b2_dynamicBody);
	}

	ball->GetPosition(ballx, bally);

	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN && ballx >= 628 && bally >= 390) {
		b2Vec2 force(0, -80);
		ball->body->ApplyForceToCenter(force, true);
	}
	//Check if Ball died
	if (bally >= 490) {
		App->physics->DestroyBody(ball->body);
		ball = nullptr;
	}
	
	//App->renderer->Blit(balltexture, ballx, bally, NULL);

	return UPDATE_CONTINUE;
}



