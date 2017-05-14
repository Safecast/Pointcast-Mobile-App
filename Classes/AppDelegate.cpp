#include "AppDelegate.h"
#include "scene/Main.hpp"
#include "lib/Util.hpp"
#include "scene/layout/helper/Display.hpp"

USING_NS_CC;

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

// if you want a different context,just modify the value of glContextAttrs
// it will takes effect on all platforms
void AppDelegate::initGLContextAttrs() {
  // set OpenGL context attributions,now can only set six attributions:
  // red,green,blue,alpha,depth,stencil
  GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

  GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
  // initialize director
  auto director = Director::getInstance();
  auto glview = director->getOpenGLView();

  Size display_size = scene::layout::helper::Display::GetDrawingArea();

  float game_screen_height = 1136.0f;
  float game_screen_width = 640.0f;
  
  if (!glview) {
    glview = GLViewImpl::createWithRect(
        "pointcast", Rect(0, 0, game_screen_width, game_screen_height));
    director->setOpenGLView(glview);
  }
  // @todo optimize device size
  director->getOpenGLView()->setDesignResolutionSize(
      game_screen_width, game_screen_height, ResolutionPolicy::EXACT_FIT);
  
  // scall to fit
  // float fit_scale = display_size.width / 640.0f;
  // director->setContentScaleFactor(fit_scale);

#if defined(ENABLE_DEBUG_MODE)
  // turn on display FPS
  director->setDisplayStats(true);
#endif

  // set FPS. the default value is 1.0/60 if you don't call this
  // director->setAnimationInterval(1.0 / 60);
  director->setAnimationInterval(1.0 / 60);

  FileUtils::getInstance()->addSearchPath("res");

  // create a scene. it's an autorelease object
  auto scene = scene::Main::createScene();

  // run
  director->runWithScene(scene);

  return true;
}

// This function will be called when the app is inactive. When comes a phone
// call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
  Director::getInstance()->stopAnimation();

  // if you use SimpleAudioEngine, it must be pause
  // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
  Director::getInstance()->startAnimation();

  // if you use SimpleAudioEngine, it must resume here
  // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
