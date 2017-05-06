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
  enum DIALOG_MODE {
    DIALOG_MODE_YES   = 0,       // Yesのみ
    DIALOG_MODE_YESNO, // Yes No
  };
  
  enum BUTTON_TYPE {
    YES_BUTTTON = 0,
    NO_BUTTON
  };
  
  Node *_p_contents;
  
  DIALOG_MODE dialog_mode_;
  
  // タイトルテキスト
  std::string title_string_;
  
  // 本文テキスト
  std::string description_string_;

  // Yesボタンテキスト
  std::string yes_button_string_;
  
  // Yesの時のコールバック
  cocos2d::CallFunc* yes_callback_function_;
  
  // Noボタンテキスト
  std::string no_button_string_;

  // Noコールバック
  cocos2d::CallFunc* no_callback_function_;
 
  // function
public:
  Dialog();

  ~Dialog();

  virtual bool init();
    
  static Dialog *create(std::string title, std::string description, std::string yes_button_string, cocos2d::CallFunc *yes_callback);

  // このメセッドを通すと、YesNoの機能を提供する
  void setNoCondition( std::string no_button_string, cocos2d::CallFunc *no_callback);

  void setYesCallback(cocos2d::CallFunc *callback);
  
  void executeLayout();

  void show();
  
  void close(BUTTON_TYPE button_type);
    
  private:
};
}
}

#endif /* Sort_hpp */
