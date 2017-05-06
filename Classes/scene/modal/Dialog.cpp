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

Dialog::Dialog()
    : dialog_mode_(DIALOG_MODE_YES), title_string_(""), description_string_(""),
      yes_button_string_(""), yes_callback_function_(nullptr),
      no_button_string_(""), no_callback_function_(nullptr) {}

Dialog::~Dialog() {}

Dialog *Dialog::create(std::string title, std::string description,
                       std::string yes_button_string,
                       cocos2d::CallFunc *yes_callback) {
  Dialog *ret = new (std::nothrow) Dialog();
  ret->title_string_ = title;
  ret->description_string_ = description;
  ret->yes_button_string_ = yes_button_string;
  ret->yes_callback_function_ = yes_callback;
  
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

  this->addChild(this->_p_contents, INT_MAX);

  return true;
}

void Dialog::setNoCondition(std::string no_button_string,
                            cocos2d::CallFunc *no_callback) {

  this->no_button_string_ = no_button_string;
  this->no_callback_function_ = no_callback;
  this->dialog_mode_ = DIALOG_MODE_YESNO;
}

void Dialog::executeLayout() {

  // オブジェクトを格納
  auto title_label = static_cast<cocos2d::ui::Text *>(
      this->_p_contents->getChildByName("txtTitle"));
  auto description_label = static_cast<cocos2d::ui::Text *>(
      this->_p_contents->getChildByName("txtDescription"));
  std::string target_button_tagname = (dialog_mode_ == DIALOG_MODE_YES)
                                          ? "panelButonsYesOnly"
                                          : "panelButonsYesNo";
  auto target_panel_with_buttons = static_cast<cocos2d::ui::Layout *>(
      this->_p_contents->getChildByName(target_button_tagname));

  // タイトル、本文をセット
  title_label->setString(title_string_);
  description_label->setString(description_string_);

  // ボタンの表示をセット
  target_panel_with_buttons->setVisible(true);
  target_panel_with_buttons->setTouchEnabled(true);
  auto yes_button = static_cast<cocos2d::ui::Button *>(
      target_panel_with_buttons->getChildByName("buttonYes"));
  yes_button->setTitleText(yes_button_string_);
  yes_button->addTouchEventListener([this](Ref *sender, ui::Widget::TouchEventType type) {
    CCLOG("yes_button touchend %d", type);
    if (type == ui::Widget::TouchEventType::ENDED) {
      this->close(YES_BUTTTON);
    }
  });
  
  if (dialog_mode_ == DIALOG_MODE_YESNO) {
    auto no_button = static_cast<cocos2d::ui::Button*>(target_panel_with_buttons->getChildByName("buttonNo"));
    no_button->setTitleText(no_button_string_);
    no_button->addTouchEventListener(
        [this](Ref *sender, ui::Widget::TouchEventType type) {
          CCLOG("no_button touchend %d", type);
          if (type == ui::Widget::TouchEventType::ENDED) {
            this->close(NO_BUTTON);
          }
        });
  }
  // 大きさを調整
}

void Dialog::show() {
  // レイアウトを整える
  this->executeLayout();
}
  
void Dialog::close(BUTTON_TYPE button_type) {

  // コールバックする
  switch(button_type) {
    case YES_BUTTTON:
      this->yes_callback_function_->execute();
      this->yes_callback_function_->release();
      break;
    case NO_BUTTON:
      this->no_callback_function_->execute();
      this->no_callback_function_->release();
      break;
  }
  
  // 親から削除する
  this->removeFromParent();
}
  
}
}

