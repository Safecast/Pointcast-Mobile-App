//
//  Board.cpp
//  pointcast
//
//  Created by Mitsuo Okada on 2015/11/20.
//
//

#include "Board.hpp"
#include "lib/Util.hpp"

namespace scene {

namespace chart {

USING_NS_CC;

Board *Board::create(Board::PrepareData prepare_data) {
  Board *pRet = new (std::nothrow) Board();
  pRet->_prepare_data = prepare_data;
  pRet->setContentSize(pRet->_prepare_data.chart_size);
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

  this->drawLabel();

  this->drawPoint();

  this->drawWeather();
}

void Board::drawBackGround() {
  static Point points[] = {
      Point(0, 0), Point(this->_prepare_data.chart_size.width, 0),
      Point(this->_prepare_data.chart_size.width,
            this->_prepare_data.chart_size.height),
      Point(0, this->_prepare_data.chart_size.height),

  };
  this->drawPolygon(points,         // 頂点の座標のデータ
                    4,              // 角数
                    Color4F::WHITE, // 図形の色
                    1,              // 枠線の太さ
                    Color4F::WHITE  // 枠線の色
                    );
}

void Board::drawFrame() {
  // Frame line vertical
  this->drawSegment(this->_prepare_data.chart_offset, // start
                    Point(this->_prepare_data.chart_offset.x,
                          this->_prepare_data.chart_size.height +
                              this->_prepare_data.chart_offset.y), // end
                    1.0f,                                          // bold
                    Color4F::GRAY                                  // color
                    );

  // Frame line horizontal
  this->drawSegment(this->_prepare_data.chart_offset, // start
                    Point(this->_prepare_data.chart_offset.x +
                              this->_prepare_data.chart_size.width,
                          this->_prepare_data.chart_offset.y), // end
                    1.0f,                                      // bold
                    Color4F::GRAY                              // color
                    );

  // line of vertical
  time_t interval =
      this->_prepare_data.end_point - this->_prepare_data.start_point;

  for (int i = 1; i <= this->_prepare_data.vertical_line; i++) {
    time_t point_time =
        (int)(interval / this->_prepare_data.vertical_line * i) +
        this->_prepare_data.start_point;

    double x = this->getX(point_time);
    this->drawSegment(
        Point(x, this->_prepare_data.chart_offset.y), // start
        Point(x, this->_prepare_data.chart_offset.y +
                     this->_prepare_data.chart_size.height), // end
        1.0f,                                                // bold
        Color4F::GRAY                                        // color
        );
  }

  for (int i = 1; i <= this->_prepare_data.horizontal_line; i++) {
    double split_value = (double)(this->_prepare_data.vertical_top_value /
                                  this->_prepare_data.horizontal_line * i);

    double y = this->getY(split_value);
    this->drawSegment(Point(this->_prepare_data.chart_offset.x, y), // start
                      Point(this->_prepare_data.chart_size.width +
                                this->_prepare_data.chart_offset.x,
                            y),     // end
                      1.0f,         // bold
                      Color4F::GRAY // color
                      );
  }
}

void Board::drawLabel() {
  // line of horizontal
  time_t interval =
      this->_prepare_data.end_point - this->_prepare_data.start_point;
  for (int i = 0; i <= this->_prepare_data.vertical_line; i++) {
    if (i % 2 == 1 && i != this->_prepare_data.vertical_line) {
      continue;
    }
    time_t point_time =
        (int)(interval / this->_prepare_data.vertical_line * i) +
        this->_prepare_data.start_point;
    double x = this->getX(point_time);
    double y = this->_prepare_data.chart_offset.y;
    const char *format = "%m/%d\n%H:%M";
    std::string time_string = lib::Util::getDatetimeString(point_time, format);
    auto p_text_time = Label::createWithSystemFont(time_string, "System", 24);
    p_text_time->setAnchorPoint(Point(0.5f, 1.0f));
    p_text_time->setPosition(Point(x, y - 10));
    p_text_time->setColor(Color3B::BLACK);
    this->addChild(p_text_time);
  }

  // line of horizontal
  double last_x, last_y;
  for (int i = 0; i <= this->_prepare_data.horizontal_line; i++) {
    if (i == 0 && i != this->_prepare_data.horizontal_line) {
      continue;
    }

    double point_value = (double)(this->_prepare_data.vertical_top_value /
                                  this->_prepare_data.horizontal_line * i);
    double x = this->_prepare_data.chart_offset.x - 20;
    double y = this->getY(point_value);

    const float usv = (double)point_value;
    std::string value_string =
        std::to_string(lib::Util::round(usv, 2)).substr(0, 4);
    // + "\n" +this->_prepare_data.vertical_unit;
    auto p_text_time = Label::createWithSystemFont(value_string, "System", 24);
    p_text_time->setAnchorPoint(Point(1.0f, 0.5f));
    p_text_time->setPosition(Point(x, y));
    p_text_time->setColor(Color3B::BLACK);
    this->addChild(p_text_time);
    last_x = x;
    last_y = y;
  }

  // unit
  auto p_text_unit = Label::createWithSystemFont("μSv/h", "System", 24);
  p_text_unit->setAnchorPoint(Point(1.0f, 0.5f));
  p_text_unit->setPosition(Point(last_x, last_y + 30));
  p_text_unit->setColor(Color3B::BLACK);
  this->addChild(p_text_unit);
}

void Board::drawPoint() {

  for (int i = 0; i < this->_prepare_data.v_chart_items.size(); i++) {
    lib::object::ChartItem chart_item = this->_prepare_data.v_chart_items.at(i);
    if (this->_prepare_data.start_point > chart_item.timestamp ||
        this->_prepare_data.end_point < chart_item.timestamp) {
      continue;
    }

    double x = this->getX(chart_item.timestamp);
    double y = this->getY((double)chart_item.value /
                          (double)this->_prepare_data.conversion_rate);

    // calculate color
    const float usv =
        ((float)chart_item.value / (float)this->_prepare_data.conversion_rate);
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

  for (int i = 0; i < this->_prepare_data.v_weather_items.size(); i++) {
    lib::object::WeatherItem weather_item =
        this->_prepare_data.v_weather_items.at(i);

    if (this->_prepare_data.start_point > weather_item.timestamp ||
        this->_prepare_data.end_point < weather_item.timestamp) {
      continue;
    }

    if (before_icon == weather_item.icon) {
      // has same wheather then continue
      continue;
    }

    double x = this->getX(weather_item.timestamp);
    double y = this->_prepare_data.chart_size.height +
               this->_prepare_data.chart_offset.y + 20.0f;

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

double Board::getX(time_t hrizontal_value) {
  double interval =
      (double)(this->_prepare_data.end_point - this->_prepare_data.start_point);

  double x = (((hrizontal_value - this->_prepare_data.start_point) / interval) *
              this->_prepare_data.chart_size.width) +
             this->_prepare_data.chart_offset.x;

  return x;
}

double Board::getY(double vertical_value) {
  double y = (((double)vertical_value /
               (double)this->_prepare_data.vertical_top_value) *
              this->_prepare_data.chart_size.height) +
             this->_prepare_data.chart_offset.y;

  return y;
}
}
}
