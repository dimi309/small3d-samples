#include <small3d/Renderer.hpp>
#include <small3d/SceneObject.hpp>
#include <GLFW/glfw3.h>

using namespace small3d;

bool downkey, leftkey, rightkey, upkey, esckey;

void keyCallback(GLFWwindow* window, int key, int scancode, int action,
		 int mods)
{
  if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    downkey = true;
  if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    upkey = true;
  if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    leftkey = true;
  if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    rightkey = true;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    esckey = true;
  if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
    downkey = false;
  if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
    upkey = false;
  if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
    leftkey = false;
  if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
    rightkey = false;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
    esckey = false;

}

int main(int argc, char **argv) {
  Renderer *renderer = &Renderer::getInstance("Ball demo");
  GLFWwindow* window = renderer->getWindow();
  SceneObject ball("ball", "resources/ball.obj");
  ball.offset = glm::vec3(0.0f, -1.0f, -8.0f);
  glfwSetKeyCallback(window, keyCallback);

  while (!glfwWindowShouldClose(window) && !esckey) {

    glfwPollEvents();
    if (esckey)
      break;
    if (upkey)
      ball.offset.z -= 0.001f;
    else if (downkey)
      ball.offset.z += 0.001f;
    else if (leftkey)
      ball.offset.x -= 0.001f;
    else if (rightkey)
      ball.offset.x += 0.001f;
    renderer->clearScreen();
    renderer->render(ball, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
    renderer->swapBuffers();
  }
  return 0;
}
