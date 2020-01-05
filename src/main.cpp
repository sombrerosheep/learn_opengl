#include <stdio.h>
#include <cmath>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <shader.hpp>
#include <camera.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#include <model.hpp>

const int screenHeight = 600;
const int screenWidth = 800;

float blend = 0.2f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(glm::vec3(3.0f, 17.0f, 11.0f), glm::vec3(0.0f, 1.0f, 0.0f), -100.0f, -20.0f);

float lastX = screenHeight / 2.0f;
float lastY = screenWidth / 2.0f;
bool firstMouse = true;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
  float cameraSpeed = 2.5f * deltaTime;
  
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.ProcessKeyboard(FORWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.ProcessKeyboard(LEFT, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.ProcessKeyboard(RIGHT, deltaTime);
  }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = (float)xpos;
    lastY = (float)ypos;
    firstMouse = false;
  }

  float xOffset = (float)xpos - lastX;
  float yOffset = lastY - (float)ypos;
  
  lastX = (float)xpos;
  lastY = (float)ypos;

  camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset) {
  camera.ProcessMouseScroll((float)yOffset);
}

int main(int argc, char** argv) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);

  if (window == NULL) {
    printf("Failed to create GLFW window\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD\n");
    return -1;
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  Shader modelShader("shaders/model/model.vert", "shaders/model/model.frag");

  Model nanoSuit("models/nanosuit/nanosuit.obj");

  float lightConstant = 1.0f;
  float lightLinear = 0.09f;
  float lightQuadratic = 0.032f;

  glEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    process_input(window);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modelShader.use();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    modelShader.setMat4("projection", projection);
    modelShader.setMat4("view", view);

    glm::mat4 model(1.0f);
    modelShader.setMat4("model", model);

    nanoSuit.Draw(modelShader);    

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();  
  return 0;
}

