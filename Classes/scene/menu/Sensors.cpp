//
//  Sensors.cpp
//  pointcast
//
//  Created by Leverages Mitsuo Okada on 2015/10/29.
//
//

#include <string>

#include "Sensors.hpp"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "lib/Util.hpp"
#include "lib/network/DataStoreSingleton.hpp"
#include "scene/layout/helper/Contents.hpp"
#include "scene/modal/Search.hpp"
#include "scene/modal/Sort.hpp"

#include "network/HttpClient.h"

#include "scene/Main.hpp"
#include "scene/layout/helper/Contents.hpp"
#include "scene/menu/MesurementsAnalytics.hpp"

USING_NS_CC;
using namespace rapidjson;

namespace scene {

namespace menu {

bool Sensors::init() {
  if (!Node::init()) {
    return false;
  }

  // set task id
  this->task_id = Task_Id_World;

  // set sort type
  this->sort_id = Sort_Id_Near_Point;

  // load sensors layout
  this->_p_contents = CSLoader::getInstance()->createNode(
      "res/MenuSensorList.csb"); // .csbを読み込んだディレクトリを保持してリソースを探す指定

  // back button
  auto p_panel =
      this->_p_contents->getChildByName<ui::Layout *>("panelBackground");

  auto p_list_view = p_panel->getChildByName<ui::ListView *>("listSensors");
  p_list_view->setInnerContainerSize(p_list_view->getContentSize());
  p_list_view->setBounceEnabled(true);
  p_list_view->setTouchEnabled(true);
  p_list_view->setScrollBarEnabled(false);
  p_list_view->addEventListener(
      [this](Ref *ref, ui::ListView::EventType eventType) {
        if (eventType == ui::ListView::EventType::ON_SELECTED_ITEM_END) {
          auto listView = static_cast<ui::ListView *>(ref);
          auto selectedIndex = listView->getCurSelectedIndex();

          // revert color all record
          for (auto widget : listView->getItems()) {
            auto p_record = widget->getChildByTag(Tag_Id_Sensor_Record);
            p_record->getChildByName<ui::Layout *>("panelRecord")
                ->setColor(Color3B::WHITE);
          }
          // change color selected record
          lib::network::DataStoreSingleton *p_datastore_singleton =
              lib::network::DataStoreSingleton::getInstance();
          auto widget = listView->getItem(selectedIndex);
          auto location_item =
              p_datastore_singleton->getLocationItem(widget->getTag());

          if (location_item.sensor_status != 1) {
            // if status inactive
            return;
          }

          auto p_record = widget->getChildByTag(Tag_Id_Sensor_Record);
          p_record->getChildByName<ui::Layout *>("panelRecord")
              ->setColor(Color3B(250, 219, 218));
          CCLOG("selected index %ld", selectedIndex);

          this->showAnalyticsDialog(widget->getTag());
        } else {
          CCLOG("touch list event type %d", eventType);
        }
      });

  //  auto label_back = p_panel->getChildByName<ui::Text *>("txtBack");
  //  label_back->addTouchEventListener(
  //      [this](Ref *sender, ui::Widget::TouchEventType type) {
  //        if (type == ui::Widget::TouchEventType::ENDED) {
  //          this->touchBack();
  //        }
  //      });

  // search
  auto button_search = p_panel->getChildByName<ui::ImageView *>("imgSearch");
  button_search->addTouchEventListener(
      [this](Ref *sender, ui::Widget::TouchEventType type) {
        auto p_sender = static_cast<cocos2d::Node *>(sender);
        if (type == ui::Widget::TouchEventType::BEGAN) {
          this->attachTouchParticle(p_sender->getPosition());
        } else if (type == ui::Widget::TouchEventType::MOVED) {

        } else if (type == ui::Widget::TouchEventType::ENDED) {
          this->touchSearch(sender);
        } else {
          this->detachTouchParticle();
        }
      });

  // sort
  auto button_sort = p_panel->getChildByName<ui::ImageView *>("imgSort");
  button_sort->addTouchEventListener(
      [this](Ref *sender, ui::Widget::TouchEventType type) {
        auto p_sender = static_cast<cocos2d::Node *>(sender);
        if (type == ui::Widget::TouchEventType::BEGAN) {
          this->attachTouchParticle(p_sender->getPosition());
        } else if (type == ui::Widget::TouchEventType::MOVED) {

        } else if (type == ui::Widget::TouchEventType::ENDED) {
          this->touchSort(sender);
        } else {
          this->detachTouchParticle();
        }
      });

  // panel world
  auto panel_world = p_panel->getChildByName<ui::Button *>("panelWorld");
  panel_world->addTouchEventListener(
      [this](Ref *sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
          this->touchPanelWorld();
        }
      });

