
#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 OutDiffuse : SV_Target)
{
	float4 normal = normalize(In.Normal);
	
	//ランバート拡散証明
	float light = 0.5 + 0.5 * -dot(normal.xyz, Light.Direction.xyz);
	light = saturate(light);

	
	OutDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	OutDiffuse.rgb *= In.Diffuse.rgb * Material.Diffuse.rgb * light;
	OutDiffuse.a *= In.Diffuse.a * Material.Diffuse.a;

	
	//リムライト
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev);
	
	float rim = 1.0 + dot(eyev, normal.xyz);
	rim = pow(rim, 5) * 0.6;
	OutDiffuse.rgb += rim;

	////線形フォグ
	//float dist = distance(In.WorldPosition.xyz, CameraPosition.xyz);
	//float fog = saturate(dist / 20.0);
	
	//OutDiffuse.rgb = OutDiffuse.rgb * (1.0 - fog) + float3(0.0, 0.0, 1.0) * fog;
}