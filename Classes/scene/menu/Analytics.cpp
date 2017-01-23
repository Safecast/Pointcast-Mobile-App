//
//  Analytics.cpp
//  pointcast
//
//  Created by Mitsuo Okada on 2015/11/13.
//
//
#include <ctime>
#include <string>

#include "Analytics.hpp"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"

#include "json/document.h"
#include "json/rapidjson.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

#include "lib/Util.hpp"
#include "lib/gesture/Pinch.hpp"
#include "lib/native/Util.h"

#include "lib/network/DataStoreSingleton.hpp"
#include "scene/Main.hpp"
#include "scene/chart/Board.hpp"
#include "scene/layout/helper/Contents.hpp"
#include "scene/layout/helper/Display.hpp"
#include "scene/layout/helper/Footer.hpp"
#include "scene/layout/helper/Chart.hpp"

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

bool Analytics::init() {
    if (!base::AbstructScene::init()) {
        return false;
    }
    
    this->_p_scroll_view = nullptr;
    this->_p_chart_nodes = nullptr;
    
    this->scheduleUpdate();
    
    this->_pinch_gesture = new lib::gesture::Pinch();
    
    this->setTouchEnabled(true);
    
    // set orientation
    this->_portlate = scene::layout::helper::Display::IsPortlate();
    
    // add notification
    Director::getInstance()->getEventDispatcher()->addCustomEventListener("orientation",[=](cocos2d::EventCustom *event) {
        CCLOG("イベント受け取ったよ > %s",event->getEventName().c_str());
        this->onDidOrientation();
    });
    
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->setEnabled(true);
    listener->onTouchesBegan = CC_CALLBACK_2(Analytics::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(Analytics::onTouchesMoved, this);
    listener->onTouchesCancelled = CC_CALLBACK_2(Analytics::onTouchesCancelled, this);
    listener->onTouchesEnded = CC_CALLBACK_2(Analytics::onTouchesEnded, this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}
    
    
void Analytics::prepare(int m_sensor_main_id) {
  // @note is here right?
  lib::network::DataStoreSingleton *p_data_store_singleton =
      lib::network::DataStoreSingleton::getInstance();

  // prepare contents
  this->_m_sensor_main_id = m_sensor_main_id;

  auto location_item =
      p_data_store_singleton->getLocationItem(m_sensor_main_id);

  this->_device_id = location_item.sensor1_device_id;

  this->_favorite = lib::Util::getFavorite(this->_m_sensor_main_id);
 
}

void Analytics::initFixedContents()
{
    
    this->_p_scene_main =
        static_cast<scene::Main *>(this->getParent()->getParent());
    
    if (this->isPortlate()) {
        this->_p_contents =
        cocos2d::CSLoader::getInstance()->createNode("res/MenuAnalytics_portlate.csb");
    } else {
        this->_p_contents =
        cocos2d::CSLoader::getInstance()->createNode("res/MenuAnalytics_landscape.csb");
        
        // disappear footer
        scene::layout::helper::Footer::Visible(false);
        
    }
    
    this->_p_panel_background =
        this->_p_contents->getChildByName<ui::Layout *>("panelBackground");
    auto label_back = this->_p_panel_background->getChildByName<ui::Text *>("txtBack");
    label_back->addTouchEventListener(
        [this](Ref *sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
            // click se
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("res/sound/se/click.mp3");
            this->close();
        }
    });
    
    // favorite
    this->_p_btn_favorite = this->_p_panel_background->getChildByName<ui::Button *>("btnFavorite");
    this->_p_btn_favorite->addTouchEventListener(
        [this](Ref *sender, ui::Widget::TouchEventType type) {
            CCLOG("p_button_favorite touchend %d", type);
            if (type == ui::Widget::TouchEventType::BEGAN) {
                this->attachTouchParticle(this->_p_btn_favorite->getPosition());
            } else if (type == ui::Widget::TouchEventType::MOVED) {
                
            } else if (type == ui::Widget::TouchEventType::ENDED) {
                // click se
                CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("res/sound/se/click.mp3");
                
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

    // scroll view
    this->_p_scroll_view = static_cast<ui::ScrollView *>(
                                  this->_p_panel_background->getChildByName("scrollView"));
    
    this->setFavoriteButtonState();
    
    cocos2d::log("size w %f h %f x %f y %f", this->getContentSize().width, this->getContentSize().height, this->getPositionX(), this->getPositionY());
    
    this->addChild(this->_p_contents);
}
    
void Analytics::initVariableContents()
{
    // prepare chart data
    lib::network::DataStoreSingleton *p_data_store_singleton =
                    lib::network::DataStoreSingleton::getInstance();
    const std::string analytics_data =
        p_data_store_singleton->getResponseAnalyticsData(this->_m_sensor_main_id);
    
    
    std::vector<lib::object::ChartItem> v_chart_items =
                        this->getChartData(analytics_data);
    
    std::vector<lib::object::WeatherItem> v_weather_items =
                        this->getWeatherData(analytics_data);
    
    
    // draw header
    auto p_header =  scene::layout::helper::Chart::prepareHeader(this, this->_m_sensor_main_id);
    this->addChild(p_header);
    
    // draw chart
    this->_p_chart_nodes = scene::layout::helper::Chart::prepareChart(this, this->_m_sensor_main_id, v_chart_items, v_weather_items);
    // p_chart = scene::layout::helper::Chart::prepareChart(this, this->_m_sensor_main_id, v_chart_items, v_weather_items);
    this->_p_scroll_view->setInnerContainerSize(this->_p_chart_nodes->getContentSize());
    this->_p_scroll_view->addChild(this->_p_chart_nodes);
    
    // detach wait animation
    this->_p_scene_main->detachWaitAnimation();

}

void Analytics::onEnter() {
  AbstructScene::onEnter();
    
  // initialize chart interval
  this->initChartInterval();

  // initialize fixed contents
  this->initFixedContents();

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
                &Analytics::onCallbackPointcastAnalytics));
    
  p_data_store_singleton->requestPointcastAnalytics(this->_m_sensor_main_id, this->_interval_start, this->_interval_end);
    
  // enable rotate
  lib::native::Util::setRotateEnable(true);
  
}

void Analytics::close() {
    
    
  scene::menu::Sensors *p_sensors =
      static_cast<scene::menu::Sensors *>(this->getParent());

  p_sensors->closeAnalyticsDialog();

  // disable event receive
  Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("orientation");
  
    if (!this->isPortlate())
    {
        // set display portlate
        lib::native::Util::changeRotate(lib::native::Util::UIDeviceOrientationPortrait);
        
        // footer appear
        scene::layout::helper::Footer::Visible(true);
    }
    
  // disable rotate
  lib::native::Util::setRotateEnable(false);
}

void Analytics::onCallbackPointcastAnalytics(
    cocos2d::network::HttpClient *sender,
    cocos2d::network::HttpResponse *response) {
  CCLOG("onCallbackPointcastAnalytics");

    
  this->initVariableContents();
    
  
}

std::vector<lib::object::ChartItem>
Analytics::getChartData(std::string analytics_data) {
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
Analytics::getWeatherData(std::string analytics_data) {
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
    
/*
ui::Widget *Analytics::prepareChartBoard(
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

  this->_p_chart_nodes =
      scene::chart::Board::create(chart_prepare_data);

  p_chart_board->addChild(this->_p_chart_nodes);

  ui::Widget *p_chart_board_widget = ui::Widget::create();
  Size chart_size = p_chart_board->getContentSize();
  chart_size.height = chart_size.height + 100;
  p_chart_board_widget->setContentSize(chart_size);
  p_chart_board_widget->setTouchEnabled(true);
  p_chart_board_widget->addChild(p_chart_board);

  return p_chart_board_widget;
}
*/
    
void Analytics::setFavoriteButtonState(void) {
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
  CCLOG("Analytics::setAnalyticsButtonState");
}

int Analytics::getDeviceId(void) { return this->_device_id; }

int Analytics::getMSensorMainId(void) {
  return this->_m_sensor_main_id;
}
    
void Analytics::onDidOrientation()
{
    cocos2d::CCLog("onDidOrientation");
    this->resetContents();
}

    
void Analytics::resetContents()
{
    // set orientation
    this->_portlate = scene::layout::helper::Display::IsPortlate();
    
    // remove contents
    this->_p_contents->removeFromParent();
    
    // init contents
    this->initFixedContents();
    
    // notify onEnter
    this->onEnter();
}
    
bool Analytics::isPortlate()
{
    return this->_portlate;
}

bool Analytics::isLandscape() { return !this->_portlate; }

void Analytics::initChartInterval()
{
    time_t now = time(NULL);
    struct tm *pnow = localtime(&now);
    pnow->tm_hour = 0;
    pnow->tm_min = 0;
    pnow->tm_sec = 0;
    
    this->_interval_end = mktime(pnow);
    // interval 1 day
    this->_interval_start = this->_interval_end - 86400;
    
}
    
time_t Analytics::getIntervalStart()
{
    return this->_interval_start;
}
    
time_t Analytics::getIntervalEnd()
{
    return this->_interval_end;
}

void Analytics::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent)
{
    cocos2d::log("onTouchesBegan");
    if(this->isGesture(touches, pEvent))
    {
        this->_pinch_gesture->init(touches, pEvent);
    }
}

void Analytics::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches,cocos2d::Event *pEvent)
{
    cocos2d::log("onTouchesMoved");
    if(this->isGesture(touches, pEvent))
    {
        this->_pinch_gesture->attachTouchesMove(touches, pEvent);
    }
}

void Analytics::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent)
{
    cocos2d::log("onTouchesEnded");
    if(this->isGesture(touches, pEvent))
    {
        this->_pinch_gesture->finish();
    }
}

void Analytics::onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent)
{
    
}
    
bool Analytics::isGesture(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent)
{
    //
    
    
    if (touches.size() != 2)
    {
        return false;
    }
    
    return true;
}
    
void Analytics::update(float delta){
  
    
    
    if (this->_p_chart_nodes != nullptr)
    {
        if (this->_pinch_gesture->isChanged())
        {
            float scale = this->_pinch_gesture->getPinchScale();
            if (scale == 0.0f) return;
            cocos2d::log("set chart scale %f", scale);
            this->_p_chart_nodes->setScale(scale);
        }
    }
}

}
}
