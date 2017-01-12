//
//  Sort.cpp
//  pointcast
//
//  Created by Mitsuo Okada on 2015/11/05.
//
//
#include "Sort.hpp"
#include "scene/base/AbstructScene.hpp"

#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;

namespace scene {

namespace modal {

Sort::Sort() {}

Sort::~Sort() {}

Sort *Sort::create() {
  Sort *ret = new (std::nothrow) Sort();
  if (ret && ret->init()) {
    ret->autorelease();
    return ret;
  } else {
    CC_SAFE_DELETE(ret);
    return nullptr;
  }
}

bool Sort::init() {
  if (!base::AbstructModal::init()) {
    return false;
  }

  // load map layout
  this->_p_contents =
      cocos2d::CSLoader::getInstance()->createNode("res/Sort.csb");

  // ページビューの作成
  auto p_page_sortitem =
      this->_p_contents->getChildByName<ui::PageView *>("pageSortItem");

  // set select button
  //  auto p_layer_background =
  //      this->_p_contents->getChildByName<ui::Layout *>("layerBackground");
  auto p_label_select =
      this->_p_contents->getChildByName<ui::Text *>("txtSelect");
  p_label_select->addTouchEventListener([this](
      Ref *sender, ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
      ui::PageView *p_page_sortitem =
          this->_p_contents->getChildByName<ui::PageView *>("pageSortItem");
      auto p_selected_item =
          p_page_sortitem->getPage(p_page_sortitem->getCurPageIndex());
      int sort_id = p_selected_item->getTag();
      auto p_panel_background =
          this->_p_contents->getChildByName<ui::Layout *>("panelBackground");
      auto p_checkbox_order =
          p_panel_background->getChildByName<ui::CheckBox *>("chkDesc");
      this->close(sort_id, p_checkbox_order->isSelected());
    }
  });

  // 3ページ作成する
  for (int i = 0; i < scene::menu::Sensors::Sort_Id_EOF; i++) {

    // create layout
    auto layout = ui::Layout::create();
    layout->setContentSize(p_page_sortitem->getContentSize());

    auto label =
        ui::Text::create(this->getLableName(i), "fonts/Marker Felt.ttf", 30);
    label->setColor(Color3B(Color3B::BLUE));
    label->setPosition(Point(layout->getContentSize().width / 2,
                             layout->getContentSize().height / 2));
    layout->addChild(label);

    // set tag
    layout->setTag(i);

    p_page_sortitem->addPage(layout);
  }

  //イベントリスナーを追加（using namespace ui; しないと参照出来ないので注意）
  p_page_sortitem->addEventListenerPageView(
      this, pagevieweventselector(Sort::sortItemCallBack));

  this->setContentSize(this->_p_contents->getContentSize());
  this->addChild(this->_p_contents);

  return true;
}

// callback
void Sort::sortItemCallBack(Ref *sender, ui::PageViewEventType type) {
  // nothing to do.
  return;
  if (type == ui::PAGEVIEW_EVENT_TURNING) {
    auto pageView = dynamic_cast<ui::PageView *>(sender);
    log("%ld", pageView->getCurPageIndex() + 1);
  }
}

void Sort::onEnter(void) { AbstructModal::onEnter(); }

void Sort::onExit(void) { AbstructModal::onExit(); }

void Sort::detachSlideIn(void) { CCLOG("Sort::detachSlideIn"); }

void Sort::close(int sort_id, bool has_desc) {

  auto p_menu_sensors =
      static_cast<menu::Sensors *>(this->getParent()->getChildByTag(
          scene::base::AbstructScene::Tag_Id_Sensor));
  p_menu_sensors->setSortId(sort_id);
  p_menu_sensors->setOrderDesc(has_desc);
  p_menu_sensors->refresh();

  base::AbstructModal::close();
}

std::string Sort::getLableName(int sort_id) {
  switch (sort_id) {
  case scene::menu::Sensors::Sort_Id_Device_Id: {
    return "Device Id";
    break;
  }
  case scene::menu::Sensors::Sort_Id_μSv_Hour: {
    return "μSv/hour";
    break;
  }
  case scene::menu::Sensors::Sort_Id_Near_Point: {
    return "Near Point";
    break;
  }
  case scene::menu::Sensors::Sort_Id_Active_Inactive: {
    return "Active / Inactive";
    break;
  }

  default:
    break;
  }

  return "";
}
}
}
