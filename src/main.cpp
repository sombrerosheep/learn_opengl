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

#include <stb_image.h>

const int screenHeight = 720;
const int screenWidth = 1280;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

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

  Shader shader("shaders/texture/basic.vert", "shaders/texture/basic.frag");  
  Shader lightShader("shaders/lighting/light.vert", "shaders/lighting/light.frag");

  float cubeVertexData[] = {
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, // 0
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, // 1
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // 2
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, // 3
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // 4
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // 5
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // 6p
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f  // 7
  };
  float planeVertexData[] = {
     5.0f, -0.5f,  5.0f, 2.0f, 0.0f,
    -5.0f, -0.5f,  5.0f, 0.0f, 0.0,
    -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
     5.0f, -0.5f, -5.0f, 2.0f, 2.0f
  };
  unsigned int planeIndices[] = {
    0, 1, 2, 2, 3, 0
  };
  unsigned int lightCubeIndices[] = {
    0, 1, 2, 2, 3, 0, // front
    4, 0, 3, 3, 7, 4, // right
    4, 7, 6, 6, 5, 4, // back
    1, 5, 6, 6, 2, 1, // left
    4, 5, 1, 1, 0, 4, // top
    3, 2, 6, 6, 7, 3  // bottom
  };

  unsigned int floorVAO, floorVBO, floorEBO;
  glGenVertexArrays(1, &floorVAO);
  glGenBuffers(1, &floorVBO);
  glGenBuffers(1, &floorEBO);

  glBindVertexArray(floorVAO);
  glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertexData), planeVertexData, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeIndices), planeIndices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (void*)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (void*)(sizeof(GL_FLOAT) * 3));
  
  unsigned int lightVAO, lightVBO, lightEBO;
  glGenVertexArrays(1, &lightVAO);
  glGenBuffers(1, &lightVBO);
  glGenBuffers(1, &lightEBO);

  glBindVertexArray(lightVAO);
  glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexData), cubeVertexData, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lightCubeIndices), lightCubeIndices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (void*)0);

  glm::vec3 lightPos[] = {
    glm::vec3{ 3.0f, 8.5f, 1.0f },
    glm::vec3{ -1.5f, 14.5f, 0.5f }
  };
  glm::vec3 dLightDirection = glm::vec3(-0.2f, -0.3f, 1.0f);
  glm::vec3 lightColor(1.0f, 1.0f, 0.9f);
  glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
  glm::vec3 diffuse = glm::vec3(0.1f, 0.1f, 0.1f);
  glm::vec3 specular = glm::vec3(0.6f, 0.6f, 0.6f);
  glm::vec3 lightAmbient = glm::vec3(0.05f, 0.05f, 0.05f);
  glm::vec3 lightDiffuse = glm::vec3(0.5f, 0.2f, 0.2f);
  glm::vec3 lightSpecular = glm::vec3(1.0f, 0.5f, 0.5f);
  float lightConstant = 0.7f;
  float lightLinear = 0.09f;
  float lightQuadratic = 0.032f;
  float materialShininess = 32.0f;

  lightShader.use();
  lightShader.setVec3("lightColor", lightColor);

  glEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    process_input(window);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model(1.0f);

    shader.use();
    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setMat4("model", model);
    glDrawElements(GL_TRIANGLES, sizeof(planeIndices), GL_UNSIGNED_INT, 0);
    
    lightShader.use();
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
    lightShader.setMat4("projection", projection);
    lightShader.setMat4("view", view);

    glm::mat4 lightModelA(1.0f);
    lightModelA = glm::rotate(lightModelA, currentFrame / 1.8f, glm::vec3(0.0f, 1.0f, 0.0f));
    lightModelA = glm::translate(lightModelA, lightPos[0]);
    lightModelA = glm::scale(lightModelA, glm::vec3(0.2f));
    lightShader.setMat4("model", lightModelA);
    glDrawElements(GL_TRIANGLES, sizeof(lightCubeIndices), GL_UNSIGNED_INT, 0);

    glm::mat4 lightModelB(1.0f);
    lightModelB = glm::rotate(lightModelB, currentFrame / 2.2f, glm::vec3(0.0f, 1.0f, 0.0f));
    lightModelB = glm::translate(lightModelB, lightPos[1]);
    lightModelB = glm::scale(lightModelB, glm::vec3(0.2f));
    lightShader.setMat4("model", lightModelB);
    glDrawElements(GL_TRIANGLES, sizeof(lightCubeIndices), GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();  
  return 0;
}

