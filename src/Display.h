#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

// SCALED BY 20
const unsigned int SCR_WIDTH = 64 * 20;
const unsigned int SCR_HEIGHT = 32*20;

class Display {
public:
	Display();
	~Display();
	void nextScreen();
	void processInput();
	void drawPixel(int* offset);
	void init();
	void End();
private:
	GLFWwindow* window;
	unsigned int EBO, VAO, VBO;
	Shader* shader;
};

