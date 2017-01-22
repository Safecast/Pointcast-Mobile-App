/*
 * CCLayerPanZoom
 *
 * Copyright (c) 2011 Alexey Lang
 * Copyright (c) 2011 Pavel Guganov
 *
 * http://www.cocos2d-x.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "CCLayerPanZoom.h"


USING_NS_CC;

void CCLayerPanZoom::setMaxScale(float maxScale)
{
    _maxScale = maxScale;
    this->setScale(1.0f);
    this->setScale(MIN(this->getScale(), _maxScale));

}

float CCLayerPanZoom::maxScale()
{
    return _maxScale;
}

void CCLayerPanZoom::setMinScale(float minScale)
{
    _minScale = minScale;
    this->setScale(MAX(this->getScale(), minScale));
}

float CCLayerPanZoom::minScale()
{
    return _minScale;
}

void CCLayerPanZoom::setRubberEffectRatio(float rubberEffectRatio)
{
    _rubberEffectRatio = rubberEffectRatio;
    
    // Avoid turning rubber effect On in frame mode.
    if (_mode == kCCLayerPanZoomModeFrame)
    {
        // CCLOGERROR(@"CCLayerPanZoom#setRubberEffectRatio: rubber effect is not supported in frame mode.");
        _rubberEffectRatio = 0.0f;
    }
    
}

float CCLayerPanZoom::rubberEffectRatio()
{
    return _rubberEffectRatio;
}


CCLayerPanZoom* CCLayerPanZoom::layer()
{
	// 'layer' is an autorelease object
	CCLayerPanZoom *layer = CCLayerPanZoom::create();
    
    // return the scene
	return layer;
}

#pragma mark Init
// on "init" you need to initialize your instance
bool CCLayerPanZoom::init()
{
    // 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}
    
    // m_bIsRelativeAnchorPoint = true;
    // m_bIsTouchEnabled = true;
    
    _maxScale = 3.0f;
    _minScale = 0.7f;
    //_touches = cocos2d::Vector<Touch>;
    //_touches->retain();
    
    _panBoundsRect = CCRectZero;
    _touchDistance = 0.0F;
    _maxTouchDistanceToClick = 315.0f;
    
    _mode = kCCLayerPanZoomModeSheet;
    _minSpeed = 100.0f;
    _maxSpeed = 1000.0f;
    _topFrameMargin = 100.0f;
    _bottomFrameMargin = 100.0f;
    _leftFrameMargin = 100.0f;
    _rightFrameMargin = 100.0f;
    
    _rubberEffectRatio = 0.0f;
    _rubberEffectRecoveryTime = 0.2f;
    _rubberEffectRecovering = false;
    _rubberEffectZooming = false;
    
    return true;
}

#pragma mark CCStandardTouchDelegate Touch events
bool CCLayerPanZoom::onTouchBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent)
{
    Touch *pTouch;
    SetIterator setIter;
    
    for (int i = 0; i < touches.size(); i++)
    {
        pTouch = (Touch *)(*setIter);
        _touches.pushBack(pTouch);
    }
    
    if (_touches.size() == 1)
    {
        _touchMoveBegan = false;
        time_t seconds;
        
        seconds = time (NULL);
        _singleTouchTimestamp = seconds/60;
    }
    else
        _singleTouchTimestamp = INFINITY;
    
    return false;
}

void CCLayerPanZoom::onTouchMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent)
{
    bool multitouch = _touches.size() > 1;
	if (multitouch)
	{
        
        // Get the two first touches
        Touch *touch1 = (Touch*)_touches.at(0);
        Touch *touch2 = (Touch*)_touches.at(1);
		// Get current and previous positions of the touches
        Point curPosTouch1 = Director::getInstance()->convertToGL(touch1->getLocationInView());
        Point curPosTouch2 = Director::getInstance()->convertToGL(touch2->getLocationInView());
        
        Point prevPosTouch1 = Director::getInstance()->convertToGL(touch1->getLocationInView());
        Point prevPosTouch2 = Director::getInstance()->convertToGL(touch2->getLocationInView());
        
		// Calculate current and previous positions of the layer relative the anchor point
		Point curPosLayer = ccpMidpoint(curPosTouch1, curPosTouch2);
		Point prevPosLayer = ccpMidpoint(prevPosTouch1, prevPosTouch2);
        
		// Calculate new scale
        float prevScale = this->getScale();
        
        
        float curScale = this->getScale() * ccpDistance(curPosTouch1, curPosTouch2) / ccpDistance(prevPosTouch1, prevPosTouch2);

        this->setScale( curScale);
        // Avoid scaling out from panBoundsRect when Rubber Effect is OFF.
        if (!_rubberEffectRatio)
        {
            this->setScale( MAX(this->getScale(), this->minPossibleScale())); 
        }
        // If scale was changed -> set new scale and fix position with new scale
        if (this->getScale() != prevScale)
        {
            if (_rubberEffectRatio)
            {
                _rubberEffectZooming = true;
            }
            Point realCurPosLayer = this->convertToNodeSpace(curPosLayer);
            float deltaX = (realCurPosLayer.x - this->getAnchorPoint().x * this->getContentSize().width) * (this->getScale() - prevScale);
            float deltaY = (realCurPosLayer.y - this->getAnchorPoint().y * this->getContentSize().height) * (this->getScale() - prevScale);
            this->setPosition(Vec2(this->getPosition().x - deltaX, this->getPosition().y - deltaY));
            _rubberEffectZooming = false;
        }
        // If current and previous position of the multitouch's center aren't equal -> change position of the layer
		if (!(prevPosLayer == curPosLayer))
		{            
            this->setPosition(Vec2(this->getPosition().x + curPosLayer.x - prevPosLayer.x,
                                this->getPosition().y + curPosLayer.y - prevPosLayer.y));
        }
        // Don't click with multitouch
		_touchDistance = INFINITY;
	}
	else
	{	        
        // Get the single touch and it's previous & current position.
        Touch *touch = (Touch*)_touches.at(0);
        Point curTouchPosition = Director::getInstance()->convertToGL(touch->getLocationInView());
        Point prevTouchPosition = CCDirector::sharedDirector()->convertToGL(touch->getPreviousLocationInView());
        
        // Always scroll in sheet mode.
        if (_mode == kCCLayerPanZoomModeSheet)
        {
            // Set new position of the layer.
            this->setPosition(ccp(this->getPosition().x + curTouchPosition.x - prevTouchPosition.x,
                                this->getPosition().y + curTouchPosition.y - prevTouchPosition.y));
        }
        
        // Accumulate touch distance for all modes.
        _touchDistance += ccpDistance(curTouchPosition, prevTouchPosition);
        
        // Inform delegate about starting updating touch position, if click isn't possible.
        if (_mode == kCCLayerPanZoomModeFrame)
        {
            if (_touchDistance > _maxTouchDistanceToClick && !_touchMoveBegan)
            {
                //ToDo add delegate here
                //[self.delegate layerPanZoom: self 
                 //  touchMoveBeganAtPosition: [self convertToNodeSpace: prevTouchPosition]];
                _touchMoveBegan = true;
            }
        }
    }	
}

void CCLayerPanZoom::onTouchEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent)
{
    _singleTouchTimestamp = INFINITY;
    
    // Process click event in single touch.
    //ToDo add delegate
    if (  (_touchDistance < _maxTouchDistanceToClick) /*&& (self.delegate) */
        && (_touches.size() == 1))
    {
        Touch *touch = (Touch*)_touches.at(0);
        Point curPos = Director::getInstance()->convertToGL(touch->getLocationInView());
        //ToDo add delegate
        /*[self.delegate layerPanZoom: self
                     clickedAtPoint: [self convertToNodeSpace: curPos]
                           tapCount: [touch tapCount]];*/
    }
    
    Touch *pTouch;
    SetIterator setIter;
    
    for (int i = 0; i < touches.size(); i++)
    {
        pTouch = (Touch *)(*setIter);
        _touches.eraseObject(pTouch);
    }
    
	if (_touches.size() == 0)
	{
		_touchDistance = 0.0f;
	}
    
    if (!_touches.size() && !_rubberEffectRecovering)
    {
        this->recoverPositionAndScale();
    }
}

