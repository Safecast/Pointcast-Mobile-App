//
//  Sensors.hpp
//  pointcast
//
//  Created by Mitsuo Okada on 2015/10/29.
//
//

#ifndef Sensors_hpp
#define Sensors_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "network/HttpClient.h"

#include "scene/base/AbstructScene.hpp"

#include "json/document.h"
#include "json/rapidjson.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

#include "lib/object/LocationItem.hpp"

namespace scene {

namespace menu {

class Sensors : public base::AbstructScene {
  // member
public:
private:
  Node *_p_contents;

  Node *_p_scroll_view;

  // function
public:
  enum Task_Id {
    // show every sensor
    Task_Id_World = 1,

    // show favorite sensor
    Task_Id_Favorite,
  };

  enum Sort_Id {
    // sort by μSv/Hour
    Sort_Id_μSv_Hour = 0,
    // sort by device id
    Sort_Id_Device_Id,
    // near point
    Sort_Id_Near_Point,
    // active / inactive
    // Sort_Id_Active_Inactive,
    // eof
    Sort_Id_EOF,
  };

  virtual bool init();

  void updateSensorData(void);
  
  void onCallbackUpdateSensorData(cocos2d::network::HttpClient *sender,
                                  cocos2d::network::HttpResponse *response);
  
  void refresh(void);
  
  void refreshNoUpdate(void);

  void showSensorListOneOfEach(void);

  void showSensorByQueue(float dt);

  void touchBack();

  void touchSearch(Ref *sender);

  void touchSort(Ref *sender);

  void touchPanelFavorite();

  void touchPanelWorld();

  void removeAnimationDone(Node *sender, ssize_t index);

  void setSortId(int sort_id) { this->sort_id = static_cast<Sort_Id>(sort_id); }

  int getSortId(void) { return this->sort_id; }

  void setKeyWord(std::string key_word) { this->key_word = key_word; }

  std::string getKeyWord(void) { return this->key_word; }

  void setOrderDesc(bool has_order_desc) { this->sort_desc = has_order_desc; }

  bool hasOrderDesc(void) { return this->sort_desc; }

  CREATE_FUNC(Sensors);

  void showAnalyticsDialog(int m_sensor_main_id);

  void closeAnalyticsDialog();

private:
  Task_Id task_id;

  Sort_Id sort_id;

  std::string key_word;

  bool sort_desc;

  void setMesurementData(cocos2d::Node *panel,
                         lib::object::LocationItem &location_item);

  void nextScene(Task_Id task_id);

  void detachWaitAnimationLocal(void);

  std::deque<int> m_sensor_main_ids;

  void updateSortType(void);
  
  void retryRequest(void);
  
  void retryCancel(void);
};
}
}
#endif /* Sensors_hpp */
