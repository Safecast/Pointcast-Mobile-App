//
//  Map.cpp
//  pointcast
//
//  Created by Leverages Mitsuo Okada on 2015/10/29.
//
//

#include "Map.hpp"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "lib/Util.hpp"
#include "lib/native/GoogleMapsManager.h"
#include "lib/network/DataStoreSingleton.hpp"
#include "scene/Main.hpp"
#include "scene/base/AbstructScene.hpp"

USING_NS_CC;

namespace scene {
namespace menu {
bool Map::init() {
  if (!Node::init()) {
    return false;
  }

  // load map layout
  this->_p_contents = CSLoader::getInstance()->createNode(
      "res/MenuMap.csb"); // .csbを読み込んだディレクトリを保持してリソースを探す指定

  // 戻るボタンの設定
  //  auto panel =
  //      this->_p_contents->getChildByName<ui::Layout *>("panelBackground");

  //    auto label_back = panel->getChildByName<ui::Text *>("txtBack");
  //  label_back->addTouchEventListener(
  //      [this](Ref *sender, ui::Widget::TouchEventType type) {
  //        if (type == ui::Widget::TouchEventType::ENDED) {
  //          this->touchBack();
  //        }
  //      });

  this->_p_contents->setTag(Main::Tag_Id_Map);
  this->addChild(this->_p_contents);

  return true;
}

void Map::touchBack() {
  auto p_parent_scene = static_cast<scene::Main *>(this->getParent());
  p_parent_scene->TouchMapBack();
}

void Map::onEnter(void) { cocos2d::Node::onEnter(); }

void Map::refresh(void) {
  // Attach Google Map
  lib::native::GoogleMapsManager::attach();

  // Attach Marker
  std::map<int, lib::object::LocationItem> m_sensors =
      lib::network::DataStoreSingleton::getInstance()->getLocationItemAll();
  for (auto item : m_sensors) {
    if (item.second.recent_value == 0 || item.second.sensor_status != 1) {
      continue;
    }
    Color3B color = lib::native::GoogleMapsManager::getMaekerColor(
        item.second.recent_value, item.second.conversion_rate);
    double usv =
        (double)item.second.recent_value / (double)item.second.conversion_rate;
    usv = lib::Util::round(usv, 2);
    std::string string_usv = std::to_string(usv).substr(0, 4); // @todo

    std::string string_captued_at =
        item.second.recent_captured_at.substr(11, 5); // @todo
    std::string info_title =
        string_usv + "μSv/h " +
        std::to_string(static_cast<int>(item.second.recent_value)) + "CPM (" +
        string_captued_at + ")"; // yyy usv/h xxx cpm (12:56)

    std::stringstream snippet;
    snippet << item.second.name_en << "(" << item.second.sensor1_device_id
            << ")";

    lib::native::GoogleMapsManager::attachMarker(
        info_title, snippet.str(), item.second.latitude, item.second.longitude,
        color, item.second.recent_value);
  }
}

void Map::onExit(void) {
  cocos2d::Node::onExit();
  lib::native::GoogleMapsManager::detach();
}
}
}
