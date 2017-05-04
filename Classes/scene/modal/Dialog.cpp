//
//  Dialog.cpp
//  pointcast
//
//  Created by Mitsuo Okada on 2017/05/04.
//
//
#include "Dialog.hpp"
#include "scene/base/AbstructScene.hpp"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;

using namespace ui;

namespace scene {

namespace modal {

Dialog::Dialog() {}

Dialog::~Dialog() {}

Dialog *Dialog::create() {
  Dialog *ret = new (std::nothrow) Dialog();
  if (ret && ret->init()) {
    ret->autorelease();
    return ret;
  } else {
    CC_SAFE_DELETE(ret);
    return nullptr;
  }
}

bool Dialog::init() {
  if (!base::AbstructModal::init()) {
    return false;
  }

  // load map layout
  this->_p_contents =
      cocos2d::CSLoader::getInstance()->createNode("res/dialogBase.csb");

 
  this->addChild(this->_p_contents, 99999);
    
  return true;
}

void Dialog::show() {
    
}

}
}

