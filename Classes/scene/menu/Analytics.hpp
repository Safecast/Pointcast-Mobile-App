//
//  Analytics.hpp
//  pointcast
//
//  Created by Mitsuo Okada on 2015/11/13.
//
//

#ifndef Analytics_hpp
#define Analytics_hpp
#include <stdio.h>
#include <vector>

#include "lib/object/ChartItem.hpp"
#include "lib/object/WeatherItem.hpp"
#include "network/HttpClient.h"
#include "scene/base/AbstructScene.hpp"

namespace scene {

namespace menu {

class Analytics : public base::AbstructScene {
  // member
public:
private:
  cocos2d::Node *_p_contents;

  int _m_sensor_main_id;

  int _device_id;

  bool _favorite;
    
  bool _portlate;

  cocos2d::ui::Button *_p_btn_favorite;

  // function
public:
  virtual bool init();
 
  void initContents();

  virtual void onEnter();
    
  void onDidOrientation();
 
  void resetContents();

  void prepare(int m_sensor_main_id);

  virtual void close();

  void onCallbackPointcastAnalytics(cocos2d::network::HttpClient *sender,
                                    cocos2d::network::HttpResponse *response);

  cocos2d::ui::Widget *prepareChartBoard(
      const std::vector<lib::object::ChartItem> v_chart_items,
      const std::vector<lib::object::WeatherItem> v_weather_items);

  std::vector<lib::object::ChartItem> getChartData(std::string analytics_data);

  std::vector<lib::object::WeatherItem>
  getWeatherData(std::string analytics_data);

  void setFavoriteButtonState(void);

  CREATE_FUNC(Analytics);

  int getDeviceId(void);

  int getMSensorMainId(void);
    
  bool isPortlate();
    
  bool isLandscape();
    
};
}
}
#endif /* Analytics_hpp */
