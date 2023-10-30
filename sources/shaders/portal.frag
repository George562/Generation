uniform float offset;
uniform vec3 point;
uniform vec4 color;
uniform mat4 matrix;
uniform sampler2D overlay;
uniform sampler2D current;

uniform vec2 u_resolution;  // Canvas size (width, height)
uniform vec2 u_mouse;       // mouse position in screen pixels
uniform float u_time;       // Time in seconds since load

uniform vec2 u_playerPosition;
uniform float u_playerRadius;

void main() {
    vec4 pixel = texture2D(overlay, gl_TexCoord[0].xy);

    vec2 playerPosition = vec2(u_playerPosition.x, u_resolution.y - u_playerPosition.y);

    vec2 normal = gl_FragCoord.xy - playerPosition;
    vec2 lightPosition = vec2(u_mouse.x, u_resolution.y - u_mouse.y) - playerPosition;
    float intensity = smoothstep(0.65, 0.85, dot(normalize(normal), normalize(lightPosition)));
    intensity += smoothstep(1.6, -0.2, length(normal) / u_playerRadius - 1.0);
    intensity = clamp(intensity, 0.1, 1.);
    gl_FragColor = gl_Color * vec4(pixel.xyz * intensity, pixel.w);
}