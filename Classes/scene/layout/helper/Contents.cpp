//
//  Contents.cpp
//  pointcast
//
//  Created by Leverages Mitsuo Okada on 2015/11/09.
//
//

#include "Contents.hpp"

USING_NS_CC;

namespace scene {
namespace layout {
namespace helper {

void Contents::SlideIn(cocos2d::Node *node, float duration,
                       Contents::E_Forward_To_Id forward_to_id,
                       CallFunc *callback, bool has_vertical_center,
                       bool has_horizontal_center) {

  CCLOG("node size h %f w %f", node->getContentSize().width,
        node->getContentSize().height);

  // adjust contents size
  Size winSize = Director::getInstance()->getWinSize();

  int height = winSize.height;
  int width = winSize.width;

  float absolute_height = 0.0f;
  if (has_vertical_center) {
    absolute_height = height / 2;
  }

  Point p;
  switch (forward_to_id) {
  case Forward_To_Top_e:
    p = Point(0, -1 * height);
    break;
  case Forward_To_Right_e:
    p = Point(-1 * width, absolute_height);
    break;
  case Forward_To_Bottom_e:
    p = Point(0, height);
    break;
  case Forward_To_Left_e:
    p = Point(width, absolute_height);
    break;
  default:
    p = Point(0, 0);
    assert(false);
    break;
  };

  // set animation
  node->setPosition(p);

  ActionInterval *action = MoveTo::create(duration, Point(0, absolute_height));
  Sequence *sequence = Sequence::create(action, callback, NULL);
  // auto easeAction = EaseBounceOut::create(action);
  node->runAction(sequence);
}

void Contents::SlideOut(cocos2d::Node *node, float duration,
                        Contents::E_Forward_To_Id forward_to_id,
                        bool remove_self, CallFunc *callback) {
  int height = node->getContentSize().height;
  int width = node->getContentSize().width;

  Point p;
  switch (forward_to_id) {
  case Forward_To_Top_e:
    p = Point(0, height);
    break;
  case Forward_To_Right_e:
    p = Point(width, 0);
    break;
  case Forward_To_Bottom_e:
    p = Point(0, -1 * height);
    break;
  case Forward_To_Left_e:
    p = Point(-1 * width, 0);
    break;
  default:
    p = Point(0, 0);
    assert(false);
    break;
  };
  // set animation
  ActionInterval *action = MoveTo::create(duration, p);
  // auto easeAction = EaseBounceOut::create(action);

  Sequence *sequence;
  if (remove_self) {
    auto remove = RemoveSelf::create(true);
    sequence = Sequence::create(action, remove, callback, NULL);
  } else {
    sequence = Sequence::create(action, callback, NULL);
  }

  node->runAction(sequence);
}
    
}
}
}
