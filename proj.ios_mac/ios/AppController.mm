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
#import "AppController.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#import "cocos2d.h"
#import "platform/ios/CCEAGLView-ios.h"
#import <Firebase/Firebase.h>
#import <UIKit/UIKit.h>

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
 
  //// Override point for customization after application launch.
  //ノーティフィケーション登録。
  [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(deviceOrientationDidChange:) name:UIDeviceOrientationDidChangeNotification object:nil];
    
  //通知開始
  [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
  [window makeKeyAndVisible];
    
  // set default rotate
  [_viewController setRotateEnable:NO];
    
    
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
    
  // Google Map Sdk
  [GMSServices provideAPIKey:@"AIzaSyBQXSpJyWV8nhjS5QVogEekt7k-cAaFQ-k"];

  // Notification
  if ([[[UIDevice currentDevice] systemVersion] compare:@"8.0.0" options:NSNumericSearch] != NSOrderedAscending) {
      // >= iOS 8.0.0
      UIUserNotificationType notificationTypes = UIUserNotificationTypeAlert | UIUserNotificationTypeBadge | UIUserNotificationTypeSound;
      UIUserNotificationSettings *settings = [UIUserNotificationSettings settingsForTypes:notificationTypes categories:nil];
      [application registerUserNotificationSettings:settings];
      [application registerForRemoteNotifications];
  } else {
        // < iOS 8.0.0
      UIRemoteNotificationType notificationTypes = (UIRemoteNotificationTypeAlert | UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound);
      [[UIApplication sharedApplication] registerForRemoteNotificationTypes:notificationTypes];
  }
    
  // Firebase
  [FIRApp configure];

  // Sort Picker
  [self initSortPicker];
    
  _instance = self;
    
  app->run();

  return YES;
}

- (void)deviceOrientationDidChange:(NSNotification*)notification {
    /*
    UIDeviceOrientation orientation;
    orientation = [UIDevice currentDevice].orientation;
    NSString *msg = nil;
    if(orientation == UIDeviceOrientationUnknown) {
        msg = @"不明";
    }
    if(orientation == UIDeviceOrientationPortrait) {
        msg = @"縦";
    }
    if(orientation == UIDeviceOrientationPortraitUpsideDown) {
        msg = @"縦（上下逆）";
    }
    if(orientation == UIDeviceOrientationLandscapeLeft) {
        msg = @"横（左側上）";
    }
    if(orientation == UIDeviceOrientationLandscapeRight) {
        msg = @"横（右側上）";
    }
    if(orientation == UIDeviceOrientationFaceDown) {
        msg = @"画面下向き"; 
    }
    // NSLog(msg);
     */
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

- (void)setRotateEnable:(BOOL)flag {
    AppController *app = [AppController getInstance];
    RootViewController *vc = [app getRootViewController];
    [vc setRotateEnable:flag];
}

- (void)application:(UIApplication *)application didRegisterUserNotificationSettings:(UIUserNotificationSettings *)notificationSettings {
    NSLog(@"application:didRegisterUserNotificationSettings: %@", notificationSettings.description);
}

-(void)application:(UIApplication*)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData*)deviceToken
{
    
    FIRInstanceIDAPNSTokenType token_type = FIRInstanceIDAPNSTokenTypeUnknown;
#if ENVIRONMENT == 101
    token_type = FIRInstanceIDAPNSTokenTypeProd;
#else
    token_type = FIRInstanceIDAPNSTokenTypeSandbox;
#endif
    [[FIRInstanceID instanceID] setAPNSToken:deviceToken type:token_type];
    NSLog(@"deviceToken1 = %@",deviceToken);
    
    // Firebase Subscribe
    [[FIRMessaging messaging] subscribeToTopic:@"/topics/all"];
}

