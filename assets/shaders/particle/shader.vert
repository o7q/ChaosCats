attribute vec2 position;
varying vec2 texCoord;
uniform vec2 texSize;
uniform vec2 resolution;
uniform vec2 translation;
uniform float zoom;

void main()
{
    // Assign scaled position to gl_Position
    gl_Position = vec4(position, 0.0, 1.0);

    // Flip texture vertically
    texCoord = position * vec2(1.0, -1.0);
}