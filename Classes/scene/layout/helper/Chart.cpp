//
//  Chart.cpp
//  pointcast
//
//  Created by Mitsuo Okada on 2017/01/09.
//
//
#include "Chart.hpp"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "lib/network/DataStoreSingleton.hpp"
#include "scene/layout/helper/Display.hpp"

#include "scene/chart/Board.hpp"

namespace scene {
namespace layout {
namespace helper {
    
USING_NS_CC;
    
    
cocos2d::ui::Widget *Chart::prepareHeader(scene::menu::Analytics* p_scene_analytics,
                                                    int m_sensor_main_id)
{
    // load chart layout
    auto p_chart_board =
    cocos2d::CSLoader::getInstance()->createNode("res/layerChartHeader.csb");
    auto p_background =
    p_chart_board->getChildByName<ui::Layout *>("panelBackground");
    
    lib::network::DataStoreSingleton *p_data_store_singleton =
    lib::network::DataStoreSingleton::getInstance();
    
    lib::object::LocationItem location_item =
    p_data_store_singleton->getLocationItem(m_sensor_main_id);
    
    auto p_text_city = p_background->getChildByName<ui::Text *>("textCity");
    assert(p_text_city);
    p_text_city->setString(location_item.name_en);
    assert(p_text_city);
    
    std::stringstream ss;
    ss << "device_id : " << location_item.sensor1_device_id << "("
    << location_item.tube_name << "/" << location_item.conversion_rate << ")";
    auto p_text_senser_id =
    p_background->getChildByName<ui::Text *>("txtSenserId");
    p_text_senser_id->setContentSize(Size(400, 150));
    p_text_senser_id->setPositionX(220);
    assert(p_text_senser_id);
    
    p_text_senser_id->setString(ss.str());
    
    auto p_text_chart_title =
    p_background->getChildByName<ui::Text *>("txtChartTitle");
    p_text_chart_title->setString("Realtime Chart");
    
    p_chart_board->setPositionX(0);
    p_chart_board->setPositionY(800);
    
    
    
    return static_cast<cocos2d::ui::Widget*>(p_chart_board);
}

void Chart::prepareChart(cocos2d::ui::Widget* p_chart_board_widget, scene::menu::Analytics* p_scene_analytics,
                                           int m_sensor_main_id,
                                           const std::vector<lib::object::ChartItem> v_chart_items,
                                           const std::vector<lib::object::WeatherItem> v_weather_items, bool is_empty)
{
    
    // get chart config
    scene::chart::Board::Config chart_config = Chart::getConfig(v_chart_items, v_weather_items, p_scene_analytics, m_sensor_main_id, is_empty);
    chart_config.is_empty = is_empty;
    
    scene::chart::Board *p_chart_nodes =
        scene::chart::Board::create(chart_config);
    
    p_chart_board_widget->setContentSize(p_chart_nodes->getContentSize());
    p_chart_board_widget->setTouchEnabled(true);
    p_chart_board_widget->setAnchorPoint(Vec2(0.0f, 0.0f));
    //
    p_chart_board_widget->addChild(p_chart_nodes);
    
    cocos2d::log("p_chart_board_widget size %f, %f pos %f, %f", p_chart_board_widget->getContentSize().width,  p_chart_board_widget->getContentSize().height, p_chart_board_widget->getPositionX(), p_chart_board_widget->getPositionY());

}
    
scene::chart::Board::Config Chart::getConfig(const std::vector<lib::object::ChartItem> &v_chart_items, const std::vector<lib::object::WeatherItem> &v_weather_items, scene::menu::Analytics* p_scene_analytics, int m_sensor_main_id, bool is_empty)
{

    lib::object::LocationItem location_item =
    lib::network::DataStoreSingleton::getInstance()->getLocationItem(m_sensor_main_id);
    
    scene::chart::Board::Config config;
    if (scene::layout::helper::Display::IsPortlate())
    {
        config.board_size = Size(640, 400);
        config.chart_offset = Point(40, 100);
    } else {
        config.board_size = Size(1280, 400);
        config.chart_offset = Point(100, 100);
    }
    config.v_chart_items = v_chart_items;
    config.v_weather_items = v_weather_items;
    config.start_point = p_scene_analytics->getIntervalStart();
    config.end_point = p_scene_analytics->getIntervalEnd();
    config.vertical_line = 8;
    config.vertical_unit = "usv";
    config.horizontal_line = 4;
    config.horizontal_unit = "time";
    config.conversion_rate = location_item.conversion_rate;
    float avg = (location_item.yesterday_average_value / location_item.dre2cpm) ;
    if (avg < 0.3f)
    {
        config.vertical_top_value = 0.5f;
    } else if(avg < 1.0f) {
        config.vertical_top_value = 1.0f;
    } else {
        config.vertical_top_value = 5.0f;
    }
    config.is_empty = is_empty;
    
    return config;
    
}

    void Chart::drawDottedLine(cocos2d::DrawNode* p_node, float line_width, float line_interval, Point start_point, Point end_point, float interval, cocos2d::Color4F color) {

        Vec2 current_vec2;
        while(true)
        {
            
            p_node->drawSegment(start_point, end_point, 1.0f, color);
            break;
        }
    }
}
}
}
