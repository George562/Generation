uniform sampler2D overlay;

uniform float u_time;       // Time in seconds since load

void main() {
    vec4 pixel = texture2D(overlay, gl_TexCoord[0].xy);
    gl_FragColor = gl_Color * pixel;
}