void CCLayerPanZoom::onTouchCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *pEvent)
{
    
    Touch *pTouch;
    SetIterator setIter;
    
    for (int i = 0; i < touches.size(); i++)
    {
        pTouch = (Touch *)(*setIter);
        _touches.eraseObject(pTouch);
    }
    
	if (_touches.size() == 0)
	{
		_touchDistance = 0.0f;
	}
}
#pragma mark Update

// Updates position in frame mode.
void  CCLayerPanZoom::update(float dt){
    // Only for frame mode with one touch.
	if ( _mode == kCCLayerPanZoomModeFrame && _touches.size() == 1 )
    {
        // Do not update position if click is still possible.
        if (_touchDistance <= _maxTouchDistanceToClick)
            return;
        
        // Do not update position if pinch is still possible.
        time_t seconds;
        
        seconds = time (NULL);
        seconds /= 60;
        if (seconds - _singleTouchTimestamp < kCCLayerPanZoomMultitouchGesturesDetectionDelay)
            return;
        
        // Otherwise - update touch position. Get current position of touch.
        Touch *touch = (Touch*)_touches.at(0);
        Point curPos = Director::getInstance()->convertToGL(touch->getLocationInView());
        
        // Scroll if finger in the scroll area near edge.
        if (this->frameEdgeWithPoint(curPos) != kCCLayerPanZoomFrameEdgeNone)
        {
            this->setPosition(Vec2(this->getPosition().x + dt * this->horSpeedWithPosition( curPos),
                                this->getPosition().y + dt * this->vertSpeedWithPosition(curPos)));
        }
        
        // Inform delegate if touch position in layer was changed due to finger or layer movement.
        CCPoint touchPositionInLayer = this->convertToNodeSpace(curPos);
        if (!(_prevSingleTouchPositionInLayer == touchPositionInLayer))
        {
            _prevSingleTouchPositionInLayer = touchPositionInLayer;
            //ToDo add delegate
            //[self.delegate layerPanZoom: self 
             //      touchPositionUpdated: touchPositionInLayer];
        }
        
    }
}

