#version 330 core

layout (location = 0) in vec3 aPos;
uniform vec3 offset;

void main() {
	vec3 adjusted = aPos + offset;
	gl_Position = vec4(adjusted, 1.0);
}