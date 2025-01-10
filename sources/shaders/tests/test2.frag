#iChannel0 "file://../../textures/noise.png"

vec2 hash(vec2 p) {
    return fract(sin(vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)))) * 43758.5453);
}

float voronoi(vec2 x) {
    vec2 n = floor(x), f = fract(x);
    float distance_type = 1., F1 = 8.0;

    for (int j = -1; j <= 1; j++) {
        for(int i = -1; i <= 1; i++) {
            vec2 g = vec2(i, j);
            vec2 o = hash(n + g);
            // o = 0.5 + 0.41 * sin(iTime + 6.2831 * o); // animate
            vec2 r = g - f + o;
            float d = distance_type < 1.0 ? dot(r, r)  :          // euclidean^2
                      distance_type < 2.0 ? sqrt(dot(r, r)) :     // euclidean
                      distance_type < 3.0 ? abs(r.x) + abs(r.y) : // manhattan
                      max(abs(r.x), abs(r.y));                    // chebyshev

            if (d < F1) F1 = d;
        }
    }
    return F1;
}

vec2 twirl(vec2 UV, float Strength, float Offset) {
    vec2 delta = UV;
    float angle = Strength * length(delta) + Offset;
    vec2 uv = delta * mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
    return uv ;
}

void main() {
    vec2 uv = gl_FragCoord.xy / iResolution.xy - 0.5;
    float d = length(uv);
    vec2 Muv = uv * 4.;
    float Strength = 1.95 * (1. + 0.2 * texture2D(iChannel0, uv + 0.05 * iTime).r);
    Muv = twirl(Muv, Strength, 0.5 * iTime);

    vec4 color = 2. * vec4(1., 0.4, 0., 1.) + vec4(0.15, 0.15, 0.15, 0.);
    float param = voronoi(Muv + vec2(iTime * 0.1)) * 2.5;
    color *= param * param * param * smoothstep(0.1, 0.3, length(uv));
    color = mix(color, vec4(-1.), smoothstep(0.7, 0.95, 2. * d));
    color.a = 1. - smoothstep(0.7, 0.95, 2. * d) * (1. - max(color.r, max(color.r, color.b) ) );
    // color += smoothstep(0.05, 0., abs(2. * length(uv) - 0.9));
    gl_FragColor = color;
}
