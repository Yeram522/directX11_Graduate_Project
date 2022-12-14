////////////////////////////////////////////////////////////////////////////////
// Filename: skydome_ps.hlsl
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer GradientBuffer
{
	float4 apexColor;
	float4 centerColor;
};


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
	float4 domePosition : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 SkyDomePixelShader(PixelInputType input) : SV_TARGET
{
	// 이 픽셀이 있는 스카이 돔에서 위치를 결정합니다.
	float height = input.domePosition.y;

	// 값의 범위는 -1.0f에서 + 1.0f이므로 양수 값만 변경하십시오.
	if(height < 0.0)
	{
		height = 0.0f;
	}

	// 스카이 돔에서 픽셀의 높이를 기준으로 정점과 중심을 보정하여 그라디언트 색상을 결정합니다.
	float4 outputColor = lerp(centerColor, apexColor, height);

    return outputColor;
}