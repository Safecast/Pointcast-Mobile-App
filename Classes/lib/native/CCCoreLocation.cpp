//
//  CCCoreLocation.cpp
//
//  Created by franck on 3/19/13.
//
//

#include "CCCoreLocation.h"
#include "cocos-ext.h"
#include "lib/network/DataStoreSingleton.hpp"

#ifndef CC_PLATFORM_IOS
#include "android/InterfaceJNI.h"
#endif

using namespace cocos2d::extension;
using namespace cocos2d;

static CCCoreLocation *myinstanceT = NULL;

CCCoreLocation *CCCoreLocation::getInstance() {
  if (myinstanceT == NULL) {
    myinstanceT = new CCCoreLocation();
  }
  return myinstanceT;
}

CCCoreLocation::CCCoreLocation() {

  _LOCATION_KEY[0] = '\0';
  CCUserDefault *d = CCUserDefault::sharedUserDefault();
  std::string buff =
      d->getStringForKey(LOCATION_KEY, std::string(_LOCATION_KEY));
  sprintf(_LOCATION_KEY, "%s", buff.c_str());
  CCLOG("loaded _LOCATION_KEY=<%s>", _LOCATION_KEY);

#ifdef CC_TARGET_OS_IPHONE
  _localisationImpl = new CoreLocationImpl();
  _localisationImpl->init();
#endif
}

bool CCCoreLocation::isLocationAvailable() {
  if (_LOCATION_KEY[0] != '\0') {
    return true;
  }
  CCLOG("isLocationAvailable returns FALSE !!");
  return false;
}

#ifdef CC_PLATFORM_IOS
void CCCoreLocation::requestLocation(void) {
  this->_localisationImpl->requestLocation();
}
#endif

void CCCoreLocation::getLocation(CCObject *pTarget,
                                 cocos2d::SEL_CallFuncND pSelector) {
  CCLOG("CCCoreLocation::getLocation");
  _pSelector = pSelector;
  _pTarget = pTarget;

#ifdef CC_PLATFORM_IOS
  _localisationImpl->init();
#else
  InterfaceJNI::getLocation();
#endif
}

void CCCoreLocation::getLocationDone(float lati, float longi) {
  lib::network::DataStoreSingleton *p_data_store_singleton =
      lib::network::DataStoreSingleton::getInstance();
  p_data_store_singleton->setLocation(lati, longi);
}

#ifndef CC_PLATFORM_IOS

#ifdef __cplusplus
extern "C" {
#endif

void Java_org_mumblecore_location_CClocation_nativeupdatePosition(JNIEnv *env,
                                                                  jobject thiz,
                                                                  jdouble a,
                                                                  jdouble b) {
  CCLOG(
      "Java_org_mumblecore_location_CClocation_nativeupdatePosition LOC %f, %f",
      float(a), float(b));
  CCCoreLocation::getInstance()->getLocationDone(float(a), float(b));
}

#ifdef __cplusplus
}
#endif

#endif
