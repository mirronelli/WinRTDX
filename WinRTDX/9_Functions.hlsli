export float3 mLightIntensity(
    float3 materialColor,
    float3 ambientLight,
    float3 pointLight,
    float3 pointLightPosition,
    float3 pixelWorldPosition,
    float3 pixelNormal,
    float3 specularColor,

    float diffusionIntensity,
    float attenuationQuadratic,
    float attenuationLinear,
    float attenuationConstant,
    float reflectiveness,
    float reflectivePower,
    float3 cameraPosition
)
{
    float3 vectorToLight = pointLightPosition - pixelWorldPosition;
    float3 vectorToCamera = cameraPosition - pixelWorldPosition;
    float distanceToLight = length(vectorToLight);
    
    float3 normalizedNormal = normalize(pixelNormal);
    float3 normalizedvectorToLight = normalize(vectorToLight);
    float angleCoefficient = max(0.0f, dot(normalizedvectorToLight, normalizedNormal));
    
    float attenuation = 1.0f / (attenuationConstant + attenuationLinear * distanceToLight + attenuationQuadratic * distanceToLight * distanceToLight);
    float3 diffusedLight = pointLight * diffusionIntensity * attenuation * angleCoefficient;
    
    float3 reflectionVector = 2.0 * normalizedNormal * dot(normalizedvectorToLight, normalizedNormal) - normalizedvectorToLight;
    float3 specularLight = pow(saturate(dot(reflectionVector, normalize(vectorToCamera))), reflectivePower) * reflectiveness;

    return materialColor * saturate((diffusedLight + ambientLight + specularColor * specularLight * attenuation));
};