  // panel favorite
  auto panel_favorite = p_panel->getChildByName<ui::Button *>("panelFavorite");
  panel_favorite->addTouchEventListener(
      [this](Ref *sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
          this->touchPanelFavorite();
        }
      });

  // last updated at
  auto label_last_updated_at =
      p_panel->getChildByName<ui::Text *>("txtLastUpdatedAt");
  label_last_updated_at->setString(
      lib::network::DataStoreSingleton::getInstance()
          ->getLastUpdatedAtToFormatString());

  this->_p_contents->setTag(Tag_Id_Sensor);
  this->addChild(this->_p_contents);

  // initialize sort type
  this->updateSortType();

  // this->nextScene(Task_Id_World);

  return true;
}

void Sensors::touchBack() {
  auto p_parent_scene = static_cast<scene::Main *>(this->getParent());
  p_parent_scene->touchSensorsBack();
}

void Sensors::touchSearch(Ref *sender) {
  CCLOG("Sensors::touchSearch");
  this->detachTouchParticle();
  // this->attachBlueEffect(20.0f, 20.0f, 3);
  // MessageBox("sorry. comming soom.", "search");

  auto p_modal_search = scene::modal::Search::create();

  CallFunc *callback = CallFunc::create(
      p_modal_search, SEL_CallFunc(&scene::modal::Search::detachSlideIn));

  scene::layout::helper::Contents::SlideIn(
      p_modal_search, 0.2f, scene::layout::helper::Contents::Forward_To_Right_e,
      callback, true);

  this->getParent()->addChild(p_modal_search, Zorders_Modal_Dialog);
}

void Sensors::touchSort(Ref *sender) {
  CCLOG("Sensors::touchSort");
  this->detachTouchParticle();
  // MessageBox("sorry. comming soom.", "sort");

  auto p_modal_sort = scene::modal::Sort::create();

  CallFunc *callback = CallFunc::create(
      p_modal_sort, SEL_CallFunc(&scene::modal::Sort::detachSlideIn));

  scene::layout::helper::Contents::SlideIn(
      p_modal_sort, 0.2f, scene::layout::helper::Contents::Forward_To_Right_e,
      callback, true);

  this->getParent()->addChild(p_modal_sort, Zorders_Modal_Dialog);
}

void Sensors::touchPanelFavorite() { this->nextScene(Task_Id_Favorite); }

void Sensors::touchPanelWorld() { this->nextScene(Task_Id_World); }

void Sensors::removeAnimationDone(Node *sender, ssize_t index) {}

void Sensors::setMesurementData(cocos2d::Node *panel,
                                lib::object::LocationItem &location_item) {

  auto p_panel_record = panel->getChildByName<ui::Layout *>("panelRecord");

  std::stringstream ss;
  auto p_text_sensor_id =
      p_panel_record->getChildByName<ui::Text *>("txtSenserId");
  ss << location_item.sensor1_device_id << "(" << location_item.tube_name << "/"
     << location_item.conversion_rate << ")";

  std::string sensor1_device_id = "device_id : " + ss.str();
  ss.clear();
  p_text_sensor_id->setContentSize(Size(400, 150));
  p_text_sensor_id->setPositionX(220);
  p_text_sensor_id->setString(sensor1_device_id);

  auto p_text_city = p_panel_record->getChildByName<ui::Text *>("textCity");
  p_text_city->setString(location_item.name_en);

  int cpm = location_item.recent_value;
  std::stringstream ss1, ss2, ss3;
  ss1 << cpm;
  auto p_text_recent_cpm =
      p_panel_record->getChildByName<ui::Text *>("textRecentCpm");
  p_text_recent_cpm->setString(ss1.str());
  ss1.clear();
  double usv =
      lib::Util::round((double)cpm / (double)location_item.conversion_rate, 3);
  ss2 << usv;
  auto p_text_recent_usv =
      p_panel_record->getChildByName<ui::Text *>("textRecentuSv");
  p_text_recent_usv->setString(ss2.str());

  auto p_text_sensor_status =
      p_panel_record->getChildByName<ui::Text *>("txtSensorStatus");
  std::string status_str;
  cocos2d::Color4B color;
  switch (location_item.sensor_status) {
  case 1: {
    status_str = "Online";
    color = Color4B(91, 192, 222, 255);
    break;
  }
  case 2: {
    status_str = "Offline";
    color = Color4B(217, 83, 79, 255);
    break;
  }
  case 3: {
    status_str = "Offline long";
    color = Color4B(217, 83, 79, 255);
    break;
  }
  }

  p_text_sensor_status->setString(status_str);
  p_text_sensor_status->setTextColor(color);

  p_text_recent_usv->setString(ss2.str());

  auto p_txt_recent_datetime =
      p_panel_record->getChildByName<ui::Text *>("txtRecentDatetime");
  std::string captured_at = location_item.recent_captured_at;
  p_txt_recent_datetime->setString(captured_at);

  // aggregations
  std::stringstream ss4, ss5, ss6, ss7;

  double avg_value = static_cast<double>(location_item.yesterday_average_value /
                                         location_item.conversion_rate);
  double peak_value = static_cast<double>(location_item.yesterday_peak_value /
                                          location_item.conversion_rate);
    
  int alarm_value =static_cast<int>(location_item.alarm_value);

  // @note round method
  avg_value = lib::Util::round(avg_value, 3);
  peak_value = lib::Util::round(peak_value, 3);
  
  ss4 << avg_value << " μSv/hour";
  ss5 << peak_value << " μSv/hour";
  ss7 << alarm_value << " cpm";

  CCLOG("peak_value : %s, avg_value : %s alarm : %s", ss5.str().c_str(),
        ss4.str().c_str(), ss7.str().c_str());

  auto p_text_avg_value =
      p_panel_record->getChildByName<ui::Text *>("txtYesterdayAverage");
  p_text_avg_value->setString(ss4.str());
  
  auto p_text_peak_value =
      p_panel_record->getChildByName<ui::Text *>("txtYesterdayPeak");
  p_text_peak_value->setString(ss5.str());
  
  auto p_text_alarm_value =
      p_panel_record->getChildByName<ui::Text *>("txtSensorAlarm");
  p_text_alarm_value->setString(ss7.str());
    

  // favorite
  auto p_button_favorite =
      p_panel_record->getChildByName<ui::Button *>("btnFavorite");

  auto flag = lib::Util::getFavorite(location_item.m_sensor_main_id);
  if (flag) {
    // enabled
    p_button_favorite->loadTextures("res/icon/menu/star_orange.png",
                                    "res/icon/menu/star_orange.png",
                                    "res/icon/menu/star_orange.png");
  } else {
    // disabled
    p_button_favorite->loadTextures("res/icon/menu/star_gray.png",
                                    "res/icon/menu/star_gray.png",
                                    "res/icon/menu/star_gray.png");
  }

  p_button_favorite->setEnabled(false);
}

