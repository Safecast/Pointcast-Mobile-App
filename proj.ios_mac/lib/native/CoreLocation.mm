//
//  CoreLocation.m
//
//  Created by franck on 4/12/13.
//
//

#include "CCCoreLocation.h"
#import "CoreLocation.h"
#include "CoreLocation_interface.h"

@implementation CoreLocation

static CoreLocationImpl *myinstanceT = NULL;

void CoreLocationImpl::requestLocation() {
   // [self requestLocation];
}

CoreLocationImpl *CoreLocationImpl::getInstance() {
  if (myinstanceT == NULL) {
    myinstanceT = new CoreLocationImpl();
  }
  return myinstanceT;
}

CoreLocationImpl::CoreLocationImpl(void) : self(NULL) {}

CoreLocationImpl::~CoreLocationImpl(void) {
  NSLog(@"WARNING CoreLocationImpl::~CoreLocationImpl");

  [(id)self dealloc];
}

bool CoreLocationImpl::init(void) {
  self = [[CoreLocation alloc] init];
  // [(id)self startStandardUpdates];
  return true;
}

void CoreLocationImpl::dealloc(void) { [(id)self dealloc]; }

- (void)dealloc {
  [locationManager release];
  [super dealloc];
}

- (id)init {
  if ((self = [super init]) != nil) {
    locationManager = nil;
  }
  return self;
}

- (void)startStandardUpdates {
  // Create the location manager if this object does not already have one
  if (locationManager == nil)
    locationManager = [[CLLocationManager alloc] init];
  if ([CLLocationManager locationServicesEnabled]) {
    [locationManager setDelegate:self];
    [locationManager setDesiredAccuracy:kCLLocationAccuracyBestForNavigation];
    [locationManager setDistanceFilter:kCLDistanceFilterNone];
    if (NSFoundationVersionNumber > NSFoundationVersionNumber_iOS_7_1) {
      [locationManager requestWhenInUseAuthorization];
    }
    [locationManager startUpdatingLocation];
  }
}

- (void)locationManager:(CLLocationManager *)manager
    didUpdateToLocation:(CLLocation *)newLocation
           fromLocation:(CLLocation *)oldLocation {
  if ([newLocation horizontalAccuracy] < 0)
    return;
  if (-[[newLocation timestamp] timeIntervalSinceNow] < 5.0)

    NSLog([NSString stringWithFormat:@"Altitude: %f Latitude: %f Longitude: %f"
                                     @" Course: %f Speed: %f",
                                     [newLocation altitude],
                                     [newLocation coordinate].latitude,
                                     [newLocation coordinate].longitude,
                                     [newLocation course],
                                     [newLocation speed]]);

  CCCoreLocation::getInstance()->getLocationDone(
      (float)[newLocation coordinate].latitude,
      (float)[newLocation coordinate].longitude);

  [locationManager stopUpdatingLocation];
}

- (void)locationManager:(CLLocationManager *)manager
     didUpdateLocations:(nonnull NSArray<CLLocation *> *)locations {
  CLLocation *clLocation = [locations lastObject];

  //  NSLog([NSString
  //      stringWithFormat:@"Altitude: %f Latitude: %f Longitude: %f"
  //                       @" Course: %f Speed: %f",
  //                       clLocation.altitude, clLocation.coordinate.latitude,
  //                       clLocation.coordinate.longitude clLocation.speed]);
  CCCoreLocation::getInstance()->getLocationDone(
      (float)clLocation.coordinate.latitude,
      (float)clLocation.coordinate.longitude);

  // [locationManager stopUpdatingLocation];
}

- (void)locationManager:(CLLocationManager *)manager
    didChangeAuthorizationStatus:(CLAuthorizationStatus)status {
  if (status == kCLAuthorizationStatusNotDetermined) {
    // ユーザが位置情報の使用を許可していない
    NSLog(@"kCLAuthorizationStatusNotDetermined");
  } else if (status == kCLAuthorizationStatusAuthorizedAlways) {
    // ユーザが位置情報の使用を常に許可している場合
    NSLog(@"kCLAuthorizationStatusAuthorizedAlways");
  } else if (status == kCLAuthorizationStatusAuthorizedWhenInUse) {
    // ユーザが位置情報の使用を使用中のみ許可している場合
    NSLog(@"kCLAuthorizationStatusAuthorizedWhenInUse");
  }
}

- (void)locationManager:(CLLocationManager *)manager
       didFailWithError:(NSError *)error {
  // 位置情報の取得に失敗した場合
  NSLog(@"%@", error);
}

- (void)requestLocation {
  if (locationManager == nil)
    locationManager = [[CLLocationManager alloc] init];
  locationManager.delegate = self;
  [locationManager requestWhenInUseAuthorization];
  [locationManager requestLocation];
}

@end