void  CCLayerPanZoom::onEnter(){
    Layer::onEnter();
    // Scheduler::sharedScheduler()->scheduleUpdateForTarget(this, 0, false);
    this->scheduleUpdate();
}

void  CCLayerPanZoom::onExit(){
  // Scheduler::sharedScheduler()->unscheduleAllSelectorsForTarget(this);
  this->unscheduleUpdate();
  Layer::onExit();
}

#pragma mark Scale and Position related

void CCLayerPanZoom::setPanBoundsRect(Rect rect){
	_panBoundsRect = rect;
    this->setScale(this->minPossibleScale());
    this->setPosition(this->getPosition());
}

void CCLayerPanZoom::setPosition(Point  position){
    Point prevPosition = this->getPosition();
    //May be it wouldnot work
    Node::setPosition(position);
    
    if (!(_panBoundsRect.equals(Rect::ZERO)) && !_rubberEffectZooming)
    {
        if (_rubberEffectRatio && _mode == kCCLayerPanZoomModeSheet)
        {
            if (!_rubberEffectRecovering)
            {
                float topDistance = this->topEdgeDistance();
                float bottomDistance = this->bottomEdgeDistance();
                float leftDistance = this->leftEdgeDistance();
                float rightDistance = this->rightEdgeDistance();
                float dx = this->getPosition().x - prevPosition.x;
                float dy = this->getPosition().y - prevPosition.y;
                if (bottomDistance || topDistance)
                {
                    Node::setPosition(ccp(this->getPosition().x,
                                            prevPosition.y + dy * _rubberEffectRatio));                    
                }
                if (leftDistance || rightDistance)
                {
                    Node::setPosition(ccp(prevPosition.x + dx * _rubberEffectRatio,
                                            this->getPosition().y));                    
                }
            }
        }
        else
        {
            Rect boundBox = this->boundingBox();
            if (this->getPosition().x - boundBox.size.width * this->getAnchorPoint().x > _panBoundsRect.origin.x)
            {
                Node::setPosition(ccp(boundBox.size.width * this->getAnchorPoint().x + _panBoundsRect.origin.x,
                                        this->getPosition().y));
            }	
            if (this->getPosition().y - boundBox.size.height * this->getAnchorPoint().y > _panBoundsRect.origin.y)
            {
                Node::setPosition(ccp(this->getPosition().x, boundBox.size.height * this->getAnchorPoint().y +
                                        _panBoundsRect.origin.y));
            }
            if (this->getPosition().x + boundBox.size.width * (1 - this->getAnchorPoint().x) < _panBoundsRect.size.width +
                _panBoundsRect.origin.x)
            {
                Node::setPosition(ccp(_panBoundsRect.size.width + _panBoundsRect.origin.x -
                                        boundBox.size.width * (1 - this->getAnchorPoint().x), this->getPosition().y));
            }
            if (this->getPosition().y + boundBox.size.height * (1 - this->getAnchorPoint().y) < _panBoundsRect.size.height + 
                _panBoundsRect.origin.y)
            {
                Node::setPosition(ccp(this->getPosition().x, _panBoundsRect.size.height + _panBoundsRect.origin.y -
                                        boundBox.size.height * (1 - this->getAnchorPoint().y)));
            }	
        }
    }
}

