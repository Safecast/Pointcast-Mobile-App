/****************************************************************************
 Copyright (c) 2010-2013 cocos2d-x.org
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
#import <UIKit/UIKit.h>
#import <GoogleMaps/GoogleMaps.h>

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate, UIPickerViewDelegate, UIPickerViewDataSource, UITextFieldDelegate, GMSMapViewDelegate> {
    UIWindow *window;
    GMSMapView *mapview;
    UIPickerView* sortTypePickerView;
    UIToolbar* sortTypeSelectBar;
    UIToolbar* searchTextInputBar;
    UITextField *searchTextField;
}

+ (AppController *)getInstance;
- (void)attachMap:(double)latitude longitude:(double)longitude;
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
- (void)showSearchWordInputText;

@property(nonatomic, readonly) RootViewController *viewController;
@property (nonatomic, strong) NSArray *sortItems;
@property (nonatomic, strong) GMSMarker* selectedMarker;

@end

