#include <stdio.h>
#include <cmath>
#include <map>

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

unsigned int load_image(const char *path) {
  int height, width, nChannels;
  unsigned int texture, format;

  unsigned char *imageData = stbi_load(path, &width, &height, &nChannels, 0);
  
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
    
  if (!imageData) {
    printf("Failed to load image data from path: %s\n", path);
    return 0;
  }

  switch (nChannels) {
    case 1: {
      format = GL_RED;
      break;
    }
    case 3: {
      format = GL_RGB;
      break;
    }
    case 4: {
      format = GL_RGBA;
      break;
    }
    default: {
      printf("Unsupported image channels: %d for image at %s\n", nChannels, path);
      stbi_image_free(imageData);
      return 0;
    }
  }

  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(imageData);

  return texture;
}

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

  glViewport(0, 0, screenWidth, screenHeight);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glCullFace(GL_FRONT);
  glFrontFace(GL_CCW);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Shader textureShader("shaders/texture/basic.vert", "shaders/texture/basic.frag");
 
  float cubeVertexData[] = {
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // 0
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // 1
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // 2
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // 3
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, // 4
    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // 5
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, // 6
     0.5f, -0.5f,  0.5f, 0.0f, 1.0f  // 7
  };
  glm::vec3 cubePositions[] = {
    glm::vec3(-1.0f, 0.0f, -1.0f),
    glm::vec3(2.0f, 0.0f, 0.0f)
  };
  float planeVertexData[] = {
     5.0f, -0.51f,  5.0f, 2.0f, 0.0f,
    -5.0f, -0.51f,  5.0f, 0.0f, 0.0,
    -5.0f, -0.51f, -5.0f, 0.0f, 2.0f,
     5.0f, -0.51f, -5.0f, 2.0f, 2.0f
  };
  unsigned int planeIndices[] = {
    0, 1, 2, 2, 3, 0
  };
  unsigned int cubeIndices[] = {
    0, 1, 2, 2, 3, 0, // front
    4, 0, 3, 3, 7, 4, // right
    4, 7, 6, 6, 5, 4, // back
    1, 5, 6, 6, 2, 1, // left
    4, 5, 1, 1, 0, 4, // top
    3, 2, 6, 6, 7, 3  // bottom
  };
  unsigned int num_windows = 5;
  float window_verts[] = {
    -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
  };
  glm::vec3 windows[] = {
    glm::vec3(-1.0f, 0.0f, -0.48f),
    glm::vec3( 2.0f, 0.0f,  0.51f),
    glm::vec3( 0.0f, 0.0f,  0.7f),
    glm::vec3(-0.3f, 0.0f, -2.3f),
    glm::vec3( 0.5f, 0.0f, -0.6f)
  };

  unsigned int texture_marble = load_image("./textures/marble.jpg");
  unsigned int texture_metal = load_image("./textures/metal.png");
  unsigned int texture_window = load_image("./textures/blending_transparent_window.png");
  
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

  unsigned int windowVAO, windowVBO;
  glGenVertexArrays(1, &windowVAO);
  glGenBuffers(1, &windowVBO);

  glBindVertexArray(windowVAO);
  glBindBuffer(GL_ARRAY_BUFFER, windowVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(window_verts), window_verts, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (void*)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (void*)(sizeof(GL_FLOAT) * 3));

  unsigned int cubeVAO, cubeVBO, cubeEBO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);
  glGenBuffers(1, &cubeEBO);

  glBindVertexArray(cubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexData), cubeVertexData, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (void*)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (void*)(sizeof(GL_FLOAT) * 3));

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    process_input(window);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model(1.0f);

    glActiveTexture(GL_TEXTURE0);

    textureShader.use();
    glBindVertexArray(floorVAO);
    glBindTexture(GL_TEXTURE_2D, texture_metal);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
    textureShader.setInt("tex", 0);
    textureShader.setMat4("projection", projection);
    textureShader.setMat4("view", view);
    textureShader.setMat4("model", model);
    glDrawElements(GL_TRIANGLES, sizeof(planeIndices), GL_UNSIGNED_INT, 0);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBindTexture(GL_TEXTURE_2D, texture_marble);
    textureShader.setInt("tex", 0);
    textureShader.setMat4("projection", projection);
    textureShader.setMat4("view", view);
    for (int i = 0; i < 2; i++) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      textureShader.setMat4("model", model);
      glDrawElements(GL_TRIANGLES, sizeof(cubeIndices), GL_UNSIGNED_INT, 0);
    }

    glDisable(GL_CULL_FACE);
    glBindVertexArray(windowVAO);
    glBindBuffer(GL_ARRAY_BUFFER, windowVBO);
    glBindTexture(GL_TEXTURE_2D, texture_window);
    textureShader.setInt("tex", 0);
    textureShader.setMat4("projection", projection);
    textureShader.setMat4("view", view);
    std::map<float, glm::vec3> sorted;
    for (unsigned int i = 0; i < num_windows; i++) {
      float distance = glm::length(camera.Position - windows[i]);
      sorted[distance] = windows[i];
    }

    for (std::map<float,glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, it->second);
      textureShader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &floorVAO);
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteBuffers(1, &floorVBO);
  glDeleteBuffers(1, &cubeVBO);
  glDeleteBuffers(1, &floorEBO);
  glDeleteBuffers(1, &cubeEBO);
  glfwTerminate();  
  return 0;
}

