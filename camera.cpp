#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "myimGui.h"
#include "input.h"
#include "mouse.h"
#include "camera.h"

#define PLAYER_RANGE_OUT (3.0f)
#define SPEED (0.3f)

static D3DXVECTOR2 g_Reverse(1.0f, 1.0f);
static float g_MouseSens = 2.0f;
//static bool g_hit = false;

//===========================================
//初期化
//===========================================
void CCamera::Init()
{
	m_Name = "camera";

	m_Position = D3DXVECTOR3(0.0f, 1.0f, -5.0f);
	m_Target = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	m_vecFront = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_vecRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_vecUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vecDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_Angle = 0.0f;
	m_Velocity = 0.0f;
	m_Mouse.velocity.x = m_Mouse.velocity.y = 0.0f;
	m_Mouse.velocity_release.x = m_Mouse.velocity_release.y = 0.0f;
	m_Mouse.pos.x = m_Mouse.pos.y = 0.0f;
	m_Mouse.pos_click.x = m_Mouse.pos_click.y = 0.0f;

	g_Reverse = D3DXVECTOR2(1.0f, 1.0f);
	g_MouseSens = 2.0f;
}

//===========================================
//終了
//===========================================
void CCamera::Uninit()
{

}

//===========================================
//更新
//===========================================
void CCamera::Update()
{
	//右クリック(視点移動)
	if (CInput::GetKeyPress(VK_RBUTTON))
	{
		m_Mouse.pos_click = CMouse::GetMousePos();	//クリック時のマウス座標取得
		//マウスの移動量
		m_Mouse.velocity.x = (m_Mouse.pos.x - m_Mouse.pos_click.x) + m_Mouse.velocity_release.x;
		m_Mouse.velocity.y = (m_Mouse.pos.y - m_Mouse.pos_click.y) + m_Mouse.velocity_release.y;

		//x軸回転の上限設定
		m_Mouse.velocity.y = std::min((LONG)SCREEN_HEIGHT / 2, m_Mouse.velocity.y);

		//注視点の回転用変数
		m_Rotation.x = (float)m_Mouse.velocity.y / (float)SCREEN_HEIGHT * g_MouseSens * g_Reverse.y;
		m_Rotation.y = (float)m_Mouse.velocity.x / (float)SCREEN_WIDTH * g_MouseSens * g_Reverse.x;

		//方向ベクトルの回転
		m_vecFront = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		m_vecRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		D3DXMATRIX rot;
		D3DXMatrixIdentity(&rot);
		D3DXMatrixRotationYawPitchRoll(&rot, -m_Rotation.y, -m_Rotation.x, m_Rotation.z);

		D3DXVec3TransformCoord(&m_vecFront, &m_vecFront, &rot);
		D3DXVec3TransformCoord(&m_vecRight, &m_vecRight, &rot);

	}
	else
	{
		//マウス座標と、クリックリリース時のマウス速度設定
		(CInput::GetKeyRelease(VK_RBUTTON)) ? m_Mouse.velocity_release = m_Mouse.velocity : m_Mouse.pos = CMouse::GetMousePos();
	}

	//前進
	if (CInput::GetKeyPress('W'))
	{
		m_vecDir += m_vecFront;
		m_Velocity = SPEED;
	}
	//後退
	else if (CInput::GetKeyPress('S'))
	{
		m_vecDir -= m_vecFront;
		m_Velocity = SPEED;
	}
	//右移動
	if (CInput::GetKeyPress('D'))
	{
		m_vecDir += m_vecRight;
		m_Velocity = SPEED;
	}
	//左移動
	else if (CInput::GetKeyPress('A'))
	{
		m_vecDir -= m_vecRight;
		m_Velocity = SPEED;
	}

	//上昇
	if (CInput::GetKeyPress(VK_SPACE))
	{
		m_vecDir += m_vecUp;
		m_Velocity = SPEED;
	}
	//下降
	else if (CInput::GetKeyPress(VK_LSHIFT))
	{
		m_vecDir -= m_vecUp;
		m_Velocity = SPEED;
	}

	//進行方向正規化
	D3DXVec3Normalize(&m_vecDir, &m_vecDir);

	m_Position += m_vecDir * m_Velocity;
	m_Velocity *= 0.9f;

	//注視点設定
	m_Target.x = m_Position.x + 5.0f * -sin(m_Rotation.y);
	m_Target.y = m_Position.y + 5.0f * sin(m_Rotation.x);
	m_Target.z = m_Position.z + 5.0f * cos(m_Rotation.y);
}

//===========================================
//描画
//===========================================
void CCamera::Draw()
{
	//ビューマトリクス設定
	D3DXMatrixLookAtLH(&m_ViewMatrix, &m_Position, &m_Target, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	CRenderer::SetViewMatrix(&m_ViewMatrix);

	//プロジェクションマトリクス設定
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	CRenderer::SetProjectionMatrix(&m_ProjectionMatrix);

	CRenderer::SetCameraPosition(m_Position);


	//GUI
	{
		////3方向ベクトル
		//std::string str = "front.x = " + std::to_string(m_vecFront.x) + "  front.y = " + std::to_string(m_vecFront.y) + "  front.z = " + std::to_string(m_vecFront.z);
		//ImGui::Text(str.c_str());

		//std::string str4 = "right.x = " + std::to_string(m_vecRight.x) + "  right.y = " + std::to_string(m_vecRight.y) + "  right.z = " + std::to_string(m_vecRight.z);
		//ImGui::Text(str4.c_str());

		//std::string str5 = "dir.x = " + std::to_string(m_vecDir.x) + "  dir.y = " + std::to_string(m_vecDir.y) + "  dir.z = " + std::to_string(m_vecDir.z);
		//ImGui::Text(str5.c_str());

		////当たり判定
		////ImGui::Text(std::to_string(g_hit).c_str());

		//ImGui::Text(u8"テストメニュー");

		////座標
		//std::string str2 = "rotation.x = " + std::to_string(m_Rotation.x) + "  rotation.y = " + std::to_string(m_Rotation.y);
		//ImGui::Text(str2.c_str());

		////速度
		//std::string str3 = "velocity.x = " + std::to_string(m_Mouse.velocity.x) + "  velocity.y = " + std::to_string(m_Mouse.velocity.y);
		//ImGui::Text(str3.c_str());

		//std::string str4 = "width = " + std::to_string((float)m_Mouse.velocity.x / (float)SCREEN_WIDTH) + "  height = " + std::to_string(SCREEN_HEIGHT);
		//ImGui::Text(str4.c_str());
	}

}

//===========================================
//逆行列の取得
//===========================================
D3DXMATRIX CCamera::GetInv()
{
	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView, NULL, &m_ViewMatrix);
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;

	return invView;
}

//===========================================
//カメラとオブジェクトの距離
//===========================================
float CCamera::GetCameraToLength(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 length_vec = pos - m_Position;
	return D3DXVec3Length(&length_vec);
}