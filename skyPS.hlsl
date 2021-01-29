
#include "common.hlsl"

///大気散乱バッファ
struct ATMOSPHERE
{
	float3 Color;
	float Dummy;
	float3 LightDir;
	float Kr;
	float Km;
	float SunPower;
	float ScaleDepth;
	float g;
};

cbuffer AtmosphereBuffer : register(b7)
{
	ATMOSPHERE atm;
}


Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

static const float fSamples = 2.0;

static const float3 v3InvWaveLength = 1.0 / pow(atm.Color, 4.0);

static const float fInnerRadius = 100.0;
static const float fOuterRadius = 102.5;
static const float PI = 3.141592;

static const float fESun = atm.SunPower;
static const float fKrESun = atm.Kr * fESun;
static const float fKmESun = atm.Km * fESun;

static const float fKr4PI = atm.Kr * 4.0 * PI;
static const float fKm4PI = atm.Km * 4.0 * PI;

static const float fScale = 1.0 / (fOuterRadius - fInnerRadius);
static const float fScaleOverScaleDepth = fScale / atm.ScaleDepth;

static const float g2 = atm.g * atm.g;

float Scale(float fcos)
{
	float x = 1.0 - fcos;
	return atm.ScaleDepth * exp(-0.00287 + x * (0.459 + x * (3.83 + x * (-6.8 + x * 5.25))));
}

//大気圏にヒットした場所
float3 IntersectionPos(float3 dir, float3 a, float radius)
{
	float b = dot(a, dir);
	float c = dot(a, a) - radius * radius;
	float d = max(b * b - c, 0.0);
 
	return a + dir * (-b + sqrt(d));
}

float4 main(in PS_IN input) : SV_Target
{	
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	
	float3 worldPos = input.WorldPosition.xyz;
	worldPos = IntersectionPos(normalize(worldPos), float3(0.0, fInnerRadius, 0.0), fOuterRadius);
	float3 v3CameraPos = float3(0.0, fInnerRadius, 0.0);
	float3 v3LightDir = normalize(atm.LightDir.xyz);
 
	float3 v3Ray = worldPos - v3CameraPos;
	float fFar = length(v3Ray);
	v3Ray /= fFar;
 
	float3 v3Start = v3CameraPos;
	float fCameraHeight = length(v3CameraPos);
	float fStartAngle = dot(v3Ray, v3Start) / fCameraHeight;
	float fStartDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fCameraHeight));
	float fStartOffset = fStartDepth * Scale(fStartAngle);
 
	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * fScale;
	float3 v3SampleRay = v3Ray * fSampleLength;
	float3 v3SamplePoint = v3Start + v3SampleRay * 0.5;
 
	//散乱方程式
	float3 v3FrontColor = 0.0;
	for (int n = 0; n < int(fSamples); n++)
	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
		float fLightAngle = dot(v3LightDir, v3SamplePoint) / fHeight;
		float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
		float fScatter = (fStartOffset + fDepth * (Scale(fLightAngle) - Scale(fCameraAngle)));
		float3 v3Attenuate = exp(-fScatter * (v3InvWaveLength * fKr4PI + fKm4PI));
		v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
		v3SamplePoint += v3SampleRay;
	}
 
	float3 c0 = v3FrontColor * (v3InvWaveLength * fKrESun);
	float3 c1 = v3FrontColor * fKmESun;
	float3 v3Direction = v3CameraPos - worldPos;
 
	float fcos = dot(v3LightDir, v3Direction) / length(v3Direction);
	float fcos2 = fcos * fcos;
 
	float rayleighPhase = 0.75 * (1.0 + fcos2); //レイリー散乱方程式
	float miePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fcos2) / pow(1.0 + g2 - 2.0 * atm.g * fcos, 1.5); //ミー散乱方程式
 
	float4 col = 1.0;
	col.rgb = rayleighPhase * c0 + miePhase * c1;
	return col;
}