#version 420

layout (binding = 0) uniform sampler2D uTex;
uniform float u_dir;

out vec4 fragColor;

in vec2 texCoordinates;

void main() 
{
	fragColor = vec4(0.0, 0.0, 0.0, 0.0);

	fragColor += texture(uTex, vec2(texCoordinates.x - 4.0 * u_dir, texCoordinates.y)) * 0.06;
	fragColor += texture(uTex, vec2(texCoordinates.x - 3.0 * u_dir, texCoordinates.y)) * 0.09;
	fragColor += texture(uTex, vec2(texCoordinates.x - 2.0 * u_dir, texCoordinates.y)) * 0.12;
	fragColor += texture(uTex, vec2(texCoordinates.x - u_dir, texCoordinates.y)) * 0.15;
	fragColor += texture(uTex, vec2(texCoordinates.x, texCoordinates.y)) * 0.16;
	fragColor += texture(uTex, vec2(texCoordinates.x + u_dir, texCoordinates.y)) * 0.15;
	fragColor += texture(uTex, vec2(texCoordinates.x + 2.0 * u_dir, texCoordinates.y)) * 0.12;
	fragColor += texture(uTex, vec2(texCoordinates.x + 3.0 * u_dir, texCoordinates.y)) * 0.09;
	fragColor += texture(uTex, vec2(texCoordinates.x + 4.0 * u_dir, texCoordinates.y)) * 0.06;
}