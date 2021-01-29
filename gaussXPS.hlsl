
#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 OutDiffuse : SV_Target)
{
	float MAP_WIDTH = texsize.x;

	float3 col = weight1.x * g_Texture.Sample(g_SamplerState, float2(In.TexCoord) + float2(+1.0f / MAP_WIDTH, 0));
	col += weight1.y * (g_Texture.Sample(g_SamplerState, In.TexCoord + float2(+3.0f / MAP_WIDTH, 0)) + g_Texture.Sample(g_SamplerState, In.TexCoord + float2(-3.0f / MAP_WIDTH, 0)));
	col += weight1.z * (g_Texture.Sample(g_SamplerState, In.TexCoord + float2(+5.0f / MAP_WIDTH, 0)) + g_Texture.Sample(g_SamplerState, In.TexCoord + float2(-5.0f / MAP_WIDTH, 0)));
	col += weight1.w * (g_Texture.Sample(g_SamplerState, In.TexCoord + float2(+7.0f / MAP_WIDTH, 0)) + g_Texture.Sample(g_SamplerState, In.TexCoord + float2(-7.0f / MAP_WIDTH, 0)));
	col += weight2.x * (g_Texture.Sample(g_SamplerState, In.TexCoord + float2(+9.0f / MAP_WIDTH, 0)) + g_Texture.Sample(g_SamplerState, In.TexCoord + float2(-9.0f / MAP_WIDTH, 0)));
	col += weight2.y * (g_Texture.Sample(g_SamplerState, In.TexCoord + float2(+11.0f / MAP_WIDTH, 0)) + g_Texture.Sample(g_SamplerState, In.TexCoord + float2(-11.0f / MAP_WIDTH, 0)));
	col += weight2.z * (g_Texture.Sample(g_SamplerState, In.TexCoord + float2(+13.0f / MAP_WIDTH, 0)) + g_Texture.Sample(g_SamplerState, In.TexCoord + float2(-13.0f / MAP_WIDTH, 0)));
	col += weight2.w * (g_Texture.Sample(g_SamplerState, In.TexCoord + float2(+15.0f / MAP_WIDTH, 0)) + g_Texture.Sample(g_SamplerState, In.TexCoord + float2(-15.0f / MAP_WIDTH, 0)));

	OutDiffuse = float4(col, 1.0f);
}