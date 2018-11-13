#include <iostream>
#include <ctime>
#include <stdlib.h>
#include "Functions.h"
#include "MatVet.h"
#include "QueryCallback.h"

Functions func;

//Callback de erro - PADR�O DA GLFW - n�o precisa mexer
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	b2Vec2 p = func.ConvertScreenToWorld(window, x, y);

	func.setMouseWorld(p);

	if (func.getMouseJoint())
	{
		func.getMouseJoint()->SetTarget(p);
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{


	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		b2Vec2 p = func.ConvertScreenToWorld(window, x, y);

		if (action == GLFW_PRESS)
		{
			func.setMouseWorld(p); //ainda nao sei se precisa

			if (func.getMouseJoint() != NULL)
			{
				return;
			}

			// Make a small box.
			b2AABB aabb;
			b2Vec2 d;
			d.Set(0.001f, 0.001f);
			aabb.lowerBound = p - d;
			aabb.upperBound = p + d;

			// Query the world for overlapping shapes.
			QueryCallback callback(p);
			b2World *w = func.getWorld();
			w->QueryAABB(&callback, aabb);

			if (callback.m_fixture)
			{
				b2Body* body = callback.m_fixture->GetBody();
				b2MouseJointDef md;
				md.bodyA = func.getLinhaBaixo();
				md.bodyB = body;
				md.target = p;
				md.maxForce = 1000.0f * body->GetMass();
				md.dampingRatio = 1.0;
				md.collideConnected = true;
				func.setMouseJoint((b2MouseJoint*)func.getWorld()->CreateJoint(&md));
				body->SetAwake(true);
			}
		}
		if (action == GLFW_RELEASE)
		{
			if (func.getMouseJoint())
				func.getWorld()->DestroyJoint(func.getMouseJoint());
			func.setMouseJoint(NULL);
		}
	}
}

int main() {

	DebugDraw deb;

	b2Vec2 gravity(0.0f, -9.8f);
	func.setWorld(new b2World(gravity));

	//Objeto para a classe que faz o desenho das formas de colis�o dos corpos r�gidos

	func.setTimeStep(1.0 / 60.0);
	func.setPositionIterations(6);
	func.setVelocityIterations(8);
	int width = 640, height = 480;

	int ang = 45, newtons = 10000;

	int angChar = 0, newtonsChar = 1000;
	b2Vec2 vetorForca = CalculaComponentesDoVetor(newtonsChar, angChar);

	//Inicializa��o da janela da aplica��o
	GLFWwindow* window;

	//Setando a callback de erro
	glfwSetErrorCallback(error_callback);

	//Inicializando a lib
	if (!glfwInit())
		exit(EXIT_FAILURE);

	//Criando a janela
	window = glfwCreateWindow(width, height, "Exercicios 1", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//Colocando a janela criada como sendo o contexto atual
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	//Setando a callback de teclado & mouse
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);


	b2Body *linhaBaixo, *linhaEsq, *linhaDir;

	func.setLinhaBaixo(func.createEdge(func.getWorld(), 0, -54, b2Vec2(-50, 15), b2Vec2(50, 15), 2, 2, 0));
	func.getLinhaBaixo()->SetUserData((void*)func.getAuxiliarBorda());
	linhaEsq = func.createEdge(func.getWorld(), -65, 0, b2Vec2(15, -50), b2Vec2(15, 50), 2, 2, 0);
	linhaEsq->SetUserData((void*)func.getAuxiliarBorda());
	linhaDir = func.createEdge(func.getWorld(), 35, 0, b2Vec2(15, -50), b2Vec2(15, 50), 2, 2, 0);
	linhaDir->SetUserData((void*)func.getAuxiliarBorda());

	b2Body *bordaEsq, *bordaDir, *bordaBaixo;

	bordaEsq = func.createEdge(func.getWorld(), 5, -55, b2Vec2(-25, 60), b2Vec2(-20, 20), 1, 1, 1);
	bordaDir = func.createEdge(func.getWorld(), 30, -55, b2Vec2(-20, 20), b2Vec2(-15, 60), 1, 1, 1);
	bordaBaixo = func.createEdge(func.getWorld(), 0, -55, b2Vec2(-15, 20), b2Vec2(10, 20), 1, 1, 1);


	int cont = 1, posX = 5;

	for (int i = 0; i < 25; i++) {
		if (posX < 0) {
			posX += 3;
		}
		b2Body *temp = func.createCircle(func.getWorld(), posX, i + 25, 2.5, 200, 10, 0);
		temp->SetUserData((void*)cont);
		if (cont > 4) { cont = 1; }
		func.getList().push_back(temp);
		cont++;
		posX -= 3;
	}

	b2Body* bolaPrincipal = func.createMainBubble();

	while (!glfwWindowShouldClose(window)) //loop da aplica��o :)
	{
		float ratio;
		int width, height;

		//aqui recupera o tamanho atual da janela, para corre��o do aspect ratio mais tarde
		glfwGetFramebufferSize(window, &width, &height);
		//setando a viewport da OpenGL, para ocupar toda a janela da aplica��o
		glViewport(0, 0, width, height);

		// Limpa a janela de visualiza��o com a cor branca
		glClearColor(0.654, 0.250, 0.372, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		//Setando a matriz de proje��o, para definir o Ortho2D (c�mera ortogr�fica 2D), respeitando o aspect ratio
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		if (width >= height)
		{
			ratio = width / (float)height;
			gluOrtho2D(func.getXMin()*ratio, func.getXMax()*ratio, func.getYMin(), func.getYMax());
		}
		else
		{
			ratio = height / (float)width;
			gluOrtho2D(func.getXMin(), func.getXMax(), func.getYMin()*ratio, func.getYMax()*ratio);
		}

		if (func.getList().size < 8){
			func.JogarBolinhas();
		}

		//Setando a matriz de modelo, para mandar desenhar as primitivas
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Mandando simular e desenhar 	
		func.RunBox2D(func.getWorld());
		func.Render(func.getWorld(), deb);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}