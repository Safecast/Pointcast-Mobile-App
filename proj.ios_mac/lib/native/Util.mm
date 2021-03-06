//
//  Util.mm
//  pointcast
//
//  Created by Mitsuo Okada on 2015/12/08.
//
//

#import "AppController.h"
#import "platform/ios/CCEAGLView-ios.h"
#include "lib/native/Util.h"

namespace lib {

namespace native {
    


void Util::closeIme() { [AppController closeIme]; }

void Util::setRotateEnable(bool flag) {
    AppController *app = [AppController getInstance];
    [app setRotateEnable:flag];
}
    
    
cocos2d::Size Util::getDisplaySize()
{
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    
    CCEAGLView *eaglview = (CCEAGLView *)glview->getEAGLView();
    cocos2d::Size size = cocos2d::Size([eaglview getWidth], [eaglview getHeight]);
    return size;
}
    
void Util::changeRotate(int type)
{
    NSNumber *value = [[NSNumber alloc] initWithInt:type];
    [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
}

void Util::showSortPicker()
{
    AppController *app = [AppController getInstance];
    [app showSortPicker];
}

void Util::showSearchWordInputText()
{
    AppController *app = [AppController getInstance];
    [app showSearchWordInputText];
}
    
}
    
}
