//
//  Menu.hpp
//  pointcast
//
//  Created by Leverages Mitsuo Okada on 2015/10/29.
//
//

#ifndef About_hpp
#define About_hpp

#include <stdio.h>

#include "cocos2d.h"

namespace scene {
namespace menu {
class About : public cocos2d::Node {
    // member
   public:
   private:
    Node* _p_contents;

    // function
   public:
    virtual bool init();

    void touchBack();

    CREATE_FUNC(About);

   private:
};
}
}
#endif /* Menu_hpp */
