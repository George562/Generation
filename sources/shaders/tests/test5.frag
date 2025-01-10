#define PI 3.14159265359
#define TWO_PI 6.28318530718

#define rot(a) mat2(cos(a), -sin(a), sin(a), cos(a))
#define rotate(uv, a) (uv) * rot(a)
#define rotateAround(uv, pos, angle) (uv - pos) * rot(angle) + pos

float angles = 4.;

float Figure(in vec2 uv, in float r, float phi ) {
    uv *= rot(phi);
    float a = atan(uv.x, uv.y) + PI;
    float k = TWO_PI / angles;
    float d = cos(floor(0.5 + a / k) * k - a) * length(uv);
    return smoothstep(0.01, 0., d - r);
}

float Circle(in vec2 uv, in float r, in float R) {
    return smoothstep(R, 0., abs(length(uv) - r));
}

float rect(in vec2 uv, vec2 pos, vec2 size, float angle) {
    uv = rotate(uv - pos, angle);
    return 1. - smoothstep(pow(length(max(2. * abs(uv - vec2(0., size.y / 2.)) - size, 0.)), 1.5), 0., 0.0009);
}

void main() {
    vec2 uv = (gl_FragCoord.xy / iResolution.xy - 0.5) * 0.28;
    uv *= rot(atan(iMouse.x, iMouse.y));
    float result = Circle(uv, 0.1, 0.01) * 2.
                 + Circle(uv, 0.12, 0.02)
                 + Circle(uv, 0.033, 0.01) * 2.
                 + rect(vec2(abs(uv.x), uv.y), vec2(0.027), vec2(0.01, 0.05), PI / 4.) * 2.
                 + rect(uv, vec2(0., -0.02), vec2(0.005, 0.04), 0.) * 2.
    ;
    result *= 0.8;
    gl_FragColor = vec4(vec3(result), step(length(uv), 0.1) + step(0.1, length(uv)) * result);
}
