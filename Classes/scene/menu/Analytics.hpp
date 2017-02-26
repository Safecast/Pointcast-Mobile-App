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
#include <map>
#include <vector>

#include "extensions/cocos-ext.h"

#include "lib/object/ChartItem.hpp"
#include "lib/object/WeatherItem.hpp"
#include "network/HttpClient.h"
#include "scene/base/AbstructScene.hpp"
#include "scene/Main.hpp"
#include "ui/CocosGUI.h"
#include "lib/external/CCLayerPanZoom.h"

namespace lib {
namespace gesture {
    class Pinch;
}
}

namespace scene {

namespace menu {

class Analytics : public base::AbstructScene {
  // member
public:
private:
    
  EventListenerTouchAllAtOnce* _pinch_listener;
    
  cocos2d::Node *_p_contents;
    
  cocos2d::ui::Layout *_p_panel_background;

  cocos2d::ui::ScrollView *_p_scroll_view;

  cocos2d::ui::PageView* _p_page_view;

  std::map<std::string, cocos2d::ui::Widget*> _p_chart_nodes;
    
  cocos2d::ui::Widget* _p_empty_page;
    
  std::string _current_cache_key;
  
  scene::Main* _p_scene_main;
    
  cocos2d::CallFunc* _p_store_callback;

  float _current_scale;
    
  time_t _interval_start;

  time_t _interval_end;

  int _m_sensor_main_id;

  int _device_id;

  bool _favorite;
    
  bool _portlate;

  cocos2d::ui::Button *_p_btn_favorite;

  lib::gesture::Pinch* _pinch_gesture;

  // function
public:
  void update(float delta);
    
  virtual bool init();

  void initFixedContents();
 
  void initVariableContents();

  void drawChart();
    
  void initChartInterval();
  
  void shiftInterval(int diff);

  virtual void onEnter();
    
  void onDidOrientation();
 
  void resetContents();

  void prepare(int m_sensor_main_id);

  virtual void close();

  void onCallbackPointcastAnalytics(cocos2d::network::HttpClient *sender,
                                    cocos2d::network::HttpResponse *response);
  void onCallbackDataStore();
  
  void pageViewEvent(cocos2d::Ref * psender, cocos2d::ui::PageView::EventType type);

/*
  cocos2d::ui::Widget *prepareChartBoard(
      const std::vector<lib::object::ChartItem> v_chart_items,
      const std::vector<lib::object::WeatherItem> v_weather_items);
*/
  std::vector<lib::object::ChartItem> getChartData(std::string analytics_data);

  std::vector<lib::object::WeatherItem>
  getWeatherData(std::string analytics_data);

  void setFavoriteButtonState(void);

  CREATE_FUNC(Analytics);

  int getDeviceId(void);

  int getMSensorMainId(void);
    
  bool isPortlate();
    
  bool isLandscape();
 
  bool isGesture(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent);
    
  time_t getIntervalStart();
    
  time_t getIntervalEnd();

  void updateChartScale();

  virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent);
  
  virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches,cocos2d::Event *pEvent);

  virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent);
  
  virtual void onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent);
    
  virtual void onExit();

};
}
}
#endif /* Analytics_hpp */
