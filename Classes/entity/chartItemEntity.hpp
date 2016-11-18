//
//  chartItemEntity.hpp
//  Pointcast-Mobile-App
//
//  Created by 岡田 光央 on 2016/11/18.
//
//

#ifndef chartItemEntity_hpp
#define chartItemEntity_hpp

#include <stdio.h>
#include "cocos2d.h"

class chartItemEntity : public cocos2d::Node
{
public:
    int device_id;
    long timestamp;
    int value;
    CREATE_FUNC(chartItemEntity);
};

#endif /* chartItemEntity_hpp */
