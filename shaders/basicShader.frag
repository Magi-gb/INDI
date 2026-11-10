#version 330 core

in vec3 FragPos;
in vec3 Normal;

in vec3 vMatamb;
in vec3 vMatdiff;
in vec3 vMatspec;
in float vMatshin;
in vec2 TexCoord;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform sampler2D colorMap;
uniform bool esSuelo; //Variable pel color del terra

//Llums nocturnes
uniform bool nightMode;
uniform vec3 flashlightPos;
uniform vec3 flashlightDir;
uniform vec3 ghostLightPos;
uniform vec3 ghostLightColor;
uniform vec3 coinLightPos[10];
uniform vec3 coinLightDir[10];

out vec4 FragColor;

void main() {

    // Normal normalizada
    vec3 norm = normalize(Normal);

    //Texturas
    vec3 texColor = texture(colorMap, TexCoord).rgb;

    // Direccion hacia la luz
    vec3 lightDir = normalize(lightPos - FragPos);

    // =========================
    // AMBIENT
    // =========================

    vec3 ambient = 0.6 * lightColor * vMatamb;

    // =========================
    // DIFFUSE
    // =========================

    float diff = max(dot(norm, lightDir), 0.0);

    //Para que el suelo sea de otro color y se vea en el minimapa, de noche no es necesario
    vec3 texFinal = esSuelo ? vec3(1.0, 0.0, 0.0) : texColor;

    vec3 diffuse = esSuelo ? diff * lightColor * texFinal : 1.5 * diff * lightColor * vMatdiff * texFinal;

    // =========================
    // SPECULAR
    // =========================

    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), vMatshin);

    vec3 specular = spec * lightColor * vMatspec;

    //LUZ FANTASMA
    vec3 ghostDiffuse = vec3(0.0);

    //LUZ MONEDA
    vec3 coinDiffuse = vec3(0.0);

    if (nightMode) {

        vec3 ghostDir = normalize(ghostLightPos - FragPos);

        float ghostDiff = max(dot(norm, ghostDir), 0.0);

        float dist = length(ghostLightPos - FragPos);

        float attenuation = 1.0 / (1.0 + dist * dist);

        ghostDiffuse = 2.0 * ghostDiff * attenuation * ghostLightColor * vMatdiff;
    }

    for (int i = 0; i < 10; i++) {
        vec3 lightVec = coinLightPos[i] - FragPos;

        float dist = length(lightVec);

        if (dist < 4.0) {
            vec3 L = normalize(lightVec);

            float diffCoin = max(dot(norm, L), 0.0);

            float focus = max(dot(coinLightDir[i], -L), 0.0);

            float range = 1.0 / (1.0 + exp(4.0 * (dist - 4.0)));

            coinDiffuse += diffCoin * focus * range * vec3(0.5, 0.42, 0.1);
        }
    }

    //CALC LUZ FINAL
    vec3 result;

    if (!nightMode) {
        result = ambient + diffuse + specular + ghostDiffuse + coinDiffuse;
    } else {
        result = 0.05 * vMatamb;

        vec3 lightDir2 = normalize(flashlightPos - FragPos);

        float spot = max(dot(-lightDir2, normalize(flashlightDir)), 0.0);

        if (spot > 0.8) {
            float diff2 = max(dot(norm, lightDir2), 0.0);

            // Así no se ve un circulo artificial
            float smoothSpot = smoothstep(0.8, 0.95, spot);

            // Atenuacion por distancia
            float dist2 = length(flashlightPos - FragPos);
            float attenuation2 = 1.0 / (1.0 + 0.3 * dist2 + 0.1 * dist2 * dist2);

            result += smoothSpot * diff2 * attenuation2 * vec3(1.0, 0.85, 0.0) * vMatdiff * texColor;
        }

        result += ghostDiffuse;
        result += coinDiffuse;
    }

    FragColor = vec4(result, 1.0);
}