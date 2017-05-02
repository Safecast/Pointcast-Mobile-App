#include "CustomUIScrollView.h"

namespace scene {
    
namespace customize {

CustomUIScrollView* CustomUIScrollView::create()
{
    CustomUIScrollView* view = new (std::nothrow) CustomUIScrollView();
    if (view && view->init())
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}
    
void CustomUIScrollView::setMultiTouchEnabled(bool enable)
{
    
    if (enable) this->getEventDispatcher()->removeEventListener(this->_touchListener);
    
    if (true)
    {
        _multiTouchListener = cocos2d::EventListenerTouchAllAtOnce::create();
        CC_SAFE_RETAIN(_multiTouchListener);
        _multiTouchListener->onTouchesBegan = CC_CALLBACK_2(CustomUIScrollView::onTouchesBegan, this);
        _multiTouchListener->onTouchesMoved = CC_CALLBACK_2(CustomUIScrollView::onTouchesMoved, this);
        _multiTouchListener->onTouchesEnded = CC_CALLBACK_2(CustomUIScrollView::onTouchesEnded, this);
        _multiTouchListener->onTouchesCancelled = CC_CALLBACK_2(CustomUIScrollView::onTouchesCancelled, this);
        // _eventDispatcher->addEventListenerWithSceneGraphPriority(_multiTouchListener, this);
        _eventDispatcher->addEventListenerWithFixedPriority(_multiTouchListener, 1);
    }
    else
    {
        /*
        _eventDispatcher->removeEventListener(_touchListener);
        CC_SAFE_RELEASE_NULL(_touchListener);
         */
    }
}
    
void CustomUIScrollView::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent)
{
    
    
    
    cocos2d::log("CustomUIScrollView::onTouchesBegan");
}

void CustomUIScrollView::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches,cocos2d::Event *pEvent)
{
    for (int i = 0; i < touches.size(); i++)
    {
        cocos2d::log("CustomUIScrollView::onTouchesMoved %d (%f, %f)", i, touches.at(i)->getLocation().x, touches.at(i)->getLocation().y);
    }
    
}

void CustomUIScrollView::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent)
{
    cocos2d::log("CustomUIScrollView::onTouchesEnded");
}

void CustomUIScrollView::onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent)
{
    cocos2d::log("CustomUIScrollView::onTouchesCancelled");
}

}
}
