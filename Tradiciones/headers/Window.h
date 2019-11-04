/**
* @file Window.h
* @class Window
* 
* @brief Inicializa el contexto de opengl
*/
#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:

	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();

	GLfloat getBufferWidth() { return (float)bufferWidth; }
	GLfloat getBufferHeight() { return (float)bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	~Window();

private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};

