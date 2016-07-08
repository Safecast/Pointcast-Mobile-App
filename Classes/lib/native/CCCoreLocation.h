//
//  CCCCCoreLocation.h
//
//  Created by franck on 3/19/13.
//
//

#ifndef __CCCoreLocation__
#define __CCCoreLocation__

#include <iostream>

#include "cocos2d.h"
#include <iostream>
// #include "grosdefine.h"

#if (CC_TARGET_PLATFORM == CC_TARGET_OS_IPHONE)
#include "CoreLocation_interface.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#include <jni.h>
#endif

#define LOCATION_KEY "LOCATION_KEY"

struct locationStruct {
  float longi;
  float lati;
};

class CCCoreLocation : public cocos2d::Object

{
public:
  char _LOCATION_KEY[1000];

private:
  cocos2d::SEL_CallFuncND _pSelector;
  cocos2d::Object *_pTarget;
#if (CC_TARGET_PLATFORM == CC_TARGET_OS_IPHONE)
  CoreLocationImpl *_localisationImpl;
#endif
  locationStruct _loc;

public:
  CCCoreLocation();
  void getLocation(cocos2d::Object *pTarget, cocos2d::SEL_CallFuncND pSelector);
  void getLocationDone(float longi, float lati);
  bool isLocationAvailable();
  static CCCoreLocation *getInstance();
#if (CC_TARGET_PLATFORM == CC_TARGET_OS_IPHONE)
  void requestLocation(void);
#endif
};

#ifndef CC_TARGET_OS_IPHONE

#ifdef __cplusplus
extern "C" {
#endif

void Java_org_mumblecore_location_CClocation_nativeupdatePosition(JNIEnv *env,
                                                                  jobject thiz,
                                                                  jdouble a,
                                                                  jdouble b);

#ifdef __cplusplus
}
#endif

#endif
#endif
