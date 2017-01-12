//
//  Menu.cpp
//  pointcast
//
//  Created by Mitsuo Okada on 2015/10/29.
//
//

#include "About.hpp"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "scene/Main.hpp"
#include "scene/base/AbstructScene.hpp"

USING_NS_CC;

namespace scene {
namespace menu {
bool About::init() {
  if (!Node::init()) {
    return false;
  }

  // load map layout
  this->_p_contents = CSLoader::getInstance()->createNode(
      "res/MenuAbout.csb"); // .csbを読み込んだディレクトリを保持してリソースを探す指定

  // 戻るボタンの設定
  auto panel =
      this->_p_contents->getChildByName<ui::Layout *>("panelBackground");
  auto label_back = panel->getChildByName<ui::Text *>("txtBack");
  label_back->addTouchEventListener(
      [this](Ref *sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
          this->touchBack();
        }
      });

  this->addChild(this->_p_contents);

  return true;
}

void About::touchBack() {
  auto p_parent_scene = static_cast<scene::Main *>(this->getParent());
  p_parent_scene->touchAboutBack();
}
}
}
