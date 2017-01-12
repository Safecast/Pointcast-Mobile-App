//
//  DataStoreSingleton.hpp
//  pointcast
//
//  Created by Mitsuo Okada on 2015/11/04.
//
//

#ifndef DataStoreSingleton_hpp
#define DataStoreSingleton_hpp

#include <map>
#include <stdio.h>

#include "cocos2d.h"
#include "network/HttpClient.h"

#include "json/document.h"
#include "json/rapidjson.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

#include "lib/object/LocationItem.hpp"

namespace lib {

namespace network {

class DataStoreSingleton {
  // members
public:
  // @kind of request
  enum E_Http_Request_Id {
    // pointcast/home
    Request_Pointcast_Home_e = 10001,

    // pointcast/analytics
    Request_Pointcast_Analytics_e,

  };
  struct location {
    float longitude;
    float latitude;
  };

private:
  /** last updated at */
  time_t last_updated_at;

  static const std::string end_point;

  /** callback object */
  cocos2d::Node *_p_callbackObject;

  /** callback function */
  cocos2d::network::SEL_HttpResponse _p_callbackFunction;

  // singleton instance
  static DataStoreSingleton *_p_instance;

  // data store
  std::map<E_Http_Request_Id, std::string> _p_m_http_response_data;

  // sensor list data store
  std::map<int, lib::object::LocationItem> _p_m_http_sensor_main_response_data;

  // analytics data store
  std::map<int, std::string> _p_m_http_analytics_response_data;

  // store request m_sensor_main_id
  int _request_m_sensor_main_id;

  // location struct
  location _loc;

  // functions
public:
  // constructer
  DataStoreSingleton();

  // destructer
  ~DataStoreSingleton();

  static DataStoreSingleton *getInstance();

  void requestPointcastHome(void);

  void requestPointcastAnalytics(int m_sensor_main_id);

  void callbackHttpPointcastHome(cocos2d::network::HttpClient *sender,
                                 cocos2d::network::HttpResponse *response);

  void callbackHttpPointcastAnalytics(cocos2d::network::HttpClient *sender,
                                      cocos2d::network::HttpResponse *response);

  void setResponseCallback(cocos2d::Node *callbackObject,
                           cocos2d::network::SEL_HttpResponse callbackFunction);

  void resetResponseCallback();

  std::string getResponseData(E_Http_Request_Id http_request_id);

  std::string getResponseAnalyticsData(int m_sensor_main_id);

  lib::object::LocationItem getLocationItem(int m_sensor_main_id);

  std::map<int, lib::object::LocationItem> getLocationItemAll(void) const;

  std::vector<int> getLocationItemIdList(void) const;

  time_t getLastUpdatedAt();

  std::string getLastUpdatedAtToFormatString();

  void setLocation(float latitude, float longtitude);

  location getLocation(void);

private:
  // store sensor list data
  void storeSensorListData(std::string response);
};
}
}
#endif /* DataStoreSingleton_hpp */
