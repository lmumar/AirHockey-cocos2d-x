#include "AppDelegate.h"
#include "GameLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace CocosDenshion;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(glview == nullptr) {
        glview = GLView::create("Air Hockey");
        director->setOpenGLView(glview);
    }

    // initialize audio engine
    auto hitwav = FileUtils::getInstance()->fullPathForFilename("hit.wav");
    auto scorewav = FileUtils::getInstance()->fullPathForFilename("score.wav");
    
    SimpleAudioEngine::getInstance()->preloadEffect(hitwav.c_str());
    SimpleAudioEngine::getInstance()->preloadEffect(scorewav.c_str());

    Size screenSize = glview->getFrameSize();
    if (screenSize.width > 768) {
        FileUtils::getInstance()->setSearchPaths({"hd"});
        director->setContentScaleFactor(2);
    } else {
        FileUtils::getInstance()->setSearchPaths({"sd"});
        director->setContentScaleFactor(1);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = GameLayer::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
