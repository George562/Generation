#define PI 3.14159265359
#define TWO_PI 6.28318530718

#define rot(a) mat2(cos(a), -sin(a), sin(a), cos(a))
#define rotate(uv, a) (uv) * rot(a)
#define rotateAround(uv, pos, angle) (uv - pos) * rot(angle) + pos

float metric(in vec2 uv) {
    return abs(uv.x + sin(uv.x)) + abs(uv.y + sin(uv.y));
}

void main() {
    vec2 uv = (gl_FragCoord.xy / iResolution.xy - 0.5) * 16.;
    float col = 0.;

    float dist = 7.;
    col += sin(metric(uv) + 1.5 * iTime) * smoothstep(dist + 1., dist, metric(uv));
    col += smoothstep(dist, dist + 1., metric(uv));
    col -= smoothstep(dist + 1., dist + 2., metric(uv));

    gl_FragColor = vec4(vec3(col), 1.0);
}
