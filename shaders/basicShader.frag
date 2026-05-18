#version 330 core

in vec3 vNormal;
in vec3 vMatamb;
in vec3 vMatdiff;
in vec3 vMatspec;
in float vMatshin;

out vec4 FragColor;

void main() {
    FragColor = vec4(vMatdiff, 1.0);
}