//
//  Util.cpp
//  pointcast
//
//  Created by Mitsuo Okada on 2015/11/10.
//
//

#include <algorithm>

#include "Util.hpp"
#include "lib/native/Util.h"
#include "lib/network/DataStoreSingleton.hpp"

#define rad2deg(a) ((a) / M_PI * 180.0) /* rad を deg に換算するマクロ関数 */
#define deg2rad(a) ((a) / 180.0 * M_PI) /* deg を rad に換算するマクロ関数 */

namespace lib {

std::string Util::createResponseBodyStringFromResponse(
    cocos2d::network::HttpResponse *response) {
  std::vector<char> *buffer = response->getResponseData();
  std::string bufStr = "";
  for (unsigned int i = 0; i < buffer->size(); ++i) {
    bufStr += (*buffer)[i];
  }
  return bufStr;
}

double Util::round(double val, int precision) {
  double dOut;

  dOut = val * pow(10.0, precision);
  dOut = (double)(int)(dOut);

  return dOut * pow(10.0, -precision);
}

std::string Util::getDatetimeString(time_t time, const char *format) {
  // prepare value
  std::tm *ptm = std::localtime(&time);
  char buffer[32];
  std::strftime(buffer, 32, format, ptm);
  std::string time_string = buffer;
  return time_string;
}

bool Util::getFavorite(int m_sensor_main_id) {
  std::stringstream ss;
  ss << "favorite_m_sensor_main_id_" << m_sensor_main_id;
  cocos2d::UserDefault *ud = cocos2d::UserDefault::getInstance();
  auto flag = ud->getBoolForKey(ss.str().c_str());
  return flag;
}

void Util::setFavorite(int m_sensor_main_id, bool flag) {
  std::stringstream ss;
  ss << "favorite_m_sensor_main_id_" << m_sensor_main_id;
  cocos2d::UserDefault *ud = cocos2d::UserDefault::getInstance();
  ud->setBoolForKey(ss.str().c_str(), flag);
  ud->flush();
  return;
}

//
//  lut.cpp
//  LutDemo
//
//  Created by Nicholas Dolezal on 1/20/16.

void Util::GetCyanHaloLUT(uint8_t **r, uint8_t **g, uint8_t **b) {
  uint8_t _rs[256] = {
      1,   8,   9,   10,  12,  14,  16,  16,  18,  18,  19,  20,  21,  22,  22,
      24,  24,  25,  25,  25,  26,  26,  26,  26,  25,  26,  27,  26,  25,  26,
      26,  24,  24,  25,  24,  21,  21,  21,  17,  16,  9,   7,   0,   7,   15,
      23,  28,  32,  34,  38,  40,  43,  45,  46,  50,  51,  54,  55,  56,  56,
      56,  58,  59,  59,  59,  59,  59,  59,  59,  59,  57,  56,  56,  56,  54,
      51,  48,  45,  43,  39,  37,  33,  29,  23,  10,  0,   29,  39,  60,  67,
      84,  90,  97,  105, 110, 120, 124, 133, 137, 143, 148, 153, 161, 163, 171,
      173, 178, 181, 185, 191, 194, 200, 202, 208, 210, 214, 217, 220, 225, 226,
      233, 235, 240, 242, 245, 249, 251, 254, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255};

  uint8_t _gs[256] = {
      1,   7,   7,   8,   10,  11,  12,  12,  13,  13,  13,  13,  14,  14,  14,
      15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  14,
      14,  14,  14,  13,  13,  12,  11,  11,  10,  9,   5,   4,   0,   9,   19,
      30,  36,  43,  46,  56,  59,  65,  70,  74,  82,  85,  94,  96,  103, 107,
      112, 118, 121, 130, 132, 140, 144, 150, 156, 160, 167, 170, 181, 184, 191,
      195, 200, 208, 213, 221, 224, 233, 237, 243, 250, 255, 252, 251, 242, 240,
      235, 231, 226, 221, 219, 212, 210, 204, 202, 197, 192, 187, 182, 180, 172,
      170, 163, 160, 156, 151, 148, 137, 134, 128, 124, 117, 112, 107, 100, 97,
      87,  83,  71,  64,  56,  44,  38,  5,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   11,  25,  36,  42,  53,  57,  68,  73,  81,
      85,  89,  96,  98,  106, 109, 119, 123, 128, 133, 136, 144, 146, 153, 156,
      162, 166, 170, 180, 183, 191, 193, 200, 204, 208, 214, 217, 224, 226, 234,
      239, 247, 251, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255};

  uint8_t _bs[256] = {
      1,   10,  12,  15,  19,  23,  29,  31,  38,  39,  43,  48,  54,  59,  61,
      70,  72,  79,  83,  89,  94,  99,  109, 112, 122, 125, 135, 140, 145, 153,
      158, 169, 173, 186, 191, 199, 206, 212, 223, 228, 239, 244, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
      255, 255, 255, 255, 255, 255, 255, 255, 254, 247, 242, 234, 227, 221, 211,
      207, 196, 192, 183, 179, 174, 166, 161, 154, 151, 140, 136, 128, 123, 119,
      113, 111, 102, 100, 94,  88,  81,  75,  72,  65,  63,  55,  52,  47,  42,
      38,  32,  29,  18,  13,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   8,   10,  20,  23,  31,  36,  41,  47,  51,  62,  65,
      74,  78,  84,  90,  94,  102, 105, 115, 118, 128, 133, 138, 145, 148, 157,
      159, 168, 172, 179, 186, 191, 198, 202, 211, 215, 222, 226, 232, 238, 242,
      253};
  /*
   uint8_t *_r = malloc(sizeof(uint8_t) * 256);
   uint8_t *_g = malloc(sizeof(uint8_t) * 256);
   uint8_t *_b = malloc(sizeof(uint8_t) * 256);
   */

  uint8_t *_r = new uint8_t[256];
  uint8_t *_g = new uint8_t[256];
  uint8_t *_b = new uint8_t[256];

  // Copy stack alloc from static constructor to heap to retain it.
  //
  // Might be easier to simply embed these as static globals.

  memcpy(_r, _rs, sizeof(uint8_t) * 256);
  memcpy(_g, _gs, sizeof(uint8_t) * 256);
  memcpy(_b, _bs, sizeof(uint8_t) * 256);

  *r = _r;
  *g = _g;
  *b = _b;
} // SimpleLutDemo_GetCyanHaloLUT

void Util::ApplyLUT(const float src, const float lut_min, const float lut_max,
                    const uint8_t *lut_r, const uint8_t *lut_g,
                    const uint8_t *lut_b, const size_t lut_n, uint8_t *r_out,
                    uint8_t *g_out, uint8_t *b_out) {
  float norm;
  size_t idx;
  const size_t log_iter = 4; // number of LOG10 iterations to perform.

  norm =
      (src - lut_min) /
      (lut_max - lut_min); // normalize the dose rate to the range of the LUT.
  norm = norm > 1.0 ? 1.0 : norm < 0.0 ? 0.0 : norm; // clamp, [0.0 ... 1.0]

  for (size_t i = 0; i < log_iter;
       i++) // rescale logarithmically, increasing contrast of low values
  {
    norm = norm * 9.0F + 1.0F; // [1.0 ... 10.0]
    norm = log10f(norm);       // [0.0 ...  1.0]
  }                            // for

  idx = (size_t)(norm * ((float)lut_n - 1.0F)); // index of color

  *r_out = lut_r[idx]; // return RGB values from LUT
  *g_out = lut_g[idx];
  *b_out = lut_b[idx];
} // SimpleLutDemo_ApplyLUT

void Util::PerformDemo(const float usvh) {
  uint8_t *lut_r = NULL;
  uint8_t *lut_g = NULL;
  uint8_t *lut_b = NULL;

  lib::Util::GetCyanHaloLUT(&lut_r, &lut_g, &lut_b);

  uint8_t red_out, green_out, blue_out;

  lib::Util::ApplyLUT(usvh, 0.03F, 65.535F, lut_r, lut_g, lut_b, 256, &red_out,
                      &green_out, &blue_out);

  printf("AppDelegate.m: SimpleLutDemo_PerformDemo: The value %1.2f uSvh is "
         "color r=%d, g=%d, b=%d.\n",
         usvh, (int)red_out, (int)green_out, (int)blue_out);

  free(lut_r);
  lut_r = NULL;
  free(lut_g);
  lut_g = NULL;
  free(lut_b);
  lut_b = NULL;
} // SimpleLutDemo_PerformDemo

cocos2d::Color3B Util::GetLutColor(const float usvh) {
  uint8_t *lut_r = NULL;
  uint8_t *lut_g = NULL;
  uint8_t *lut_b = NULL;

  lib::Util::GetCyanHaloLUT(&lut_r, &lut_g, &lut_b);

  uint8_t red_out, green_out, blue_out;

  lib::Util::ApplyLUT(usvh, 0.03F, 65.535F, lut_r, lut_g, lut_b, 256, &red_out,
                      &green_out, &blue_out);

  printf("AppDelegate.m: SimpleLutDemo_PerformDemo: The value %1.2f uSvh is "
         "color r=%d, g=%d, b=%d.\n",
         usvh, (int)red_out, (int)green_out, (int)blue_out);

  free(lut_r);
  lut_r = NULL;
  free(lut_g);
  lut_g = NULL;
  free(lut_b);
  lut_b = NULL;

  cocos2d::Color3B color =
      cocos2d::Color3B((int)red_out, (int)green_out, (int)blue_out);

  return color;
}

void Util::Filter(std::map<int, lib::object::LocationItem> &m_sensors,
                  std::string key_word) {
  std::vector<int> v_erase_index;

  for (auto item : m_sensors) {
    auto location_item = item.second;
    std::string name_en = location_item.name_en;
    transform(name_en.begin(), name_en.end(), name_en.begin(), ::tolower);
    transform(key_word.begin(), key_word.end(), key_word.begin(), ::tolower);
    if (name_en.find(key_word) == std::string::npos) {
      v_erase_index.push_back(item.first);
    }
  }

  for (int i = 0; i < v_erase_index.size(); i++) {
    m_sensors.erase(v_erase_index.at(i));
  }
}

std::vector<std::pair<int, int>> Util::GetSortedLocationIndex(
    scene::menu::Sensors::Sort_Id sort_id, bool order_desc,
    std::map<int, lib::object::LocationItem> m_sensors) {

  std::vector<std::pair<int, int>> v_sensors_key_value;

  // make key valu
  for (auto item : m_sensors) {
    std::pair<int, int> key_value;
    switch (sort_id) {
    case scene::menu::Sensors::Sort_Id_Device_Id: {
      key_value = std::make_pair(item.second.sensor1_device_id, item.first);
      break;
    }
    case scene::menu::Sensors::Sort_Id_μSv_Hour: {
      int value = static_cast<int>(item.second.recent_value /
                                   (float)item.second.conversion_rate * 1000);
      key_value = std::make_pair(value, item.first);
      break;
    }
    case scene::menu::Sensors::Sort_Id_Near_Point: {

      // now location
      lib::network::DataStoreSingleton::location loc =
          lib::network::DataStoreSingleton::getInstance()->getLocation();

      double distance = Util::GetLocationDistance(loc.latitude, loc.longitude,
                                                  item.second.latitude,
                                                  item.second.longitude);
      CCLOG("city %s distance %lf ", item.second.name_en.c_str(), distance);
      key_value = std::make_pair(static_cast<int>(distance * 1000), item.first);
      break;
    }
    case scene::menu::Sensors::Sort_Id_Active_Inactive: {
      key_value = std::make_pair(item.second.sensor_status, item.first);
      break;
    }

    default: {
      key_value = std::make_pair(item.second.sensor1_device_id, item.first);
      break;
    }
    }

    v_sensors_key_value.push_back(key_value);
  }

  // sort
  if (!order_desc) {
    // asc
    std::sort(v_sensors_key_value.begin(), v_sensors_key_value.end());
  } else {
    // desc
    std::sort(v_sensors_key_value.rbegin(), v_sensors_key_value.rend());
  }

  for (int i = 0; i < v_sensors_key_value.size(); i++) {
    printf("(%d,%d) ", v_sensors_key_value[i].first,
           v_sensors_key_value[i].second);
  }

  return v_sensors_key_value;
}

double Util::GetLocationDistance(float from_latitude, float from_longtitude,
                                 float to_latitude, float to_longtitude) {
  double earth_r = 6378.137;
  double laRe, loRe, NSD, EWD, distance;
  laRe = deg2rad(to_latitude - from_latitude);
  loRe = deg2rad(to_longtitude - from_longtitude);
  NSD = earth_r * laRe;
  EWD = cos(deg2rad(from_latitude)) * earth_r * loRe;
  distance = sqrt(pow(NSD, 2) + pow(EWD, 2));

  return distance;
}
    
}
