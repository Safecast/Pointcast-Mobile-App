//
//  ChartItem.hpp
//  pointcast
//
//  Created by Leverages Mitsuo Okada on 2015/11/18.
//
//

#ifndef ChartItem_hpp
#define ChartItem_hpp

#include <stdio.h>

namespace lib {
namespace object {
struct ChartItem {
   public:
    int value;
    int major_label;
    int middle_label;
    int minor_label;
    time_t timestamp;
    ChartItem() {
        value = 0;
        major_label = 0;
        middle_label = 0;
        minor_label = 0;
        timestamp = 0;
    }
};
}
}

#endif /* ChartItem_hpp */
