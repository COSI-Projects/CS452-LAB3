#version 130

in vec3 pos;
uniform mat4 trans;

void main() {
	gl_Position = trans * vec4(pos, 1);
}
