export float3 mLightIntensity(
    float3 ambientLight, 
    float3 pointLight, 
    float3 pointLightPosition, 
    float3 pixelWorldPosition, 
    float3 pixelNormal, 
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

    return saturate(diffusedLight + ambientLight + specularLight * attenuation);
}

struct VsColorInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 color : COLOR;
};

struct VsColorOutput
{
    float3 color : COLOR;
    float4 position : SV_Position;
    float3 worldPosition : POSITION;
    float3 normal : NORMAL;
};

struct VsTextureOutput
{
    float2 textureCoordinates : TEXCOORD;
    float4 position : SV_Position;
    float3 worldPosition : POSITION;
    float3 normal : NORMAL;
};

struct VsTextureInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 textureCoordinates : TEXCOORD;
};

struct LightBuffer
{
    float4 lightPosition;
    float4 lightColor;
    float4 ambientLight;
    float diffueseIntensity;
    float attenuationQuadratic;
    float attenuationLinear;
    float attenuationConstant;
};

struct DrawablePixelBuffer
{
    float reflectiveness;
    float reflectivePower;
};

struct DrawableVertexBuffer
{
    row_major matrix worldTransform;
};

struct VertexPerFrameBuffer  // TODO: rename to frame buffer since this is used per frame
{
    row_major matrix viewPerspectiveTransform;
};

struct PixelPerFrameBuffer  // TODO: rename to frame buffer since this is used per frame
{
    float3 cameraPosition;
};