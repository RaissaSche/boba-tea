#pragma once
#include "UserData.h"
#include <Box2D/Box2D.h>

class MyContactListener : public b2ContactListener
{
public:
	MyContactListener();
	~MyContactListener();
	void BeginContact(b2Contact* contact);
	int getId();
	void addId();

private:
	int id;
};
