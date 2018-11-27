#include "MyContactListener.h"

MyContactListener::MyContactListener() {}

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

	if (userDataA->getIsIgual() >= 3)
	{
		//for (b = func.getWorld()->GetBodyList(); b; b = b->GetNext()) 
		//{
			//HOW THE DAMN WILL I GET THE SAME FUNCTION??????

			//delete the balls???
		//}
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