void CCLayerPanZoom::setScale(float scale){
    Layer::setScale( MIN(MAX(scale, _minScale), _maxScale));
}

#pragma mark Ruber Edges related

void CCLayerPanZoom::recoverPositionAndScale(){
    if (!(_panBoundsRect.equals(Rect::ZERO)))
	{    
        Size winSize = Director::getInstance()->getWinSize();
        float rightEdgeDistance = this->rightEdgeDistance();
        float leftEdgeDistance = this->leftEdgeDistance();
        float topEdgeDistance = this->topEdgeDistance();
        float bottomEdgeDistance = this->bottomEdgeDistance();
        float scale = this->minPossibleScale();
        
        if (!rightEdgeDistance && !leftEdgeDistance && !topEdgeDistance && !bottomEdgeDistance)
        {
            return;
        }
        
        if (this->getScale() < scale)
        {
            _rubberEffectRecovering = true;
            Point newPosition = Vec2();
            if (rightEdgeDistance && leftEdgeDistance && topEdgeDistance && bottomEdgeDistance)
            {
                float dx = scale * this->getContentSize().width * (this->getAnchorPoint().x - 0.5f);
                float dy = scale * this->getContentSize().height * (this->getAnchorPoint().y - 0.5f);
                newPosition = Vec2(winSize.width * 0.5f + dx, winSize.height * 0.5f + dy);
            }
            else if (rightEdgeDistance && leftEdgeDistance && topEdgeDistance)
            {
                float dx = scale * this->getContentSize().width * (this->getAnchorPoint().x - 0.5f);
                float dy = scale * this->getContentSize().height * (1.0f - this->getAnchorPoint().y);            
                newPosition = Vec2(winSize.width * 0.5f + dx, winSize.height - dy);
            }
            else if (rightEdgeDistance && leftEdgeDistance && bottomEdgeDistance)
            {
                float dx = scale * this->getContentSize().width * (this->getAnchorPoint().x - 0.5f);
                float dy = scale * this->getContentSize().height * this->getAnchorPoint().y;            
                newPosition = Vec2(winSize.width * 0.5f + dx, dy);
            }
            else if (rightEdgeDistance && topEdgeDistance && bottomEdgeDistance)
            {
                float dx = scale * this->getContentSize().width * (1.0f - this->getAnchorPoint().x);
                float dy = scale * this->getContentSize().height * (this->getAnchorPoint().y - 0.5f);            
                newPosition = Vec2(winSize.width  - dx, winSize.height  * 0.5f + dy);
            }
            else if (leftEdgeDistance && topEdgeDistance && bottomEdgeDistance)
            {
                float dx = scale * this->getContentSize().width * this->getAnchorPoint().x;
                float dy = scale * this->getContentSize().height * (this->getAnchorPoint().y - 0.5f);            
                newPosition = Vec2(dx, winSize.height * 0.5f + dy);
            }
            else if (leftEdgeDistance && topEdgeDistance)
            {
                float dx = scale * this->getContentSize().width * this->getAnchorPoint().x;
                float dy = scale * this->getContentSize().height * (1.0f - this->getAnchorPoint().y);            
                newPosition = Vec2(dx, winSize.height - dy);
            } 
            else if (leftEdgeDistance && bottomEdgeDistance)
            {
                float dx = scale * this->getContentSize().width * this->getAnchorPoint().x;
                float dy = scale * this->getContentSize().height * this->getAnchorPoint().y;            
                newPosition = Vec2(dx, dy);
            } 
            else if (rightEdgeDistance && topEdgeDistance)
            {
                float dx = scale * this->getContentSize().width * (1.0f - this->getAnchorPoint().x);
                float dy = scale * this->getContentSize().height * (1.0f - this->getAnchorPoint().y);            
                newPosition = Vec2(winSize.width - dx, winSize.height - dy);
            } 
            else if (rightEdgeDistance && bottomEdgeDistance)
            {
                float dx = scale * this->getContentSize().width * (1.0f - this->getAnchorPoint().x);
                float dy = scale * this->getContentSize().height * this->getAnchorPoint().y;            
                newPosition = Vec2(winSize.width - dx, dy);
            } 
            else if (topEdgeDistance || bottomEdgeDistance)
            {
                float dy = scale * this->getContentSize().height * (this->getAnchorPoint().y - 0.5f);            
                newPosition = Vec2(this->getPosition().x, winSize.height * 0.5f + dy);
            }
            else if (leftEdgeDistance || rightEdgeDistance)
            {
                float dx = scale * this->getContentSize().width * (this->getAnchorPoint().x - 0.5f);
                newPosition = Vec2(winSize.width * 0.5f + dx, this->getPosition().y);
            } 
            
            MoveTo *moveToPosition = MoveTo::create(_rubberEffectRecoveryTime, newPosition);
            ScaleTo *scaleToPosition = ScaleTo::create( _rubberEffectRecoveryTime, scale);
            
            cocos2d::CallFunc *funcRecoverEnded=
                CallFunc::create(CC_CALLBACK_0(CCLayerPanZoom::recoverEnded, this));
            
            FiniteTimeAction *sequence = Spawn::create(scaleToPosition, moveToPosition, funcRecoverEnded, nullptr);
            this->runAction(sequence);
            
        }
        else
        {
            _rubberEffectRecovering = false;
            MoveTo *moveToPosition = MoveTo::create(_rubberEffectRecoveryTime,Vec2(this->getPosition().x + rightEdgeDistance - leftEdgeDistance,
                                                                  this->getPosition().y + topEdgeDistance - bottomEdgeDistance));
            
            cocos2d::CallFunc *funcRecoverEnded=
                CallFunc::create(CC_CALLBACK_0(CCLayerPanZoom::recoverEnded, this));
            
            FiniteTimeAction *sequence = Spawn::create(moveToPosition,funcRecoverEnded, NULL);
            this->runAction(sequence);
            
        }
	}
}

