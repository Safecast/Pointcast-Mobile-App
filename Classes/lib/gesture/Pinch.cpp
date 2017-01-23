#include "Pinch.hpp"

USING_NS_CC;

namespace lib {
    
namespace gesture {

Pinch::Pinch()
{

}
    
Pinch::~Pinch()
{
    
}
    
void Pinch::init(const std::vector<cocos2d::Touch*>& touches,cocos2d::Event *pEvent)
{
    
    Touch* p_first_finger = touches.at(E_FISRT_FINGER);
    Touch* p_second_finger = touches.at(E_SECOND_FINGER);
    
    this->_init_point0 = p_first_finger->getLocation();
    this->_init_point1 = p_second_finger->getLocation();
    
    this->_init_distance = this->_init_point0.getDistance(_init_point1);
    
    cocos2d::log("init point0 x %f y %f : point1 x %f y %f distance %f", this->_init_point0.x, this->_init_point0.y,this->_init_point1.x, this->_init_point1.y, this->_init_distance);
    
}

void Pinch::attachTouchesMove(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent)
{
    Touch* p_first_finger = touches.at(E_FISRT_FINGER);
    Touch* p_second_finger = touches.at(E_SECOND_FINGER);
    this->_current_distance = p_first_finger->getLocation().getDistance(p_second_finger->getLocation());
    
    cocos2d::log("move point0 x %f y %f : point1 x %f y %f distance %f", p_first_finger->getLocation().x, p_first_finger->getLocation().y, p_second_finger->getLocation().x, p_second_finger->getLocation().y, this->_current_distance);
}
    
void Pinch::finish()
{
    this->_init_point0 = cocos2d::Point::ZERO;
    this->_init_point1 = cocos2d::Point::ZERO;
    this->_init_distance = 0.0f;
    this->_current_distance = 0.0f;
    this->_previous_distance = 0.0f;
    
}

float Pinch::getPinchScale()
{

    if (_current_distance > 0.0f)
    {
        this->_previous_distance = (((this->_init_distance / this->_current_distance)));
        this->_current_distance = this->_previous_distance;
        return this->_previous_distance;
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

