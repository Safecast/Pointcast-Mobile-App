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
#include "scene/chart/Board.hpp"

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
    
    static void prepareChart(cocos2d::ui::Widget* p_chart_board_widget, scene::menu::Analytics* p_scene_analytics,
                                                  int m_sensor_main_id,
                                                  const std::vector<lib::object::ChartItem> v_chart_items,
                                                  const std::vector<lib::object::WeatherItem> v_weather_items);
    
    static scene::chart::Board::Config getConfig(const std::vector<lib::object::ChartItem> &v_chart_items, const std::vector<lib::object::WeatherItem> &v_weather_items, scene::menu::Analytics* p_scene_analytics, int m_sensor_main_id);
    
    static void drawDottedLine(cocos2d::DrawNode* p_node, float line_width, float line_interval, cocos2d::Point start_point, cocos2d::Point end_point, float interval, cocos2d::Color4F color);
private:
  };
        }
    }
    }
#endif /* Chart_hpp */
