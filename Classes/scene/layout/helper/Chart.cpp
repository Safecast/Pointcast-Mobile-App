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
    

}

cocos2d::ui::Widget* Chart::prepareChartBoard(scene::menu::Analytics* p_scene_analytics,
                                           int m_sensor_main_id,
                                           const std::vector<lib::object::ChartItem> v_chart_items,
                                           const std::vector<lib::object::WeatherItem> v_weather_items)
{
    // load chart layout
    auto p_chart_board =
    cocos2d::CSLoader::getInstance()->createNode("res/layerChart.csb");
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
    
    // if nothing data
    if (v_chart_items.size() == 0) {
        ui::Widget *p_chart_board_widget = ui::Widget::create();
        p_chart_board_widget->setContentSize(p_chart_board->getContentSize());
        p_chart_board_widget->addChild(p_chart_board);
        return p_chart_board_widget;
    }
    
    scene::chart::Board::PrepareData chart_prepare_data;
    
    // next 6hour (end point)
    time_t now = time(NULL);
    struct tm *pnow = localtime(&now);
    int mod = pnow->tm_hour % 6;
    pnow->tm_hour = pnow->tm_hour + (6 - mod);
    pnow->tm_min = 0;
    pnow->tm_sec = 0;
    
    time_t end_time_stamp = mktime(pnow);
    
    pnow->tm_hour = pnow->tm_hour - 48;
    pnow->tm_min = 0;
    pnow->tm_sec = 0;
    
    time_t start_time_stamp = mktime(pnow);
    
    CCLOG("start %ld end %ld", start_time_stamp, end_time_stamp);
    
    // prepare chart_
    chart_prepare_data.chart_size = Size(640, 600);
    chart_prepare_data.chart_offset = Point(100, 100 + 100);
    chart_prepare_data.v_chart_items = v_chart_items;
    chart_prepare_data.v_weather_items = v_weather_items;
    chart_prepare_data.start_point = start_time_stamp;
    chart_prepare_data.end_point = end_time_stamp;
    chart_prepare_data.vertical_line = 8;
    chart_prepare_data.vertical_unit = "usv";
    chart_prepare_data.horizontal_line = 4;
    chart_prepare_data.horizontal_unit = "time";
    chart_prepare_data.conversion_rate = location_item.conversion_rate;
    
    //  chart_prepare_data.vertical_top_value =
    //      (int)(((int)((location_item.yesterday_average_value +
    //                    VERTICAL_OPTIONAL_VALUE) /
    //                   VERTICAL_VALUE_BASE) *
    //             VERTICAL_VALUE_BASE) +
    //            VERTICAL_VALUE_BASE);
    
    chart_prepare_data.vertical_top_value = 1.0f;
    
    // chart_boardのサイズを変更
    p_chart_board->setContentSize(chart_prepare_data.chart_size);
    
    scene::chart::Board *p_chart_nodes =
    scene::chart::Board::create(chart_prepare_data);
    
    p_chart_board->addChild(p_chart_nodes);
    
    ui::Widget *p_chart_board_widget = ui::Widget::create();
    Size chart_size = p_chart_board->getContentSize();
    chart_size.height = chart_size.height + 100;
    p_chart_board_widget->setContentSize(chart_size);
    p_chart_board_widget->setTouchEnabled(true);
    p_chart_board_widget->addChild(p_chart_board);
    
    return p_chart_board_widget;

}
    
}
}
}
