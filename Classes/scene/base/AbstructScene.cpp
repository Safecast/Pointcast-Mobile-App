//
//  AbstructScene.cpp
//  pointcast
//
//  Created by Leverages Mitsuo Okada on 2015/10/29.
//
//

#include "AbstructScene.hpp"
#include "lib/external/SpriteBlur.h"
#include "scene/modal/Indicator.hpp"

USING_NS_CC;

namespace scene {
namespace base {
bool AbstructScene::Init() {
  if (!Layer::init()) {
    return false;
  }
  return true;
}

void AbstructScene::AttachWaitAnimation(void) {

  scene::modal::Indicator *p_indicator = static_cast<scene::modal::Indicator *>(
      this->getChildByTag(Tag_Id_Wait_Animation_e));
  if (p_indicator != NULL) {
    return;
  } else {
    p_indicator = scene::modal::Indicator::create();
    p_indicator->attachAnimation();
    p_indicator->setTag(Tag_Id_Wait_Animation_e);
    this->addChild(p_indicator, Zorders_WaitAnimation);
  }
}

void AbstructScene::DetachWaitAnimation(void) {
  CCLOG("disapper wait animation");
  auto p_indicator = static_cast<scene::modal::Indicator *>(
      this->getChildByTag(Tag_Id_Wait_Animation_e));
  if (p_indicator != NULL) {
    p_indicator->close();
  }
}

void AbstructScene::AttachTouchParticle(Point p) {
  CCLOG("attachTouchParticle");

  // 作成したパーティクルのプロパティリストを読み込み
  ParticleSystemQuad *p_particle =
      ParticleSystemQuad::create("res/particle/particle_texture.plist");
  // パーティクルを開始
  p_particle->resetSystem();
  // パーティクルを表示する場所の設定
  p_particle->setPosition(p);
  // パーティクルを配置
  p_particle->setTag(Tag_Id_Touch_Particle_e);
  this->addChild(p_particle);
}

void AbstructScene::DetachTouchParticle(void) {
  CCLOG("detachTouchParticle");
  auto p_particle = this->getChildByTag(Tag_Id_Touch_Particle_e);
  if (p_particle != NULL) {
    p_particle->removeFromParent();
  }
}

void AbstructScene::AttachBlueEffect(float blurRadius, float blurSampleNum,
                                     int repeat) {
  Size winSize = Director::getInstance()->getWinSize();
  // screenshot
  auto rt1 = RenderTexture::create(winSize.width, winSize.height,
                                   Texture2D::PixelFormat::RGBA8888);
  rt1->begin();
  this->visit();
  rt1->end();

  RenderTexture *rt = rt1;

  for (int i = 0; i < repeat; i++) {
    // SpriteBlur
    auto sp1 = new SpriteBlur();
    sp1->initWithTexture(rt->getSprite()->getTexture(),
                         Rect(Vec2::ZERO, winSize));
    sp1->setAnchorPoint(Vec2::ZERO);
    sp1->setPosition(Vec2::ZERO);
    sp1->setFlippedY(true);
    sp1->setBlurRadius(blurRadius);
    sp1->setBlurSampleNum(blurSampleNum);

    // render blurred sprite
    rt = RenderTexture::create(winSize.width, winSize.height,
                               Texture2D::PixelFormat::RGBA8888);
    rt->begin();
    sp1->visit();
    rt->end();

    delete sp1;
  }

  // final sprite
  auto sp2 = Sprite::createWithTexture(rt->getSprite()->getTexture());
  sp2->setFlippedY(true);
  sp2->setPosition(winSize / 1);
  sp2->setColor(Color3B(0x88, 0x88, 0x88));

  this->addChild(sp2, INT_MAX);
}

void AbstructScene::DetachlueEffect(void) {}
}
}
