#ifndef SpriteBlur_h
#define SpriteBlur_h

#include "cocos2d.h"

class SpriteBlur : public cocos2d::Sprite {
public:
  ~SpriteBlur();
  bool initWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect &rect);
  void initGLProgram();

  static SpriteBlur *create(const char *pszFileName);
  void setBlurRadius(float radius);
  void setBlurSampleNum(float num);

protected:
  float _blurRadius;
  float _blurSampleNum;
};

#endif