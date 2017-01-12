//
//  Util.hpp
//  pointcast
//
//  Created by Mitsuo Okada on 2015/11/10.
//
//

#ifndef Util_hpp
#define Util_hpp

#include <map>

#include "cocos2d.h"
#include "lib/object/LocationItem.hpp"
#include "network/HttpClient.h"
#include "scene/menu/Sensors.hpp"
#include <stdio.h>

namespace lib {

class Util {
  // member
public:
private:
  // function
public:
  static std::string createResponseBodyStringFromResponse(
      cocos2d::network::HttpResponse *response);

  static double round(double val, int precision);

  static std::string getDatetimeString(time_t time, const char *format);

  /* favorite */
  static bool getFavorite(int m_sensor_main_id);

  static void setFavorite(int m_sensor_main_id, bool flag);

  static void getCyanHaloLUT(uint8_t **r, uint8_t **g, uint8_t **b);

  static void applyLUT(const float src, const float lut_min,
                       const float lut_max, const uint8_t *lut_r,
                       const uint8_t *lut_g, const uint8_t *lut_b,
                       const size_t lut_n, uint8_t *r_out, uint8_t *g_out,
                       uint8_t *b_out);

  static void performDemo(const float usvh);

  static cocos2d::Color3B GetLutColor(const float usvh);

  static void filter(std::map<int, lib::object::LocationItem> &m_sensors,
                     std::string key_word);

  static std::vector<std::pair<int, int>>
  GetSortedLocationIndex(scene::menu::Sensors::Sort_Id sort_id, bool order_desc,
                         std::map<int, lib::object::LocationItem> m_sensors);

  static double getLocationDistance(float from_latitude, float from_longtitude,
                                    float to_latitude, float to_longtitude);
  
private:
};
}
#endif /* Util_hpp */