void Sensors::showAnalyticsDialog(int m_sensor_main_id) {
  auto p_modal_mesurements_analytics =
      scene::menu::MesurementsAnalytics::create();
  p_modal_mesurements_analytics->setTag(Tag_Id_Mesurements_Analytics);
  p_modal_mesurements_analytics->prepare(m_sensor_main_id);

  // Slide In Animation
  scene::layout::helper::Contents::SlideIn(
      p_modal_mesurements_analytics, 0.2f,
      scene::layout::helper::Contents::Forward_To_Left_e);

  //    scene::layout::helper::Contents::SlideOut(
  //        this, 0.4f, scene::layout::helper::Contents::Forward_To_Left_e,
  //        false);

  this->addChild(p_modal_mesurements_analytics);
}

void Sensors::closeAnalyticsDialog() {
  auto p_modal_mesurements_analytics =
      static_cast<scene::menu::MesurementsAnalytics *>(
          this->getChildByTag(Tag_Id_Mesurements_Analytics));

  // Slide Out Animation
  scene::layout::helper::Contents::SlideOut(
      p_modal_mesurements_analytics, 0.2f,
      scene::layout::helper::Contents::Forward_To_Right_e, true);

  // Update Select Record
  // read list view
  auto p_panel =
      this->_p_contents->getChildByName<ui::Layout *>("panelBackground");
  auto p_list_view = p_panel->getChildByName<ui::ListView *>("listSensors");
  auto p_widget = p_list_view->getItem(p_list_view->getCurSelectedIndex());
  lib::object::LocationItem location_item =
      lib::network::DataStoreSingleton::getInstance()->getLocationItem(
          p_modal_mesurements_analytics->getMSensorMainId());

  auto p_record = p_widget->getChildByTag(Tag_Id_Sensor_Record);
  this->setMesurementData(p_record, location_item);
}

