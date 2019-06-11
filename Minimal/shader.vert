#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view; 

out vec2 TexCoord;
out vec3 fragPosition;
out vec3 fragNormal;

void main()
{	
	gl_Position = projection * view * model * vec4(position, 1.0);
	TexCoord = texCoord;
	fragPosition = vec3(model * vec4(position, 1));
	fragNormal = vec3(model * vec4(normal, 0));
}
