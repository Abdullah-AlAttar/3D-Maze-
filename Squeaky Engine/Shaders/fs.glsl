#version 330 core
out vec4 color;

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

uniform vec3 viewPos;
uniform sampler2D textureSampler;

struct Material {

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

        
    float constant;
    float linear;
    float quadratic;
	
};
struct SpotLight {
    
    vec3 position;
    vec3 direction;
 
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;  

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
  
};
uniform Light defaultLight; 
uniform DirectionalLight dirLight;
uniform PointLight pointLights[3];
uniform SpotLight spotLight;
uniform Material material;
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir,int idx);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
void main()
{
    // Ambient

    vec3 ambient = defaultLight.ambient;// * vec3(texture(textureSampler, TexCoords));
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(defaultLight.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * defaultLight.diffuse;
    
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),material.shininess);
    vec3 specular = material.specular * spec * defaultLight.specular;  

   vec3 result =  (ambient + diffuse + specular);
   // result=vec3(0,0,0);
	result+= CalcDirLight(dirLight,norm,viewDir);
     for(int i =0; i < 3; i++)
     {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir,i);     
     }
    result+=CalcSpotLight(spotLight,norm,FragPos,viewDir);
    color = vec4(result, 1.0f) * texture(textureSampler,TexCoords);
} 

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse 
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular 
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient ;
    vec3 diffuse = light.diffuse *diff;
    vec3 specular = light.specular*spec;
    return (ambient + diffuse + specular);
}

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir,int idx)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // Combine results
    vec3 ambient = light.ambient *attenuation;
    vec3 diffuse = light.diffuse * diff*attenuation;
    vec3 specular = light.specular /** spec*/ *attenuation;
   // if(idx==2)
       // specular*=spec;
   // if(idx==2)
    return (ambient + diffuse + specular);
}

// Calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // Spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // Combine results
    vec3 ambient = light.ambient ;
    vec3 diffuse = light.diffuse*diff;
    vec3 specular = light.specular*spec;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}