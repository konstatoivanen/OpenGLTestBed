#pragma once
#include HLSLSupport.glsl

struct PKRawPointLight
{
    float4 color;
    float4 position;
};

struct PKRawSpotLight
{
    float4 color;
    float4 position;
    float4 direction;
};

struct PKLight
{
    float3 color;
    float3 direction;
};

struct PKIndirect
{
    float3 diffuse;
    float3 specular;
};

struct PKGI
{
    PKLight light;
    PKIndirect indirect;
};

struct SurfaceData
{
    float3 albedo;      
    float3 normal;      
    float3 emission;
    float metallic;     
    float roughness;
    float occlusion;
    float alpha;
};

PKLight EmptyLight()
{
    return PKLight(float3(0,0,0), float3(0,1,0));
}

PKIndirect EmptyIndirect()
{
    return PKIndirect(float3(0,0,0), float3(0,0,0));
}

uniform int pk_LightCount;
PK_DECLARE_BUFFER(PKRawPointLight, pk_Lights);