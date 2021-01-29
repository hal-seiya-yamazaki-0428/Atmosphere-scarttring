
#include "common.hlsl"

void main(in PS_IN In, out float4 OutDiffuse : SV_Target)
{
	OutDiffuse.rgb = In.Diffuse.rgb * Material.Diffuse.rgb;
	OutDiffuse.a = Material.Diffuse.a;
}