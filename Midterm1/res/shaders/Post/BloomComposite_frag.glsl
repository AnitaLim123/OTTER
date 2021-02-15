#version 420

layout (binding = 0) uniform sampler2D uScene;
layout (binding = 1) uniform sampler2D uBloom;

in vec2 TexCoords;

out vec4 fragColor;
 
void main() 
{
    vec4 color1 = texture(uScene, TexCoords);
    vec4 color2 = texture(uBloom, TexCoords);

    fragColor = 1.0 - (1.0 - color1) * (1.0 - color2);
}