void Sensors::refresh(void) {

  scene::Main *p_scene_main = static_cast<scene::Main *>(this->getParent());
  p_scene_main->attachWaitAnimation();

  auto p_panel =
      this->_p_contents->getChildByName<ui::Layout *>("panelBackground");

  // last updated at
  auto label_last_updated_at =
      p_panel->getChildByName<ui::Text *>("txtLastUpdatedAt");
  label_last_updated_at->setString(
      lib::network::DataStoreSingleton::getInstance()
          ->getLastUpdatedAtToFormatString());

  auto p_tab_world = p_panel->getChildByName<LayerGradient *>("panelWorld");
  auto p_tab_favorite =
      p_panel->getChildByName<LayerGradient *>("panelFavorite");

  const Color3B enbale_color = Color3B(30, 144, 255);
  const Color3B disable_color = Color3B(191, 191, 191);

  if (this->task_id == Task_Id_World) {
    p_tab_world->setStartColor(enbale_color);
    p_tab_favorite->setStartColor(disable_color);
  } else if (this->task_id == Task_Id_Favorite) {
    p_tab_world->setStartColor(disable_color);
    p_tab_favorite->setStartColor(enbale_color);
  }

  // clear list
  // ひょっとすると消さないで現在の値を更新する必要があるかも
  auto p_list_view = p_panel->getChildByName<ui::ListView *>("listSensors");
  p_list_view->removeAllChildren();

  this->showSensorListOneOfEach();

  this->updateSortType();
}

void Sensors::nextScene(Task_Id task_id) {

  Task_Id before_task_id = this->task_id;
  this->task_id = task_id;

  switch (this->task_id) {
  case Task_Id_World:
    this->refresh();
    break;
  case Task_Id_Favorite:
    this->refresh();
    break;
  default:
    break;
  }
}

void Sensors::detachWaitAnimationLocal(void) {
  CCLOG("Sensors::detachWaitAnimation");
  scene::Main *p_scene_main = static_cast<scene::Main *>(this->getParent());
  p_scene_main->detachWaitAnimation();
}

void Sensors::showSensorListOneOfEach(void) {
  CCLOG("Sensors::showSensorListOneOfEach");

  std::map<int, lib::object::LocationItem> m_sensors =
      lib::network::DataStoreSingleton::getInstance()->getLocationItemAll();

  // search(filter)
  lib::Util::Filter(m_sensors, this->getKeyWord());

  // get sorted index
  std::vector<std::pair<int, int>> v_sensors_index =
      lib::Util::GetSortedLocationIndex(
          static_cast<scene::menu::Sensors::Sort_Id>(this->getSortId()),
          this->hasOrderDesc(), m_sensors);

  this->m_sensor_main_ids.clear();

  for (auto sensor_index : v_sensors_index) {
    auto favorite = lib::Util::getFavorite(sensor_index.second);
    if (this->task_id == Task_Id_Favorite && !favorite) {
      continue;
    }
    // one make que list
    this->m_sensor_main_ids.push_back(sensor_index.second);
  }

  // schedule
  // SetScheduler
  float interval = 0.05f; // @todo optimize
  schedule(schedule_selector(Sensors::showSensorByQueue), interval);
}

void Sensors::showSensorByQueue(float dt) {

  auto p_panel =
      this->_p_contents->getChildByName<ui::Layout *>("panelBackground");
  auto p_list_view = p_panel->getChildByName<ui::ListView *>("listSensors");

  static int count_down = 5;

  if (this->m_sensor_main_ids.empty()) {
    // Unschedule
    unschedule(schedule_selector(Sensors::showSensorByQueue));
    p_list_view->jumpToTop();
    this->detachWaitAnimationLocal();
    return;
  }

  int m_sensor_main_id = this->m_sensor_main_ids.front();
  this->m_sensor_main_ids.pop_front();
  count_down--;
  CCLOG("Sensors::showSensorByQueue m_sensor_main_id %d", m_sensor_main_id);

  lib::object::LocationItem item =
      lib::network::DataStoreSingleton::getInstance()->getLocationItem(
          m_sensor_main_id);

  auto favorite = lib::Util::getFavorite(item.m_sensor_main_id);
  if (this->task_id == Task_Id_Favorite && !favorite) {
    return;
  }

  auto p_record = CSLoader::getInstance()->createNode("res/sensors_record.csb");
  this->setMesurementData(p_record, item);

  p_record->setTag(Tag_Id_Sensor_Record);

  if (count_down > 0) {
    // set animation
    p_record->setCascadeOpacityEnabled(true);
    p_record->setOpacity(0);
    auto action = FadeIn::create(0.3f);
    p_record->runAction(action);
  }

  auto widget = ui::Widget::create();

  widget->addChild(p_record);

  widget->setTag(item.m_sensor_main_id);

  widget->setContentSize(p_record->getContentSize());
  p_list_view->pushBackCustomItem(widget);

  widget->setTouchEnabled(true); // enable listview touch event }
}

void Sensors::updateSortType(void) {
  // get text widget
  auto p_panel =
      this->_p_contents->getChildByName<ui::Layout *>("panelBackground");

  auto p_text_sort_type = p_panel->getChildByName<ui::Text *>("txtSortType");

  std::string label_name = scene::modal::Sort::getLableName(this->sort_id);

  if (!this->sort_desc) {
    label_name = label_name + " ASC";
  } else {
    label_name = label_name + " DESC";
  }

  p_text_sort_type->setString(label_name);
}
}
}
