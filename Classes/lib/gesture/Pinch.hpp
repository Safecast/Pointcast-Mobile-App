#ifndef Pinch_h
#define Pinch_h

#include "cocos2d.h"

namespace lib {
    
namespace gesture {

class Pinch
{
    
// properties
private:
    
    enum E_FINGER_TYPE
    {
      E_FISRT_FINGER = 0,
        
      E_SECOND_FINGER
    };
    
    cocos2d::Vec2 _init_point0;
    
    cocos2d::Vec2 _init_point1;
    
    float _init_distance;
    
    float _current_distance;
    
public:
    
// functions
private:
public:
    Pinch();
    
    ~Pinch();
    
    void init(const std::vector<cocos2d::Touch*>& touches,cocos2d::Event *pEvent);
    
    void finish();
    
    void attachTouchesMove(const std::vector<cocos2d::Touch*>& touches,cocos2d::Event *pEvent);
    
    float getPinchScale();
    
};

#endif

}

}
