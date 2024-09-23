uniform sampler2D texture;
uniform float time;

float speed = 4;
float spacing = 5.0;
float brightness = 1.25;

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    // multiply it by the color
    gl_FragColor = gl_Color * pixel;
    
    if (mod(round(gl_TexCoord[0].y * 150 - (time*speed) ),spacing) == 0.0)
    {
        gl_FragColor = gl_FragColor * brightness;
    }
}