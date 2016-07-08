//
//  Indicator.cpp
//  pointcast
//
//  Created by Leverages Mitsuo Okada on 2015/11/05.
//
//
#include "Indicator.hpp"

USING_NS_CC;

namespace scene {

namespace modal {

Indicator::Indicator() {
  this->_p_activity_indicator = CCActivityIndicator::create();
  this->addChild(this->_p_activity_indicator, 9999999);
}

Indicator::~Indicator() {}

Indicator *Indicator::create() {
  Indicator *ret = new (std::nothrow) Indicator();
  if (ret && ret->init()) {
    ret->autorelease();
    return ret;
  } else {
    CC_SAFE_DELETE(ret);
    return nullptr;
  }
}

bool Indicator::init() {
  if (!base::AbstructModal::init()) {
    return false;
  }

  return true;
}

void Indicator::onEnter(void) { AbstructModal::onEnter(); }

void Indicator::onExit(void) { AbstructModal::onExit(); }

void Indicator::attachAnimation(void) {
  this->_p_activity_indicator->startAnimating();
}

void Indicator::deatachAnimation(void) {
  this->_p_activity_indicator->stopAnimating();
  base::AbstructModal::close();
}

void Indicator::close() { base::AbstructModal::close(); }
}
}