#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	int stage[178] = {
		238, 513,
		248, 508,
		246, 495,
		247, 481,
		248, 467,
		242, 453,
		216, 436,
		184, 422,
		161, 414,
		140, 401,
		120, 392,
		108, 378,
		95, 364,
		74, 336,
		62, 313,
		52, 291,
		48, 276,
		82, 256,
		115, 236,
		173, 209,
		189, 231,
		197, 242,
		232, 220,
		258, 203,
		249, 176,
		274, 163,
		292, 178,
		323, 161,
		344, 162,
		423, 117,
		480, 76,
		489, 71,
		494, 76,
		509, 84,
		521, 92,
		540, 93,
		556, 93,
		581, 99,
		607, 110,
		620, 126,
		632, 144,
		642, 170,
		647, 190,
		650, 208,
		654, 228,
		655, 259,
		656, 302,
		656, 328,
		654, 365,
		652, 400,
		648, 415,
		628, 413,
		627, 382,
		630, 344,
		632, 306,
		631, 271,
		629, 244,
		628, 225,
		621, 199,
		613, 173,
		603, 147,
		591, 128,
		572, 120,
		557, 114,
		544, 113,
		536, 123,
		537, 140,
		539, 163,
		548, 189,
		552, 215,
		560, 242,
		568, 262,
		586, 285,
		594, 304,
		600, 320,
		601, 338,
		599, 354,
		594, 374,
		580, 385,
		564, 395,
		540, 400,
		514, 401,
		481, 410,
		450, 424,
		424, 441,
		400, 456,
		400, 476,
		403, 496,
		404, 513
	};
	CreateChain(0, 0, stage, 178, b2_staticBody);

	//left kicker ball

	PhysBody* leftkickerball = CreateCircle(350, 470, 9, 0, b2_staticBody);

	//left kicker
	b2Vec2 upperleftvertices[6];
	upperleftvertices[0].Set(0, 0);
	upperleftvertices[1].Set(0.029, -0.15);
	upperleftvertices[2].Set(0.104, -0.3);
	upperleftvertices[3].Set(0.894, -0.16);
	upperleftvertices[4].Set(0.934, 0);
	upperleftvertices[5].Set(0, 0);
	b2Vec2 bottomleftvertices[6];
	bottomleftvertices[0].Set(0, 0);
	bottomleftvertices[1].Set(0.029, 0.15);
	bottomleftvertices[2].Set(0.104, 0.3);
	bottomleftvertices[3].Set(0.894, 0.1);
	bottomleftvertices[4].Set(0.934, 0);
	bottomleftvertices[5].Set(0, 0);

	PhysBody* leftkicker = CreatePolygons(upperleftvertices, bottomleftvertices, 6, 6, b2_dynamicBody);
	left_joint=CreateRevoluteJoint(left_joint, leftkickerball->body, leftkicker->body);
	
	//right kicker ball

	PhysBody* rightkickerball = CreateCircle(400, 470, 9, 0, b2_staticBody);

	

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, float Rest, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.restitution = Rest;
	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;


	return pbody;
}

PhysBody* ModulePhysics::CreatePolygons(b2Vec2* vertices1, b2Vec2* vertices2, int count1, int count2, b2BodyType type)
{
	b2BodyDef polygonbody;
	polygonbody.type = type;
	polygonbody.position.Set(PIXEL_TO_METERS(350), PIXEL_TO_METERS(470));

	b2Body* b = world->CreateBody(&polygonbody);

	b2PolygonShape polygon1shape;
	polygon1shape.Set(vertices1, count1);
	b2FixtureDef polygon1fix;
	polygon1fix.shape = &polygon1shape;
	b->CreateFixture(&polygon1fix);

	b2PolygonShape polygon2shape;
	polygon2shape.Set(vertices2, count2);
	b2FixtureDef polygon2fix;
	polygon2fix.shape = &polygon2shape;
	b->CreateFixture(&polygon2fix);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);

	return pbody;
}

b2RevoluteJoint* ModulePhysics::CreateRevoluteJoint(b2RevoluteJoint* joint, b2Body* bodyA, b2Body* bodyB) {

	b2RevoluteJointDef JointDef;
	JointDef.bodyA = bodyA;
	JointDef.bodyB = bodyB;
	b2Vec2 anchor = bodyA->GetLocalCenter();
	JointDef.localAnchorA = bodyA->GetLocalCenter();
	JointDef.localAnchorB = bodyB->GetLocalCenter();
	return(b2RevoluteJoint*)world->CreateJoint(&JointDef);
}

// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}

			if(App->input->GetMouseButton(SDL_BUTTON_LEFT)==KEY_DOWN && mousein==false){
				b2Vec2 mouse(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));
				if (f->TestPoint(mouse)) {
					mousein = true;
					mousebody = b;
				}
			}
		}
	}

	if (mousein == true && mouse_joint==nullptr) {
		b2Vec2 mousepos(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));

		b2MouseJointDef def;
		def.bodyA = ground;
		def.bodyB = mousebody;
		def.target = mousepos;
		def.dampingRatio = 0.5f;
		def.frequencyHz = 2.0f;
		def.maxForce = 100.0f * mousebody->GetMass();
		mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && mousein==true) {
		b2Vec2 mousepos(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));

		mouse_joint->SetTarget(mousepos);

		App->renderer->DrawLine(METERS_TO_PIXELS(mousepos.x), METERS_TO_PIXELS(mousepos.y), METERS_TO_PIXELS(mousebody->GetPosition().x), METERS_TO_PIXELS(mousebody->GetPosition().y), 255, 0, 0);

	}
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && mousein == true) {
		world->DestroyJoint(mouse_joint);
		mouse_joint = nullptr;
		mousein = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN) {
		b2Vec2 force(80, -80);
		left_joint->GetBodyA()->ApplyAngularImpulse(DEGTORAD * -360, true);
		left_joint->GetBodyB()->ApplyAngularImpulse(DEGTORAD * -360, true);
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}

void ModulePhysics::DestroyBody(b2Body* bodytodestroy) {

	world->DestroyBody(bodytodestroy);
}