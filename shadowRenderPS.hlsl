
#include "common.hlsl"

void main(in PS_IN In, out float4 OutDiffuse : SV_Target)
{
	//float z = In.ShadowPosition.z / 100.0;
	float z = In.ShadowPosition.z / In.ShadowPosition.w;

	OutDiffuse.r = z;
	OutDiffuse.g = z * z;
	OutDiffuse.b = 0.0;
	OutDiffuse.a = 1.0;
}