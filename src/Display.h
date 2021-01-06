#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

#define MAX_WIDTH 64
#define MAX_HEIGHT 32

// SCALED BY 20
const unsigned int SCR_WIDTH = MAX_WIDTH * 20;
const unsigned int SCR_HEIGHT = MAX_HEIGHT * 20;

class Display {
public:
	Display();
	~Display();
	void nextScreen(unsigned char* gfx);
	void processInput();
	unsigned short WaitForInput();
	void drawPixel(float* offset);
	void init();
	void End();
	bool StayOpen();
	Shader* shader;
private:
	GLFWwindow* window;
	unsigned int EBO, VAO, VBO;
};

