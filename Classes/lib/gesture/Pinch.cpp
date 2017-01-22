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
    
    this->point0 = p_first_finger->getLocation();
    this->point1 = p_second_finger->getLocation();
}

void Pinch::attachTouchesMove(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent)
{
    
}

float Pinch::getPinchScale()
{

}

}

}

