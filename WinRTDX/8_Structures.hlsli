//vertices
struct VertexWithNormal
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct VertexWithColor
{
    float3 position : POSITION;
    float3 color : COLOR;
};

struct VertexWithNormalColor
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 color : COLOR;
};

struct VertexWithNormalTexture
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
struct PixelWithColor
{
    float3 worldPosition : POSITION;
    float4 position : SV_Position;
    float3 color : COLOR;
};

struct PixelWithNormal
{
    float3 worldPosition : POSITION;
    float4 position : SV_Position;
    float3 normal : NORMAL;
};

struct PixelWithNormalColor
{
    float3 worldPosition : POSITION;
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float3 color : COLOR;
};

struct PixelWithNormalTexture
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
};

struct PixelPerInstanceColorSpecularBuffer
{
    float4 color;
    float reflectiveness;
    float reflectivePower;
    float2 padding;
};

struct PixelPerFrameCameraBuffer
{
    float3 cameraPosition;
};