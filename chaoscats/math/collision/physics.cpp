#include "collision.hpp"

#include <vector>
#include <cmath>

float getTimeToCollision(Rect<float> rect, std::vector<Segment<float>> &segments, float maxDt, float minDt, float vx, float vy)
{
    float dt = maxDt;
    float step = maxDt;
    Rect<float> rect_future = rect;
    rect_future.incrementPosition(vx * maxDt, vy * maxDt);

    // Check for initial collision
    if (isRectColliding(rect, segments)) return -2.0;
    if (!isRectColliding(rect_future, segments)) return -1.0;

    const int maxIterations = 10000;
    int iterations = 0;
    while (true)
    {
        bool collided = isRectColliding(rect_future, segments);

        if (collided && step < minDt) return dt;

        step /= 2.0f;

        if (collided) {
            rect_future.incrementPosition(vx * -step, vy * -step);
            dt -= step;
        } else {
            rect_future.incrementPosition(vx * step, vy * step);
            dt += step; 
        }

        if (++iterations > maxIterations) {
            std::cout << "[physics.cpp] Max iterations reached!" << std::endl;
            break;
        }
    }

    return dt;
}

void simulationStep(Rect<float> rect, std::vector<Segment<float>> &segments, float dt, float vx, float vy, float &x, float &y)
{
    float time = getTimeToCollision(rect, segments, dt, 0.001, vx, vy);
    float timeToCollision = time > 0 ? time : dt;

    std::cout << time << std::endl;

    int collisions = 0;
    for (Segment<float> segment : segments)
    {
        Rect<float> rect_future = rect;
        rect_future.incrementPosition(vx * timeToCollision, vy * timeToCollision);

        int collide_r1 = pointRect(segment.p1, rect_future);
        int collide_r2 = pointRect(segment.p2, rect_future);

        bool collision = segmentRect(segment, rect_future);

        if (collision)
        {
            if (++collisions > 1)
            {
                return;
            }

            Point<float> lineVec;
            lineVec = Point<float>(segment.p2.x - segment.p1.x, segment.p2.y - segment.p1.y);
            float magnitude = std::sqrt(lineVec.x * lineVec.x + lineVec.y * lineVec.y);

            if (magnitude != 0)
            {
                lineVec.x /= magnitude;
                lineVec.y /= magnitude;
            }
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
    }

    const float epsilon = 0.1f;
    Rect<float> temp = rect;
    temp.incrementPosition(vx * (dt + epsilon), vy * (dt + epsilon));

    if (isRectColliding(temp, segments))
    {
        return;
    }

    x += vx * dt;
    y += vy * dt;
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