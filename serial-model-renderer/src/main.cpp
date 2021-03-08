#include <small3d/Renderer.hpp>
#include <small3d/SceneObject.hpp>
#include <GLFW/glfw3.h>
#include <filesystem>

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

int main(int argc, char** argv) {
  try {
    Renderer* renderer = &Renderer::getInstance("Serial renderer", 800, 600);
    GLFWwindow* window = renderer->getWindow();
    glfwSetKeyCallback(window, keyCallback);

    for (const auto& entry : std::filesystem::directory_iterator("resources/models")) {
      const std::string objTextureName = "objTexture";
      LOGINFO("Loading " + entry.path().string());
      SceneObject obj("object", entry.path().string(), "");

      float scale = 2.0f / ((obj.boundingBoxSet.boxExtremes[0].maxX - obj.boundingBoxSet.boxExtremes[0].minX) / obj.getModel().origScale.x);

      obj.getModel().scale = glm::vec3(scale);

      obj.offset = glm::vec3(0.0f, -1.0f, -6.0f);

      bool hasTexture = false;

      if (obj.getModel().defaultTextureImage != nullptr) {
        hasTexture = true;
        renderer->generateTexture(objTextureName, *obj.getModel().defaultTextureImage);
      }

      bool done = false;

      double startSeconds = glfwGetTime();
      double seconds = glfwGetTime();
      double prevSeconds = seconds;
      const uint32_t framerate = 60;
      constexpr double secondsInterval = 1.0 / framerate;
      obj.startAnimating();
      while (!glfwWindowShouldClose(window) && !done && seconds - startSeconds < 3.0) {

        glfwPollEvents();
        seconds = glfwGetTime();
        if (seconds - prevSeconds > secondsInterval) {
          prevSeconds = seconds;
          if (esckey)
            break;
          obj.animate();
          obj.rotation.y += 0.02f;
          renderer->clearScreen();
          hasTexture ?
            renderer->render(obj, objTextureName) :
            renderer->render(obj, glm::vec4(0.4f, 1.0f, 0.4f, 1.0f));
          renderer->swapBuffers();
        }
        if (esckey) done = true;
      }

      renderer->deleteTexture(objTextureName);
      renderer->clearBuffers(obj);

      if (done) break;
    }
  }
  catch (std::runtime_error& e) {
    LOGERROR(e.what());
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
