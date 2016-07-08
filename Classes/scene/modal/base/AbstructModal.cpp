//
//  AbstructModal.cpp
//  pointcast
//
//  Created by Leverages Mitsuo Okada on 2015/11/07.
//
//

#include "AbstructModal.hpp"

namespace scene {

namespace modal {

namespace base {

USING_NS_CC;

AbstructModal *AbstructModal::create() {
  AbstructModal *ret = new (std::nothrow) AbstructModal();
  if (ret && ret->init()) {
    ret->autorelease();
    return ret;
  } else {
    CC_SAFE_DELETE(ret);
    return nullptr;
  }
}

bool AbstructModal::init() {
  if (!Layer::init()) {
    return false;
  }

  this->_background = NULL;

  //ダイアログ外透過カラーレイヤー生成
  this->_colorSheet = LayerColor::create(Color4B(0, 0, 0, 90));
  this->_colorSheet->setContentSize(Size(640 * 3, 1136 * 3));
  this->_colorSheet->setPosition(Point(-640, -1136));

  this->addChild(_colorSheet, 0, 999);

  //タッチイベント設定
  _setupEventListener();

  return true;
}

void AbstructModal::_setupEventListener() {
  //タッチ用イベントリスナーの生成
  auto touchListener = EventListenerTouchOneByOne::create();

  //自身より奥にある部品へタッチイベントを流さないための設定
  touchListener->setSwallowTouches(true);

  //タッチダウンイベント設定
  touchListener->onTouchBegan =
      CC_CALLBACK_2(AbstructModal::onTouchBegan, this);

  //タッチリリースイベント設定
  //    touchListener->onTouchEnded =
  //        CC_CALLBACK_2(AbstructModal::onTouchEnded, this);

  //生成したイベントリスナーをディスパッチャーへ登録
  _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool AbstructModal::onTouchBegan(Touch *touch, Event *event) {
  //ダイアログ背景設定されている場合
  if (_background != nullptr) {
    //背景の矩形範囲を取得
    Rect rect = _background->getBoundingBox();
    //タッチ座標取得
    Vec2 location = touch->getLocation();

    //タッチ座標が背景矩形範囲内かの判定
    if (rect.containsPoint(location)) {
      CCLOG("on Touch Background!!");

      //ダイアログ削除は、ダイアログ外タッチ(カラーレイヤーのタッチ)で行うため
      //背景がタッチされた場合は、リリース処理を行わないためにfalseを返却する
      return false;
    }
  }

  CCLOG("on Touch ColorSheet!!");

  //「ダイアログ背景内のタッチがない == ダイアログ外タッチ」のため
  // trueを返却し、タッチリリース処理を行う
  return true;
}

void AbstructModal::onTouchEnded(Touch *touch, Event *event) {
  //タッチリリースのタイミングで、ダイアログ削除要求
  this->close();
}

void AbstructModal::close() {
  //自身の削除
  this->removeFromParentAndCleanup(true);
}
}
}
}