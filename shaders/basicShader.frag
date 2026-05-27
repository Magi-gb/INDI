#version 330 core

in vec3 FragPos;
in vec3 Normal;

in vec3 vMatamb;
in vec3 vMatdiff;
in vec3 vMatspec;
in float vMatshin;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec3 viewPos;

out vec4 FragColor;

void main() {

    // Normal normalizada
    vec3 norm = normalize(Normal);

    // Direccion hacia la luz
    vec3 lightDir = normalize(lightPos - FragPos);

    // =========================
    // AMBIENT
    // =========================

    vec3 ambient = 0.2 * lightColor * vMatamb;

    // =========================
    // DIFFUSE
    // =========================

    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuse = diff * lightColor * vMatdiff;

    // =========================
    // SPECULAR
    // =========================

    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), vMatshin);

    vec3 specular = spec * lightColor * vMatspec;

    // Resultado final
    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}