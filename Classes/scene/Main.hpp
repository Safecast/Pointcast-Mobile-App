//
//  Main.hpp
//  pointcast
//
//  Created by Leverages Mitsuo Okada on 2015/10/29.
//
//

#ifndef Home_hpp
#define Home_hpp

#include <stdio.h>

#include "network/HttpClient.h"
#include "scene/base/AbstructScene.hpp"

namespace scene {

class Main : public scene::base::AbstructScene {
  // member
public:
  //表示されているシーン
  enum E_Scene_Id {
    // @brief initialized
    Scene_Initialized_e,

    // @brief topic shown
    Scene_Topic_Opend_e,

    // @brief list shown
    Scene_Sensors_Opend_e,

    // @brief map shown
    Scene_Map_Opend_e,

    // @brief about shown
    Scene_About_Opend_e,
  };

private:
  // contents
  cocos2d::Node *_p_contents;

  // fotter
  cocos2d::Node *_p_footer;

  // scene id
  E_Scene_Id _e_scene_id;

  scene::base::AbstructScene::Object_Tag_Id _current_contents_tag_id;

  // function
public:
  static cocos2d::Scene *createScene();

  void onEnter(void);

  void onCallbackPointcastHome(cocos2d::network::HttpClient *sender,
                               cocos2d::network::HttpResponse *response);

  void attachWelcomeAnimation(void);

  void setLowerMenu(void);

  virtual bool init();

  void touchTopic(void);

  void touchSensors(void);

  void touchSensorsBack(void);

  void touchMap(void);

  void touchMapBack(void);

  void touchAbout(void);

  void touchAboutBack(void);

  void nextScene(E_Scene_Id next_scene_id);

  void setScheduleHome(void);

  void updateHome(float dt);

  void onCallbackScheduleHome(cocos2d::network::HttpClient *sender,
                              cocos2d::network::HttpResponse *response);

  void unScheduleHome(void);

  CREATE_FUNC(Main);

private:
};
}

#endif /* Home_hpp */
