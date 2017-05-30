#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normal; 
out vec4 color;

float ambientstrength=.3;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


uniform vec3 lightcolor;
uniform vec3 materialcolor;
uniform vec3 lightpos;
 
uniform mat4 MVP;

void main(){
 
gl_Position = projection * view *  model * vec4(vertexPosition_modelspace,1);

 // gl_Position.xyz = vertexPosition_modelspace;
//  gl_Position.w = 1;
//color=vertexPosition_modelspace;
//color=vec4(lightcolor*materialcolor,1);

 vec3 FragPos = vec3(model * vec4(vertexPosition_modelspace, 1.0f));

   // transform normal to camera space and normalize it
 //   vec3 n = normalize(m_normal * normal);

	vec3 normalEyeSpace = vec3(model*view * vec4(normal,0.0));
	vec3 Normal = mat3(transpose(inverse(model))) * normalEyeSpace;  
    vec3 norm = normalize(Normal);
	

vec3 lightDir = normalize(lightpos-FragPos); 

float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * lightcolor;

vec3 ambient=ambientstrength*lightcolor;

vec3 result=(ambient+diffuse)*materialcolor;


color=vec4(result,1);



}
