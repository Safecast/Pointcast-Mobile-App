//
//  Chart.cpp
//  pointcast
//
//  Created by Mitsuo Okada on 2017/01/09.
//
//

#ifndef Chart_hpp
#define Chart_hpp

#include "cocos2d.h"

#include "lib/object/ChartItem.hpp"
#include "lib/object/WeatherItem.hpp"

#include "scene/menu/Analytics.hpp"

namespace scene {
namespace layout {
namespace helper {

class Chart {
  // member
public:
private:
public:
    // function
    static cocos2d::ui::Widget *prepareHeader(scene::menu::Analytics* p_scene_analytics,
                                                        int m_sensor_main_id);
    
    static cocos2d::ui::Widget *prepareChart(scene::menu::Analytics* p_scene_analytics,
                                                  int m_sensor_main_id,
                                                  const std::vector<lib::object::ChartItem> v_chart_items,
                                                  const std::vector<lib::object::WeatherItem> v_weather_items);
private:
  };
        }
    }
    }
#endif /* Chart_hpp */
