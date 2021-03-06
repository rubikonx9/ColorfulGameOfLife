#version 330

layout(location = 0)
in vec3 vertexPosition;

layout(location = 1)
in vec2 texCoord;

out vec3 vPosition;
out vec2 vTexCoord;

void main() {
	vPosition = vertexPosition;
	vTexCoord = texCoord;
	gl_Position = vec4(vertexPosition, 1.0);
}