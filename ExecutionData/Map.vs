//グローバル--------------------------------
//行列
cbuffer MaterixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

//カメラの位置情報
cbuffer CameraBuffer
{
	float3 camera_position;
};
//------------------------------------------

//タイプ
struct VertexInputType
{
	float4 position	: POSITION;				//位置
	float2 tex		: TEXCOORD0;			//UV
	float3 normal	: NORMAL;				//法線
	float3 tangent	: TANGENT;				//接空間の情報
	float3 binormal	: BINORMAL;				//normalとtangentの外積(Y軸)
};

struct PixelInputType
{
	float4 position	: SV_POSITION;			//位置
	float2 tex		: TEXCOORD0;			//UV
	float3 normal	: NORMAL;				//法線
	float3 tangent	: TANGENT;				//接空間の情報
	float3 binormal	: BINORMAL;				//normalとtangentの外積(Y軸)
	float3 view_direction	: TEXCOORD1;	//カメラの方向情報
};

PixelInputType MapVertexShader(VertexInputType input)
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

	//鏡面反射光の計算用---------------------
	//ワールドの頂点の位置を計算
	float4 world_position = mul(input.position, worldMatrix);

	//カメラの位置とワールドの頂点の位置に基づいて表示方向おｗ決定
	output.view_direction = camera_position.xyz - world_position.xyz;

	//表示方向ベクトルを正規化
	output.view_direction = normalize(output.view_direction);

	//---------------------------------------

	return output;
}