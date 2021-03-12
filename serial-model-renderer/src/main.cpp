#include <small3d/Renderer.hpp>
#include <small3d/SceneObject.hpp>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <small3d/GlbFile.hpp>
#include <memory>

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

};

struct Object {
  std::shared_ptr<SceneObject> so;
  std::string textureName = "";
};

int main(int argc, char** argv) {
  try {
    Renderer* renderer = &Renderer::getInstance("Serial renderer", 800, 600, 0.78f, 1.0f, 24.0f, "resources/shaders/", 500);
    renderer->cameraPosition = glm::vec3(0.0f, 0.0f, 0.9f);
    GLFWwindow* window = renderer->getWindow();
    glfwSetKeyCallback(window, keyCallback);

    for (const auto& entry : std::filesystem::directory_iterator("resources/models")) {

      std::vector<Object> objects;
      LOGINFO("Loading " + entry.path().string());

      float maxX = 0.0f;
      float minX = 0.0f;
      std::vector<std::string> names;

      try
      {
        names = GlbFile(entry.path().string()).getMeshNames();
      }
      catch (std::runtime_error& e) {
        names.push_back("");
      }

      for (auto& name : names) {
        Object ob;
        ob.so = std::shared_ptr<SceneObject>(new SceneObject(name, entry.path().string(), name));
        if (ob.so->getModel().defaultTextureImage != nullptr) {
          renderer->generateTexture(name, *ob.so->getModel().defaultTextureImage);
          ob.textureName = name;
        }

        if (ob.so->boundingBoxSet.boxExtremes[0].minX < minX) minX = ob.so->boundingBoxSet.boxExtremes[0].minX;
        if (ob.so->boundingBoxSet.boxExtremes[0].maxX > maxX) maxX = ob.so->boundingBoxSet.boxExtremes[0].maxX;

        objects.push_back(ob);

      }


      float scale = 2.0f / (maxX - minX);

      for (auto& ob : objects) {
        ob.so->getModel().scale = glm::vec3(scale);
        ob.so->offset += glm::vec3(0.0f, 0.0f, -5.0f);
        ob.so->startAnimating();
      }

      bool done = false;

      double startSeconds = glfwGetTime();
      double seconds = glfwGetTime();
      double prevSeconds = seconds;
      const uint32_t framerate = 60;
      constexpr double secondsInterval = 1.0 / framerate;

      while (!glfwWindowShouldClose(window) && !done && seconds - startSeconds < 5.0) {

        glfwPollEvents();
        seconds = glfwGetTime();
        if (seconds - prevSeconds > secondsInterval) {
          prevSeconds = seconds;
          if (esckey)
            break;
          renderer->clearScreen();
          for (auto& ob : objects) {

            ob.so->animate();
            ob.so->rotation.y += 0.02f;

            ob.textureName != "" ?
              renderer->render(*ob.so, ob.textureName) :
              renderer->render(*ob.so, glm::vec4(0.4f, 1.0f, 0.4f, 1.0f));
          }


          renderer->swapBuffers();
        }
        if (esckey) done = true;
      }

      for (auto& ob : objects) {
        renderer->deleteTexture(ob.textureName);
        renderer->clearBuffers(*ob.so);
      }
      if (done) break;
    }
  }
  catch (std::runtime_error& e) {
    LOGERROR(e.what());
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