-(void)application:(UIApplication*)application didFailToRegisterForRemoteNotificationsWithError:(NSError*)error
{
    NSLog(@"application:didFailToRegisterForRemoteNotificationsWithError: %@", error);
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler {
    // If you are receiving a notification message while your app is in the background,
    // this callback will not be fired till the user taps on the notification launching the application.
    // TODO: Handle data of notification

    // Print message ID.
    NSLog(@"Message ID: %@", userInfo[@"gcm.message_id"]);

    // Pring full message.
    NSLog(@"%@", userInfo);
}

- (void)initSortPicker
{
    self->_sortItems = [[NSArray alloc] initWithObjects:
                        @"Radiation Value",
                        @"Device ID",
                        @"Location Distance",
                        @"Sensor Status",
                        nil];
}

- (void)hideSortPicker
{
    [sortTypePickerView removeFromSuperview];
    [sortTypeSelectBar removeFromSuperview];
}

- (void)showSortPicker
{
    
    sortTypePickerView =
    [[UIPickerView alloc] initWithFrame:CGRectMake(0,
                                                   0,
                                                   0,
                                                   0)];
    sortTypePickerView.backgroundColor = [UIColor whiteColor];
    sortTypePickerView.delegate = self;
    sortTypePickerView.showsSelectionIndicator = YES;
    sortTypePickerView.layer.masksToBounds = true;
    sortTypePickerView.layer.borderWidth = 1;
    sortTypePickerView.layer.borderColor = [UIColor blackColor].CGColor;
    sortTypePickerView.layer.cornerRadius = 8.0f;
    [sortTypePickerView selectRow:0 inComponent:0 animated:YES];
    
    sortTypeSelectBar = [[UIToolbar alloc] init];
    sortTypeSelectBar.backgroundColor = [UIColor whiteColor];
    sortTypeSelectBar.translucent = NO;
    sortTypeSelectBar.barTintColor =[UIColor whiteColor];
    
    sortTypeSelectBar.frame=CGRectMake(0,0,320,30);
    sortTypeSelectBar.barStyle = UIBarStyleBlackTranslucent;
    sortTypeSelectBar.autoresizingMask = UIViewAutoresizingFlexibleHeight;
    
    UILabel *lblTitle = [[UILabel alloc] initWithFrame:CGRectMake(50, 50, 150, 20)];
    lblTitle.backgroundColor = [UIColor clearColor];
    lblTitle.textColor = [UIColor blackColor];
    lblTitle.textAlignment = NSTextAlignmentLeft;
    lblTitle.text = @"Which is sort ?";
    
    UIBarButtonItem *toolBarTitle = [[UIBarButtonItem alloc] initWithCustomView:lblTitle];
    
    sortTypeSelectBar.items = [NSArray arrayWithObjects:
                     [[UIBarButtonItem alloc]initWithTitle:@"Cancel" style:UIBarButtonItemStyleBordered target:self action:@selector(cancelPickSort:)],
                           [[UIBarButtonItem alloc]initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace target:nil action:nil],toolBarTitle,
                               
                     [[UIBarButtonItem alloc]initWithTitle:@"Done" style:UIBarButtonItemStyleDone target:self action:@selector(donePickSort:)],
                           nil];

    
    [self.viewController.view addSubview:sortTypePickerView];
    [self.viewController.view addSubview:sortTypeSelectBar];
    
    return;
}

// デリゲートメソッドの実装
// 列数を返す例
-(NSInteger)numberOfComponentsInPickerView:(UIPickerView*)pickerView{
    return 1; //列数は２つ
}

// 行数を返す例
-(NSInteger)pickerView:(UIPickerView*)pickerView
numberOfRowsInComponent:(NSInteger)component{
    
    return [self->_sortItems count];
    
}

// 表示する内容を返す例
-(NSString*)pickerView:(UIPickerView*)pickerView
           titleForRow:(NSInteger)row forComponent:(NSInteger)component{
    
    // 行インデックス番号を返す
    return [NSString stringWithFormat:@"%@", self->_sortItems[row]];
}

//選択されたピッカービューを取得
- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
    //0列目の選択している行番号を取得
    NSInteger selectedRow = [pickerView selectedRowInComponent:0];
    NSLog(@"%d", selectedRow);
}

- (void)donePickSort:(id)sender
{
    NSLog(@"%s", "donePickSort");
    
    [self hideSortPicker];
    
    NSInteger select_sort_type = [sortTypePickerView selectedRowInComponent:0];
    
    cocos2d::EventCustom customEvent("select_sort_type");
    auto value = cocos2d::Value((int)select_sort_type);
    customEvent.setUserData(&value);
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(
                                                                          &customEvent);

}

- (void)cancelPickSort:(id)sender
{
    NSLog(@"%s", "cancelPickSort");
    [self hideSortPicker];
}

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer
       shouldReceiveTouch:(UITouch *)touch
{

}

@end
