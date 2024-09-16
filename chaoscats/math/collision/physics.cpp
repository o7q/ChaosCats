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
        Rect<float> rect_future = rect;
        rect_future.setPosition(rect.getPosition().x + vx * ttc, rect.getPosition().y + vy * ttc);

        int collide_r1 = pointRect(segment.p1, rect_future);
        int collide_r2 = pointRect(segment.p2, rect_future);

        bool collision = segmentRect(segment, rect_future);

        if (collision)
        {
            std::cout << "collision" << std::endl;
            if (++collisions > 1)
            {
                return;
            }

            Point<float> lineVec;
            lineVec = Point<float>(segment.p2.x - segment.p1.x, segment.p2.y - segment.p1.y);
            float dir = vx * lineVec.x + vy * lineVec.y;
            float vx2 = lineVec.x * dir;
            float vy2 = lineVec.y * dir;

            if (collide_r1 || collide_r2)
            {
                int side = (collide_r1 || collide_r2) - 1;
                Point<int> dirs[] = {Point<int>(1, 0), Point<int>(1, 0), Point<int>(0, 1), Point<int>(0, 1), Point<int>(0, 0)};
                Point<int> dir = dirs[side];

                vx *= dir.x;
                vy *= dir.y;
                continue;
            }

            vx = vx2;
            vy = vy2;
        }

        Rect<float> temp = rect;
        temp.incrementPosition(vx * dt, vy * dt);

        if (isRectColliding(temp, segments))
        {
            return;
        }

        x += vx * dt;
        y += vy * dt;
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