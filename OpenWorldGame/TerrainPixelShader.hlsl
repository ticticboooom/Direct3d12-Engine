struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL0;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PixelShaderInput input) : SV_TARGET
{
	float3 DiffuseLightDirection = float3(1, 1, -1);
	float4 DiffuseColor = float4(1, 1, 1, 1);
	float4 textureColor = g_texture.Sample(g_sampler, input.texCoord);
	float4 newColor = textureColor * 0.99f;
	newColor += saturate(dot(DiffuseLightDirection, input.normal) * (DiffuseColor * 0.6) * textureColor);
	newColor.a = 1.0f;
	return newColor;
}