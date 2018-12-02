#include "Functions.h"

Functions::Functions()
{
	pressionou = false;
	xMin = -40.0;
	xMax = 40.0;
	yMin = -40.0;
	yMax = 40.0; //ortho2D
	cont = 1;
	posX = 5;
	timer = new Timer;
}
Functions::~Functions() {}

b2Body* Functions::createBox(float posX, float posY, float sizeY, float sizeX, float density, float coefF, float coefR)
{
	b2Body *novoObjeto;

	b2BodyDef box;
	box.position.Set(posX, posY);
	box.type = b2_dynamicBody;
	novoObjeto = world->CreateBody(&box);

	b2PolygonShape caixa;
	caixa.SetAsBox(sizeX / 2, sizeY / 2);
	b2FixtureDef f;
	f.shape = &caixa;
	f.density = density;
	f.friction = coefF;
	f.restitution = coefR;

	novoObjeto->CreateFixture(&f);

	return novoObjeto;
}

b2Body* Functions::createCircle(float posX, float posY, float radius, float density, float coefF, float coefR)
{
	b2Body* novoObjeto;

	b2BodyDef circle;
	circle.position.Set(posX, posY);
	circle.type = b2_dynamicBody;
	novoObjeto = world->CreateBody(&circle);

	b2CircleShape circulo;
	circulo.m_radius = radius;
	circulo.m_p.Set(posX, posY);
	b2FixtureDef f;
	f.shape = &circulo;
	f.density = density;
	f.friction = coefF;
	f.restitution = coefR;

	novoObjeto->CreateFixture(&f);

	return novoObjeto;
}

b2Body* Functions::createStaticCircle(float posX, float posY, float radius, float density, float coefF, float coefR)
{
	b2Body* novoObjeto;

	b2BodyDef circle;
	circle.position.Set(posX, posY);
	circle.type = b2_staticBody;
	novoObjeto = world->CreateBody(&circle);

	b2CircleShape circulo;
	circulo.m_radius = radius;
	circulo.m_p.Set(posX, posY);

	b2FixtureDef f;
	f.shape = &circulo;
	f.density = density;
	f.friction = coefF;
	f.restitution = coefR;

	novoObjeto->CreateFixture(&f);

	return novoObjeto;
}

b2Body* Functions::createEdge(float posX, float posY, b2Vec2 vec1, b2Vec2 vec2, float density, float coefF, float coefR)
{
	b2Body *novoObjeto;

	b2BodyDef edge;
	edge.position.Set(posX, posY);
	edge.type = b2_staticBody;
	novoObjeto = world->CreateBody(&edge);

	b2EdgeShape linha;
	linha.Set(vec1, vec2);

	b2FixtureDef f;
	f.shape = &linha;
	f.density = density;
	f.friction = coefF;
	f.restitution = coefR;

	novoObjeto->CreateFixture(&f);

	return novoObjeto;
}

void Functions::Render(DebugDraw renderer)
{
	b2Body *b;
	glColor3f(1, 0, 0);
	glPointSize(5);
	glLineWidth(3);

	b2Color pink;
	pink.r = 0.956;
	pink.g = 0.458;
	pink.b = 0.611;

	b2Color blue;
	blue.r = 0.282;
	blue.g = 0.572;
	blue.b = 0.654;

	b2Color yellow;
	yellow.r = 0.964;
	yellow.g = 0.933;
	yellow.b = 0.556;

	b2Color green;
	green.r = 0.462;
	green.g = 0.756;
	green.b = 0.462;


	if (pressionou)
	{
		desenharMira();
	}

	for (b = world->GetBodyList(); b; b = b->GetNext())
	{
		UserData* userDataAuxiliarBorda = (UserData*)b->GetUserData();

		if (userDataAuxiliarBorda)
		{

			if (userDataAuxiliarBorda->getCont() == 1)
			{
				DrawBody(b, blue, renderer);
			}
			else if (userDataAuxiliarBorda->getCont() == 2)
			{
				DrawBody(b, yellow, renderer);
			}
			else if (userDataAuxiliarBorda->getCont() == 3)
			{
				DrawBody(b, pink, renderer);
			}
			else if (userDataAuxiliarBorda->getCont() == 4)
			{
				DrawBody(b, green, renderer);
			}
			else if (userDataAuxiliarBorda->getAuxiliarBorda() != 'b')
			{
				DrawBody(b, blue, renderer);
			}
		}

	}
}

