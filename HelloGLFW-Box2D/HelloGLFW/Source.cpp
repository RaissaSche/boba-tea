#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <chrono>
#include <glm/glm.hpp>
#include "Functions.h"
#include "MatVet.h"
#include "QueryCallback.h"
#include "Timer.h"

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
	func.setPosicaoMouse(p);
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
			func.setPressionou(true);
		}
		if (action == GLFW_RELEASE)
		{
			func.setPressionou(false);
			func.getBolaAtiravel()->SetGravityScale(9);

			//criar vetor AB
			b2Vec2 vetorForca = func.getPosicaoMouse() - func.getBolaAtiravel()->GetWorldCenter();
			//passar pra um vetor glm pra usar a normalização
			glm::vec2 vetorAuxiliar;
			vetorAuxiliar.x = vetorForca.x;
			vetorAuxiliar.y = vetorForca.y;
			glm::vec2 vetorAuxiliarNormalizado = glm::normalize(vetorAuxiliar);
			//devolver pro formato que a box entende, multiplicado pela força
			vetorForca.x = vetorAuxiliarNormalizado.x * 3000;
			vetorForca.y = vetorAuxiliarNormalizado.y * 3000;

			func.getBolaAtiravel()->ApplyForceToCenter(vetorForca, true);

			b2Body* novaBolaAtiravel = func.createCircle(10, 10, 2.5, 0.2, 0.3, 0.5);
			novaBolaAtiravel->SetGravityScale(0);
			novaBolaAtiravel->SetUserData(new UserData);
			func.setBolaAtiravel(novaBolaAtiravel);
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
	int width = 480, height = 640;

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
	window = glfwCreateWindow(width, height, "Boba Tea Bubble Shooter", NULL, NULL);
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

	//Criando as bordas da tela e do copo
	b2Body *linhaBaixo, *linhaEsq, *linhaDir;

	UserData* userDataBorda = new UserData;
	userDataBorda->setAuxiliarBorda('b');

	func.setLinhaBaixo(func.createEdge(0, -54, b2Vec2(-50, 15), b2Vec2(50, 15), 2, 2, 0));
	func.getLinhaBaixo()->SetUserData(userDataBorda);
	linhaEsq = func.createEdge(-65, 0, b2Vec2(15, -50), b2Vec2(15, 50), 2, 2, 0);
	linhaEsq->SetUserData(userDataBorda);
	linhaDir = func.createEdge(35, 0, b2Vec2(15, -50), b2Vec2(15, 50), 2, 2, 0);
	linhaDir->SetUserData(userDataBorda);

	b2Body *bordaEsq, *bordaDir, *bordaBaixo;

	UserData* userDataPlaceholer = new UserData;

	bordaEsq = func.createEdge(5, -55, b2Vec2(-25, 60), b2Vec2(-20, 20), 1, 1, 1);
	bordaEsq->SetUserData(userDataPlaceholer);
	bordaDir = func.createEdge(30, -55, b2Vec2(-20, 20), b2Vec2(-15, 60), 1, 1, 1);
	bordaDir->SetUserData(userDataPlaceholer);
	bordaBaixo = func.createEdge(0, -55, b2Vec2(-15, 20), b2Vec2(10, 20), 1, 1, 1);
	bordaBaixo->SetUserData(userDataPlaceholer);

	func.JogarBolinhas();

	b2Body* bolaPrincipal = func.createMainBubble();

	Timer* timer = new Timer;
	timer->start();

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

		/*if (func.getList().size < 8){
			func.JogarBolinhas();
		}*/

		if (timer->getElapsedTime() >= 5)
		{
			b2Body* bolaAtiravel = func.createCircle(0, 30, 2.5, 0.2, 0.3, 0.5);
			bolaAtiravel->SetGravityScale(0);
			bolaAtiravel->SetUserData(new UserData);
			func.setBolaAtiravel(bolaAtiravel);
		}

		//Setando a matriz de modelo, para mandar desenhar as primitivas
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Mandando simular e desenhar 	
		func.RunBox2D();
		func.Render(deb);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}