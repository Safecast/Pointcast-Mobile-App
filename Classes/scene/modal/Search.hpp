//
//  Search.hpp
//  pointcast
//
//  Created by Leverages Mitsuo Okada on 2015/11/05.
//
//

#ifndef Search_hpp
#define Search_hpp

#include <stdio.h>
#include <string>

#include "cocos2d.h"
#include "scene/menu/Sensors.hpp"
#include "scene/modal/base/AbstructModal.hpp"
#include "ui/CocosGUI.h"

namespace scene {

namespace modal {

class Search : public base::AbstructModal {
  // member
public:
private:
  Node *_p_contents;

  // function
public:
  Search();

  ~Search();

  void onEnter(void);

  void onExit(void);

  void detachSlideIn(void);

  void searchItemCallBack(Ref *sender, cocos2d::ui::PageViewEventType type);

  virtual bool init();

  static Search *create();

  virtual void close(void);

  std::string getLableName(int sort_id);

  virtual void close(std::string key_words);

private:
};
}
}

#endif /* Search_hpp */
