//
//  GoogleMapsManager.h
//  pointcast
//
//  Created by Mitsuo Okada on 2015/12/08.
//
//

#ifndef GoogleMapsManager_h
#define GoogleMapsManager_h

#include "cocos2d.h"
#include "lib/Util.hpp"

namespace lib {

namespace native {

class GoogleMapsManager {
  // member
public:
private:
  // function
public:
  static void attach(void);

  static void detach(void);

  static void attachMarker(std::string title, std::string snippet,
                           double latitude, double longtitude,
                           cocos2d::Color3B color, int zorder);

  static cocos2d::Color3B getMaekerColor(double cpm, int conversion_rate) {

    const double usv = (double)cpm / (double)conversion_rate;
    cocos2d::Color3B color = lib::Util::GetLutColor(usv);
    return color;

    /*
     old pattern

  if (usv < 0.23) {
    // green (normaly) < 0.23usv/h
    color = cocos2d::Color3B::GREEN;
  } else if (usv < 0.50f) {
    // yellow (coution) < 0.50usv/h
    color = cocos2d::Color3B(255, 255, 0);
  } else {
    // red (denger) more
    color = cocos2d::Color3B::RED;
  }
  return color;
     */
  }

private:
};
}
}

#endif /* GoogleMapsManager_h */
