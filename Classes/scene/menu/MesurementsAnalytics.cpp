//
//  MesurementsAnalytics.cpp
//  pointcast
//
//  Created by Leverages Mitsuo Okada on 2015/11/13.
//
//
#include <ctime>
#include <string>

#include "MesurementsAnalytics.hpp"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "json/document.h"
#include "json/rapidjson.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

#include "lib/Util.hpp"
#include "lib/network/DataStoreSingleton.hpp"
#include "scene/Main.hpp"
#include "scene/chart/Board.hpp"
#include "scene/menu/Sensors.hpp"

// #define CHART_WIDTH 560.0f
#define CHART_WIDTH 1200.0f
#define CHART_HEIGHT 400.0f
#define CHART_OFFSET_X 100.0f
#define CHART_OFFSET_Y 20.0f
#define LABEL_OFFSET_X -20.0f
#define LABEL_OFFSET_Y -20.0f
#define CHART_LINE_BOLD 2.0f
#define VERTICAL_VALUE_BASE 50.0f
#define VERTICAL_OPTIONAL_VALUE 10.0f

USING_NS_CC;
using namespace rapidjson;

namespace scene {

namespace menu {

void MesurementsAnalytics::prepare(int m_sensor_main_id) {
  // @note is here right?
  lib::network::DataStoreSingleton *p_data_store_singleton =
      lib::network::DataStoreSingleton::getInstance();

  // prepare contents
  this->_m_sensor_main_id = m_sensor_main_id;

  auto location_item =
      p_data_store_singleton->getLocationItem(m_sensor_main_id);

  this->_device_id = location_item.sensor1_device_id;

  this->_favorite = lib::Util::getFavorite(this->_m_sensor_main_id);

  this->setFavoriteButtonState();
}

bool MesurementsAnalytics::init() {
  if (!base::AbstructScene::init()) {
    return false;
  }

  this->_p_contents =
      cocos2d::CSLoader::getInstance()->createNode("res/MenuAnalytics.csb");

  auto panel =
      this->_p_contents->getChildByName<ui::Layout *>("panelBackground");
  auto label_back = panel->getChildByName<ui::Text *>("txtBack");
  label_back->addTouchEventListener(
      [this](Ref *sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
          this->close();
        }
      });

  // favorite
  this->_p_btn_favorite = panel->getChildByName<ui::Button *>("btnFavorite");
  this->_p_btn_favorite->addTouchEventListener(
      [this](Ref *sender, ui::Widget::TouchEventType type) {
        CCLOG("p_button_favorite touchend %d", type);
        if (type == ui::Widget::TouchEventType::BEGAN) {
          this->attachTouchParticle(this->_p_btn_favorite->getPosition());
        } else if (type == ui::Widget::TouchEventType::MOVED) {

        } else if (type == ui::Widget::TouchEventType::ENDED) {
          // reverse flag
          this->_favorite = !this->_favorite;
          this->detachTouchParticle();
          lib::Util::setFavorite(this->_m_sensor_main_id, this->_favorite);
          this->setFavoriteButtonState();
          CCLOG("p_button_favorite touchend");
        } else {
          this->detachTouchParticle();
        }
      });

  this->addChild(this->_p_contents);

