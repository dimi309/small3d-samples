#include <android_native_app_glue.h>
#include <jni.h>
#include <android/asset_manager.h>
#include <android/configuration.h>

#include <iostream>
#include <unistd.h>
#include "include/GameLogic.hpp"

extern "C" {
#include "vkzos.h"
}

using namespace small3d;
using namespace std;
using namespace AvoidTheBug3D;

GameLogic *gameLogic;
KeyInput input;

float halfx = 0.0f;
float halfy = 0.0f;

static bool gameLogicInstantiated = false;
static bool gameActive = false;

JNIEnv *env = NULL;
jobject tm;
jmethodID adjustVolume;

extern "C" {

void handle_cmd(android_app *pApp, int32_t cmd) {
  switch (cmd) {
    case APP_CMD_INIT_WINDOW:
    case APP_CMD_GAINED_FOCUS:
      if (!gameActive) {
        if (!gameLogicInstantiated) {
          gameLogic = new GameLogic();
          gameLogicInstantiated = true;
        } else {
          gameLogic->resume();
        }
        halfx = gameLogic->renderer->getScreenWidth() / 2;
        halfy = gameLogic->renderer->getScreenHeight() / 2;
        gameActive = true;
      }
      break;

    case APP_CMD_TERM_WINDOW:
    case APP_CMD_LOST_FOCUS:
    case APP_CMD_SAVE_STATE:
    case APP_CMD_STOP:
    if(gameActive) {
      gameActive = false;
      gameLogic->pause();
    }
      break;

    default:
      LOGDEBUG("event not handled: " + std::to_string(cmd));
  }
}

void initVolumeControl(){

  vkz_android_app->activity->vm->AttachCurrentThread(&env, NULL);

  jclass context = env->FindClass("android/content/Context");

  if (!context) {
    LOGERROR("Failed to get context class!");
  }

  jfieldID audioServiceField = env->GetStaticFieldID(context, "AUDIO_SERVICE", "Ljava/lang/String;");

  jstring jstr = (jstring)env->GetStaticObjectField(context, audioServiceField);

  jmethodID getSystemServiceID = env->GetMethodID(context, "getSystemService",
    "(Ljava/lang/String;)Ljava/lang/Object;");

  tm = env->CallObjectMethod(vkz_android_app->activity->clazz, getSystemServiceID, jstr);

  jclass AudioManager = env->FindClass("android/media/AudioManager");

  if(!AudioManager) {
    throw std::runtime_error("Could not get AudioManager.");
  }

  adjustVolume = env->GetMethodID(AudioManager, "adjustVolume", "(II)V");

  if(!adjustVolume) {
    throw std::runtime_error("Could not get adjustVolume method id.");
  }

}

int32_t handle_input(android_app *app, AInputEvent *event) {
  LOGDEBUG("Input event");
  if (gameActive) {
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
      input.enter = true;

      float xval = AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_X, 0);
      float yval = AMotionEvent_getAxisValue(event, AMOTION_EVENT_AXIS_Y, 0);

      if (xval > halfx) {
        input.right = true;
      } else {
        input.left = true;
      }

      if (yval > halfy) {
        input.down = true;
      } else {
        input.up = true;
      }
    } else if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY) {
      switch (AKeyEvent_getKeyCode(event)) {
        case AKEYCODE_VOLUME_DOWN:
          env->CallVoidMethod(tm, adjustVolume, -1, 1);
          break;
        case AKEYCODE_VOLUME_UP:
          env->CallVoidMethod(tm, adjustVolume, 1, 1);
          break;
        default:
          // do nothing
          break;
      }

    }
  }

  return 1;
}

void android_main(struct android_app *state) {

  vkz_android_app = state;

  state->onAppCmd = handle_cmd;
  state->onInputEvent = handle_input;

  initLogger();

  int events;
  android_poll_source *pSource;

  initVolumeControl();

  const uint32_t frameRate = 60;
  double seconds = gameLogic->currentTimeInSeconds();
  double prevSeconds = seconds;
  double secondsInterval = 1.0 / frameRate;

  do {
    input = {};
    if (ALooper_pollAll(0, nullptr, &events, (void **) &pSource) >= 0) {
      if (pSource != NULL) {
        pSource->process(state, pSource);
      }
    }

    if (gameActive) {
      seconds = gameLogic->currentTimeInSeconds();

      if (seconds - prevSeconds > secondsInterval) {
        gameLogic->process(input);
        prevSeconds = seconds;
        gameLogic->render();
      }
    }

  } while (!state->destroyRequested);

}
}