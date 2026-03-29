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
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	
	GLfloat getAnguloCofre() { return anguloCofre; }
	GLfloat getAnguloRuedas() { return anguloRuedas; }
	GLfloat getMueveCoche() { return mueveCoche; }


	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	// Agrega estas funciones públicas en tu clase Window
	bool getMandiAction() { return keys[GLFW_KEY_1]; }
	bool getPata1Action() { return keys[GLFW_KEY_2]; }
	bool getPata2Action() { return keys[GLFW_KEY_3]; }
	bool getPata3Action() { return keys[GLFW_KEY_4]; }
	bool getPata4Action() { return keys[GLFW_KEY_5]; }
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
	GLfloat muevex;
	GLfloat anguloCofre;
	GLfloat anguloRuedas;
	GLfloat mueveCoche;



	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

