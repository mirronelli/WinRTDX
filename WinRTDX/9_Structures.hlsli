//vertices
struct VertexSimple
{
    float3 position : POSITION;
};

struct VertexSimpleWithNormal
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct VertexColored
{
    float3 position : POSITION;
    float3 color : COLOR;
};

struct VertexColoredWithNormal
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 color : COLOR;
};

struct VertexTextured
{
    float3 position : POSITION;
    float2 textureCoordinates : TEXCOORD;
};

struct VertexTexturedWithNormal
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 textureCoordinates : TEXCOORD;
};

struct VertexPerInstanceTransformBuffer
{
    row_major matrix worldTransform;
};

struct VertexPerFrameTransformBuffer
{
    row_major matrix viewPerspectiveTransform;
};


// pixels
struct PixelSimple
{
    float3 worldPosition : POSITION;
    float4 position : SV_Position;
};

struct PixelSimpleWithNormal
{
    float3 worldPosition : POSITION;
    float4 position : SV_Position;
    float3 normal : NORMAL;
};

struct PixelColored
{
    float3 worldPosition : POSITION;
    float4 position : SV_Position;
    float3 color : COLOR;
};

struct PixelColoredWithNormal
{
    float3 worldPosition : POSITION;
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float3 color : COLOR;
};

struct PixelTextured
{
    float3 worldPosition : POSITION;
    float4 position : SV_Position;
    float2 textureCoordinates : TEXCOORD;
};

struct PixelTexturedWithNormal
{
    float3 worldPosition : POSITION;
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float2 textureCoordinates : TEXCOORD;
};

struct PixelPerLevelLightBuffer
{
    float4 lightPosition;
    float4 lightColor;
    float4 ambientLight;
    float diffueseIntensity;
    float attenuationQuadratic;
    float attenuationLinear;
    float attenuationConstant;
};

struct PixelPerInstanceSpecularBuffer
{
    float reflectiveness;
    float reflectivePower;
    float2 padding;
};

struct PixelPerInstanceColorSpecularBuffer
{
    float4 color;
    float reflectiveness;
    float reflectivePower;
    float2 padding;
};

struct PixelPerInstanceAllColorsBuffer
{
    float4 diffuseColor;
    float4 specularColor;
    float4 ambientColor;
    float4 emmisiveColor;
    float reflectiveness;
    float reflectivePower;
    float2 padding;
};

struct PixelPerFrameCameraBuffer
{
    float3 cameraPosition;
};