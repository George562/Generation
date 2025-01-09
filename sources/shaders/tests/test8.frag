#define PI 3.14159265359
#define TWO_PI 6.28318530718

#define rot(a) mat2(cos(a), -sin(a), sin(a), cos(a))
#define rotate(uv, a) (uv) * rot(a)
#define rotateAround(uv, pos, angle) (uv - pos) * rot(angle) + pos

float random(float x) {
    return fract(sin(x * 12753.1 + 12345.6532) * 43758.5453);
}
float random(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

float rectangle(in vec2 uv, vec2 pos, vec2 size, float angle) {
    uv = rotate(uv - pos, angle);
    vec2 res = smoothstep(0.008, 0., abs(uv - vec2(0.,  size.y / 2.)) - size / 2.);
    return res.x * res.y;
}

float noise (in vec2 uv) {
    vec2 i = floor(uv);
    vec2 u = smoothstep(0.2, 0.8, fract(uv));
    return mix(
        mix(random(i), random(i + vec2(1.0, 0.0)), u.x),
        mix(random(i + vec2(0.0, 1.0)), random(i + vec2(1.0, 1.0)), u.x), u.y);
}

void main() {
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    float res = 0.;
    uv *= 10.;

    // float pos = random(floor(vec2(uv.x + iTime, uv.y + iTime * 0.5)));
    // res += rectangle(fract(uv), vec2(0., 0.1 + pos * 0.8), vec2(0.2, 1.), PI / 2.);
    // res += noise(uv);
    vec2 pos = vec2(random(floor(uv.x)), random(floor(uv.y)));
    res += rectangle(fract(uv), vec2(0.1 + pos.x * 0.8, 0.), vec2(0.2, 1.), 0.);
    res += rectangle(fract(uv), vec2(0., 0.1 + pos.y * 0.8), vec2(0.2, 1.), PI / 2.);
    gl_FragColor = vec4(vec3(res), 1.0);
}
