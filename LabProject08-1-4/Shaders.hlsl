cbuffer cbFrameworkInfo : register(b0)
{
    float gfCurrentTime;
    float gfElapsedTime;
    float2 gf2CursorPos;
};

cbuffer cbCameraInfo : register(b1)
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
};

cbuffer cbPlayerInfo : register(b2)
{
    matrix gmtxPlayerWorld : packoffset(c0);
};

cbuffer cbGameObjectInfo : register(b3)
{
    matrix gmtxGameObject : packoffset(c0);
    matrix gmtxTextureTransforms[2] : packoffset(c4);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
struct VS_DIFFUSED_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VS_DIFFUSED_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VS_DIFFUSED_OUTPUT VSPlayer(VS_DIFFUSED_INPUT input)
{
    VS_DIFFUSED_OUTPUT output;

    output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxPlayerWorld), gmtxView), gmtxProjection);
    output.color = input.color;

    return (output);
}

float4 PSPlayer(VS_DIFFUSED_OUTPUT input) : SV_TARGET
{
    return (input.color);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Texture2D gtxtTexture : register(t0);
SamplerState gSamplerState : register(s0);

struct VS_TEXTURED_INPUT
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_TEXTURED_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VS_TEXTURED_OUTPUT VSTextured(VS_TEXTURED_INPUT input)
{
    VS_TEXTURED_OUTPUT output;

    output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
    output.uv = input.uv;

    return (output);
}

float4 PSTextured(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
    float4 cColor = gtxtTexture.Sample(gSamplerState, input.uv);

    return (cColor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Texture2D gtxtTerrainBaseTexture : register(t1);
Texture2D gtxtTerrainDetailTexture : register(t2);

struct VS_TERRAIN_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

struct VS_TERRAIN_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

VS_TERRAIN_OUTPUT VSTerrain(VS_TERRAIN_INPUT input)
{
    VS_TERRAIN_OUTPUT output;

    output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
    output.color = input.color;
    output.uv = input.uv;

    return (output);
}

float4 PSTerrain(VS_TERRAIN_OUTPUT input) : SV_TARGET
{
    float4 cBaseTexColor = gtxtTerrainBaseTexture.Sample(gSamplerState, input.uv);
    float4 cColor = input.color * cBaseTexColor;

    return (cColor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Texture2D gtxtTextures[3] : register(t3);

struct VS_TRANSFORMED_TEXTURED_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct VS_TRANSFORMED_TEXTURED_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv0 : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
};

VS_TRANSFORMED_TEXTURED_OUTPUT VSTextureTransform(VS_TRANSFORMED_TEXTURED_INPUT input)
{
    VS_TRANSFORMED_TEXTURED_OUTPUT output;

    output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
    output.uv0 = mul(float3(input.uv, 1.0f), (float3x3)gmtxTextureTransforms[0]).xy;
    output.uv1 = mul(float3(input.uv, 1.0f), (float3x3)gmtxTextureTransforms[1]).xy;
	 
    return (output);
}

float4 PSTextureTransform(VS_TRANSFORMED_TEXTURED_OUTPUT input) : SV_TARGET
{
    float4 cColor0 = gtxtTextures[0].Sample(gSamplerState, input.uv0); //Main + Alpha
    float4 cColor1 = gtxtTextures[1].Sample(gSamplerState, input.uv1); //Lava Texture
    float4 cColor = lerp(cColor1, cColor0, cColor0.a);

    return (cColor);
}

float4 PSTextureTransformDistortion(VS_TRANSFORMED_TEXTURED_OUTPUT input) : SV_TARGET
{
    float4 cColor0 = gtxtTextures[0].Sample(gSamplerState, input.uv0); //Main + Alpha
    float4 cColor2 = gtxtTextures[2].Sample(gSamplerState, input.uv0); //Distortion Texture
    float2 uv = input.uv1 - cColor2.r * float2(gmtxTextureTransforms[1]._41, gmtxTextureTransforms[1]._42);
    float4 cColor1 = gtxtTextures[1].Sample(gSamplerState, uv); //Lava Texture
    float4 cColor = lerp(cColor1, cColor0, cColor0.a);

    return (cColor);
}

float4 PSTextureTransformDistortionTransparent(VS_TRANSFORMED_TEXTURED_OUTPUT input) : SV_TARGET
{
    float4 cColor0 = gtxtTextures[0].Sample(gSamplerState, input.uv0); //Main + Alpha
    float4 cColor2 = gtxtTextures[2].Sample(gSamplerState, input.uv0); //Distortion Texture
    float2 uv = input.uv1 - cColor2.r * float2(gmtxTextureTransforms[1]._41, gmtxTextureTransforms[1]._42);
    float4 cColor1 = gtxtTextures[1].Sample(gSamplerState, uv); //Lava Texture(Transparent)
    float4 cColor = lerp(cColor1, cColor0, cColor0.a);

    return (cColor);
}
