#pragma once

#include "../geometry/geometry_common.hpp"

namespace jmath
{
    namespace col
    {
        bool segmentSegment(jmath::geo::Segment<float> a, jmath::geo::Segment<float> b);
        bool segmentRectangle(jmath::geo::Segment<float> segment, jmath::geo::Rectangle<float> rectangle);
    }
}