#version 330 core
in vec3 OurColor;
in vec2 OurTextureCoords;
out vec4 color;
uniform sampler2D OurTexture1;
uniform sampler2D OurTexture2;
uniform float time;
void main()
{
	//color = vec4(OurColor, 1.0f);
	color = mix(texture(OurTexture1, OurTextureCoords), texture(OurTexture2, OurTextureCoords), time);
}