void CCLayerPanZoom::recoverEnded(){
    _rubberEffectRecovering = false;
}


#pragma mark Helpers

float CCLayerPanZoom::topEdgeDistance(){
    Rect boundBox = this->getBoundingBox();
    return round(MAX(_panBoundsRect.size.height + _panBoundsRect.origin.y - this->getPosition().y - 
                     boundBox.size.height * (1 - this->getAnchorPoint().y), 0));
}

float CCLayerPanZoom::leftEdgeDistance(){
    Rect boundBox = this->getBoundingBox();
    return round(MAX(this->getPosition().x - boundBox.size.width * this->getAnchorPoint().x - _panBoundsRect.origin.x, 0));
}    

float CCLayerPanZoom::bottomEdgeDistance(){
    Rect boundBox = this->getBoundingBox();
    return round(MAX(this->getPosition().y - boundBox.size.height * this->getAnchorPoint().y - _panBoundsRect.origin.y, 0));
}

float CCLayerPanZoom::rightEdgeDistance(){
    Rect boundBox = this->getBoundingBox();
    return round(MAX(_panBoundsRect.size.width + _panBoundsRect.origin.x - this->getPosition().x - 
                     boundBox.size.width * (1 - this->getAnchorPoint().x), 0));
}

float CCLayerPanZoom::minPossibleScale(){
    if (!_panBoundsRect.equals(Rect::ZERO))
	{
		return MAX(_panBoundsRect.size.width / this->getContentSize().width,
				   _panBoundsRect.size.height /this->getContentSize().height);
	}
	else 
	{
		return _minScale;
	}
}

