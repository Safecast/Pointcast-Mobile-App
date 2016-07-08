//
//  Map.hpp
//  pointcast
//
//  Created by Leverages Mitsuo Okada on 2015/10/29.
//
//

#ifndef Map_hpp
#define Map_hpp

#include <stdio.h>

#include "cocos2d.h"

namespace scene {
namespace menu {
class Map : public cocos2d::Node {
  // member
public:
private:
  Node *_p_contents;
  // function
public:
  void onEnter(void);

  void onExit(void);

  void refresh(void);

  virtual bool init();

  void touchBack();

  CREATE_FUNC(Map);

private:
};
}
}
#endif /* Map_hpp */
