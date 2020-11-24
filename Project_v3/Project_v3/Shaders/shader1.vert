#version 330                                                                  
                                                                              
layout (location = 0) in vec3 pos;											  

uniform mat4 model;
uniform mat4 projection ;
uniform mat4 view ;

uniform float xMove;

                                                                              
void main()                                                                   
{                                                                             
    gl_Position = projection * view * model * vec4(0.4 * pos.x + xMove, 0.4 * pos.y, pos.z, 1.0);
}