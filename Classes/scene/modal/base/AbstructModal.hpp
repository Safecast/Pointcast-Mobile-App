//
//  AbstructModal.hpp
//  pointcast
//
//  Created by Mitsuo Okada on 2015/11/07.
//
//

#ifndef AbstructModal_hpp
#define AbstructModal_hpp

#include <stdio.h>

#include "cocos2d.h"

namespace scene {

namespace modal {

namespace base {

USING_NS_CC;

class AbstructModal : public Layer {
   public:
    static AbstructModal* create();
    virtual bool init();

    //ダイアログ削除処理
    void close();

    //タッチ・ダウン処理
    virtual bool onTouchBegan(Touch* touch, Event* event);

    //タッチ・リリース処理
    virtual void onTouchEnded(Touch* touch, Event* event);

   protected:
    //ダイアログ背景用Node
    Node* _background;

    //ダイアログ外の透過カラーレイヤー
    LayerColor* _colorSheet;

    //タッチイベント登録
    void _setupEventListener();
};
}
}
}

#endif /* AbstructModal_hpp */
