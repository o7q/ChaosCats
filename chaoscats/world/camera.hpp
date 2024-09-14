#pragma once

class WorldCamera
{
public:
    void translate(int x, int y)
    {
        offsetX += x;
        offsetY += y;
    }

    void setTranslation(int x, int y)
    {
        offsetX = x;
        offsetY = y;
    }

    void zoom(float x, float y)
    {
        // Calculate the center before zooming
        float beforeX, beforeY;
        screenToWorld(640, 360, beforeX, beforeY);

        scaleX *= x;
        scaleY *= y;

        float afterX, afterY;
        screenToWorld(640, 360, afterX, afterY);
        offsetX += (beforeX - afterX);
        offsetY += (beforeY - afterY);
    }

    void setZoom(float x, float y)
    {
        scaleX = x;
        scaleY = y;
    }

    int getTranslateX()
    {
        return offsetX;
    }

    int getTranslateY()
    {
        return offsetY;
    }

    float getZoomX()
    {
        return scaleX;
    }

    float getZoomY()
    {
        return scaleY;
    }

    void worldToScreen(float worldX, float worldY, int &screenX, int &screenY)
    {
        screenX = static_cast<int>((worldX - offsetX) * scaleX);
        screenY = static_cast<int>((worldY - offsetY) * scaleY);
    }

    void screenToWorld(int screenX, int screenY, float &worldX, float &worldY)
    {
        worldX = static_cast<float>(screenX) / scaleX + offsetX;
        worldY = static_cast<float>(screenY) / scaleY + offsetY;
    }

private:
    int offsetX = 0;
    int offsetY = 0;

    float scaleX = 1.0f;
    float scaleY = 1.0f;
};