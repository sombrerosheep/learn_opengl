#include <cmath>
#include <map>
#include <stdio.h>

#include <glad/glad.h>

#include <camera.hpp>
#include <shader.hpp>

#include <SDL.h>
#include <glm.hpp>
#include <gtc/constants.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <model.hpp>
#include <stb_image.h>

#include <texture.hpp>

const int screenHeight = 720;
const int screenWidth = 1280;

float deltaTime = 0.0f;
Uint64 lastFrame = 0;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

bool firstMouse = true;

void process_input() {
  const Uint8 *keys = SDL_GetKeyboardState(NULL);

  if (keys[SDL_GetScancodeFromKey(SDLK_w)]) {
    camera.ProcessKeyboard(FORWARD, deltaTime);
  }
  if (keys[SDL_GetScancodeFromKey(SDLK_s)]) {
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  }
  if (keys[SDL_GetScancodeFromKey(SDLK_a)]) {
    camera.ProcessKeyboard(LEFT, deltaTime);
  }
  if (keys[SDL_GetScancodeFromKey(SDLK_d)]) {
    camera.ProcessKeyboard(RIGHT, deltaTime);
  }
}

void logGlInit() {
  printf("Vendor   : %s\n", glGetString(GL_VENDOR));
  printf("Renderer : %s\n", glGetString(GL_RENDERER));
  printf("Version  : %s\n", glGetString(GL_VERSION));
  printf("GLSL     : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

  int maj, min;
  SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &maj);
  SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &min);
  printf("Context  : %d.%d\n", maj, min);

  glGetIntegerv(GL_MAJOR_VERSION, &maj);
  glGetIntegerv(GL_MINOR_VERSION, &min);
  printf("Context  : %d.%d\n\n", maj, min);
}

#define ASSERT_SDL_SUCCESS(x)                                                  \
  if ((x) != 0) {                                                              \
    printf("SDL Error: %s (%s:%d)\n", SDL_GetError(), __FILE__, __LINE__);     \
    return -1;                                                                 \
  }

