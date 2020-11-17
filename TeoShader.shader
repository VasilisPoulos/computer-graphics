#shader vertex
#version 330															
																			
layout(location = 0) in vec3 position;									
																			
uniform mat4 mv_matrix;														
uniform mat4 proj_matrix;													
																			
out vec4 varyingColor;														

void main()																	
{																			
	gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);				
	varyingColor = vec4(1.0,0.0,0.0,0.5) ;
	//varyingColor = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.3);
};



#shader fragment
#version 330

in vec4 varyingColor;								
													
out vec4 color;										
uniform mat4 mv_matrix;								
uniform mat4 proj_matrix;							
													
void main()											
{													
	color = varyingColor;							
	//color = vec4(varyingColor,1.0f);
}
