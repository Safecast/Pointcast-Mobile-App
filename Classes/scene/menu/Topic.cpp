//
//  Topic.cpp
//  pointcast
//
//  Created by Leverages Mitsuo Okada on 2015/10/29.
//
//

#include "Topic.hpp"

USING_NS_CC;

namespace scene {
namespace menu {
bool Topic::init() {
    if (!Node::init()) {
        return false;
    }

    // add text
    Size winSize = Director::getInstance()->getVisibleSize();
    auto text = Label::createWithSystemFont("Topic", "HiraKakuProN-W6", 32);
    text->setPosition(Point(winSize.width / 2, winSize.height / 6));
    text->setColor(Color3B::WHITE);
    this->addChild(text);

    return true;
}
}
}