//
//  Contents.hpp
//  pointcast
//
//  Created by Mitsuo Okada on 2015/11/09.
//
//

#ifndef Contents_hpp
#define Contents_hpp

#include <stdio.h>

#include "cocos2d.h"

namespace scene {
namespace layout {
namespace helper {

class Contents {
  // member
public:
  enum E_Forward_To_Id {
    Forward_To_Top_e = 1,
    Forward_To_Right_e = 2,
    Forward_To_Bottom_e = 3,
    Forward_To_Left_e = 4,
  };

private:
  // function
public:
  static void SlideIn(cocos2d::Node *node, float duration,
                      E_Forward_To_Id forward_to_id,
                      cocos2d::CallFunc *callback = nullptr,
                      bool has_vertical_center = false,
                      bool has_horizontal_center = false);

  static void SlideOut(cocos2d::Node *node, float duration,
                       E_Forward_To_Id forward_to_id, bool remove_self = true,
                       cocos2d::CallFunc *callback = nullptr);


private:
    
};
    
}
}
}
#endif /* Contents_hpp */
