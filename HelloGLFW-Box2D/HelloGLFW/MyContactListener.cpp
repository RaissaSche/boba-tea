#include "MyContactListener.h"

MyContactListener::MyContactListener(b2World * world)
{
	contactWorldCopy = world;
}

MyContactListener::~MyContactListener() {}

void MyContactListener::BeginContact(b2Contact * contact)
{
	b2Fixture* a = contact->GetFixtureA();
	b2Fixture* b = contact->GetFixtureB();

	UserData* userDataA = (UserData*)a->GetUserData();
	UserData* userDataB = (UserData*)b->GetUserData();

	if (userDataA->getCont() == userDataB->getCont())
	{
		userDataA->addIsIgual();
		userDataA->setId(getId());

		userDataB->addIsIgual();
		userDataB->setId(userDataA->getId());
		addId();
	}

	b2Body* body;

	if (userDataA->getIsIgual() >= 3)
	{
		for (body = contactWorldCopy->GetBodyList(); body; body = body->GetNext())
		{
			body->GetWorld()->DestroyBody(body);
		}
	}
}

void MyContactListener::addId()
{
	id++;
}

int MyContactListener::getId()
{
	return id;
}
