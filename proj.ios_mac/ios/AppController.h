#import <GoogleMaps/GoogleMaps.h>
#import <UIKit/UIKit.h>

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate, UIPickerViewDelegate, UIPickerViewDataSource> {
  UIWindow *window;
  GMSMapView *mapview;
  UIPickerView* sortTypePickerView;
  UIToolbar* sortTypeSelectBar;
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
- (void)setRotateEnable:(BOOL)flag;
- (void)showSortPicker;


@property(nonatomic, readonly) RootViewController *viewController;
@property (nonatomic, strong) NSArray *sortItems;

@end
