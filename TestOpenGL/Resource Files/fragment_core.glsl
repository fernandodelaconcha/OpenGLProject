#version 440

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffuseTex;
	sampler2D specularTex;
};

struct PointLight
{
	vec3 position;
	float intensity;
	vec3 color;
	float constant;
	float linear;
	float quadratic;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

// Uniforms
uniform Material material;
uniform PointLight pointLight;
uniform vec3 cameraPos;

//Functions
vec3 calculateAmbient(Material material)
{
	return material.ambient;
}
vec3 calculateDiffuse(Material material, vec3 vs_position, vec3 vs_normal, vec3 position)
{
	vec3 posToLightDirVec = normalize(position - vs_position);
	float diffuse = clamp(dot(posToLightDirVec, normalize(vs_normal)), 0, 1);
	return material.diffuse * diffuse;
}
vec3 calculateSpecular(Material material, vec3 vs_position, vec3 vs_normal, vec3 position, vec3 cameraPos)
{
	vec3 lightToPosDirVec = normalize(vs_position - position);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
	vec3 posToViewDirVec = normalize(cameraPos - vs_position);
	float speculatConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 30);
	return material.specular * speculatConstant * texture(material.specularTex, vs_texcoord).rgb;
}

void main()
{
	vec3 ambientFinal = calculateAmbient(material);
	vec3 diffuseFinal = calculateDiffuse(material, vs_position, vs_normal, pointLight.position);
	vec3 specularFinal = calculateSpecular(material, vs_position, vs_normal, pointLight.position, cameraPos);
	
	float distance = length(pointLight.position - vs_position);
	float attenuation = pointLight.constant / (1.f + pointLight.linear * distance + pointLight.quadratic * (distance * distance));

	ambientFinal *= attenuation;
	diffuseFinal *= attenuation;
	specularFinal *= attenuation;
	
	fs_color = texture(material.diffuseTex, vs_texcoord) * (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
	//fs_color = (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
}