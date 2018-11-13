#pragma once
#include "Render.h"
#include <cstdlib>
#include <list>

class Functions
{
public:
	Functions();
	~Functions();

	// Fun��o de Execu��o da Simula��o
	void RunBox2D(b2World *world);

	b2Body* createBox(b2World *world, float posX, float posY, float sizeY, float sizeX, float density, float coefF, float coefR);
	b2Body* createCircle(b2World *world, float posX, float posY, float radius, float density, float coefF, float coefR);
	b2Body* createEdge(b2World *world, float posX, float posY, b2Vec2 vec1, b2Vec2 vec2, float density, float coefF, float coefR);
	void Render(b2World *world, DebugDraw renderer);
	void DrawBody(b2Body *b, b2Color color, DebugDraw renderer);
	void DrawFixture(b2Fixture* fixture, b2Color color, DebugDraw renderer);
	b2Vec2 ConvertScreenToWorld(GLFWwindow* window, int32 x, int32 y);
	b2Body* createMainBubble();
	void BeginContact(b2Contact* contact);
	void JogarBolinhas();

#pragma region GETs e SETs

	void setTimeStep(float32 timeStep);
	void setVelocityIterations(int32 velocityIterations);
	void setPositionIterations(int32 positionIterations);

	float getXMin();
	float getXMax();
	float getYMin();
	float getYMax();

	b2World* getWorld();
	void setWorld(b2World* world);
	b2Vec2 getMouseWorld();
	void setMouseWorld(b2Vec2 mouseWorld);
	b2MouseJoint* getMouseJoint();
	void setMouseJoint(b2MouseJoint* mouseJoint);
	char getAuxiliarBorda();
	b2Body* getLinhaBaixo();
	void setLinhaBaixo(b2Body* linhaCima);
	std::list<b2Body*> getList();
	void setBolaAtiravel(b2Body* bolaAtiravel);
	b2Body* getBolaAtiravel();

#pragma endregion

private:

	float32 timeStep;
	int32 velocityIterations;
	int32 positionIterations;
	float xMin = -40.0, xMax = 40.0, yMin = -40.0, yMax = 40.0; //ortho2D
	DebugDraw d;
	b2World *world;
	b2Vec2 mouseWorld;
	b2MouseJoint* mouseJoint;
	b2Body* linhaBaixo;
	char auxiliarBorda;
	std::list<b2Body*> bolas;
	b2Body* bolaAtiravel;
};