//
//  DataStoreSingleton.cpp
//  pointcast
//
//  Created by Mitsuo Okada on 2015/11/04.
//
//
#include "DataStoreSingleton.hpp"
#include <time.h>

#include "lib/Util.hpp"

USING_NS_CC;
using namespace rapidjson;

namespace lib {

namespace network {

DataStoreSingleton *DataStoreSingleton::_p_instance = NULL;

const std::string DataStoreSingleton::end_point =

#if ENVIRONMENT == 101
    "http://pointcast-mobile-app.safecast.org/pointcast/";
#elif ENVIRONMENT == 201
    "http://pointcast-mobile-app.safecast.org/pointcast/";
#elif ENVIRONMENT == 301
    "http://pointcast-mobile-app.safecast.org/pointcast/";
#endif

const std::string DataStoreSingleton::CACHE_DIR = "cahce";

DataStoreSingleton::DataStoreSingleton():_p_store_callback(nullptr) {
  // default -> safecast office
  this->_loc.latitude = 35.656064f;
  this->_loc.longitude = 139.695505f;
}

DataStoreSingleton::~DataStoreSingleton() {
  if (this->_p_instance != NULL) {
    delete this->_p_instance;
  }
  this->_p_instance = NULL;
  return;
}

DataStoreSingleton *DataStoreSingleton::getInstance() {
  if (DataStoreSingleton::_p_instance == NULL) {
    DataStoreSingleton::_p_instance = new DataStoreSingleton();
  }
  return DataStoreSingleton::_p_instance;
}

void DataStoreSingleton::setResponseCallback(
    cocos2d::Node *callbackObject,
    cocos2d::network::SEL_HttpResponse callbackFunction) {
  // set callback object
  this->_p_callbackObject = callbackObject;

  // set callback function
  this->_p_callbackFunction = callbackFunction;
}

void DataStoreSingleton::resetResponseCallback() {
  // insert null value
  this->_p_callbackObject = NULL;
  this->_p_callbackFunction = NULL;
}

void DataStoreSingleton::requestPointcastHome(void) {
  // @todo if exist cache, response from cache data. if can't. just request.

  cocos2d::network::HttpRequest *request = new cocos2d::network::HttpRequest();
  const std::string url = DataStoreSingleton::end_point + "home.json";
  request->setUrl(url.c_str());
  request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
  request->setResponseCallback(
      CC_CALLBACK_2(DataStoreSingleton::callbackHttpPointcastHome, this));

  // write the post data
  const char *postData =
      "visitor=cocos2d&TestSuite=Extensions Test/NetworkTest";
  request->setRequestData(postData, strlen(postData));

  cocos2d::network::HttpClient::getInstance()->send(request);
  request->release();
}

void DataStoreSingleton::callbackHttpPointcastHome(
    cocos2d::network::HttpClient *sender,
    cocos2d::network::HttpResponse *response) {
  std::vector<char> *buffer = response->getResponseData();
  const char *data = reinterpret_cast<char *>(&(buffer->front()));

  CCLOG("CallbackHttpPointcastHome %ld %s", response->getResponseCode(), data);

  // @todo check error
  // assert(response);
  // assert(response->getResponseCode() == 200);

  if (response->getResponseCode() == 200) {
    // last updated at
    time(&this->last_updated_at);
    
    std::string string_response_data =
    lib::Util::createResponseBodyStringFromResponse(response);

    // store data
    this->_p_m_http_response_data[Request_Pointcast_Home_e] =
    string_response_data;
    
    this->storeSensorListData(string_response_data);
  }

  if (this->_p_callbackObject && this->_p_callbackFunction) {
    (this->_p_callbackObject->*this->_p_callbackFunction)(sender, response);
  }
}

std::string DataStoreSingleton::getResponseData(E_Http_Request_Id http_request_id) {
  return this->_p_m_http_response_data[http_request_id];
}

std::string DataStoreSingleton::getResponseAnalyticsData(int m_sensor_main_id, time_t start_time, time_t end_time) {
    
  std::string cache_key = DataStoreSingleton::getAnalyticsCacheKey(m_sensor_main_id, start_time, end_time);
  
  // 本日分のデータはキャッシュから取らない
  time_t now = time(NULL);
  if (start_time < now && now < end_time)
  {
    return "";
  }
  
  // cahceからの取得を試みる
  if (!this->hasAnalyticsData(cache_key))
  {
      return "";
  }

  return this->_p_m_http_analytics_response_data[cache_key];
}

void DataStoreSingleton::requestPointcastAnalytics(int m_sensor_main_id, time_t start_time, time_t end_time) {
  cocos2d::network::HttpRequest *request = new cocos2d::network::HttpRequest();

  std::string cache_key = this->getAnalyticsCacheKey(m_sensor_main_id, start_time, end_time);
  request->setTag(cache_key);
    
  this->_request_m_sensor_main_id = m_sensor_main_id;

  std::stringstream ss;
  ss << "analytics/" << m_sensor_main_id << "/.json";
  const std::string url = DataStoreSingleton::end_point + ss.str();
  request->setUrl(url.c_str());
  request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
  request->setResponseCallback(
      CC_CALLBACK_2(DataStoreSingleton::callbackHttpPointcastAnalytics, this));

  // make post data
  std::stringstream start_buf;
  start_buf << start_time;
  std::stringstream end_buf;
  end_buf << end_time;

  std::string postDataString = "start_time=" + start_buf.str() + "&end_time=" + end_buf.str();
    
  // write the post data
  request->setRequestData(postDataString.c_str(), strlen(postDataString.c_str()));

  cocos2d::network::HttpClient::getInstance()->send(request);
  request->release();
}

void DataStoreSingleton::storeAnalyticsData(int m_sensor_main_id, time_t start_time,
                                        time_t end_time, bool force_store, cocos2d::CallFunc* callback)
{
    
    this->_p_store_callback = callback;
    
    // get cache key
    std::string cache_key = this->getAnalyticsCacheKey(m_sensor_main_id, start_time, end_time);
    
    // check cached data
    if (!force_store)
    {
        if (this->hasAnalyticsData(cache_key))
        {
            this->_p_store_callback->execute();
        }
    }
    
    // http request
    this->requestPointcastAnalytics(m_sensor_main_id, start_time, end_time);
}
    
std::string DataStoreSingleton::getAnalyticsCacheFilePath(std::string cache_key)
{
    std::string cache_file_path = cocos2d::FileUtils::getInstance()->getWritablePath();
    cache_file_path = cache_file_path + DataStoreSingleton::CACHE_DIR + "/" + cache_key + ".dat";
    return cache_file_path;
}
    
bool DataStoreSingleton::hasAnalyticsData(std::string cache_key)
{
    // check memory cache
    std::map<std::string, std::string>::iterator it;
    it = this->_p_m_http_analytics_response_data.find(cache_key);
    if (it != this->_p_m_http_analytics_response_data.end())
    {
        return true;
    }
    
    // check file cache
    std::string cache_file_path = this->getAnalyticsCacheFilePath(cache_key);
    if (cocos2d::FileUtils::getInstance()->isFileExist(cache_file_path))
    {
        // get saved data
        std::string saved_data = cocos2d::FileUtils::getInstance()->getStringFromFile(cache_file_path);
        this->_p_m_http_analytics_response_data[cache_key] = saved_data;
        return true;
    }
    
    return false;

}

void DataStoreSingleton::callbackHttpPointcastAnalytics(
    cocos2d::network::HttpClient *sender,
    cocos2d::network::HttpResponse *response) {
  CCLOG("callbackHttpPointcastAnalytics");

  std::vector<char> *buffer = response->getResponseData();
    
  const char *data = reinterpret_cast<char *>(&(buffer->front()));

  CCLOG("callbackHttpPointcastAnalytics %ld %s", response->getResponseCode(),
        data);

  // @todo check error
  // assert(response);
  CCLOG("response code %ld", response->getResponseCode());
  // assert(response->getResponseCode() == 200);
  
  if (response->getResponseCode() == 200) {
    std::string string_response_data =
      lib::Util::createResponseBodyStringFromResponse(response);
    
      // store data to memory
      std::string cache_key = response->getHttpRequest()->getTag();
      this->_p_m_http_analytics_response_data[cache_key] =
      string_response_data;
      
      // store data to file
      std::string cache_file_path = this->getAnalyticsCacheFilePath(cache_key);
      cocos2d::FileUtils::getInstance()->writeStringToFile(string_response_data, cache_key);
    
  }

  this->_p_store_callback->execute();
  
  if (this->_p_callbackObject && this->_p_callbackFunction) {
    (this->_p_callbackObject->*this->_p_callbackFunction)(sender, response);
  }
  
}

void DataStoreSingleton::storeSensorListData(std::string response) {
  // JSON解析
  rapidjson::Document document;
  document.Parse<0>(response.c_str());
  if (document.HasParseError()) {
    // 解析エラー
    assert(false);
    return;
  }

  // データ取得
  if (document.IsObject()) {
    // JSON配列取得
    if (document.HasMember("sensors") && document["sensors"].IsArray()) {
      const rapidjson::Value &sensor_records = document["sensors"];
      // 要素取得
      for (int i = 0, max = sensor_records.Size(); i < max; i++) {
        const rapidjson::Value &record = sensor_records[i];
        lib::object::LocationItem location_item;

        if (record.HasMember("m_sensor_main_id")) {
          location_item.m_sensor_main_id = record["m_sensor_main_id"].GetInt();
        }

        if (record.HasMember("latitude")) {
          location_item.latitude = record["latitude"].GetDouble();
        }

        if (record.HasMember("longitude")) {
          location_item.longitude = record["longitude"].GetDouble();
        }

        if (record.HasMember("sensor1_device_id")) {
          location_item.sensor1_device_id =
              record["sensor1_device_id"].GetInt();
        }
        if (record.HasMember("name_en")) {
          location_item.name_en = record["name_en"].GetString();
        }
        if (record.HasMember("name_jp")) {
          location_item.name_jp = record["name_jp"].GetString();
        }

        if (record.HasMember("name")) {
          location_item.tube_name = record["name"].GetString();
        }

        if (record.HasMember("conversion_rate")) {
          location_item.conversion_rate = record["conversion_rate"].GetInt();
        }

        if (record.HasMember("sensor_status")) {
          location_item.sensor_status = record["sensor_status"].GetInt();
        }

        if (record.HasMember("dre2cpm")) {
          location_item.dre2cpm = record["dre2cpm"].GetDouble();
        }
          
        if (record.HasMember("alarm")) {
          location_item.alarm_value = record["alarm"].GetDouble();
        }

        if (record.HasMember("recent") && record["recent"].IsObject()) {
          const rapidjson::Value &recent = record["recent"];
          if (recent.HasMember("value")) {
            location_item.recent_value = recent["value"].GetInt();
            location_item.recent_captured_at =
                recent["captured_at"].GetString();
          }
        }

        // aggregations
        if (record.HasMember("aggregation") &&
            record["aggregation"].IsObject()) {
          const rapidjson::Value &aggregation = record["aggregation"];
          if (aggregation.HasMember("yesterday") &&
              aggregation["yesterday"].IsObject()) {
            const rapidjson::Value &yesterday = aggregation["yesterday"];
            location_item.yesterday_average_value =
                yesterday["avg_value"].GetDouble();
            location_item.yesterday_peak_value =
                yesterday["peak_value"].GetDouble();

          } else {
          }
        }
        if (location_item.m_sensor_main_id != 0) {
          this->_p_m_http_sensor_main_response_data[location_item
                                                        .m_sensor_main_id] =
              location_item;
        }
      }
    }
  }
}

lib::object::LocationItem
DataStoreSingleton::getLocationItem(int m_sensor_main_id) {
  return this->_p_m_http_sensor_main_response_data[m_sensor_main_id];
}

std::map<int, lib::object::LocationItem>
DataStoreSingleton::getLocationItemAll(void) const {
  return this->_p_m_http_sensor_main_response_data;
}

std::vector<int> DataStoreSingleton::getLocationItemIdList(void) const {
  std::vector<int> m_sensor_main_ids;
  for (auto item : this->_p_m_http_sensor_main_response_data) {
    m_sensor_main_ids.push_back(item.second.m_sensor_main_id);
  }
}

time_t DataStoreSingleton::getLastUpdatedAt() { return this->last_updated_at; }

std::string DataStoreSingleton::getLastUpdatedAtToFormatString() {

  struct tm *ptm;
  char buf[128];

  ptm = localtime(&this->last_updated_at);
  strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M", ptm);

  return std::string("Last Update : ") + std::string(buf);
}

void DataStoreSingleton::setLocation(float latitude, float longtitude) {
  this->_loc.latitude = latitude;
  this->_loc.longitude = longtitude;
}

DataStoreSingleton::location DataStoreSingleton::getLocation(void) {
  return this->_loc;
}
    
std::string DataStoreSingleton::getAnalyticsCacheKey(int m_sensor_main_id, time_t start_time, time_t end_time) {
    
    std::stringstream ss;
    ss << m_sensor_main_id << "_" << start_time << "_" << end_time;
    
    return ss.str();
}
    
}
}