int main(int argc, char **argv) {
  bool running = true;
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("Error initializing SDL: %s\n", SDL_GetError());
    return -1;
  }

  SDL_Window *window = NULL;
  if ((window = SDL_CreateWindow("LearnOpenGL", 100, 100, screenWidth,
                                 screenHeight, SDL_WINDOW_OPENGL)) == NULL) {
    printf("Error creating SDL window: %s\n", SDL_GetError());
    return -1;
  }

  ASSERT_SDL_SUCCESS(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3));
  ASSERT_SDL_SUCCESS(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3));
  ASSERT_SDL_SUCCESS(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                                         SDL_GL_CONTEXT_PROFILE_CORE));
  ASSERT_SDL_SUCCESS(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));

  SDL_GLContext context = SDL_GL_CreateContext(window);

  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    printf("Failed to iniailize GLAD\n");
    return -1;
  }

  glViewport(0, 0, screenWidth, screenHeight);

  logGlInit();

  if (SDL_SetRelativeMouseMode(SDL_TRUE) != 0) {
    printf("Error capturing mouse: %s\n", SDL_GetError());
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glCullFace(GL_FRONT);
  glFrontFace(GL_CCW);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Shader textureShader("shaders/texture/basic.vert",
                       "shaders/texture/basic.frag");

  float cubeVertexData[] = {
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, // 0
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, // 1
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // 2
      0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, // 3
      0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // 4
      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, // 5
      -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, // 6
      0.5f,  -0.5f, 0.5f,  0.0f, 1.0f  // 7
  };
  glm::vec3 cubePositions[] = {glm::vec3(-1.0f, 0.0f, -1.0f),
                               glm::vec3(2.0f, 0.0f, 0.0f)};
  float planeVertexData[] = {5.0f, -0.51f, 5.0f,   2.0f,  0.0f,   -5.0f, -0.51f,
                             5.0f, 0.0f,   0.0,    -5.0f, -0.51f, -5.0f, 0.0f,
                             2.0f, 5.0f,   -0.51f, -5.0f, 2.0f,   2.0f};
  unsigned int planeIndices[] = {0, 1, 2, 2, 3, 0};
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
      -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
      0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, 0.5f,  -0.5f, 0.0f, 1.0f, 1.0f,
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, -0.5f, 0.5f,  0.0f, 0.0f, 0.0f,
  };
  glm::vec3 windows[] = {
      glm::vec3(-1.0f, 0.0f, -0.48f), glm::vec3(2.0f, 0.0f, 0.51f),
      glm::vec3(0.0f, 0.0f, 0.7f), glm::vec3(-0.3f, 0.0f, -2.3f),
      glm::vec3(0.5f, 0.0f, -0.6f)};

  Texture marble_tex("./textures/marble.jpg", "texture");
  Texture metal_tex("./textures/metal.png", "texture");
  Texture window_tex("./textures/blending_transparent_window.png", "texture");

  unsigned int floorVAO, floorVBO, floorEBO;

  glGenVertexArrays(1, &floorVAO);
  glGenBuffers(1, &floorVBO);
  glGenBuffers(1, &floorEBO);

  glBindVertexArray(floorVAO);
  glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertexData), planeVertexData,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeIndices), planeIndices,
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5,
                        (void *)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5,
                        (void *)(sizeof(GL_FLOAT) * 3));

  unsigned int windowVAO, windowVBO;
  glGenVertexArrays(1, &windowVAO);
  glGenBuffers(1, &windowVBO);

  glBindVertexArray(windowVAO);
  glBindBuffer(GL_ARRAY_BUFFER, windowVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(window_verts), window_verts,
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5,
                        (void *)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5,
                        (void *)(sizeof(GL_FLOAT) * 3));

  unsigned int cubeVAO, cubeVBO, cubeEBO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);
  glGenBuffers(1, &cubeEBO);

  glBindVertexArray(cubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexData), cubeVertexData,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices,
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5,
                        (void *)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5,
                        (void *)(sizeof(GL_FLOAT) * 3));

  SDL_Event event;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }

      if (event.type == SDL_WINDOWEVENT) {
        switch (event.window.event) {
        case SDL_WINDOWEVENT_RESIZED: {
          glViewport(0, 0, event.window.data1, event.window.data2);
        }
        }
      }

      if (event.type == SDL_MOUSEMOTION) {
        float xOffset, yOffset;

        if (firstMouse) {
          xOffset = 0.f;
          yOffset = 0.f;
          firstMouse = false;
        } else {
          xOffset = event.motion.xrel;
          yOffset = -event.motion.yrel;
        }

        camera.ProcessMouseMovement(xOffset, yOffset);
      }

      if (event.type == SDL_MOUSEWHEEL) {
        camera.ProcessMouseScroll(event.wheel.y);
      }

      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          running = false;
        }
      }
    }

    Uint64 frame_ticks = SDL_GetTicks();
    deltaTime = (float)(frame_ticks - lastFrame) / 1000.f;
    lastFrame = frame_ticks;

    process_input();

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);

    glm::mat4 projection = glm::perspective(
        glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight,
        0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model(1.0f);

    glActiveTexture(GL_TEXTURE0);

    textureShader.use();
    glBindVertexArray(floorVAO);
    glBindTexture(GL_TEXTURE_2D, metal_tex.GetID());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
    textureShader.setInt("tex", 0);
    textureShader.setMat4("projection", projection);
    textureShader.setMat4("view", view);
    textureShader.setMat4("model", model);
    glDrawElements(GL_TRIANGLES, sizeof(planeIndices), GL_UNSIGNED_INT, 0);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBindTexture(GL_TEXTURE_2D, marble_tex.GetID());
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
    glBindTexture(GL_TEXTURE_2D, window_tex.GetID());
    textureShader.setInt("tex", 0);
    textureShader.setMat4("projection", projection);
    textureShader.setMat4("view", view);
    std::map<float, glm::vec3> sorted;
    for (unsigned int i = 0; i < num_windows; i++) {
      float distance = glm::length(camera.Position - windows[i]);
      sorted[distance] = windows[i];
    }

    for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin();
         it != sorted.rend(); ++it) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, it->second);
      textureShader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    SDL_GL_SwapWindow(window);
  }

  glDeleteVertexArrays(1, &floorVAO);
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteBuffers(1, &floorVBO);
  glDeleteBuffers(1, &cubeVBO);
  glDeleteBuffers(1, &floorEBO);
  glDeleteBuffers(1, &cubeEBO);

  SDL_DestroyWindow(window);
  return 0;
}
