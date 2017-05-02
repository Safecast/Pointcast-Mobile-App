//
//  LocationItem.hpp
//  pointcast
//
//  Created by Mitsuo Okada on 2015/11/18.
//
//

#ifndef LocationItem_hpp
#define LocationItem_hpp

#include <stdio.h>
#include <string>

namespace lib {
namespace object {
struct LocationItem {
public:
  int m_sensor_main_id;
  std::string name_en;
  std::string name_jp;
  double latitude;
  double longitude;
  int sensor1_device_id;
  int sensor_status;
  double dre2cpm;
  double recent_value;
  std::string recent_captured_at;
  double yesterday_peak_value;
  double yesterday_average_value;
  double alarm_value;
  double lastweek_peak_value;
  double lastweekaverage_value;
  double lastmonth_peak_value;
  double lastmonth_value;
  std::string tube_name;
  int conversion_rate;
  // initialize
  LocationItem() {
    m_sensor_main_id = 0;
    name_en = "";
    name_jp = "";
    latitude = 0.0f;
    longitude = 0.0f;
    sensor1_device_id = 0;
    sensor_status = 0;
    dre2cpm = 0.0f;
    recent_value = 0.0f;
    recent_captured_at = "";
    yesterday_peak_value = 0.0f;
    yesterday_average_value = 0.0f;
    alarm_value = 0.0f;
    lastweek_peak_value = 0.0f;
    lastweekaverage_value = 0.0f;
    lastmonth_peak_value = 0.0f;
    lastmonth_value = 0.0f;
    tube_name = "";
    conversion_rate = 0;
  }
};
}
}

#endif /* LocationItem_hpp */
