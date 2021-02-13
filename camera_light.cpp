
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "myimGui.h"

#include "camera_light.h"
#include "atmosphere.h"

static float g_distance = 15.0f;

void CLightCamera::Init()
{
	m_Name = "camera";

	m_Position = D3DXVECTOR3(0.0f, 1.0f, -5.0f);
	m_Target = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	m_Near = 10.0f;
	m_Far = 100.0f;
	g_distance = 15.0f;

	m_vecFront = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_vecRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vecDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void CLightCamera::Uninit()
{

}

void CLightCamera::Update()
{
	m_Position = CAtmosphere::GetLightDir() * g_distance;
}

void CLightCamera::Draw()
{
	LIGHT light;
	light.Enable = true;
	light.Direction = -D3DXVECTOR4(CAtmosphere::GetLightDir().x, CAtmosphere::GetLightDir().y, CAtmosphere::GetLightDir().z, 1.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ビューマトリクス設定
	D3DXMatrixLookAtLH(&light.ViewMatrix, &m_Position, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	//プロジェクションマトリクス設定
	D3DXMatrixPerspectiveFovLH(&light.ProjectionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, m_Near, m_Far);

	CRenderer::SetViewMatrix(&light.ViewMatrix);
	CRenderer::SetProjectionMatrix(&light.ProjectionMatrix);

	CRenderer::SetCameraPosition(m_Position);

	CRenderer::SetLight(light);
}

void CLightCamera::ImGuiDraw()
{
	const ImVec2 size = ImVec2(256.0f, 256.0f);

	ImGui::SetNextWindowSize(size, ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 400.0f), ImGuiCond_Once);

	ImGui::Begin(u8"影ライトデバッグ");

	//Nearの設定
	ImGui::SetNextTreeNodeOpen(false, ImGuiCond_Once);
	if (ImGui::TreeNode(u8"Near"))
	{
		ImGui::Text(u8"スライダー");
		ImGui::SliderFloat("##Near_slider", &m_Near, 1.0f, 100.0f);
		ImGui::Text(u8"入力");
		ImGui::InputFloat("##Near_input", &m_Near);
		if (ImGui::Button(u8"リセット"))
		{
			m_Near = 10.0f;
		}

		ImGui::TreePop();
	}
	ImGui::Separator();

	//Farの設定
	ImGui::SetNextTreeNodeOpen(false, ImGuiCond_Once);
	if (ImGui::TreeNode(u8"Far"))
	{
		ImGui::Text(u8"スライダー");
		ImGui::SliderFloat("##Far_slider", &m_Far, 10.0f, 1000.0f);
		ImGui::Text(u8"入力");
		ImGui::InputFloat("##Far_input", &m_Far);
		if (ImGui::Button(u8"リセット"))
		{
			m_Far = 100.0f;
		}

		ImGui::TreePop();
	}
	ImGui::Separator();

	//距離の設定
	ImGui::SetNextTreeNodeOpen(false, ImGuiCond_Once);
	if (ImGui::TreeNode(u8"距離"))
	{
		ImGui::Text(u8"スライダー");
		ImGui::SliderFloat("##Distance_slider", &g_distance, 1.0f, 100.0f);
		ImGui::Text(u8"入力");
		ImGui::InputFloat("##Distance_input", &g_distance);
		if (ImGui::Button(u8"リセット"))
		{
			g_distance = 15.0f;
		}

		ImGui::TreePop();
	}
	ImGui::Separator();

	ImGui::End();
}