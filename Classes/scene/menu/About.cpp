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

  // Url Link
  for(int i = Link_Id_WhatIsSafeCast_e; i < Link_Id_End_e; i++) {
    std::stringstream button_name;
    button_name << "buttonUrlLink" << i;
    log("Url Link buttton name %s", button_name.str().c_str());
    auto buttonUrlLink = panel->getChildByName<ui::Button *>(button_name.str());
    assert(buttonUrlLink != nullptr);
    buttonUrlLink->addTouchEventListener(
     [this](Ref *sender, ui::Widget::TouchEventType type) {
       auto object = static_cast<cocos2d::ui::Button*>(sender);
       if (type == ui::Widget::TouchEventType::ENDED) {
         Tag_Link_Id_e tag_id = static_cast<Tag_Link_Id_e>(object->getTag());
         this->touchUrlLink(tag_id);
       }
     });
  }
  
  this->addChild(this->_p_contents);

  return true;
}

void About::touchBack() {
  auto p_parent_scene = static_cast<scene::Main *>(this->getParent());
  p_parent_scene->touchAboutBack();
}

void About::refresh() { return; }

void About::touchUrlLink(Tag_Link_Id_e tag_id) {
  log("About::touchWhatIsSafecast tag_id: %d", tag_id);
  Application::getInstance()->openURL(this->getLinkUrl(tag_id));
}

std::string About::getLinkUrl(Tag_Link_Id_e tag_id) {
  
  std::string url = "http://";
  switch (tag_id) {
    case Link_Id_WhatIsSafeCast_e:
      url += "blog.safecast.org";
      break;
    case Link_Id_AboutPointcast_e:
      url += "blog.safecast.org/?s=pointcast";
      break;
    case Link_Id_ReadOurBlog_e:
      url += "blog.safecast.org/news";
      break;
    case Link_Id_AccessTheMap_e:
      url += "safecast.org/tilemap";
      break;
    case Link_Id_Faq_e:
      url += "blog.safecast.org/faq";
      break;
    default:
      url += "safecast.org";
      break;
  };
  
  return url;
}
  
}
}
