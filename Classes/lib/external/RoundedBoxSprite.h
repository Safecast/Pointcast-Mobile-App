//
//  RoundedBoxSprite.h
//  ForTest
//
//  Created by lp6m on 2015/04/02.
//
//

#ifndef __ForTest__RoundedBoxSprite__
#define __ForTest__RoundedBoxSprite__

#include <stdio.h>
#include <cocos2d.h>
USING_NS_CC;
class RoundedBoxSprite : public cocos2d::Sprite
{
private:
    
public:
    void setParam(Size size,Color3B BGColor,float CornerRadius,int CornerSegment,std::string String,Color3B FontColor,int FontSize);
    void appendCubicBezier(int startPoint, std::vector<Vec2>& verts, const Vec2& from, const Vec2& control1, const Vec2& control2, const Vec2& to, uint32_t segments);
    Node* createRoundedRectMaskNode(Size size, float radius,int cornerSegments);
    CREATE_FUNC(RoundedBoxSprite);
};
#endif /* defined(__ForTest__RoundedBoxSprite__) */
