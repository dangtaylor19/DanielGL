#version 330 core
out vec4 FragColor;

in vec2 textureCoord;
in vec3 normal;
in vec3 fragPos;

uniform sampler2D texture1;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

struct PointLight { 
    vec3 position;
    float range;
    float intensity;
    vec3 color;
};

#define MAX_POINT_LIGHTS 16 
uniform float lightCount;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

void main()
{
    vec4 textureColor = texture(texture1, textureCoord);
    if(textureColor.a < 0.1)
        discard;


    //Ambient light
    vec3 ambientLight = vec3(0.2);

    vec3 result = ambientLight;
    
    for(int i = 0; i < lightCount; i++){
        //Diffuse
        vec3 normalized = normalize(normal);
        vec3 lightDir = normalize(pointLights[i].position - fragPos);
        float diff = max(dot(normalized, lightDir), 0.0);
        vec3 diffuse = diff * pointLights[i].color * max((1 - (distance(pointLights[i].position, fragPos) / (pointLights[i].range+0.0001))), 0.0);

        //Specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, normalized);

        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = min(specularStrength * spec * pointLights[i].color, 1.0); 

        result = result + (diffuse + specular) * pointLights[i].intensity;
    }

    FragColor = textureColor * vec4(result, 1.0);
    //FragColor = vec4(textureCoord, 0.0, 1.0);
}