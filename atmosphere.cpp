
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "myimGui.h"

#include "atmosphere.h"

//===========================================
//マクロ定義
//===========================================
#define DEFAULT_COLOR (D3DXVECTOR3(0.68f, 0.55f, 0.44f))
#define DEFAULT_KR (0.0025f)
#define DEFAULT_KM (0.001f)
#define DEFAULT_SUN_POWER (20.0f)
#define DEFAULT_G (-0.99f)
#define DEFAULT_LUM_X (3.0f)
#define DEFAULT_LUM_Y (5.5f)
#define DEFAULT_LUM_Z (6.3f)
#define DEFAULT_LUM_W (2.2f)
#define DEFAULT_LUM (D3DXVECTOR4(DEFAULT_LUM_X, DEFAULT_LUM_Y, DEFAULT_LUM_Z, DEFAULT_LUM_W))

//===========================================
//static宣言
//===========================================
ID3D11ShaderResourceView *CAtmosphere::m_Texture[2];
Atmosphere				  CAtmosphere::m_Atm;
bool					  CAtmosphere::m_Stop;
bool					  CAtmosphere::m_On;
int						  CAtmosphere::m_TimeRate;
float					  CAtmosphere::m_TimeFrame;

//===========================================
//初期化
//===========================================
void CAtmosphere::Init()
{
	m_Atm.Color = DEFAULT_COLOR;
	m_Atm.LightDir = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_Atm.Kr = DEFAULT_KR;
	m_Atm.Km = DEFAULT_KM;
	m_Atm.SunPower = DEFAULT_SUN_POWER;
	m_Atm.ScaleDepth = 0.25f;
	m_Atm.g = DEFAULT_G;
	m_Atm.lum = DEFAULT_LUM;

	m_Stop = false;
	m_On = false;
	m_TimeRate = 1;
	m_TimeFrame = 0;

	CRenderer::SetAtmosphere(m_Atm);

	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(
		CRenderer::GetDevice(),
		"asset/texture/playbuck.png",
		NULL,
		NULL,
		&m_Texture[0],
		NULL);

	assert(m_Texture[0]);

	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(
		CRenderer::GetDevice(),
		"asset/texture/stop.png",
		NULL,
		NULL,
		&m_Texture[1],
		NULL);

	assert(m_Texture[1]);
}

//===========================================
//終了
//===========================================
void CAtmosphere::Uninit()
{
	m_Texture[0]->Release();
	m_Texture[1]->Release();
}

//===========================================
//更新
//===========================================
void CAtmosphere::Update()
{
	CRenderer::SetAtmosphere(m_Atm);

	if (m_Stop)
	{
		return;
	}

	m_Atm.LightDir.x = -cos(m_TimeFrame) * 2.0f;
	m_Atm.LightDir.y = sin(m_TimeFrame) * 1.0f;

	m_TimeFrame += 0.001f * (float)m_TimeRate;
}

//===========================================
//描画
//===========================================
void CAtmosphere::Draw()
{
	//大気散乱設定
	{
		const ImVec2 size = ImVec2(400.0f, 400.0f);
		ImGui::SetNextWindowSize(size, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2(10.0f, SCREEN_HEIGHT - size.y), ImGuiCond_Once);

		ImGui::Begin("atmosphere", (bool*)false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

		//時間設定
		ImGui::SetNextTreeNodeOpen(false, ImGuiCond_Once);
		if (ImGui::TreeNode(u8"時間設定"))
		{
			const float image_size = 32.0f;	//再生アイコン、停止アイコンのサイズ
			//再生ボタン
			if (ImGui::ImageButton(m_Texture[0], ImVec2(image_size, image_size)))
			{
				m_Stop = false;
			}
			ImGui::SameLine();
			//一時停止ボタン
			if (ImGui::ImageButton(m_Texture[1], ImVec2(image_size, image_size)))
			{
				m_Stop = true;
			}
			ImGui::SliderInt("##rate_sldier", &m_TimeRate, 1, 10);

			ImGui::TreePop();
		}
		ImGui::Separator();

		//色設定
		ImGui::SetNextTreeNodeOpen(false, ImGuiCond_Once);
		if (ImGui::TreeNode(u8"色設定"))
		{
			CMyImGui::ColorPicker3(m_Atm.Color);
			if (ImGui::Button(u8"リセット"))
			{
				m_Atm.Color = DEFAULT_COLOR;
			}

			ImGui::TreePop();
		}
		ImGui::Separator();

		//太陽ベクトル設定
		ImGui::SetNextTreeNodeOpen(false, ImGuiCond_Once);
		if (ImGui::TreeNode(u8"太陽ベクトル設定"))
		{
			ImGui::DragFloat3("##dir_slider", m_Atm.LightDir, 0.01f);
			ImGui::InputFloat3("##dir_input", m_Atm.LightDir);

			ImGui::TreePop();
		}
		ImGui::Separator();

		//太陽光設定
		ImGui::SetNextTreeNodeOpen(false, ImGuiCond_Once);
		if (ImGui::TreeNode(u8"太陽光設定"))
		{
			ImGui::Text(u8"明るさ");
			ImGui::SliderFloat("##sun_power_slider", &m_Atm.SunPower, 0.0f, 100.0f);
			ImGui::InputFloat("##sun_power_input", &m_Atm.SunPower);
			ImGui::Text(u8"ガウス");
			ImGui::DragFloat(u8"##g_drag", &m_Atm.g, 0.001f, -0.999f, -0.1f);
			if (ImGui::Button(u8"リセット"))
			{
				m_Atm.SunPower = DEFAULT_SUN_POWER;
				m_Atm.g = DEFAULT_G;
			}

			ImGui::TreePop();
		}
		ImGui::Separator();

		//散乱設定
		ImGui::SetNextTreeNodeOpen(false, ImGuiCond_Once);
		if (ImGui::TreeNode(u8"散乱設定"))
		{
			ImGui::DragFloat(u8"レイリー散乱", &m_Atm.Kr, 0.001f);
			ImGui::DragFloat(u8"ミー散乱", &m_Atm.Km, 0.001f);
			if (ImGui::Button(u8"リセット"))
			{
				m_Atm.Kr = DEFAULT_KR;
				m_Atm.Km = DEFAULT_KM;
			}

			ImGui::TreePop();
		}
		ImGui::Separator();

		//最大輝度設定
		ImGui::SetNextTreeNodeOpen(false, ImGuiCond_Once);
		if (ImGui::TreeNode(u8"最大輝度設定"))
		{
			ImGui::DragFloat(u8"指数値（大）", &m_Atm.lum.x, 0.1f, 1.0f, 50.0f);
			ImGui::InputFloat("##lun_input1", &m_Atm.lum.x);
			ImGui::DragFloat(u8"指数値（小）", &m_Atm.lum.y, 0.1f, 1.0f, 50.0f);
			ImGui::InputFloat("##lum_input2", &m_Atm.lum.y);
			ImGui::DragFloat(u8"対数値", &m_Atm.lum.z, 0.1f, 1.0f, 50.0f);
			ImGui::InputFloat("##lum_input3", &m_Atm.lum.z);
			ImGui::DragFloat(u8"ガンマ値", &m_Atm.lum.w, 0.1f, 1.0f, 50.0f);
			ImGui::InputFloat("##lum_input4", &m_Atm.lum.w);
			if (ImGui::Button(u8"リセット"))
			{
				m_Atm.lum = DEFAULT_LUM;
			}

			ImGui::TreePop();
		}
		ImGui::Separator();

		ImGui::End();
	}
}