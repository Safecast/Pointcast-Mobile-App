//
//  Util.mm
//  pointcast
//
//  Created by Leverages Mitsuo Okada on 2015/12/08.
//
//

#import "AppController.h"
#import "platform/ios/CCEAGLView-ios.h"
#include "lib/native/Util.h"

namespace lib {

namespace native {
    


void Util::CloseIme() { [AppController closeIme]; }

void Util::SetRotateEnable(bool flag) {
    AppController *app = [AppController getInstance];
    [app setRotateEnable:flag];
}
    
    
cocos2d::Size Util::GetDisplaySize()
{
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    
    CCEAGLView *eaglview = (CCEAGLView *)glview->getEAGLView();
    cocos2d::Size size = cocos2d::Size([eaglview getWidth], [eaglview getHeight]);
    return size;
}
    
void Util::ChangeRotate(int type)
{
    NSNumber *value = [[NSNumber alloc] initWithInt:type];
    [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
}

}
    
}
