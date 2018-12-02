#pragma once
#define _GLIBCXX_USE_NANOSLEEP
#include "Render.h"
#include "UserData.h"
#include "Timer.h"
#include <cstdlib>
#include <list>

class Functions
{
public:
	Functions();
	~Functions();

	// Fun��o de Execu��o da Simula��o
	void RunBox2D();

	b2Body* createBox(float posX, float posY, float sizeY, float sizeX, float density, float coefF, float coefR);
	b2Body* createCircle(float posX, float posY, float radius, float density, float coefF, float coefR);
	b2Body* createStaticCircle(float posX, float posY, float radius, float density, float coefF, float coefR);
	b2Body* createEdge(float posX, float posY, b2Vec2 vec1, b2Vec2 vec2, float density, float coefF, float coefR);
	void createMainBubble();
	void createMainBubbleCallback();
	void Render(DebugDraw renderer);
	void DrawBody(b2Body *b, b2Color color, DebugDraw renderer);
	void DrawFixture(b2Fixture* fixture, b2Color color, DebugDraw renderer);
	b2Vec2 ConvertScreenToWorld(GLFWwindow* window, int32 x, int32 y);
	bool JogarBolinhas();
	void desenharMira();

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
	b2Body* getLinhaBaixo();
	void setLinhaBaixo(b2Body* linhaCima);
	std::list<b2Body*> getList();
	void setBolaAtiravel(b2Body* bolaAtiravel);
	b2Body* getBolaAtiravel();
	void setPosicaoMouse(b2Vec2 posicaoMouse);
	b2Vec2 getPosicaoMouse();
	void setPressionou(bool pressionou);
	bool getPressionou();
	void setUserData(UserData userData);
	UserData getUserData();
	Timer* getTimer();

#pragma endregion

private:

	float32 timeStep;
	int32 velocityIterations;
	int32 positionIterations;
	float xMin, xMax, yMin, yMax; //ortho2D
	DebugDraw d;
	b2World *world;
	b2Vec2 mouseWorld;
	b2Body* linhaBaixo;
	b2Body* bolaAtiravel;
	b2Vec2 posicaoMouse;
	bool pressionou;
	UserData userData;
	int cont;
	int	posX;
	Timer* timer;
};