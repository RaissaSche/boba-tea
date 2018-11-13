#pragma once
#include "Functions.h"
#include "QueryCallback.h"

class FuncoesGLFW
{
public:
	FuncoesGLFW();
	~FuncoesGLFW();

	static void error_callback(int error, const char* description);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	Functions getFunc();
	void setFunc(Functions func);

private:
	Functions func;
};