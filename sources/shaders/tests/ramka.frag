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
    vec2 uv = (gl_FragCoord.xy / iResolution.xy - 0.5);
    float result = 0.;
    result += 1. - Figure(uv, 0.47, 0.);
    result += rect(uv, vec2(0.34, 0.47), vec2(0.005, 0.13), PI);
    result += rect(uv, vec2(0.41, 0.34), vec2(0.13, 0.005), 0.);
    // result += Circle(uv - vec2(0.5), 0.2, 0.02) * 3.;
    vec3 color = vec3(result);
    result = rect(uv, vec2(0.38, 0.45), vec2(0.005, 0.1), PI * 3. / 4.);
    result += rect(uv, vec2(0.45, 0.45), vec2(0.005, 0.1), PI * 5. / 4.);
    // result = Figure(uv - vec2(0.42), 0.05, 0.);
    // result = Circle(uv - vec2(0.5), 0.1, 0.1) * 4.;
    color.r += result;
    gl_FragColor = vec4(color, 1.);
}