  return true;
}

void MesurementsAnalytics::onEnter() {
  AbstructScene::onEnter();

  // get Analytics Data
  // Http Request For Analytics Data
  lib::network::DataStoreSingleton *p_data_store_singleton =
      lib::network::DataStoreSingleton::getInstance();

  scene::Main *p_scene_main =
      static_cast<scene::Main *>(this->getParent()->getParent());
  p_scene_main->attachWaitAnimation();

  // http request pointcast/home.json
  p_data_store_singleton->setResponseCallback(
      this, (cocos2d::network::SEL_HttpResponse)(
                &MesurementsAnalytics::onCallbackPointcastAnalytics));
  p_data_store_singleton->requestPointcastAnalytics(this->_m_sensor_main_id);
}

void MesurementsAnalytics::close() {
  scene::menu::Sensors *p_sensors =
      static_cast<scene::menu::Sensors *>(this->getParent());

  p_sensors->closeAnalyticsDialog();
}

void MesurementsAnalytics::onCallbackPointcastAnalytics(
    cocos2d::network::HttpClient *sender,
    cocos2d::network::HttpResponse *response) {
  CCLOG("onCallbackPointcastAnalytics");

  scene::Main *p_scene_main =
      static_cast<scene::Main *>(this->getParent()->getParent());
  p_scene_main->detachWaitAnimation();

  // @note is here right?
  lib::network::DataStoreSingleton *p_data_store_singleton =
      lib::network::DataStoreSingleton::getInstance();

  const std::string analytics_data =
      p_data_store_singleton->getResponseAnalyticsData(this->_m_sensor_main_id);

  CCLOG("analytics_data %s", analytics_data.c_str());

  std::vector<lib::object::ChartItem> v_chart_items =
      this->getChartData(analytics_data);

  std::vector<lib::object::WeatherItem> v_weather_items =
      this->getWeatherData(analytics_data);

  auto p_chart_board_widget =
      this->prepareChartBoard(v_chart_items, v_weather_items);

  // base scene
  auto p_panel_background = static_cast<ui::Layout *>(
      this->_p_contents->getChildByName("panelBackground"));
  // scroll view
  auto p_scroll_view = static_cast<ui::ScrollView *>(
      p_panel_background->getChildByName("scrollView"));
  Size size = p_chart_board_widget->getContentSize();
  p_scroll_view->setInnerContainerSize(p_chart_board_widget->getContentSize());
  p_chart_board_widget->setAnchorPoint(Point(0.0f, 0.0f));
  p_scroll_view->addChild(p_chart_board_widget);
}

std::vector<lib::object::ChartItem>
MesurementsAnalytics::getChartData(std::string analytics_data) {
  // parse json
  rapidjson::Document document;
  document.Parse<0>(analytics_data.c_str());
  if (document.HasParseError()) {
    // error
    assert(false);
  }

  std::vector<lib::object::ChartItem> v_chart_items;

  if (document.IsObject()) {
    // JSON配列取得
    if (document.HasMember("chart") && document["chart"].IsObject()) {
      const rapidjson::Value &chart = document["chart"];
      if (chart.HasMember("realtime") && chart["realtime"].IsObject()) {
        const rapidjson::Value &realtime = chart["realtime"];
        if (realtime.HasMember("sensors") && realtime["sensors"].IsObject()) {
          const rapidjson::Value &sensors = realtime["sensors"];
          for (rapidjson::Value::ConstMemberIterator itr =
                   sensors.MemberBegin();
               itr != sensors.MemberEnd(); itr++) {
            const char *device_id = itr->name.GetString();
            CCLOG("device_id %s", device_id);
            if (sensors.HasMember(device_id) && sensors[device_id].IsArray()) {
              const rapidjson::Value &analytics = sensors[device_id];
              SizeType num = analytics.Size();
              for (SizeType i = 0; i < num; i++) {
                lib::object::ChartItem item;
                item.value = analytics[i]["value"].GetInt();
                item.major_label = analytics[i]["major_label"].GetInt();
                item.middle_label = analytics[i]["middle_label"].GetInt();
                item.minor_label = analytics[i]["minor_label"].GetInt();
                item.timestamp = analytics[i]["timestamp"].GetInt();

                v_chart_items.push_back(item);
              }
            }
          }
        }
      }
    }
  }

  return v_chart_items;
}

std::vector<lib::object::WeatherItem>
MesurementsAnalytics::getWeatherData(std::string analytics_data) {
  // parse json
  rapidjson::Document document;
  document.Parse<0>(analytics_data.c_str());
  if (document.HasParseError()) {
    // error
    assert(false);
  }

  std::vector<lib::object::WeatherItem> v_weather_items;

  if (document.IsObject()) {
    // JSON配列取得
    if (document.HasMember("chart") && document["chart"].IsObject()) {
      const rapidjson::Value &chart = document["chart"];
      if (chart.HasMember("realtime") && chart["realtime"].IsObject()) {
        const rapidjson::Value &realtime = chart["realtime"];
        if (realtime.HasMember("weather") && realtime["weather"].IsObject()) {
          const rapidjson::Value &weather = realtime["weather"];
          for (rapidjson::Value::ConstMemberIterator itr =
                   weather.MemberBegin();
               itr != weather.MemberEnd(); itr++) {
            const char *device_id = itr->name.GetString();
            CCLOG("device_id %s", device_id);
            if (weather.HasMember(device_id) && weather[device_id].IsArray()) {
              const rapidjson::Value &analytics = weather[device_id];
              SizeType num = analytics.Size();
              for (SizeType i = 0; i < num; i++) {
                lib::object::WeatherItem item;
                item.weather_main = analytics[i]["weather_main"].GetString();
                item.icon = analytics[i]["icon"].GetString();
                item.timestamp = analytics[i]["timestamp"].GetInt();

                v_weather_items.push_back(item);
              }
            }
          }
        }
      }
    }
  }

  return v_weather_items;
}

ui::Widget *MesurementsAnalytics::prepareChartBoard(
    const std::vector<lib::object::ChartItem> v_chart_items,
    const std::vector<lib::object::WeatherItem> v_weather_items) {
  // load chart layout
  auto p_chart_board =
      CSLoader::getInstance()->createNode("res/layerChart.csb");
  auto p_background =
      p_chart_board->getChildByName<ui::Layout *>("panelBackground");

  lib::network::DataStoreSingleton *p_data_store_singleton =
      lib::network::DataStoreSingleton::getInstance();

  lib::object::LocationItem location_item =
      p_data_store_singleton->getLocationItem(this->_m_sensor_main_id);

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
  chart_prepare_data.chart_size = Size(CHART_WIDTH, CHART_HEIGHT);
  chart_prepare_data.chart_offset = Point(CHART_OFFSET_X, CHART_OFFSET_Y + 100);
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

void MesurementsAnalytics::setFavoriteButtonState(void) {
  if (this->_favorite) {
    this->_p_btn_favorite->loadTextures("res/icon/menu/star_orange_wide.png",
                                        "res/icon/menu/star_orange_wide.png",
                                        "res/icon/menu/star_orange_wide.png");
  } else {
    // disabled
    this->_p_btn_favorite->loadTextures("res/icon/menu/star_gray_wide.png",
                                        "res/icon/menu/star_gray_wide.png",
                                        "res/icon/menu/star_gray_wide.png");
  }
  CCLOG("MesurementsAnalytics::setAnalyticsButtonState");
}

int MesurementsAnalytics::getDeviceId(void) { return this->_device_id; }

int MesurementsAnalytics::getMSensorMainId(void) {
  return this->_m_sensor_main_id;
}
}
}