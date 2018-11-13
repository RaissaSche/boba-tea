#include "FuncoesGLFW.h"

FuncoesGLFW::FuncoesGLFW() {}
FuncoesGLFW::~FuncoesGLFW() {}

//Callback de erro - PADRÃO DA GLFW - não precisa mexer
static void FuncoesGLFW::error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void FuncoesGLFW::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

static void FuncoesGLFW::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
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

Functions FuncoesGLFW::getFunc()
{
	return func;
}

void FuncoesGLFW::setFunc(Functions func)
{
	this->func = func;
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
				md.bodyA = func.getLinhaCima();
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