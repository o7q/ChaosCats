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

    void zoom(float value)
    {
        scale *= value;
    }

    void setZoom(float value)
    {
        scale = value;
    }

    int getTranslateX()
    {
        return offsetX;
    }

    int getTranslateY()
    {
        return offsetY;
    }

    float getZoom()
    {
        return scale;
    }

    // void worldToScreen(float worldX, float worldY, int &screenX, int &screenY)
    // {
    //     screenX = static_cast<int>((worldX - offsetX) * scaleX);
    //     screenY = static_cast<int>((worldY - offsetY) * scaleY);
    // }

    // void screenToWorld(int screenX, int screenY, float &worldX, float &worldY)
    // {
    //     worldX = static_cast<float>(screenX) / scaleX + offsetX;
    //     worldY = static_cast<float>(screenY) / scaleY + offsetY;
    // }

private:
    int offsetX = 0;
    int offsetY = 0;

    float scale = 1.0f;
};