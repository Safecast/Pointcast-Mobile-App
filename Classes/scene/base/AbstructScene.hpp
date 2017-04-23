//
//  AbstructScene.hpp
//  pointcast
//
//  Created by Mitsuo Okada on 2015/10/29.
//
//

#ifndef AbstructScene_hpp
#define AbstructScene_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

namespace scene {
namespace base {

class AbstructScene : public cocos2d::Layer {
  // member
public:
  enum Object_Tag_Id {
    // Sensors
    // analytics dialog
    Tag_Id_Mesurements_Analytics = 1001,

    Tag_Id_Main,

    // sensors list record
    Tag_Id_Sensor,

    Tag_Id_Sensor_Record,

    Tag_Id_Map,

    // Animation
    Tag_Id_Wait_Animation_e = 9001,

    // Particle
    Tag_Id_Touch_Particle_e = 10001,

  };

  enum Zorders {

    Zorders_Main_Contents = 1,
      
    Zorders_Analytics_Dialog,

    Zorders_Footer,

    Zorders_Header,

    Zorders_Modal_Dialog,

    Zorders_WaitAnimation,
  };

private:
  // function
public:
  virtual bool init();

  virtual void attachWaitAnimation(void);

  virtual void detachWaitAnimation(void);

  virtual void attachTouchParticle(cocos2d::Point p);

  virtual void detachTouchParticle(void);

  virtual void attachBlueEffect(float blurRadius, float blurSampleNum,
                                int repeat);

  virtual void detachlueEffect(void);

  CREATE_FUNC(AbstructScene);

private:
};
}
}

#endif /* AbstructScene_hpp */
