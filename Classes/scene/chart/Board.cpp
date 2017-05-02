//
//  Board.cpp
//  pointcast
//
//  Created by Mitsuo Okada on 2015/11/20.
//
//

#include "Board.hpp"
#include "lib/Util.hpp"
#include "scene/layout/helper/Chart.hpp"

namespace scene {

namespace chart {

USING_NS_CC;

Board *Board::create(Board::Config config) {
  Board *pRet = new (std::nothrow) Board();
  pRet->_config = config;
  // pRet->setContentSize(pRet->_config.board_size);
  if (pRet && pRet->init()) {
    pRet->autorelease();
    return pRet;
  } else {
    delete pRet;
    pRet = NULL;
    return NULL;
  }
}

void Board::onEnter() {
  DrawNode::onEnter();

  this->drawBackGround();

  this->drawFrame();

    if (!this->_config.is_empty) {
        this->drawLabel();
        
        this->drawPoint();
        
        this->drawWeather();
    }
}

void Board::drawBackGround() {
  static Point points[] = {
      Point(this->_config.chart_offset.x, this->_config.chart_offset.y), Point(this->_config.board_size.width - this->_config.chart_offset.x, this->_config.chart_offset.y),
      Point(this->_config.board_size.width - this->_config.chart_offset.x,
            this->_config.board_size.height + this->_config.chart_offset.y),
      Point(this->_config.chart_offset.x, this->_config.board_size.height + this->_config.chart_offset.y),

  };
    
    
  Color4F fill_color = Color4F((225.0f/255.0f), (225.0f/255.0f), (255.0f/255.0f), 1.0f);
  Color4F border_color = Color4F(0.0f, (178.0f/255.0f), (255.0f/255.0f), 1.0f);
    
  this->drawPolygon(points,         // 頂点の座標のデータ
                    4,              // 角数
                    fill_color, // 図形の色
                    0.0f,              // 枠線の太さ
                    border_color  // 枠線の色
                    );
}

void Board::drawFrame() {
  // Frame line vertical
  // line of vertical(縦線)
  time_t interval = this->_config.end_point - this->_config.start_point;
  for (int i = 0; i <= this->_config.vertical_line; i++) {
      
    time_t point_time = (int)(interval / this->_config.vertical_line * i) +
                            this->_config.start_point;
  
    double x = this->getX(point_time);
    float line_weight = (i==0 || i == this->_config.vertical_line) ? 1.5f : 0.5f;
    this->drawSegment(Point(x, this->_config.chart_offset.y), // start
                      Point(x, this->_config.chart_offset.y +
                                   this->getChartSize().height), // end
                      line_weight,                                          // bold
                      Color4F::GRAY                                  // color
                      );
  }

  // line of horizontal(横線)
  for (int i = 0; i <= this->_config.horizontal_line; i++) {
    float bold;
    float offset_diff;
    Color4F color;
      
    this->getVerticalLineConfig(bold,offset_diff,color,i,this->_config.horizontal_line);
    
    double split_value = (double)(this->_config.vertical_top_value /
                                  this->_config.horizontal_line * i);

    double y = this->getY(split_value);
    
      Point start_point = Point(this->_config.chart_offset.x - offset_diff, y);
      Point end_point = Point(this->_config.board_size.width,
                              y);
      
      
        this->drawSegment(Point(this->_config.chart_offset.x, y), // start
                          Point(this->_config.board_size.width - this->_config.chart_offset.x,
                                y),     // end
                          bold,         // bold
                          color // color
                          );
   
  }
}
    
void Board::getVerticalLineConfig(float &bold, float &offset_diff,  cocos2d::Color4F &color, int i, int length)
{
    if (i == 0 || i == length)
    {
        bold = 1.5f;
        offset_diff = 20.0f;
        color = Color4F(128, 128, 128, 100);
    } else if(i == 2) {
        bold = 0.5f;
        offset_diff = 0.0f;
        color = Color4F(128, 128, 128, 100);
    } else {
        bold = 0.5f;
        offset_diff = 0.0f;
        color = Color4F(64, 64, 64, 100);
    }

}

void Board::drawLabel() {
    
  // label of horizontal(hour)
  time_t interval =
      this->_config.end_point - this->_config.start_point;
  for (int i = 0; i <= this->_config.vertical_line; i++) {
    if (i % 2 == 1) {
      continue;
    }
    time_t point_time =
        (int)(interval / this->_config.vertical_line * i) +
        this->_config.start_point;
    double x = this->getX(point_time);
    double y = this->_config.chart_offset.y;
    const char *format = "%H";
    std::string time_string = lib::Util::getDatetimeString(point_time, format);
    auto p_text_time = Label::createWithSystemFont(time_string, "System", 24);
    p_text_time->setAnchorPoint(Point(0.5f, 1.0f));
    p_text_time->setPosition(Point(x, y - 10));
    p_text_time->setColor(Color3B::BLACK);
    this->addChild(p_text_time);
  }

  // label of horizontal
  Color3B unit_color = Color3B(0, 0, 0);
  double last_x, last_y;
  for (int i = 0; i <= this->_config.horizontal_line; i++) {
    // 条件にハマったらラベル書かない
    if (i % 2 == 1 || i == 0) {
      continue;
    }

    double point_value = (double)(this->_config.vertical_top_value /
                                  this->_config.horizontal_line * i);
    double x = this->_config.chart_offset.x + 30;
    double y = this->getY(point_value);

    const float usv = (double)point_value;
    std::string value_string =
        std::to_string(lib::Util::round(usv, 2)).substr(0, 4);
    // + "\n" +this->_prepare_data.vertical_unit;
    auto p_text_time = Label::createWithSystemFont(value_string, "System", 24);
    p_text_time->setAnchorPoint(Point(1.0f, 0.5f));
    p_text_time->setPosition(Point(x, y));
    p_text_time->setColor(unit_color);
    this->addChild(p_text_time);
    last_x = x;
    last_y = y;
  }

  // unit
  auto p_text_unit = Label::createWithSystemFont("μSv/h", "System", 24);
  p_text_unit->setAnchorPoint(Point(1.0f, 0.5f));
  p_text_unit->setPosition(Point(last_x, last_y + 30));
  p_text_unit->setColor(unit_color);
  this->addChild(p_text_unit);
    
  // date
  const char *format = "%b %dth, %Y";
  std::string date_string = lib::Util::getDatetimeString(this->_config.start_point, format);
  auto p_text_date = Label::createWithSystemFont(date_string, "System", 30);
  p_text_date->setAnchorPoint(Point(0.0f, 0.0f));
  p_text_date->setPosition(Point(10, this->_config.board_size.height + this->_config.chart_offset.y + 60));
  p_text_date->setColor(unit_color);
  this->addChild(p_text_date);
    
}

void Board::drawPoint() {

  for (int i = 0; i < this->_config.v_chart_items.size(); i++) {
    lib::object::ChartItem chart_item = this->_config.v_chart_items.at(i);
    if (this->_config.start_point > chart_item.timestamp ||
        this->_config.end_point < chart_item.timestamp) {
      continue;
    }

    double x = this->getX(chart_item.timestamp);
    double y = this->getY((double)chart_item.value /
                          (double)this->_config.conversion_rate);

    // calculate color
    const float usv =
        ((float)chart_item.value / (float)this->_config.conversion_rate);
    cocos2d::Color3B color = lib::Util::GetLutColor(usv);

    this->drawDot(Point(x, y), 3.0f, Color4F(color));

    //    Sprite *dot = Sprite::create("res/circle_skyblue.png");
    //    BlendFunc func;
    //    func.src = GL_SRC_ALPHA;
    //    func.dst = GL_ONE;
    //    dot->setBlendFunc(func);
    //    dot->setPosition(cocos2d::Vec2(x, y));
    //    this->addChild(dot);
  }
}

void Board::drawWeather() {
  CCLOG("Board::drawWeather");

  static std::string before_icon = "";

  for (int i = 0; i < this->_config.v_weather_items.size(); i++) {
    lib::object::WeatherItem weather_item =
        this->_config.v_weather_items.at(i);

    if (this->_config.start_point > weather_item.timestamp ||
        this->_config.end_point < weather_item.timestamp) {
      continue;
    }

    if (before_icon == weather_item.icon) {
      // has same wheather then continue
      continue;
    }

    double x = this->getX(weather_item.timestamp);
    double y = this->_config.board_size.height +
               this->_config.chart_offset.y + 20.0f;

    std::string icon = "res/icon/weather/" + weather_item.icon + ".png";
    auto p_weather = Sprite::create(icon);
    if (!p_weather) {
      assert(false);
      continue;
    }
    p_weather->setAnchorPoint(Point(0.5f, 0.5f));
    p_weather->setPosition(x, y);
    this->addChild(p_weather, -1 * i);

    before_icon = weather_item.icon;
  }
}

double Board::getX(time_t horizontal_value) {
  double interval =
      (double)(this->_config.end_point - this->_config.start_point);

  double x = (((horizontal_value - this->_config.start_point) / interval) *
              this->getChartSize().width) +
             this->_config.chart_offset.x;

  return x;
}

double Board::getY(double vertical_value) {
  double y = (((double)vertical_value /
               (double)this->_config.vertical_top_value) *
              this->_config.board_size.height) +
             this->_config.chart_offset.y;

  return y;
}
    
Size Board::getChartSize() {
    
    Size size;
    size.width = this->_config.board_size.width - (this->_config.chart_offset.x * 2);
    size.height = this->_config.board_size.height;
    
    return size;
}
    
    
}
}