//Para chamar a rotina de desenho das fixtures de um corpo
void Functions::DrawBody(b2Body *b, b2Color color, DebugDraw renderer)
{
	//Desenha todas as fixtures do corpo r�gido
	b2Fixture *f;
	for (b = world->GetBodyList(); b; b = b->GetNext())
	{
		UserData* userDataAuxiliar = (UserData*)b->GetUserData();
		if (userDataAuxiliar)
		{
			f = b->GetFixtureList();
			if (userDataAuxiliar->getAuxiliarBorda() != 'b')
			{
				DrawFixture(f, color, renderer);
			}
		}
	}
}

//Fun��o que chama as rotinas de desenho para cada fixture de um corpo r�gido, de acordo com o tipo da forma de colis�o dela e 
void Functions::DrawFixture(b2Fixture* fixture, b2Color color, DebugDraw renderer)
{
	const b2Transform& xf = fixture->GetBody()->GetTransform();

	switch (fixture->GetType())
	{

	case b2Shape::e_circle:
	{
		b2CircleShape* circle = (b2CircleShape*)fixture->GetShape();

		b2Vec2 center = b2Mul(xf, circle->m_p);
		float32 radius = circle->m_radius;

		renderer.DrawCircle(center, radius, color);
	}
	break;

	case b2Shape::e_polygon:
	{
		b2PolygonShape* poly = (b2PolygonShape*)fixture->GetShape();
		int32 vertexCount = poly->m_count;
		b2Assert(vertexCount <= b2_maxPolygonVertices);
		b2Vec2 vertices[b2_maxPolygonVertices];

		for (int32 i = 0; i < vertexCount; ++i)
		{
			vertices[i] = b2Mul(xf, poly->m_vertices[i]);
		}

		renderer.DrawPolygon(vertices, vertexCount, color);
	}
	break;

	case b2Shape::e_edge:
	{
		b2EdgeShape* edge = (b2EdgeShape*)fixture->GetShape();
		int32 vertexCount;

		b2Vec2 vertices[b2_maxPolygonVertices];
		int i = 0;

		if (edge->m_hasVertex0)
		{
			vertices[i] = b2Mul(xf, edge->m_vertex0);
			i++;
		}
		vertices[i] = b2Mul(xf, edge->m_vertex1); i++;
		vertices[i] = b2Mul(xf, edge->m_vertex2); i++;
		if (edge->m_hasVertex3)
		{
			vertices[i] = b2Mul(xf, edge->m_vertex3);
			i++;
		}

		vertexCount = i;
		renderer.DrawPolygon(vertices, vertexCount, color);
	}
	break;

	}
}

//Convers�o de coordenadas de tela para coords de universo (leva em considera��o o ortho)
//Rotina que converte coordenadas de tela em coordenadas do mundo
b2Vec2 Functions::ConvertScreenToWorld(GLFWwindow* window, int32 x, int32 y)
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	float32 u = x / float32(width);
	float32 v = (height - y) / float32(height);

	float32 ratio = float32(width) / float32(height);
	b2Vec2 extents(ratio * 40.0f, 40.0f); //aqui t� no bra�o
										  //extents *= viewZoom;

	b2Vec2 lower = -extents;
	b2Vec2 upper = extents;

	b2Vec2 p;
	p.x = (1.0f - u) * lower.x + u * upper.x;
	p.y = (1.0f - v) * lower.y + v * upper.y;
	return p;
}

