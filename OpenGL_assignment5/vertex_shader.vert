#version 330 core
layout(location = 1) in vec3 vertex_position;
layout(location = 2) in vec2 Tex_coords;

out vec2 texture_coords;

void main(){
gl_Position = vec4(vertex_position.x,vertex_position.y,0.0,1);
texture_coords= Tex_coords;

}

