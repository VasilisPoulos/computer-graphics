#shader fragment
#version 330 core
                                                                       
// Interpolated values from the vertex shaders                         
in vec2 UV;                                                            
in vec3 color;
                                                          
// Ouput data                                                          
out vec3 out_color;                                                        
                                                                       
// Values that stay constant for the whole mesh.                       
uniform sampler2D myTextureSampler;                                    
uniform vec3 textureFlag;

void main() {                                                          
                                                                       
    // Output color = color of the texture at the specified UV  
    // using textureFlag to set a color if there is there is no texture
    out_color = (textureFlag * texture(myTextureSampler, UV).rgb) + (1.0 - textureFlag) * color;                         
}

#shader vertex
#version 330 core 
// Input vertex data, different for all executions of this shader.     
layout(location = 0) in vec3 vertexPosition_modelspace;                
layout(location = 1) in vec2 vertexUV;                                 
                                                                       
// Output data ; will be interpolated for each fragment.               
out vec2 UV;                                                           
out vec3 color;              

// Values that stay constant for the whole mesh.                       
uniform mat4 MVP;                                                      
                                                                       
void main() {                                                          
                                                                       
    // Output position of the vertex, in clip space : MVP * position   
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);            
                                                                       
    // UV of the vertex. No special space for this one.                
    UV = vertexUV;

    color = vec3(1.0f, 0.0f, 0.0f);
}