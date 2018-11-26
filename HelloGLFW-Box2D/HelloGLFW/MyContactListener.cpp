#include "MyContactListener.h"

MyContactListener::MyContactListener() {}

MyContactListener::~MyContactListener() {}

void MyContactListener::BeginContact(b2Contact * contact)
{
	b2Fixture* a = contact->GetFixtureA();
	b2Fixture* b = contact->GetFixtureB();

	if ((UserData*)a->GetUserData()->getCont() == (UserData*)b->GetUserData()->getCont())
	{
		(UserData*)a->GetUserData()->addIsIgual();
		(UserData*)a->GetUserData()->setid(getId());

		(UserData*)b->GetUserData()->addIsIgual();
		(UserData*)b->GetUserData()->setid(a->GetUserData()->getId());
		addId();
	}

	if (a->GetUserData()->getIsIgual() >= 3)
	{
		for (b = func.getWorld()->GetBodyList(); b; b = b->GetNext()) 
		{
			//HOW THE DAMN WILL I GET THE SAME FUNCTION??????

			//delete the balls???
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
