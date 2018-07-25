//
//  Search.cpp
//  pointcast
//
//  Created by Mitsuo Okada on 2015/11/05.
//
//
#include "Search.hpp"
#include "cocostudio/CocoStudio.h"
#include "lib/native/Util.h"

USING_NS_CC;
using namespace ui;

namespace scene {

namespace modal {

Search::Search() {}

Search::~Search() {}

Search *Search::create() {
  Search *ret = new (std::nothrow) Search();
  if (ret && ret->init()) {
    ret->autorelease();
    return ret;
  } else {
    CC_SAFE_DELETE(ret);
    return nullptr;
  }
}

bool Search::init() {
  if (!base::AbstructModal::init()) {
    return false;
  }

  // load map layout
  this->_p_contents =
      cocos2d::CSLoader::getInstance()->createNode("res/Search.csb");

  // ページビューの作成
  /*
  auto p_page_sortitem =
    this->_p_contents->getChildByName<ui::PageView *>("pageSortItem");
  */
  // set select button

  auto p_panel_background =
      this->_p_contents->getChildByName<ui::Layout *>("panelBackground");
  auto p_input_text =
      p_panel_background->getChildByName<ui::TextField *>("txtInput");
  p_input_text->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);

  auto p_layer_background =
      this->_p_contents->getChildByName<ui::Layout *>("layerBackground");
  auto p_label_decide =
      this->_p_contents->getChildByName<ui::Text *>("txtDecide");
  p_label_decide->addTouchEventListener([this](
      Ref *sender, ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
      auto p_panel_background =
          this->_p_contents->getChildByName<ui::Layout *>("panelBackground");
      auto p_input_text =
          p_panel_background->getChildByName<ui::TextField *>("txtInput");
      CCLOG("text is %s", p_input_text->getString().c_str());
      // detach keyboard
      lib::native::Util::closeIme();
      this->close(p_input_text->getString());
    }
  });

  //イベントリスナーを追加（using namespace ui; しないと参照出来ないので注意）
  //  p_page_sortitem->addEventListenerPageView(
  //      this, pagevieweventselector(Search::sor));
  this->setContentSize(this->_p_contents->getContentSize());
  this->addChild(this->_p_contents);

  return true;
}

// callback
void Search::searchItemCallBack(Ref *sender, ui::PageViewEventType type) {
  // nothing to do.
  return;
  if (type == ui::PAGEVIEW_EVENT_TURNING) {
    auto pageView = dynamic_cast<ui::PageView *>(sender);
    log("%ld", pageView->getCurPageIndex() + 1);
  }
}

void Search::onEnter(void) { AbstructModal::onEnter(); }

void Search::onExit(void) { AbstructModal::onExit(); }

void Search::detachSlideIn(void) { CCLOG("Sort::detachSlideIn"); }

void Search::close(void) {

  auto p_menu_sensors = static_cast<menu::Sensors *>(this->getParent());
  // p_menu_sensors->setSortId(sort_id);
  // p_menu_sensors->setOrderDesc(has_desc);
  p_menu_sensors->refresh();

  base::AbstructModal::close();
}

std::string Search::getLableName(int sort_id) {
  switch (sort_id) {
  case scene::menu::Sensors::Sort_Id_Device_Id: {
    return "Device Id";
    break;
  }
  case scene::menu::Sensors::Sort_Id_μSv_Hour: {
    return "μSv/hour";
    break;
  }
  default:
    break;
  }

  return "";
}

void Search::close(std::string key_words) {

  auto p_menu_sensors =
      static_cast<menu::Sensors *>(this->getParent()->getChildByTag(
          scene::base::AbstructScene::Tag_Id_Sensor));
  p_menu_sensors->setKeyWord(key_words);
  p_menu_sensors->refresh();

  base::AbstructModal::close();
}
}
}
