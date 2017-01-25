#include "Pinch.hpp"

USING_NS_CC;

namespace lib {
    
namespace gesture {

Pinch::Pinch(float max, float min):_max(max), _min(min), _init_distance(0.0f), _current_distance(0.0f),_initialized(false), _previous_distance(0.0f), _previous_scale(0.0f), _init_point0(cocos2d::Vec2::ZERO), _init_point1(cocos2d::Vec2::ZERO)
{
    
}
    
Pinch::~Pinch()
{
    this->init();
}
    
void Pinch::setBasePoint(const std::vector<cocos2d::Touch*>& touches,cocos2d::Event *pEvent, float current_scale)
{
    
    Touch* p_first_finger = touches.at(E_FISRT_FINGER);
    Touch* p_second_finger = touches.at(E_SECOND_FINGER);
    
    this->_init_point0 = p_first_finger->getLocation();
    this->_init_point1 = p_second_finger->getLocation();
    this->_current_scale = current_scale;
    
    this->_init_distance = this->_init_point0.getDistance(_init_point1);
    
    this->_initialized = true;
    
    cocos2d::log("init point0 x %f y %f : point1 x %f y %f distance %f", this->_init_point0.x, this->_init_point0.y,this->_init_point1.x, this->_init_point1.y, this->_init_distance);
    
}

void Pinch::attachTouchesMove(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent)
{
    
    if (!this->_initialized)
    {
        this->setBasePoint(touches, pEvent, this->_current_scale);
    }
    
    Touch* p_first_finger = touches.at(E_FISRT_FINGER);
    Touch* p_second_finger = touches.at(E_SECOND_FINGER);
    this->_current_distance = p_first_finger->getLocation().getDistance(p_second_finger->getLocation());
    
    cocos2d::log("move point0 x %f y %f : point1 x %f y %f distance %f", p_first_finger->getLocation().x, p_first_finger->getLocation().y, p_second_finger->getLocation().x, p_second_finger->getLocation().y, this->_current_distance);
}
    
void Pinch::init()
{
    this->_init_point0 = cocos2d::Point::ZERO;
    this->_init_point1 = cocos2d::Point::ZERO;
    this->_init_distance = 0.0f;
    this->_previous_distance = 0.0f;
    this->_current_distance = 0.0f;
    this->_previous_scale = 0.0f;
    this->_initialized = false;
}


float Pinch::getPinchScale()
{

    cocos2d::log("current_distance %f", this->_current_distance);
    if (_current_distance > 0.0f)
    {
        float relative_scale = ((((this->_init_distance / this->_current_distance))) - 1.0f) * 0.2f;
        cocos2d::log("relative_scale %f", relative_scale);
        this->_current_scale += relative_scale;
        
        if (this->_current_scale > this->_max) this->_current_scale = this->_max;
        if (this->_current_scale < this->_min) this->_current_scale = this->_min;
        this->_previous_scale = this->_current_scale;
        this->_previous_distance = this->_current_distance;
        return this->_current_scale;
    } else {
        return 1.0f;
    }
}

bool Pinch::isChanged()
{
    return (this->_previous_distance != this->_current_distance);
}
    
}

}

