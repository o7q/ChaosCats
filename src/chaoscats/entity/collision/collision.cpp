#include "collision.hpp"

#include "../../math/intersection.hpp"

void solveWorldCollision(
    const Segment<float> &collisionSegment,
    const EdgeData &edge,
    const Rect<float> &collisionBox,
    Vector<float> &position,
    Vector<float> &velocity,
    const float &dt)
{
    Segment<float> futureSegment(
        collisionSegment.p1.x + velocity.x * dt,
        collisionSegment.p1.y + velocity.y * dt,
        collisionSegment.p2.x + velocity.x * dt,
        collisionSegment.p2.y + velocity.y * dt);

    Segment<float> futureSegmentTop(
        collisionSegment.p1.x,
        collisionSegment.p1.y,
        futureSegment.p1.x,
        futureSegment.p1.y);
    Segment<float> futureSegmentBottom(
        collisionSegment.p2.x,
        collisionSegment.p2.y,
        futureSegment.p2.x,
        futureSegment.p2.y);

    SS_IntersectData centerSegmentIntersect = segmentSegment(futureSegment, edge.segment);
    SS_IntersectData topSegmentIntersect = segmentSegment(futureSegmentTop, edge.segment);
    SS_IntersectData bottomSegmentIntersect = segmentSegment(futureSegmentBottom, edge.segment);

    Vector<float> centerSegmentIntersectPoint = Vector<float>(centerSegmentIntersect.x, centerSegmentIntersect.y);

    // handle flat ceilings
    if (edge.slopeType == SlopeType::Horizontal &&
        centerSegmentIntersect.uA < 0.5f &&
        centerSegmentIntersect.intersected)
    {
        position.y = centerSegmentIntersectPoint.y + collisionBox.half_size.y;
        velocity.y = 0;
    }

    // handle floors/sloped floors
    if ((edge.slopeType == SlopeType::UpHill ||
         edge.slopeType == SlopeType::DownHill ||
         edge.slopeType == SlopeType::Horizontal) &&
        centerSegmentIntersect.uA > 0.5f &&
        centerSegmentIntersect.intersected)
    {
        position.y = centerSegmentIntersectPoint.y - collisionBox.half_size.y;
        velocity.y = 0;
    }

    // handle sloped ceilings
    if ((edge.slopeType == SlopeType::UpHill ||
         edge.slopeType == SlopeType::DownHill) &&
        centerSegmentIntersect.uA < 0.5f &&
        centerSegmentIntersect.intersected)
    {
        velocity.x = 0;
        velocity.y = velocity.y < 0.0f ? 0.0f : velocity.y;
    }

    // handle walls
    if (edge.slopeType == SlopeType::Vertical &&
        (topSegmentIntersect.intersected ||
         bottomSegmentIntersect.intersected))
    {
        velocity.x = 0;
    }
}