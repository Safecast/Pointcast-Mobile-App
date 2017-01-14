//
//  Main.cpp
//  pointcast
//
//  Created by Mitsuo Okada on 2015/10/29.
//
//

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

#include "lib/native/CCCoreLocation.h"
#include "lib/network/DataStoreSingleton.hpp"
#include "scene/layout/helper/Contents.hpp"
#include "scene/menu/About.hpp"
#include "scene/menu/Map.hpp"
#include "scene/menu/Sensors.hpp"
#include "scene/menu/Topic.hpp"

#include "scene/Main.hpp"

USING_NS_CC;

namespace scene {

cocos2d::Scene *Main::createScene() {
  CCLOG("createScene");
  auto scene = Scene::create();
  auto layer = Main::create();
  scene->addChild(layer);
  return scene;
}

bool Main::init() {
  if (!AbstructScene::init()) {
    return false;
  }

  // add notification
  Director::getInstance()->getEventDispatcher()->addCustomEventListener("footer_visible",[=](cocos2d::EventCustom *event) {
      CCLOG("イベント受け取ったよ > %s",event->getEventName().c_str());
      auto visible = (cocos2d::Value *)event->getUserData();
      this->setLowerMenuVisible(visible->asBool());
  });
    
  // Corelocation から現在位置を取得しておく
  CCCoreLocation *p_core_location = new CCCoreLocation();
  p_core_location->requestLocation();
  bool location_enable = p_core_location->isLocationAvailable();
  CCLOG("location enable bool %d", location_enable);

  // initial scene
  this->_e_scene_id = Scene_Initialized_e;

  // main scene を読み込み
  this->_p_contents = CSLoader::getInstance()->createNode("res/MenuMain.csb");

  // SET footer
  this->_p_footer = CSLoader::getInstance()->createNode("res/layerFooter.csb");

  // lower menu の振る舞いを設定
  this->setLowerMenu();

  // set initial scene
  this->nextScene(E_Scene_Id::Scene_Topic_Opend_e);

  // set first view
  // disble welcome animation
  // this->attachWelcomeAnimation();

  // lower menuを追加
  this->addChild(this->_p_contents, Zorders_Main_Contents);

  // add footer
  this->addChild(this->_p_footer, Zorders_Footer);

  return true;
}

void Main::onEnter(void) {
  // super class onEnter
  scene::base::AbstructScene::onEnter();

  // Http Request For Home Data
  lib::network::DataStoreSingleton *p_data_store_singleton =
      lib::network::DataStoreSingleton::getInstance();

  // this->attachWelcomeAnimation();

  // http request pointcast/home.json
  p_data_store_singleton->setResponseCallback(
      this,
      (cocos2d::network::SEL_HttpResponse)(&Main::onCallbackPointcastHome));
  p_data_store_singleton->requestPointcastHome();
}

void Main::onCallbackPointcastHome(cocos2d::network::HttpClient *sender,
                                   cocos2d::network::HttpResponse *response) {
  // remove wait animation
  // this->detachWaitAnimation();

  // and after automatically
  this->setScheduleHome();

  // Go Sensors
  this->touchSensors();

  CCLOG("Home::onCallbackPointcastHome");
}

void Main::setLowerMenu(void) {
  auto panelBackground =
      this->_p_footer->getChildByName<ui::Layout *>("panelBackground");

  //    panelBackground->addTouchEventListener(
  //        [this](Ref* sender, ui::Widget::TouchEventType type) {
  //            if (type == ui::Widget::TouchEventType::BEGAN) {
  //                CCLOG("panelBackground ON BEGAN");
  //                return false;
  //            }
  //        });

  // Topic
  /*
  auto buttonTopic =
      panelBackground->getChildByName<ui::Button*>("buttonHome");
  buttonTopic->addTouchEventListener(
      [this](Ref* sender, ui::Widget::TouchEventType type) {
          if (type == ui::Widget::TouchEventType::ENDED) {
              this->touchTopic();
          }
      });
   */
  // List
  auto buttonList =
      panelBackground->getChildByName<ui::Button *>("buttonSensors");
  buttonList->addTouchEventListener(
      [this](Ref *sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
          this->touchSensors();
        }
      });

  // Map
  auto buttonMap = panelBackground->getChildByName<ui::Button *>("buttonMap");
  buttonMap->addTouchEventListener(
      [this](Ref *sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
          this->touchMap();
        }
      });

  // about
  //  auto buttonAbout =
  //      panelBackground->getChildByName<ui::Button *>("buttonAbout");
  //  buttonAbout->addTouchEventListener(
  //      [this](Ref *sender, ui::Widget::TouchEventType type) {
  //        if (type == ui::Widget::TouchEventType::ENDED) {
  //          this->touchAbout();
  //        }
  //      });
}

void Main::touchTopic(void) {
  CCLOG("touchTopic");
  
  // click se
  CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("res/sound/se/click.mp3");

  this->nextScene(E_Scene_Id::Scene_Topic_Opend_e);
}

void Main::touchSensors(void) {
  CCLOG("touchList");
    
  // click se
  CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("res/sound/se/click.mp3");
    
  this->nextScene(E_Scene_Id::Scene_Sensors_Opend_e);
}

void Main::touchSensorsBack() {
  auto p_current_contents = this->getChildByTag(this->_e_scene_id);

  // click se
  CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(
      "res/sound/se/click.mp3");

  if (p_current_contents == NULL) {
    return;
  }

  scene::layout::helper::Contents::SlideOut(
      p_current_contents, 0.2f,
      scene::layout::helper::Contents::Forward_To_Bottom_e);
}

