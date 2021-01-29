
#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 OutDiffuse : SV_Target)
{
	float MAP_HEIGHT = texsize.y;

	float3 col = weight1.x * g_Texture.Sample(g_SamplerState, float2(In.TexCoord));
	col += weight1.y * (g_Texture.Sample(g_SamplerState, In.TexCoord + float2(0, +2.0f / MAP_HEIGHT)) + g_Texture.Sample(g_SamplerState, In.TexCoord + float2(0, -2.0f / MAP_HEIGHT)));
	col += weight1.z * (g_Texture.Sample(g_SamplerState, In.TexCoord + float2(0, +4.0f / MAP_HEIGHT)) + g_Texture.Sample(g_SamplerState, In.TexCoord + float2(0, -4.0f / MAP_HEIGHT)));
	col += weight1.w * (g_Texture.Sample(g_SamplerState, In.TexCoord + float2(0, +6.0f / MAP_HEIGHT)) + g_Texture.Sample(g_SamplerState, In.TexCoord + float2(0, -6.0f / MAP_HEIGHT)));
	col += weight2.x * (g_Texture.Sample(g_SamplerState, In.TexCoord + float2(0, +8.0f / MAP_HEIGHT)) + g_Texture.Sample(g_SamplerState, In.TexCoord + float2(0, -8.0f / MAP_HEIGHT)));
	col += weight2.y * (g_Texture.Sample(g_SamplerState, In.TexCoord + float2(0, +10.0f / MAP_HEIGHT)) + g_Texture.Sample(g_SamplerState, In.TexCoord + float2(0, -10.0f / MAP_HEIGHT)));
	col += weight2.z * (g_Texture.Sample(g_SamplerState, In.TexCoord + float2(0, +12.0f / MAP_HEIGHT)) + g_Texture.Sample(g_SamplerState, In.TexCoord + float2(0, -12.0f / MAP_HEIGHT)));
	col += weight2.w * (g_Texture.Sample(g_SamplerState, In.TexCoord + float2(0, +14.0f / MAP_HEIGHT)) + g_Texture.Sample(g_SamplerState, In.TexCoord + float2(0, -14.0f / MAP_HEIGHT)));

	OutDiffuse = float4(col, 1.0f);
}