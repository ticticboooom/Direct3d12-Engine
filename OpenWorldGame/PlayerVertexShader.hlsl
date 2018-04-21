static const int maxBonesCount = 19;
static const matrix Identity =
{
	{ 1, 0, 0, 0 },
	{ 0, 1, 0, 0 },
	{ 0, 0, 1, 0 },
	{ 0, 0, 0, 1 }
};

cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
};

cbuffer AnimationFrame : register(b1)
{
	float4x4 bones[maxBonesCount];
};

//cbuffer Rotator : register(b2)
//{
//	matrix rotator;
//};

struct VertexShaderInput
{
	float3 pos : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL0;
	float4 weights1 : TEXCOORD1;
	float4 weights2 : TEXCOORD2;
	float4 bones1 : TEXCOORD3;
	float4 bones2 : TEXCOORD4;
	int isAnimated : TEXCOORD5;
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

	float4x4 transform;
	if (input.isAnimated == 1) {
		if (input.weights1.x > 0) {
			transform = input.weights1.x *  bones[input.bones1.x];
		}
		if (input.weights1.y > 0) {
			transform += input.weights1.y * bones[input.bones1.y];
		}
		if (input.weights1.z > 0) {
			transform += input.weights1.z *  bones[input.bones1.z];
		}
		if (input.weights1.w > 0) {
			transform += input.weights1.w * bones[input.bones1.w];
		}

		if (input.weights2.x > 0) {
			transform += input.weights2.x * bones[input.bones2.x];
		}
		if (input.weights2.y > 0) {
			transform += input.weights2.y * bones[input.bones2.y];
		}
		if (input.weights2.z > 0) {
			transform += input.weights2.z * bones[input.bones2.z];
		}
		if (input.weights2.w > 0) {
			transform += input.weights2.w * bones[input.bones2.w];
		}
	}

	PixelShaderInput output;

	if (input.isAnimated == 1 && any(transform)) {
		pos = mul(pos, transform);
	}

	//pos = mul(pos, rotator);
	pos = mul(pos, model);
	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.pos = pos;
	output.texCoord = input.texCoord;
	if (input.isAnimated == 1) {
		output.normal = normalize(mul(input.normal, transform));
	}
	//output.normal = normalize(mul(input.normal, rotator));
	output.normal = normalize(mul(input.normal, model));
	return output;
}