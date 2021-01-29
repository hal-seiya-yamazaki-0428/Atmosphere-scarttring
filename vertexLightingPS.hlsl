
#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 OutDiffuse : SV_Target)
{
	OutDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	OutDiffuse *= In.Diffuse;
}