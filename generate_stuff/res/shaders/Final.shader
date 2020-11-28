#shader vertex
#version 330 core 
  
layout(location = 0) in vec3 vertexPosition_modelspace;                
layout(location = 1) in vec2 vertexUV;                                 
                                                                                     
out vec2 UV;                                                           
out vec3 color;              
                     
uniform mat4 model_matrix;                                                      
uniform mat4 view_matrix;                                                      
uniform mat4 proj_matrix;   
uniform vec3 set_color;                                                                       

void main() {                                                          
                                                                       
    // Output position of the vertex, in clip space
    gl_Position = proj_matrix * view_matrix * model_matrix * vec4(vertexPosition_modelspace, 1);            
                                                                 
    // UV of the vertex. No special space for this one.                
    UV = vertexUV;

    color = set_color;
}

#shader fragment
#version 330 core
                                                                                           
in vec2 UV;                                                            
in vec3 color;
                                                                                                                 
out vec4 out_color;                                                        
                                                                                            
uniform sampler2D myTextureSampler;                                    
uniform vec4 textureFlag;
uniform float transparency;

void main() {                                                          
                                                                       
    // Output color = color of the texture at the specified UV  
    // using textureFlag to set a color if there is there is no texture
    out_color = textureFlag * vec4((texture(myTextureSampler, UV).rgb), 1.0f) + (1.0 - textureFlag) * vec4( color, transparency);                         
}