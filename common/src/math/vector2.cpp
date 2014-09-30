//
//  vector2.cpp
//
//  Created by Jingshu Huang on 1/17/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "vector2.h"


template<> const Vector2 Vector2::ZERO( 0, 0);
template<> const Vector2 Vector2::UNIT_X( 1, 0);
template<> const Vector2 Vector2::UNIT_Y( 0, 1);
template<> const Vector2 Vector2::NEGATIVE_UNIT_X( -1,  0);
template<> const Vector2 Vector2::NEGATIVE_UNIT_Y(  0, -1);
template<> const Vector2 Vector2::UNIT_SCALE(1, 1);
