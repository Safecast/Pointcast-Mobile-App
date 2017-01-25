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
    
    float _max;

    float _min;
    
    cocos2d::Vec2 _init_point0;
    
    cocos2d::Vec2 _init_point1;
    
    float _init_distance;
    
    float _current_distance;
    
    float _previous_distance;
    
    float _previous_scale;
    
    float _current_scale;
    
    bool _initialized;
    
public:
    
// functions
private:
public:
    Pinch(float max, float min);
    
    ~Pinch();
    
    void setBasePoint(const std::vector<cocos2d::Touch*>& touches,cocos2d::Event *pEvent, float current_scale);
    
    void init();
    
    void attachTouchesMove(const std::vector<cocos2d::Touch*>& touches,cocos2d::Event *pEvent);
    
    float getPinchScale();
    
    bool isChanged();
    
};

#endif

}

}
