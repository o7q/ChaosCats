#include "collision.hpp"

#include <vector>
#include <cmath>

float getTimeToCollision(Rect<float> rect, std::vector<Segment<float>> &segments, float maxDt, float minDt)
{
    float dt = maxDt;
    float step = maxDt;

    for (Segment<float> segment : segments)
    {
        bool collision = segmentRect(segment, rect);

        if (collision)
        {
            return -2.0f;
        }
    }

    bool collided = true;
    while (true)
    {
        collided = false;
        for (Segment<float> segment : segments)
        {
            bool collision = segmentRect(segment, rect);

            if (collision)
            {
                collided = true;
                break;
            }
        }

        if (!collided && std::fabs(step - maxDt) < 0.0001)
        {
            return -1;
        }

        if (step < minDt && collided)
        {
            break;
        }

        step /= 2.0f;
        dt += collided ? -step : step;
    }

    return dt;
}

void simulationStep(Rect<float> rect, std::vector<Segment<float>> &segments, float dt, float vx, float vy, float &x, float &y)
{
    float t = getTimeToCollision(rect, segments, dt, 0.001);
    float ttc = t > 0 ? t : dt;

    int collisions = 0;
    for (Segment<float> segment : segments)
    {
        Rect<float> rect_future;
        rect_future.position.x = (rect.position.x + rect.half_size.w) + vx * ttc;
        rect_future.position.y = (rect.position.y + rect.half_size.h) + vy * ttc;
        rect_future.size.w = rect.size.w;
        rect_future.size.h = rect.size.h;

        int collide_r1 = pointRect(segment.p1, rect_future);
        int collide_r2 = pointRect(segment.p2, rect_future);

        bool collision = segmentRect(segment, rect_future);

        if (collision)
        {
            if (++collisions > 1)
            {
                return;
            }

            float dir = vx * segment.p1.x + vy * segment.p1.y;
            Segment<float> test;
            test.p1 = Point2D<float>(0, 0);
            test.p2 = Point2D<float>(segment.p2.x - segment.p1.x, segment.p2.y - segment.p1.y);
            float vx2 = test.p2.x * dir;
            float vy2 = test.p2.y * dir;

            if (collide_r1 || collide_r2)
            {
                int side = (collide_r1 || collide_r2) - 1;
                Point2D<int> dirs[] = {Point2D<int>(1, 0), Point2D<int>(1, 0), Point2D<int>(0, 1), Point2D<int>(0, 1), Point2D<int>(0, 0)};
                Point2D<int> dir = dirs[side];

                vx *= dir.x;
                vy *= dir.y;
                continue;
            }

            vx = vx2;
            vy = vy2;
        }

        Rect<float> temp = rect;
        rect.position.x += vx * dt;
        rect.position.y += vy * dt;

        if (isRectColliding(temp, segments))
        {
            return;
        }

        x += vx *= dt;
        y += vy *= dt;
    }
}

bool isRectColliding(Rect<float> rect, std::vector<Segment<float>> segments)
{
    for (Segment<float> segment : segments)
    {
        bool collision = segmentRect(segment, rect);

        if (collision)
        {
            return true;
        }
    }

    return false;
}