//
//  Board.hpp
//  pointcast
//
//  Created by Mitsuo Okada on 2015/11/20.
//
//

#ifndef Board_hpp
#define Board_hpp

#include <stdio.h>
#include <vector>

#include "cocos2d.h"

#include "lib/object/ChartItem.hpp"
#include "lib/object/WeatherItem.hpp"

namespace scene {

namespace chart {

class Board : public cocos2d::DrawNode {
public:
  struct Config {
  public:
    std::vector<lib::object::ChartItem> v_chart_items;
    std::vector<lib::object::WeatherItem> v_weather_items;
    cocos2d::Size chart_size;
    cocos2d::Point chart_offset;
    time_t start_point;
    time_t end_point;
    int vertical_line;
    int horizontal_line;
    std::string vertical_unit;
    std::string horizontal_unit;
    double vertical_top_value;
    int conversion_rate;
    Config() {
      std::vector<lib::object::ChartItem> v_chart_items;
      chart_size = cocos2d::Size();
      chart_offset = cocos2d::Point();
      start_point = 0;
      end_point = 0;
      vertical_line = 0;
      horizontal_line = 0;
      vertical_unit = "";
      horizontal_unit = "";
      vertical_top_value = 0.0f;
      conversion_rate = 0;
    }
  };

private:
  Config _config;

  double getX(time_t hrizontal_value);

  double getY(double vertical_value);

public:
  static Board *create(Config config);

  void onEnter();

  void drawBackGround();

  void drawFrame();

  void drawLabel();

  void drawPoint();

  void drawWeather();

private:
};
}
}

#endif /* Board_hpp */
