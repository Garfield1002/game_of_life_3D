#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

layout (location = 3) in mat4 instanceMatrix;

uniform mat4 projection;
uniform mat4 view;

out vec3 FragPos;
out vec3 Normal;

void main()
{
	gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0f);
	FragPos = vec3(instanceMatrix * vec4(aPos, 1.0));
	Normal = aNormal;
}