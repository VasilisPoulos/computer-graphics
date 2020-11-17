#shader vertex
#version 330 core

in vec3 position;

void main()
{
    gl_Position = vec4(position, 1.0);
}


#shader fragment
#version 330 core

in vec3 Color;

out vec4 outColor;

void main()
{
    outColor = vec4(1.0, 0.0, 0.0, 1.0);
}