/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org

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
#import <Firebase/Firebase.h>
#import "AppController.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#import "cocos2d.h"
#import "platform/ios/CCEAGLView-ios.h"

#define HEADER_HEIGHT 100
#define FOOTER_HEIGHT 100

@implementation AppController

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplicatio;

static AppController *_instance;

+ (AppController *)getInstance {
  return _instance;
}

- (RootViewController *)getRootViewController {
  return self.viewController;
}

- (BOOL)application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
  cocos2d::Application *app = cocos2d::Application::getInstance();
  app->initGLContextAttrs();
  cocos2d::GLViewImpl::convertAttrs();

  // Override point for customization after application launch.

  // Add the view controller's view to the window and display.
  window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];

  // Init the CCEAGLView
  CCEAGLView *eaglView =
      [CCEAGLView viewWithFrame:[window bounds]
                    pixelFormat:(NSString *)cocos2d::GLViewImpl::_pixelFormat
                    depthFormat:cocos2d::GLViewImpl::_depthFormat
             preserveBackbuffer:NO
                     sharegroup:nil
                  multiSampling:NO
                numberOfSamples:0];

  // Enable or disable multiple touches
  [eaglView setMultipleTouchEnabled:NO];

  // Use RootViewController manage CCEAGLView
  _viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
  _viewController.wantsFullScreenLayout = YES;
  _viewController.view = eaglView;

  // Set RootViewController to window
  if ([[UIDevice currentDevice].systemVersion floatValue] < 6.0) {
    // warning: addSubView doesn't work on iOS6
    [window addSubview:_viewController.view];
  } else {
    // use this method on ios6
    [window setRootViewController:_viewController];
  }

  [window makeKeyAndVisible];

  [[UIApplication sharedApplication] setStatusBarHidden:true];

  // IMPORTANT: Setting the GLView should be done after creating the
  // RootViewController
  cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView(eaglView);
  cocos2d::Director::getInstance()->setOpenGLView(glview);

  cocos2d::Director::getInstance()->setProjection(
      cocos2d::DisplayLinkDirector::Projection::_3D);

  [GMSServices provideAPIKey:@"AIzaSyBQXSpJyWV8nhjS5QVogEekt7k-cAaFQ-k"];

  _instance = self;

  // google firebase
  [FIRApp configure];
    
  app->run();

  return YES;
}

- (void)
  applicationWillResignActive:
      (UIApplication *)application{
          /*
           Sent when the application is about to move from active to inactive
           state.
           This can occur for certain types of temporary interruptions (such as
           an
           incoming phone call or SMS message) or when the user quits the
           application
           and it begins the transition to the background state.
           Use this method to pause ongoing tasks, disable timers, and throttle
           down
           OpenGL ES frame rates. Games should use this method to pause the
           game.
           */
          // We don't need to call this method any more. It will interupt user
          // defined
          // game pause&resume logic
          /* cocos2d::Director::getInstance()->pause(); */
      }

      - (void)
   applicationDidBecomeActive:
       (UIApplication *)application{
           /*
            Restart any tasks that were paused (or not yet started) while the
            application was inactive. If the application was previously in the
            background, optionally refresh the user interface.
            */
           // We don't need to call this method any more. It will interupt user
           // defined
           // game pause&resume logic
           /* cocos2d::Director::getInstance()->resume(); */
       }

       - (void)
applicationDidEnterBackground:(UIApplication *)application {
  /*
   Use this method to release shared resources, save user data, invalidate
   timers, and store enough application state information to restore your
   application to its current state in case it is terminated later.
   If your application supports background execution, called instead of
   applicationWillTerminate: when the user quits.
   */
  cocos2d::Application::getInstance()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
  /*
   Called as part of  transition from the background to the inactive
   state:
   here you can undo many of the changes made on entering the background.
   */
  cocos2d::Application::getInstance()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
  /*
   Called when the application is about to terminate.
   See also applicationDidEnterBackground:.
   */
}

#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:
    (UIApplication *)application{
        /*
         Free up as much memory as possible by purging cached data objects that
         can
         be recreated (or reloaded from disk) later.
         */
    }

    - (void)dealloc {
  [window release];
  [super dealloc];
}

- (NSUInteger)application:(UIApplication *)
    applicationsupportedInterfaceOrientationsForWindow:(UIWindow *)window {
  return UIInterfaceOrientationMaskAll;
}

- (CGRect)getMapSize {

  CGRect window_rect = self.viewController.view.bounds;
  CGRect map_rect;

  float height_scale = window_rect.size.height / 1136;
  float width = height_scale * 640;

  map_rect.size.width = width;
  map_rect.size.height = window_rect.size.height -
                         ((HEADER_HEIGHT + FOOTER_HEIGHT) * height_scale);
  map_rect.origin.x = ((window_rect.size.width - map_rect.size.width) / 2.0f);
  map_rect.origin.y = HEADER_HEIGHT * height_scale;

  return map_rect;
}

- (void)attachMap {
  // cocos2d::Director::getInstance()->pause();
  GMSCameraPosition *camera =
      [GMSCameraPosition cameraWithLatitude:35.39 longitude:140.0 zoom:8];

  CGRect map_rect = [self getMapSize];
  self->mapview = [GMSMapView
      mapWithFrame:CGRectMake(map_rect.origin.x, map_rect.origin.y,
                              map_rect.size.width, map_rect.size.height)
            camera:camera];
  self->mapview.myLocationEnabled = YES;

  [self.viewController.view addSubview:self->mapview];
  [self.viewController.view bringSubviewToFront:self->mapview];

  // overlay usv colors
  UIImage *img_scale = [UIImage imageNamed:@"res/scale64_60x854.png"];
  UIImage *img_scale_resize; // リサイズ後UIImage
  float widthPer = 0.5;      // リサイズ後幅の倍率
  float heightPer = 0.5;     // リサイズ後高さの倍率
  CGSize sz = CGSizeMake(img_scale.size.width * widthPer,
                         img_scale.size.height * heightPer);
  UIGraphicsBeginImageContext(sz);
  [img_scale drawInRect:CGRectMake(0, 0, sz.width, sz.height)];
  img_scale_resize = UIGraphicsGetImageFromCurrentImageContext();
  UIGraphicsEndImageContext();
  UIImageView *imgView = [[UIImageView alloc] initWithImage:img_scale_resize];
  imgView.frame = CGRectMake(0, 0, [img_scale_resize size].width,
                             [img_scale_resize size].height);
  [self->mapview addSubview:imgView];
  [self->mapview bringSubviewToFront:imgView];
}

- (void)detachMap {
  [self->mapview removeFromSuperview];
}

- (void)attachMarker:(NSString *)info_title
        info_snippet:(NSString *)info_snippet
            latitude:(double)latitude
           longitude:(double)longitude
               color:(UIColor *)color
              zorder:(int)zorder {
  GMSMarker *marker = [[GMSMarker alloc] init];
  marker.position = CLLocationCoordinate2DMake(latitude, longitude);
  marker.title = info_title;
  marker.snippet = info_snippet;
  marker.icon = [GMSMarker markerImageWithColor:color];
  marker.zIndex = zorder;
  marker.map = self->mapview;
}

+ (void)closeIme {
  AppController *app = [AppController getInstance];
  RootViewController *vc = [app getRootViewController];
  [vc.view resignFirstResponder];
}
@end
