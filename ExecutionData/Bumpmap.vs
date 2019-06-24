//グローバル
cbuffer MaterixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

//タイプ
struct VertexInputType
{
	float4 position	: POSITION;
	float2 tex		: TEXCOORD0;
	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
	float3 binormal	: BINORMAL;
};

struct PixelInputType
{
	float4 position	: SV_POSITION;
	float2 tex		: TEXCOORD0;
	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
	float3 binormal	: BINORMAL;
};

PixelInputType BumpMapVertexShader(VertexInputType input)
{
	PixelInputType output;

	//適切な行列計算のために位置ベクトルを4単位に変更
	input.position.w = 1.0f;

	//ワールド、ビュー、投影行列に対して頂点の位置を計算
	output.position = mul (input.position, worldMatrix);
	output.position = mul (output.position,viewMatrix);
	output.position = mul (output.position,projectionMatrix);

	//ピクセルシェーダのテクスチャ座標を格納
	output.tex = input.tex;

	//ワールド行列に対してのみ法線ベクトルを計算してから、最終地を正規化する
	output.normal = mul(input.normal,(float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	//ワールド行列に対してのみ接線ベクトルを計算してから、最終値を正規化する
	output.tangent = mul(input.tangent,(float3x3) worldMatrix);
	output.tangent = normalize(output.tangent);

	//ワールド行列に対してのみ二項ベクトルを計算し、それから最終値を正規化する
	output.binormal = mul(input.binormal,(float3x3) worldMatrix);
	output.binormal = normalize(output.binormal);

	return output;
}