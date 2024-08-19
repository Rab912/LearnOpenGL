#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Model.h"

const int VP_WIDTH = 800;
const int VP_HEIGHT = 600;

float timeStep = 0.0f;
float lastFrame = 0.0f;

float lastx = 400;
float lasty = 300;

bool firstMouse = true;

Camera camera(45.0f, (float)VP_WIDTH / (float)VP_HEIGHT, 0.1f, 100.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboardInput(CameraMotion::FORWARD, timeStep);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboardInput(CameraMotion::LEFT, timeStep);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboardInput(CameraMotion::BACKWARD, timeStep);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboardInput(CameraMotion::RIGHT, timeStep);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.processKeyboardInput(CameraMotion::UP, timeStep);

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.processKeyboardInput(CameraMotion::DOWN, timeStep);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastx = xPos;
        lasty = yPos;

        firstMouse = false;
    }

    float xoffs = (float)xPos - lastx;
    float yoffs = (float)yPos - lasty;

    lastx = (float)xPos;
    lasty = (float)yPos;

    const float sensitivity = 0.1f;

    xoffs *= sensitivity;
    yoffs *= sensitivity;

    camera.processMouseMovementInput((float)xoffs, (float)yoffs);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.processMouseScrollInput((float)yoffset);
}

int main()
{
    // === Initialize GLFW ===

    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // === Create window ===

    GLFWwindow* window = glfwCreateWindow(VP_WIDTH, VP_HEIGHT, "LearnOpenGL", nullptr, nullptr);

    if (!window)
    {
        glfwTerminate();
        std::cout << "Failed to create GLFW window" << std::endl;
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // === Initialize GLAD ===

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    stbi_set_flip_vertically_on_load(true);

    Shader shader("src/shaders/Vertex_model.shd", "src/shaders/Fragment_model.shd");

    Model backpackModel("res/backpack/backpack.obj");

    // Wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        timeStep = currentFrame - lastFrame;
        lastFrame = currentFrame;

        process_input(window);

        // Clear the screen after each frame
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the shader
        shader.use();
        shader.setMat4("projection", camera.getProjection());
        shader.setMat4("view", camera.getView());

        glm::mat4 modelMatrix(1.0f);
        shader.setMat4("model", modelMatrix);

        // Draw the model
        backpackModel.draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}