#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define SCR_WIDTH 64*20
#define SCR_HEIGHT 32*20

class Display {
public:
	Display();
	~Display();
	void init();
	void nextScreen();
	void processInput();
private:
	GLFWwindow* window;
};

