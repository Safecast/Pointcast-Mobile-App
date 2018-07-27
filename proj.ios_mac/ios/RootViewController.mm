/****************************************************************************
 Copyright (c) 2013      cocos2d-x.org
 Copyright (c) 2013-2016 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "RootViewController.h"
#import "cocos2d.h"
#import "platform/ios/CCEAGLView-ios.h"
#include "lib/Util.hpp"
#include "scene/layout/helper/Display.hpp"

@implementation RootViewController

    BOOL rotateEnable = false;
/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
    // Initialize the CCEAGLView
    CCEAGLView *eaglView = [CCEAGLView viewWithFrame: [UIScreen mainScreen].bounds
                                         pixelFormat: (__bridge NSString *)cocos2d::GLViewImpl::_pixelFormat
                                         depthFormat: cocos2d::GLViewImpl::_depthFormat
                                  preserveBackbuffer: NO
                                          sharegroup: nil
                                       multiSampling: NO
                                     numberOfSamples: 0 ];
    
    // Enable or disable multiple touches
    [eaglView setMultipleTouchEnabled:YES];
    
    // Set EAGLView as view of RootViewController
    self.view = eaglView;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
}

- (void)viewDidDisappear:(BOOL)animated {
    [super viewDidDisappear:animated];
}


// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger)supportedInterfaceOrientations {
#ifdef __IPHONE_6_0
    // return UIInterfaceOrientationMaskPortrait; //縦
    // return UIInterfaceOrientationMaskLandscapeLeft; 左
    // return UIInterfaceOrientationMaskLandscapeRight; 右
    // return UIInterfaceOrientationMaskLandscape; 左右
    // return UIInterfaceOrientationMaskPortraitUpsideDown; 縦（上下逆）
    return UIInterfaceOrientationMaskAll;  // 全方位
    // return UIInterfaceOrientationMaskAllButUpsideDown; 縦（上下逆）を除いた全方位
#endif
}

- (BOOL) shouldAutorotate {
    NSLog(@"rt: %d",rotateEnable);
    return rotateEnable;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];

    auto glview = cocos2d::Director::getInstance()->getOpenGLView();

    if (glview) {
        CCEAGLView *eaglview = (CCEAGLView *)glview->getEAGLView();
        
        if (eaglview) {
            cocos2d::Size drawing_area_size = scene::layout::helper::Display::GetDrawingArea();
            cocos2d::Application::getInstance()->applicationScreenSizeChanged(
                                                                              (int)drawing_area_size.width, (int)drawing_area_size.height);
            
            UIScreen *sc = [UIScreen mainScreen];
            CGRect rect = sc.bounds;
            NSLog(@"%.1f, %.1f", rect.size.width, rect.size.height);
            auto director = cocos2d::Director::getInstance();
            auto glview = director->getOpenGLView();
            glview->setFrameSize((int)drawing_area_size.width, (int)drawing_area_size.height);
            director->getOpenGLView()->setDesignResolutionSize(
                                                               (int)drawing_area_size.width, (int)drawing_area_size.height,
                                                               ResolutionPolicy::NO_BORDER);
        }
    }
    // push event notification
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("orientation");
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];

    // Release any cached data, images, etc that aren't in use.
}

- (void)setRotateEnable:(BOOL)flag {
    rotateEnable = flag;
}

- (BOOL)getRotateEnable {
    return rotateEnable;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:
(UIInterfaceOrientation)interfaceOrientation {
    return UIInterfaceOrientationIsLandscape(interfaceOrientation);
}

@end
