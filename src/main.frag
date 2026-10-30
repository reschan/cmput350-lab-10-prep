#version 120 // glsl version 1.2 for OpenGL 2.1

uniform vec2 resolution;
uniform vec2 mousePos;
uniform float time;

void main(void) {

    // Fragment shader should output into gl_FragColor an RGBA:
    
    vec2 px = gl_FragCoord.xy / resolution.xy;
    vec2 pos = mousePos.xy / resolution.xy;

    px.x *= resolution.x / resolution.y;
    pos.x *= resolution.x / resolution.y;

    float radius = 10 / resolution.x * (resolution.x / resolution.y);

    vec3 pixel;
    if ( distance(px, pos) < radius ) {
        pixel = vec3(1.0, 1.0, 1.0);
    } else {
        pixel = vec3(px.x, px.y, 0.0);
        pixel += vec3(0, 0, time/10);
    }


    gl_FragColor = vec4(pixel, 1.0);
}
