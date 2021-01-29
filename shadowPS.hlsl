
#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_DepthTexture : register(t1);
SamplerState g_SamplerState : register(s0);

float VSM_Filter(float2 texcoord, float fragDepth)
{
	float4 depth = g_DepthTexture.Sample(g_SamplerState, texcoord);

	float depth_sq = depth.x * depth.x;
	float variance = depth.y - depth_sq;
	variance = max(variance, 0.01);
	float md = fragDepth - depth.x;
	float p = variance / (variance + (md * md));
	
	return saturate(max(p, fragDepth <= depth.x));
}

void main(in PS_IN In, out float4 OutDiffuse : SV_Target)
{
	OutDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	OutDiffuse *= In.Diffuse;
	
	In.ShadowPosition.xyz /= In.ShadowPosition.w;
	In.ShadowPosition.x = In.ShadowPosition.x * 0.5 + 0.5;
	In.ShadowPosition.y = -In.ShadowPosition.y * 0.5 + 0.5;
	
	// [“xƒ}ƒbƒv‚Ì”ÍˆÍ“à‚Í‰e‚Â‚¯
	if (!(In.ShadowPosition.x < 0.0 || In.ShadowPosition.x > 1.0 || In.ShadowPosition.y < 0.0 || In.ShadowPosition.y > 1.0))
	{
		OutDiffuse *= VSM_Filter(In.ShadowPosition.xy, In.ShadowPosition.z);
	}

	OutDiffuse.a = 1.0;
}