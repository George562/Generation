uniform sampler2D overlay;

uniform vec2 u_resolution; // Canvas size (width, height)
uniform vec2 u_mouse;      // mouse position in screen pixels
uniform float u_time;      // Time in seconds since load

uniform vec2 u_playerPosition;
uniform float u_playerRadius;

vec3 pallete(float t) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.0, 0.333, 0.667);
    
    return a + b * cos(6.28318 * (c * t + d));
}

void main() {
    vec4 pixel = texture2D(overlay, gl_TexCoord[0].xy);

    vec2 playerPosition = vec2(u_playerPosition.x, u_resolution.y - u_playerPosition.y);

    vec2 normal = gl_FragCoord.xy - playerPosition;
    vec2 lightPosition = vec2(u_mouse.x, u_resolution.y - u_mouse.y) - playerPosition;
    float intensity = smoothstep(0.65, 0.85, dot(normalize(normal), normalize(lightPosition)));
    intensity += smoothstep(1.6, -0.2, length(normal) / u_playerRadius - 1.0);
    intensity = clamp(intensity, 0.4, 1.);
    gl_FragColor = gl_Color * vec4(mix(pixel.xyz, pallete(u_time * 0.1), clamp(abs(sin(u_time * 2.)), 0.0, 0.3)) * intensity, pixel.w);
}
