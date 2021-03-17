#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D diffuseMap[1];

void main()
{    
    FragColor = texture(diffuseMap[0], TexCoords);
}