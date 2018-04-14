
cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
};

struct VertexShaderInput
{
	float3 pos : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL0;
};
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL0;
};
PixelShaderInput main(VertexShaderInput input)
{
	float4 pos = float4(input.pos, 1.0f);
	pos = mul(pos, model);
	pos = mul(pos, view);
	pos = mul(pos, projection);

	PixelShaderInput output;
	output.pos = pos;
	output.texCoord = input.texCoord;
	output.normal = input.normal;

	return output;
}