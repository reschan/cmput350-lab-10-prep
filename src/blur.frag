#version 120 // glsl version 1.2 for OpenGL 2.1

uniform sampler2D texture;
uniform vec2 resolution;

void main()
{
    // lookup the pixel in the texture
    vec2 coords = gl_TexCoord[0].xy;
    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    int count;

    for (float j = -2; j < 3; j++) {
        //if (j < 0 || j > resolution.y) { continue; }
        for (float i = -2; i < 3; i++) {
            //if (i < 0 || i > resolution.x) { continue; }
            color += texture2D(texture, vec2(i / resolution.x + gl_TexCoord[0].x, j / resolution.y + gl_TexCoord[0].y));
            count++;
        }
    }

    color /= count;
    color[3] = 1.0;

    // multiply it by the color
    gl_FragColor = color;
}