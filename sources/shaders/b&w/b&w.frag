uniform sampler2D overlay;

void main() {
    vec4 pixel = texture2D(overlay, gl_TexCoord[0].xy);
    gl_FragColor = gl_Color * vec4(vec3(pixel.r * 0.15 + pixel.g * 0.3 + pixel.b * 0.05), pixel.w);
}
