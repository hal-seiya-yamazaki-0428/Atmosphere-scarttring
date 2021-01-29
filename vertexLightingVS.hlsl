
#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	
	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);
	
	//ÉâÉìÉoÅ[ÉgägéUè∆ñæ
	//float light = 0.5 * -dot(Light.Direction.xyz, worldNormal.xyz) + 0.5;
	float light = 0.5 * -dot(float3(0.0f, -1.0f, 0.0f), worldNormal.xyz) + 0.5;
	light = saturate(light);
	
	Out.Diffuse = light;
	Out.Diffuse.a = In.Diffuse.a;
	
	Out.Position = mul(In.Position, wvp);
	Out.Normal = worldNormal;
	Out.TexCoord = In.TexCoord;

}