#version 330 core
layout(location = 1) in vec3 vertexPosition_modelspace;
 
//out vec4 color;

//float ambientstrength=.3;

 
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
 
gl_Position = projection * view * model * vec4(vertexPosition_modelspace,1);


}
