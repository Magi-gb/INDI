#version 330 core

in vec3 vertex;
in vec3 normal;
in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

uniform mat4 TG;
uniform mat4 PM;
uniform mat4 VM;

out vec3 vNormal;
out vec3 vMatamb;
out vec3 vMatdiff;
out vec3 vMatspec;
out float vMatshin;

void main() {
    vNormal = normal;
    vMatamb = matamb;
    vMatdiff = matdiff;
    vMatspec = matspec;
    vMatshin = matshin;

    gl_Position = PM * VM * TG * vec4(vertex, 1.0);
}