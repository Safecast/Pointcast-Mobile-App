#import <GoogleMaps/GoogleMaps.h>
#import <UIKit/UIKit.h>

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate> {
  UIWindow *window;
  GMSMapView *mapview;
}

+ (AppController *)getInstance;
- (void)attachMap;
- (void)detachMap;
- (void)attachMarker:(NSString *)info_title
        info_snippet:(NSString *)info_snippet
            latitude:(double)latitude
           longitude:(double)longitude
               color:(UIColor *)color
              zorder:(int)zorder;
+ (void)closeIme;

@property(nonatomic, readonly) RootViewController *viewController;

@end