#version 330 core
//out vec4 result;

out vec4 colors;
in vec3 Color;

void main(){
colors = vec4(Color,1);
}