#ifndef CustomScrollView_h
#define CustomScrollView_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace scene {

namespace customize {

class CustomUIScrollView : public cocos2d::ui::ScrollView
{
private:
    cocos2d::EventListenerTouchAllAtOnce* _multiTouchListener;
    
public:
    
private:
    
public:
    static CustomUIScrollView* create();
    
    void setMultiTouchEnabled(bool enable);
    
    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent);
    
    virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches,cocos2d::Event *pEvent);
    
    virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent);
    
    virtual void onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent);
};
    
}
}
    
#endif