void Functions::createMainBubble()
{
	getBolaAtiravel()->GetWorld()->DestroyBody(getBolaAtiravel());

	b2Body* bolaAtiravel = createCircle(0, 20, 2.5, 0.2, 0.3, 0.5);
	bolaAtiravel->SetGravityScale(9);
	bolaAtiravel->SetUserData(new UserData);
	setBolaAtiravel(bolaAtiravel);

	std::this_thread::sleep_for(std::chrono::seconds(2));

	b2Body* bolaAtiravelStatic = createStaticCircle(0, 20, 2.5, 0.2, 0.3, 0.5);
	bolaAtiravelStatic->SetGravityScale(9);
	bolaAtiravelStatic->SetUserData(new UserData);
	setBolaAtiravel(bolaAtiravelStatic);
}

// Fun��o de Execu��o da Simula��o
void Functions::RunBox2D()
{
	world->Step(timeStep, velocityIterations, positionIterations);
	world->ClearForces();
}

bool Functions::JogarBolinhas() {

	//Criando as bolas e setando o userData delas (usado pra setar a cor)

	const int QTD_BOLINHAS = 25;

	for (int i = 0; i < QTD_BOLINHAS; i++)
	{
		if (posX < 0)
		{
			posX += 3;
		}

		b2Body *bubble = createCircle(posX, i + 25, 2.5, 200, 10, 0);
		UserData* userDataCont = new UserData;
		userDataCont->setCont(cont);
		bubble->SetUserData(userDataCont);

		if (cont > 4) { cont = 1; }
		cont++;
		posX -= 3;
	}

	b2Body* bolaExtra = createCircle(getMouseWorld().x, getMouseWorld().y, 2.5, 1, 0.3, 0.5);

	return true;
}

void Functions::desenharMira()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex2i(posicaoMouse.x, posicaoMouse.y);
	glVertex2i(bolaAtiravel->GetWorldCenter().x, bolaAtiravel->GetWorldCenter().y);
	glEnd();
}

b2World* Functions::getWorld() { return world; }
void Functions::setWorld(b2World * world) { this->world = world; }

b2Vec2 Functions::getMouseWorld() { return mouseWorld; }
void Functions::setMouseWorld(b2Vec2 mouseWorld) { this->mouseWorld = mouseWorld; }

b2Body* Functions::getLinhaBaixo() { return linhaBaixo; }
void Functions::setLinhaBaixo(b2Body * linhaBaixo) { this->linhaBaixo = linhaBaixo; }

void Functions::setTimeStep(float32 timeStep) { this->timeStep = timeStep; }
void Functions::setVelocityIterations(int32 velocityIterations) { this->velocityIterations = velocityIterations; }
void Functions::setPositionIterations(int32 positionIterations) { this->positionIterations = positionIterations; }

float Functions::getXMin() { return xMin; }
float Functions::getXMax() { return xMax; }
float Functions::getYMin() { return yMin; }
float Functions::getYMax() { return yMax; }

void Functions::setBolaAtiravel(b2Body * _bolaAtiravel)
{
	bolaAtiravel = _bolaAtiravel;
}

b2Body * Functions::getBolaAtiravel()
{
	return bolaAtiravel;
}

void Functions::setPosicaoMouse(b2Vec2 posicaoMouse)
{
	this->posicaoMouse = posicaoMouse;
}

b2Vec2 Functions::getPosicaoMouse()
{
	return posicaoMouse;
}

void Functions::setPressionou(bool pressionou)
{
	this->pressionou = pressionou;
}

bool Functions::getPressionou()
{
	return pressionou;
}

void Functions::setUserData(UserData userData)
{
	this->userData = userData;
}

UserData Functions::getUserData()
{
	return userData;
}

Timer * Functions::getTimer()
{
	return timer;
}