CCLayerPanZoomFrameEdge CCLayerPanZoom::frameEdgeWithPoint( Point point){
    bool isLeft = point.x <= _panBoundsRect.origin.x + _leftFrameMargin;
    bool isRight = point.x >= _panBoundsRect.origin.x + _panBoundsRect.size.width - _rightFrameMargin;
    bool isBottom = point.y <= _panBoundsRect.origin.y + _bottomFrameMargin;
    bool isTop = point.y >= _panBoundsRect.origin.y + _panBoundsRect.size.height - _topFrameMargin;
    
    if (isLeft && isBottom)
    {
        return kCCLayerPanZoomFrameEdgeBottomLeft;
    }
    if (isLeft && isTop)
    {
        return kCCLayerPanZoomFrameEdgeTopLeft;
    }
    if (isRight && isBottom)
    {
        return kCCLayerPanZoomFrameEdgeBottomRight;
    }
    if (isRight && isTop)
    {
        return kCCLayerPanZoomFrameEdgeTopRight;
    }
    
    if (isLeft)
    {
        return kCCLayerPanZoomFrameEdgeLeft;
    }
    if (isTop)
    {
        return kCCLayerPanZoomFrameEdgeTop;
    }
    if (isRight)
    {
        return kCCLayerPanZoomFrameEdgeRight;
    }
    if (isBottom)
    {
        return kCCLayerPanZoomFrameEdgeBottom;
    }
    
    return kCCLayerPanZoomFrameEdgeNone;
}

float CCLayerPanZoom::horSpeedWithPosition(Point pos){
    CCLayerPanZoomFrameEdge edge = this->frameEdgeWithPoint(pos);
    float speed = 0.0f;
    if (edge == kCCLayerPanZoomFrameEdgeLeft)
    {
        speed = _minSpeed + (_maxSpeed - _minSpeed) * 
        (_panBoundsRect.origin.x + _leftFrameMargin - pos.x) / _leftFrameMargin;
    }
    if (edge == kCCLayerPanZoomFrameEdgeBottomLeft || edge == kCCLayerPanZoomFrameEdgeTopLeft)
    {
        speed = _minSpeed + (_maxSpeed - _minSpeed) * 
        (_panBoundsRect.origin.x + _leftFrameMargin - pos.x) / (_leftFrameMargin * sqrt(2.0f));
    }
    if (edge == kCCLayerPanZoomFrameEdgeRight)
    {
        speed = - (_minSpeed + (_maxSpeed - _minSpeed) * 
                   (pos.x - _panBoundsRect.origin.x - _panBoundsRect.size.width + 
                    _rightFrameMargin) / _rightFrameMargin);
    }
    if (edge == kCCLayerPanZoomFrameEdgeBottomRight || edge == kCCLayerPanZoomFrameEdgeTopRight)
    {
        speed = - (_minSpeed + (_maxSpeed - _minSpeed) * 
                   (pos.x - _panBoundsRect.origin.x - _panBoundsRect.size.width + 
                    _rightFrameMargin) / (_rightFrameMargin * sqrt(2.0f)));
    }
    return speed;
}

float CCLayerPanZoom::vertSpeedWithPosition(Point pos){
    CCLayerPanZoomFrameEdge edge = this->frameEdgeWithPoint(pos);
    float speed = 0.0f;
    if (edge == kCCLayerPanZoomFrameEdgeBottom)
    {
        speed = _minSpeed + (_maxSpeed - _minSpeed) * 
        (_panBoundsRect.origin.y + _bottomFrameMargin - pos.y) / _bottomFrameMargin;
    }
    if (edge == kCCLayerPanZoomFrameEdgeBottomLeft || edge == kCCLayerPanZoomFrameEdgeBottomRight)
    {
        speed = _minSpeed + (_maxSpeed - _minSpeed) * 
        (_panBoundsRect.origin.y + _bottomFrameMargin - pos.y) / (_bottomFrameMargin * sqrt(2.0f));
    }
    if (edge == kCCLayerPanZoomFrameEdgeTop)
    {
        speed = - (_minSpeed + (_maxSpeed - _minSpeed) * 
                   (pos.y - _panBoundsRect.origin.y - _panBoundsRect.size.height + 
                    _topFrameMargin) / _topFrameMargin);
    }
    if (edge == kCCLayerPanZoomFrameEdgeTopLeft || edge == kCCLayerPanZoomFrameEdgeTopRight)
    {
        speed = - (_minSpeed + (_maxSpeed - _minSpeed) * 
                   (pos.y - _panBoundsRect.origin.y - _panBoundsRect.size.height + 
                    _topFrameMargin) / (_topFrameMargin * sqrt(2.0f)));
    }
    return speed;
} 



