#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_albedo[4];

void main()
{    
    FragColor = texture(texture_albedo[0], TexCoords);
}