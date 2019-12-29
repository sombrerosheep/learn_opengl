#include <stdio.h>
#include <cmath>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <shader.hpp>
#include <camera.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

const int screenHeight = 600;
const int screenWidth = 800;

float blend = 0.2f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(
  glm::vec3(0.0f, 0.0f, 10.0f),
  glm::vec3(0.0f, 1.0f, 0.0f),
  -90.0f,
  0.0f
);

glm::vec3 lightPos(2.0, 2.0f, 4.0f);

float lastX = screenHeight / 2.0f;
float lastY = screenWidth / 2.0f;
bool firstMouse = true;

float clampZeroOne(float value) {
  if (value > 1.0f) {
    return 1.0f;
  } else if (value < 0.0f) {
    return 0.0f;
  }

  return value;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
  float cameraSpeed = 2.5f * deltaTime;
  
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    blend += 0.015f * (float)glfwGetTime() / 1000.f;
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    blend -= 0.015f * (float)glfwGetTime() / 1000.f;
  }
  blend = clampZeroOne(blend);

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

  Shader ourShader("shaders/basic/shader.vert", "shaders/basic/shader.frag");
  Shader lampShader("shaders/basic/shader.vert", "shaders/basic/lightSource.frag");

  glm::vec3 cubePositions[] = {
    glm::vec3{0.0f, 0.0f, 0.0f},
    glm::vec3{0.0f, 2.0f, 0.0f},
    glm::vec3{2.0f, 2.0f, 0.0f},
    glm::vec3{2.0f, 0.0f, 0.0f},
    glm::vec3{2.0f, -2.0f, 0.0f},
    glm::vec3{0.0f, -2.0f, 0.0f},
    glm::vec3{-2.0f, -2.0f, 0.0f},
    glm::vec3{-2.0f, 0.0f, 0.0f},
    glm::vec3{-2.0f, 2.0f, 0.0f}
  };

  glm::vec3 cubeAmbients[] = {
    glm::vec3(0.0215f, 0.1745f, 0.0215f), // emerald
    glm::vec3(0.05375f, 0.05f, 0.06625f), // obsidian
    glm::vec3(0.1f, 0.18725f, 0.1745f), // turquoise
    glm::vec3(0.2125f, 0.1275f, 0.054f), // bronze
    glm::vec3(0.19225f, 0.19225f, 0.19225f), // silver
    glm::vec3(0.0f, 0.1f, 0.06f), // cyan plastic
    glm::vec3(0.0f, 0.0f, 0.0f), // red plastic
    glm::vec3(0.0f, 0.05f, 0.0f), // green rubber
    glm::vec3(0.05f, 0.05f, 0.05f) // white rubber
  };

  glm::vec3 cubeDiffuses[] = {
    glm::vec3(0.07568f, 0.61424f, 0.07568f),
    glm::vec3(0.18275f, 0.17f, 0.22525f),
    glm::vec3(0.396f, 0.74151f, 0.69102f),
    glm::vec3(0.714f, 0.4284f, 0.18144f),
    glm::vec3(0.50754f, 0.50754f, 0.50754f),
    glm::vec3(0.0f, 0.50980392f, 0.50980392f),
    glm::vec3(0.5f, 0.0f, 0.0f),
    glm::vec3(0.4f, 0.5f, 0.4f),
    glm::vec3(0.5f, 0.5f, 0.5f)
  };

  glm::vec3 cubeSpeculars[] = {
    glm::vec3(0.633f, 0.727811f, 0.633),
    glm::vec3(0.332741f, 0.328634f, 0.346435),
    glm::vec3(0.297254f, 0.30829f, 0.306678),
    glm::vec3(0.393548f, 0.271906f, 0.166721),
    glm::vec3(0.508273f, 0.508273f, 0.508273),
    glm::vec3(0.50196078f, 0.50196078f, 0.50196078),
    glm::vec3(0.7f, 0.6f, 0.6),
    glm::vec3(0.04f, 0.7f, 0.04),
    glm::vec3(0.7f, 0.7f, 0.7)
  };

  float cubeShininesses[] = {
    0.6f,
    0.3f,
    0.1f,
    0.2f,
    0.4f,
    0.25f,
    0.25f,
    0.078125f,
    0.078125f
  };

  float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
  };

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(1);

  unsigned int lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glEnable(GL_DEPTH_TEST);

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    process_input(window);
    glm::mat4 lightPosition(1.0f);
    lightPosition = glm::rotate(lightPosition, currentFrame, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::vec3 theLightPos = lightPosition * glm::vec4(lightPos, 1.0f);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ourShader.use();
    ourShader.setVec3("lightColor", glm::vec3(1.0f));
    ourShader.setVec3("viewPos", camera.Position);
    ourShader.setVec3("light.position", theLightPos);
    ourShader.setVec3("light.ambient", glm::vec3(1.0f));
    ourShader.setVec3("light.diffuse", glm::vec3(1.0f));
    ourShader.setVec3("light.specular", glm::vec3(1.0));

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    ourShader.setMat4("projection", projection);
    ourShader.setMat4("view", view);

    glBindVertexArray(VAO);
    for (int i = 0; i < 9; i++) {
      ourShader.setVec3("material.ambient", cubeAmbients[i]);
      ourShader.setVec3("material.diffuse", cubeDiffuses[i]);
      ourShader.setVec3("material.specular", cubeSpeculars[i]);
      ourShader.setFloat("material.shininess", cubeShininesses[i] * 128.0f);
    
      glm::mat4 model(1.0f);
      model = glm::translate(model, cubePositions[i]);
      ourShader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    lampShader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, theLightPos);
    model = glm::scale(model, glm::vec3(0.2));
    lampShader.setMat4("model", model);
    lampShader.setMat4("view", view);
    lampShader.setMat4("projection", projection);
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  glfwTerminate();  
  return 0;
}

