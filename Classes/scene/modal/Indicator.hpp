//
//  Inidicator.hpp
//  pointcast
//
//  Created by Mitsuo Okada on 2015/11/05.
//
//

#ifndef Inidicator_hpp
#define Inidicator_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "lib/external/CCActivityIndicator.h"
#include "scene/modal/base/AbstructModal.hpp"

namespace scene {

namespace modal {

class Indicator : public base::AbstructModal {
  // member
public:
private:
  cocos2d::DrawNode *_p_draw_node;

  CCActivityIndicator *_p_activity_indicator;
  // function
public:
  Indicator();

  ~Indicator();

  void onEnter(void);

  void onExit(void);

  void attachAnimation(void);

  void deatachAnimation(void);

  virtual bool init();

  static Indicator *create();

  virtual void close();

private:
};
}
}

#endif /* Inidicator_hpp */
