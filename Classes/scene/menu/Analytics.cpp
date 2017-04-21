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
#define CHART_WIDTH 600.0f
#define CHART_HEIGHT 400.0f
#define CHART_OFFSET_X 20.0f
#define CHART_OFFSET_Y 20.0f
#define LABEL_OFFSET_X -20.0f
#define LABEL_OFFSET_Y -20.0f
#define CHART_LINE_BOLD 2.0f
#define VERTICAL_VALUE_BASE 50.0f
#define VERTICAL_OPTIONAL_VALUE 10.0f

USING_NS_CC;
using namespace ui;

using namespace rapidjson;

namespace scene {

namespace menu {

bool Analytics::init() {
    if (!base::AbstructScene::init()) {
        return false;
    }
    
    this->_p_scroll_view = nullptr;
    this->_current_scale = 1.0f;
    
    this->scheduleUpdate();
    
    this->_pinch_gesture = new lib::gesture::Pinch(2.0f, 1.0f);
    
    this->setTouchEnabled(true);
    
    // set orientation
    this->_portlate = scene::layout::helper::Display::IsPortlate();
    
    // add notification
    Director::getInstance()->getEventDispatcher()->addCustomEventListener("orientation",[=](cocos2d::EventCustom *event) {
        CCLOG("イベント受け取ったよ > %s",event->getEventName().c_str());
        this->onDidOrientation();
    });
    
    this->_p_store_callback = CallFunc::create(CC_CALLBACK_0(Analytics::onCallbackDataStore, this));
    
    /*
    this->_pinch_listener = EventListenerTouchAllAtOnce::create();
    this->_pinch_listener->setEnabled(true);
    this->_pinch_listener->onTouchesBegan = CC_CALLBACK_2(Analytics::onTouchesBegan, this);
    this->_pinch_listener->onTouchesMoved = CC_CALLBACK_2(Analytics::onTouchesMoved, this);
    this->_pinch_listener->onTouchesCancelled = CC_CALLBACK_2(Analytics::onTouchesCancelled, this);
    this->_pinch_listener->onTouchesEnded = CC_CALLBACK_2(Analytics::onTouchesEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(this->_pinch_listener, this);
    */
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

    this->_p_page_view = static_cast<ui::PageView *>(
        this->_p_panel_background->getChildByName("pageView"));
    
    this->_p_page_view->addEventListener(CC_CALLBACK_2(Analytics::pageViewEvent, this));
    
    
    // generate empty page
    this->_p_empty_page = cocos2d::ui::Widget::create();
    this->_p_page_view->insertPage(this->_p_empty_page, 0);
    
    this->setFavoriteButtonState();
    
    cocos2d::log("size w %f h %f x %f y %f", this->getContentSize().width, this->getContentSize().height, this->getPositionX(), this->getPositionY());
    
    this->addChild(this->_p_contents);

    
    /*
     
    this->_prev_button = RoundedBoxSprite::create();
    std::string prev_str = "Prev";
    auto prev_button_size = Size(200, 60);
    this->_prev_button->setParam(prev_button_size, Color3B(111,201,88), 10, 10, prev_str, Color3B::WHITE, 24);
    this->_prev_button->setPosition(100, 200);
    this->_prev_button->setContentSize(prev_button_size);
    this->_prev_button->setAnchorPoint(Vec2(0.0f, 0.5f));
    
    this->_next_button = RoundedBoxSprite::create();
    std::string next_str = "Next";
    auto next_button_size = Size(200, 60);
    this->_next_button->setParam(next_button_size, Color3B(111,201,88), 10, 10, next_str, Color3B::WHITE, 24);
    this->_next_button->setPosition(640, 200);
    this->_next_button->setContentSize(next_button_size);
    this->_next_button->setAnchorPoint(Vec2(0.5f, 0.5f));
    
    
    this->_p_contents->addChild(this->_prev_button);
    this->_p_contents->addChild(this->_next_button);
    
    
    //イベントリスナーを作成
    auto listener = EventListenerTouchOneByOne::create();
    
    //タッチ開始
    listener->onTouchBegan = [this](Touch* touch, Event* event){
        Point touchPoint = touch->getLocation();
        if (this->_prev_button->getBoundingBox().containsPoint(touchPoint))
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("res/sound/se/click.mp3");
            ssize_t idx = this->_p_page_view->getCurrentPageIndex() - 1;
            this->_p_page_view->setCurrentPageIndex(idx - 1);
            this->changePage(idx);

            return true;
        }
        
        if (this->_next_button->getBoundingBox().containsPoint(touchPoint))
        {
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("res/sound/se/click.mp3");
            ssize_t idx = this->_p_page_view->getCurrentPageIndex() + 1;
            this->_p_page_view->setCurrentPageIndex(idx);
            this->changePage(idx);
            
            return true;
        }

        return false;
    };
    
    //イベントリスナーを登録
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this->_prev_button);
    // this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this->_next_button);
    */
    
}
    
void Analytics::initVariableContents()
{

    // draw header
    auto p_header =  scene::layout::helper::Chart::prepareHeader(this, this->_m_sensor_main_id);
    this->addChild(p_header);
    
    // draw chart
    this->drawChart();
    
    // detach wait animation
    this->_p_scene_main->detachWaitAnimation();
    
    this->_p_page_view->setCurrentPageIndex(1);
}

void Analytics::drawChart()
{
    // prepare chart data
    lib::network::DataStoreSingleton *p_data_store_singleton =
                        lib::network::DataStoreSingleton::getInstance();
    
    const std::string analytics_data =
                        p_data_store_singleton->getResponseAnalyticsData(this->_m_sensor_main_id, this->_interval_start, this->_interval_end);
    
    this->_current_cache_key = p_data_store_singleton->getAnalyticsCacheKey(this->_m_sensor_main_id, this->_interval_start, this->_interval_end);
    
    std::vector<lib::object::ChartItem> v_chart_items;
    std::vector<lib::object::WeatherItem> v_weather_items;

    if (analytics_data.length() > 0)
    {
        v_chart_items = this->getChartData(analytics_data);
        v_weather_items = this->getWeatherData(analytics_data);
    }
    
    // draw chart
    scene::layout::helper::Chart::prepareChart(this->_p_empty_page, this, this->_m_sensor_main_id, v_chart_items, v_weather_items);
    this->_p_chart_nodes[this->_current_cache_key] = this->_p_empty_page;
    
    // generate empty page
    this->_p_empty_page = cocos2d::ui::Widget::create();
    scene::layout::helper::Chart::prepareChart(this->_p_empty_page, this, this->_m_sensor_main_id, v_chart_items, v_weather_items, true);
    this->_p_page_view->insertPage(this->_p_empty_page, 0);

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
  
  this->_p_store_callback->retain();
  this->attachWaitAnimation();
  p_data_store_singleton->storeAnalyticsData(this->_m_sensor_main_id, this->_interval_start, this->_interval_end, true, this->_p_store_callback);
    
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

void Analytics::onCallbackDataStore()
{
    this->initVariableContents();
    this->detachWaitAnimation();
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
              rapidjson::SizeType num = analytics.Size();
              for (rapidjson::SizeType i = 0; i < num; i++) {
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
              rapidjson::SizeType num = analytics.Size();
              for (rapidjson::SizeType i = 0; i < num; i++) {
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
    
    this->_interval_start = mktime(pnow);
    this->_interval_end = this->_interval_start + 86400;
}

void Analytics::shiftInterval(int diff)
{
    this->_interval_start += diff;
    this->_interval_end += diff;
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
    cocos2d::log("Analytics::onTouchesBegan touches %lu", touches.size());
    if(this->isGesture(touches, pEvent))
    {
        this->_pinch_gesture->setBasePoint(touches, pEvent, this->_current_scale);
    }
}

void Analytics::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches,cocos2d::Event *pEvent)
{
    cocos2d::log("Analytics::onTouchesMoved touches %lu", touches.size());
    if(this->isGesture(touches, pEvent))
    {
        this->_pinch_gesture->attachTouchesMove(touches, pEvent);
    }
}

void Analytics::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent)
{
    cocos2d::log("Analytics::onTouchesEnded touches %lu", touches.size());
    if(this->isGesture(touches, pEvent))
    {
        this->_pinch_gesture->init();
    }
    
    // prev button
    /*
    for(int i=0; i < touches.size(); i++){
        //targetBox : タッチされたスプライトの領域
        Rect target_box = this->_prev_button->getBoundingBox();
        
        //touchPoint : タッチされた場所
        Point touch_point = Vec2(touches.at(i)->getLocationInView().x, touches.at(i)->getLocationInView().y);
        
        //touchPointがtargetBoxの中に含まれているか判定
        if (target_box.containsPoint(touch_point))
        {
            log("tapped!!");
        }
    }
     */
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
  
    
    /*
    if (this->_p_chart_nodes[this->_current_cache_key] != nullptr)
    {
        if (this->_pinch_gesture->isChanged())
        {
            float scale = this->_pinch_gesture->getPinchScale();
            if (scale == 0.0f) return;
            cocos2d::log("set chart scale %f", scale);
            this->_current_scale = scale;
            this->_p_chart_nodes[this->_current_cache_key]->setScale(this->_current_scale);
            this->_p_scroll_view->setInnerContainerSize(this->_p_chart_nodes[this->_current_cache_key]->getBoundingBox().size);
            this->_p_chart_nodes[this->_current_cache_key]->setPositionY(0.0f);
        }
    }
     */
}
    
void Analytics::updateChartScale()
{
    this->_p_chart_nodes[this->_current_cache_key]->setScale(this->_current_scale);
    this->_p_scroll_view->setInnerContainerSize(this->_p_chart_nodes[this->_current_cache_key]->getBoundingBox().size);
    this->_p_chart_nodes[this->_current_cache_key]->setPositionY(0.0f);
}
    
void Analytics::onExit()
{
    
    AbstructScene::onExit();
    
    this->getEventDispatcher()->removeEventListener(this->_pinch_listener);

}
    
void Analytics::pageViewEvent(cocos2d::Ref * pSender, cocos2d::ui::PageView::EventType type)
{
    auto p_page_view = static_cast<cocos2d::ui::PageView*>(pSender);
    ssize_t current_page_index = p_page_view->getCurrentPageIndex();
    this->changePage(current_page_index);
}
    
void Analytics::changePage(ssize_t index)
{
    if (index == 0)
    {
        // 日付を1日戻す
        this->shiftInterval(-1 * 86400);
        
        // ページがないはずなので作る
        lib::network::DataStoreSingleton *p_data_store_singleton =
        lib::network::DataStoreSingleton::getInstance();
        this->_p_store_callback->retain();
        this->attachWaitAnimation();
        
        p_data_store_singleton->storeAnalyticsData(this->_m_sensor_main_id, this->_interval_start, this->_interval_end, true,this->_p_store_callback);
    }

}

}
}
