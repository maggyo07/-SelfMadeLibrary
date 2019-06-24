//グローバル変数
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBffer
{
	float3 cameraPosition;
	float  padding;
};
//タイプ
struct VertexInputType
{
	float4 position	: POSITION;
	float2 tex		: TEXCOORD;
	float3 normal	: NORMAL;
};

struct PixelInputType
{
	float4 position		: SV_POSITION;
	float2 tex			: TEXCOORD;
	float3 normal		: NORMAL;
	float3 viewDirection	:TEXCOORD1;
};

//バーテックスシェーダ
PixelInputType VS(VertexInputType input)
{
	PixelInputType output;
	float4 world_position;	//頂点のワールド位置

	//適切な行列計算のために位置ベクトルを4単位に変更する
	input.position.w = 1.0f;

	//ワールド、ビュー、および投影行列に対して頂点の位置を計算
	output.position = mul (input.position, worldMatrix);
	output.position = mul (output.position, viewMatrix);
	output.position = mul (output.position, projectionMatrix);
	
	//ピクセルシェーダのテクスチャ座標を格納
	output.tex = input.tex;
	
	//法線ベクトルをワールド行列に対してのみ計算する
	output.normal = mul (input.normal, (float3x3) worldMatrix);

	//法線ベクトルを正規化
	output.normal = normalize(output.normal);

	//ワールドの頂点位置を計算
	world_position = mul (input.position,worldMatrix);

	//カメラの位置と世界の頂点の位置に基づいて視線方向を決定する
	output.viewDirection = cameraPosition.xyz - world_position.xyz;

	//視線要綱ベクトルｗ正規化
	output.viewDirection = normalize(output.viewDirection);

	return output;	
}
