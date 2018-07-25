//
//  WeatherItem.hpp
//  pointcast
//
//  Created by Mitsuo Okada on 2015/11/18.
//
//

#ifndef WeatherItem_hpp
#define WeatherItem_hpp

#include <stdio.h>

namespace lib {
namespace object {
struct WeatherItem {
   public:
    std::string weather_main;
    std::string icon;
    time_t timestamp;
    WeatherItem() {
        weather_main = "";
        icon = "";
        timestamp = 0;
    }
};
}
}

#endif /* WeatherItem_hpp */
