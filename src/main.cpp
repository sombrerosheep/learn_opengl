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
  Shader lightShader("shaders/lighting/light.vert", "shaders/lighting/light.frag");

  Model nanoSuit("models/nanosuit/nanosuit.obj");

  float lightCubeVertices[] = {
     0.5f,  0.5f,  0.5f, // 0
    -0.5f,  0.5f,  0.5f, // 1
    -0.5f, -0.5f,  0.5f, // 2
     0.5f, -0.5f,  0.5f, // 3
     0.5f,  0.5f, -0.5f, // 4
    -0.5f,  0.5f, -0.5f, // 5
    -0.5f, -0.5f, -0.5f, // 6
     0.5f, -0.5f, -0.5f  // 7
  };
  unsigned int lightCubeIndices[] = {
    0, 1, 2, 2, 3, 0, // front
    4, 0, 3, 3, 7, 4, // right
    4, 7, 6, 6, 5, 4, // back
    1, 5, 6, 6, 2, 1, // left
    4, 5, 1, 1, 0, 4, // top
    3, 2, 6, 6, 7, 3  // bottom
  };
  
  unsigned int lightVAO, lightVBO, lightEBO;
  glGenVertexArrays(1, &lightVAO);
  glGenBuffers(1, &lightVBO);
  glGenBuffers(1, &lightEBO);

  glBindVertexArray(lightVAO);
  glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(lightCubeVertices), lightCubeVertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lightCubeIndices), lightCubeIndices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, (void*)0);

  glm::vec3 lightPos[] = {
    glm::vec3{ 4.0f, 15.0f, 1.0f },
    glm::vec3{ -4.0f, 2.0f, -4.0f }
  };
  glm::vec3 dLightDirection = glm::vec3(-0.2f, -1.0f, -0.3f);
  glm::vec3 lightColor(1.0f, 1.0f, 0.9f);
  glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
  glm::vec3 diffuse = glm::vec3(0.3f, 0.3f, 0.3f);
  glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
  float lightConstant = 1.0f;
  float lightLinear = 0.09f;
  float lightQuadratic = 0.032f;
  float materialShininess = 32.0f;

  lightShader.use();
  lightShader.setVec3("lightColor", lightColor);

  modelShader.use();
  modelShader.setVec3("pointLights[0].position", lightPos[0]);
  modelShader.setVec3("pointLights[0].ambient", ambient);
  modelShader.setVec3("pointLights[0].diffuse", diffuse);
  modelShader.setVec3("pointLights[0].specular", specular);
  modelShader.setFloat("pointLights[0].constant", lightConstant);
  modelShader.setFloat("pointLights[0].linear", lightLinear);
  modelShader.setFloat("pointLights[0].quadratic", lightQuadratic);
  
  modelShader.setVec3("pointLights[1].position", lightPos[1]);
  modelShader.setVec3("pointLights[1].ambient", ambient);
  modelShader.setVec3("pointLights[1].diffuse", diffuse);
  modelShader.setVec3("pointLights[1].specular", specular);
  modelShader.setFloat("pointLights[1].constant", lightConstant);
  modelShader.setFloat("pointLights[1].linear", lightLinear);
  modelShader.setFloat("pointLights[1].quadratic", lightQuadratic);
  
  modelShader.setVec3("dLight.direction", dLightDirection);
  modelShader.setVec3("dLight.ambient", ambient);
  modelShader.setVec3("dLight.diffuse", diffuse);
  modelShader.setVec3("dLight.specular", specular);
  modelShader.setFloat("shininess", materialShininess);
    
  glEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    process_input(window);

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modelShader.use();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    modelShader.setMat4("projection", projection);
    modelShader.setMat4("view", view);

    glm::mat4 model(1.0f);
    modelShader.setMat4("model", model);
    modelShader.setVec3("viewPos", camera.Position);

    nanoSuit.Draw(modelShader);

    lightShader.use();
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
    
    for (int i = 0; i < 2; i++) {
      glm::mat4 lightModel(1.0f);
      lightModel = glm::translate(lightModel, lightPos[i]);
      lightModel = glm::scale(lightModel, glm::vec3(0.2f));
      lightShader.setMat4("model", lightModel);
      lightShader.setMat4("projection", projection);
      lightShader.setMat4("view", view);
      
      glDrawElements(GL_TRIANGLES, sizeof(lightCubeIndices), GL_UNSIGNED_INT, 0);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();  
  return 0;
}