void Main::touchMap(void) {
  CCLOG("touchMap");
  // click se
  CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("res/sound/se/click.mp3");
  this->nextScene(E_Scene_Id::Scene_Map_Opend_e);
}

void Main::touchAbout(void) {
  CCLOG("touchAbout");
  // click se
  CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("res/sound/se/click.mp3");
  this->nextScene(E_Scene_Id::Scene_About_Opend_e);
}

void Main::nextScene(E_Scene_Id next_scene_id) {
  // temprary scene_id
  E_Scene_Id current_scene_id = this->_e_scene_id;

  // if same scene
  if (next_scene_id == current_scene_id) {
    return;
  }

  Node *p_next_scene = NULL;
  CallFunc *callback = nullptr;
  AbstructScene::Object_Tag_Id tag;

  auto p_current_contents = this->getChildByTag(this->_current_contents_tag_id);
  if (p_current_contents != NULL) {
    p_current_contents->removeFromParent();
  }

  switch (next_scene_id) {

  case Scene_Topic_Opend_e:
    p_next_scene = menu::Topic::create();
    tag = Tag_Id_Main;
    break;
  case Scene_Sensors_Opend_e:
    p_next_scene = menu::Sensors::create();
    tag = Tag_Id_Sensor;
    callback =
        CallFunc::create(p_next_scene, SEL_CallFunc(&menu::Sensors::refresh));
    break;
  case Scene_Map_Opend_e:
    p_next_scene = menu::Map::create();
    tag = Tag_Id_Map;
    callback =
        CallFunc::create(p_next_scene, SEL_CallFunc(&menu::Map::refresh));
    break;
  default:
    assert(false);
    break;
  }

  // set first contents
  p_next_scene->setTag(tag);
  this->_current_contents_tag_id = tag;

  scene::layout::helper::Contents::SlideIn(
      p_next_scene, 0.0f, scene::layout::helper::Contents::Forward_To_Top_e,
      callback);
  this->_e_scene_id = next_scene_id;
  this->addChild(p_next_scene, Zorders_Main_Contents);
}

void Main::attachWelcomeAnimation(void) {
  // welcome
  auto text_welcome = Label::createWithSystemFont("welcome", "System", 48);
  text_welcome->setPosition(Point(280, 750));
  text_welcome->setColor(Color3B(52, 152, 219));
  text_welcome->setOpacity(0);
  auto action_welcome = FadeTo::create(2, 255);
  text_welcome->runAction(action_welcome);

  this->_p_contents->addChild(text_welcome);

  // to
  auto delay_to = DelayTime::create(1);
  auto text_to = Label::createWithSystemFont("to", "System", 48);
  text_to->setPosition(Point(430, 750));
  text_to->setColor(Color3B(52, 152, 219));
  text_to->setOpacity(0);
  auto action_to = FadeTo::create(3, 255);
  auto sequence_to = Sequence::create(delay_to, action_to, NULL);
  text_to->runAction(sequence_to);

  this->_p_contents->addChild(text_to);

  // safecast
  auto sprite_logo = Sprite::create("safecast-square-ct.png");
  sprite_logo->setPosition(Point(330, 500));
  sprite_logo->setOpacity(0);

  auto delay_logo = DelayTime::create(3);
  auto action_logo = FadeTo::create(5, 255);
  auto sequence_logo = Sequence::create(delay_logo, action_logo, NULL);
  sprite_logo->runAction(sequence_logo);

  this->_p_contents->addChild(sprite_logo);
}

void Main::touchMapBack(void) {
  auto p_current_contents = this->getChildByTag(this->_e_scene_id);
  scene::layout::helper::Contents::SlideOut(
      p_current_contents, 0.2f,
      scene::layout::helper::Contents::Forward_To_Bottom_e);
}

void Main::touchAboutBack(void) {
  auto p_current_contents = this->getChildByTag(this->_e_scene_id);
  scene::layout::helper::Contents::SlideOut(
      p_current_contents, 0.2f,
      scene::layout::helper::Contents::Forward_To_Bottom_e);
}

void Main::setScheduleHome(void) {
  // SetScheduler
  float interval = 300.0f; // @todo optimize
  schedule(schedule_selector(::scene::Main::updateHome), interval);
}

void Main::updateHome(float dt) {

  // @todo mutex lock
  // Http Request For Home Data
  lib::network::DataStoreSingleton *p_data_store_singleton =
      lib::network::DataStoreSingleton::getInstance();

  // http request pointcast/home.json
  p_data_store_singleton->setResponseCallback(
      this,
      (cocos2d::network::SEL_HttpResponse)(&Main::onCallbackScheduleHome));
  p_data_store_singleton->requestPointcastHome();

  if (this->_e_scene_id == Scene_Sensors_Opend_e) {
    // if sensors appear update list
    menu::Sensors *p_current_scene = static_cast<menu::Sensors *>(
        this->getChildByTag(Scene_Sensors_Opend_e));
    if (p_current_scene) {
      p_current_scene->refresh();
    }
  }

  CCLOG("Main::updateHome");
}

void Main::onCallbackScheduleHome(cocos2d::network::HttpClient *sender,
                                  cocos2d::network::HttpResponse *response) {

  CCLOG("Home::onCallbackScheduleHome");
  // @todo mutex unlock
}

void Main::unScheduleHome(void) {}
    
void Main::setLowerMenuVisible(bool visible)
{
    this->_p_footer->setVisible(visible);
}
    
}
