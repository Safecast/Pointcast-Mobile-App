//
//  Sort.hpp
//  pointcast
//
//  Created by Mitsuo Okada on 2015/11/05.
//
//

#ifndef Sort_hpp
#define Sort_hpp

#include <stdio.h>
#include <string>

#include "cocos2d.h"
#include "scene/menu/Sensors.hpp"
#include "scene/modal/base/AbstructModal.hpp"
#include "ui/CocosGUI.h"

namespace scene {

namespace modal {

class Sort : public base::AbstructModal {
  // member
public:
private:
  Node *_p_contents;

  // function
public:
  Sort();

  ~Sort();

  void onEnter(void);

  void onExit(void);

  void detachSlideIn(void);

  void sortItemCallBack(Ref *sender, cocos2d::ui::PageViewEventType type);

  virtual bool init();

  static Sort *create();

  virtual void close(int sort_id, bool has_desc = false);

  static std::string getLableName(int sort_id);

private:
};
}
}

#endif /* Sort_hpp */
