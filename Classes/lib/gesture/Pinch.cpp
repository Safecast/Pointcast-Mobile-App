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
    
}

void Pinch::attachTouchesMove(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent)
{
    Touch* p_first_finger = touches.at(E_FISRT_FINGER);
    Touch* p_second_finger = touches.at(E_SECOND_FINGER);
    this->_current_distance = p_first_finger->getLocation().getDistance(p_second_finger->getLocation());
}
    
void Pinch::finish()
{
    this->_init_point0 = cocos2d::Point::ZERO;
    this->_init_point1 = cocos2d::Point::ZERO;
    this->_init_distance = 0.0f;
    this->_current_distance = 0.0f;
    
}

float Pinch::getPinchScale()
{
    if (_init_distance > 0.0f && _current_distance > 0.0f)
    {
        return (((this->_init_distance / this->_current_distance) * 100) / 3);
    } else {
        return 100;
    }
}

}

}

