#include "Display.h"
Display::Display() {
    
}

void Display::init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Chip 8 Emulator", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    //GLFW framebuffer size func in the form of lambda function
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
    shader = new Shader("C:\\Users\\nateh\\Desktop\\Projects\\chip8\\src\\shader.vs", "C:\\Users\\nateh\\Desktop\\Projects\\chip8\\src\\shader.fs");
    shader->use();
    float const xPixel = 2.0 / MAX_WIDTH;
    float const yPixel = 2.0 / MAX_HEIGHT;

    float vertices[] = {
    -1.0f + xPixel, 1.0f,          0.0f,  // top right
    -1.0f + xPixel, 1.0f - yPixel, 0.0f,  // bottom right
    -1.0f,          1.0f - yPixel, 0.0f,  // bottom left
    -1.0f,          1.0f,          0.0f   // top left 
    };
    unsigned int indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 4. then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}

void Display::nextScreen(unsigned char* gfx)
{   
    WindowCloseCheck();
    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    int x = 0, y = 0;
    float pos[3];
    pos[2] = 0;

    for (int i = 0; i < MAX_WIDTH * MAX_HEIGHT; ++i) {
        x = (i % 64);
        y = (i / 64);
        pos[0] = x / (float)(MAX_WIDTH) * 2;
        pos[1] = -y / (float)(MAX_HEIGHT) * 2;
        //TODO figure out GFX
        if (gfx[i])
            drawPixel(pos);
    }
    
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Display::WindowCloseCheck()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}



void Display::drawPixel(float* offset)
{
    int vertexOffsetLocation = glGetUniformLocation(shader->ID, "offset");
    glUniform3f(vertexOffsetLocation, offset[0], offset[1], offset[2]);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Display::End() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    delete shader;
    glfwTerminate();
}

bool Display::StayOpen()
{
    return !glfwWindowShouldClose(window);
}

Display::~Display()
{
    
}
