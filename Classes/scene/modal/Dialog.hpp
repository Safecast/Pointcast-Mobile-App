//
//  Dialog.hpp
//  pointcast
//
//  Created by Mitsuo Okada on 2017/05/04.
//
//

#ifndef Dialog_hpp
#define Dialog_hpp

#include <stdio.h>
#include <string>

#include "cocos2d.h"
#include "scene/modal/base/AbstructModal.hpp"
#include "ui/CocosGUI.h"

namespace scene {

namespace modal {

class Dialog : public base::AbstructModal {
  // member
public:
private:
  Node *_p_contents;

  // function
public:
  Dialog();

  ~Dialog();

  virtual bool init();
    
  static Dialog *create();
  
  void show();
    
  private:
};
}
}

#endif /* Sort_hpp */
