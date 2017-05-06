//
//  Menu.hpp
//  pointcast
//
//  Created by Mitsuo Okada on 2015/10/29.
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
  // LinkUrlとの紐付けのためのENUM。
  // 101始まりの連番で、最後に必ずLink_Id_End_eを設置すること
  // buttonName->linkUrlで連動している
  enum Tag_Link_Id_e {
    Link_Id_WhatIsSafeCast_e = 101,
    
    Link_Id_AboutPointcast_e,
    
    Link_Id_ReadOurBlog_e,
    
    Link_Id_AccessTheMap_e,
    
    Link_Id_Faq_e,
    
    Link_Id_End_e,
  };
  
  
  Node *_p_contents;

  // function
public:
  virtual bool init();

  void touchBack();

  void refresh();
  
  void touchUrlLink(Tag_Link_Id_e tag_id);
  
  std::string getLinkUrl(Tag_Link_Id_e tag_id);

  CREATE_FUNC(About);

private:
};
}
}
#endif /* Menu_hpp */
