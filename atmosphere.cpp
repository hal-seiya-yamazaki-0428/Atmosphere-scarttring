
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "myimGui.h"

#include "atmosphere.h"

//===========================================
//�}�N����`
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
//static�錾
//===========================================
ID3D11ShaderResourceView *CAtmosphere::m_Texture[2];
Atmosphere				  CAtmosphere::m_Atm;
bool					  CAtmosphere::m_Stop;
bool					  CAtmosphere::m_On;
int						  CAtmosphere::m_TimeRate;
float					  CAtmosphere::m_TimeFrame;

//===========================================
//������
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

	//�e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(
		CRenderer::GetDevice(),
		"asset/texture/playbuck.png",
		NULL,
		NULL,
		&m_Texture[0],
		NULL);

	assert(m_Texture[0]);

	//�e�N�X�`���ǂݍ���
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
//�I��
//===========================================
void CAtmosphere::Uninit()
{
	m_Texture[0]->Release();
	m_Texture[1]->Release();
}

//===========================================
//�X�V
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
//�`��
//===========================================
void CAtmosphere::Draw()
{
	//��C�U���ݒ�
	{
		const ImVec2 size = ImVec2(400.0f, 400.0f);
		ImGui::SetNextWindowSize(size, ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2(10.0f, SCREEN_HEIGHT - size.y), ImGuiCond_Once);

		ImGui::Begin("atmosphere", (bool*)false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

		//���Ԑݒ�
		ImGui::SetNextTreeNodeOpen(false, ImGuiCond_Once);
		if (ImGui::TreeNode(u8"���Ԑݒ�"))
		{
			const float image_size = 32.0f;	//�Đ��A�C�R���A��~�A�C�R���̃T�C�Y
			//�Đ��{�^��
			if (ImGui::ImageButton(m_Texture[0], ImVec2(image_size, image_size)))
			{
				m_Stop = false;
			}
			ImGui::SameLine();
			//�ꎞ��~�{�^��
			if (ImGui::ImageButton(m_Texture[1], ImVec2(image_size, image_size)))
			{
				m_Stop = true;
			}
			ImGui::SliderInt("##rate_sldier", &m_TimeRate, 1, 10);

			ImGui::TreePop();
		}
		ImGui::Separator();

		//�F�ݒ�
		ImGui::SetNextTreeNodeOpen(false, ImGuiCond_Once);
		if (ImGui::TreeNode(u8"�F�ݒ�"))
		{
			CMyImGui::ColorPicker3(m_Atm.Color);
			if (ImGui::Button(u8"���Z�b�g"))
			{
				m_Atm.Color = DEFAULT_COLOR;
			}

			ImGui::TreePop();
		}
		ImGui::Separator();

		//���z�x�N�g���ݒ�
		ImGui::SetNextTreeNodeOpen(false, ImGuiCond_Once);
		if (ImGui::TreeNode(u8"���z�x�N�g���ݒ�"))
		{
			ImGui::DragFloat3("##dir_slider", m_Atm.LightDir, 0.01f);
			ImGui::InputFloat3("##dir_input", m_Atm.LightDir);

			ImGui::TreePop();
		}
		ImGui::Separator();

		//���z���ݒ�
		ImGui::SetNextTreeNodeOpen(false, ImGuiCond_Once);
		if (ImGui::TreeNode(u8"���z���ݒ�"))
		{
			ImGui::Text(u8"���邳");
			ImGui::SliderFloat("##sun_power_slider", &m_Atm.SunPower, 0.0f, 100.0f);
			ImGui::InputFloat("##sun_power_input", &m_Atm.SunPower);
			ImGui::Text(u8"�K�E�X");
			ImGui::DragFloat(u8"##g_drag", &m_Atm.g, 0.001f, -0.999f, -0.1f);
			if (ImGui::Button(u8"���Z�b�g"))
			{
				m_Atm.SunPower = DEFAULT_SUN_POWER;
				m_Atm.g = DEFAULT_G;
			}

			ImGui::TreePop();
		}
		ImGui::Separator();

		//�U���ݒ�
		ImGui::SetNextTreeNodeOpen(false, ImGuiCond_Once);
		if (ImGui::TreeNode(u8"�U���ݒ�"))
		{
			ImGui::DragFloat(u8"���C���[�U��", &m_Atm.Kr, 0.001f);
			ImGui::DragFloat(u8"�~�[�U��", &m_Atm.Km, 0.001f);
			if (ImGui::Button(u8"���Z�b�g"))
			{
				m_Atm.Kr = DEFAULT_KR;
				m_Atm.Km = DEFAULT_KM;
			}

			ImGui::TreePop();
		}
		ImGui::Separator();

		//�ő�P�x�ݒ�
		ImGui::SetNextTreeNodeOpen(false, ImGuiCond_Once);
		if (ImGui::TreeNode(u8"�ő�P�x�ݒ�"))
		{
			ImGui::DragFloat(u8"�w���l�i��j", &m_Atm.lum.x, 0.1f, 1.0f, 50.0f);
			ImGui::InputFloat("##lun_input1", &m_Atm.lum.x);
			ImGui::DragFloat(u8"�w���l�i���j", &m_Atm.lum.y, 0.1f, 1.0f, 50.0f);
			ImGui::InputFloat("##lum_input2", &m_Atm.lum.y);
			ImGui::DragFloat(u8"�ΐ��l", &m_Atm.lum.z, 0.1f, 1.0f, 50.0f);
			ImGui::InputFloat("##lum_input3", &m_Atm.lum.z);
			ImGui::DragFloat(u8"�K���}�l", &m_Atm.lum.w, 0.1f, 1.0f, 50.0f);
			ImGui::InputFloat("##lum_input4", &m_Atm.lum.w);
			if (ImGui::Button(u8"���Z�b�g"))
			{
				m_Atm.lum = DEFAULT_LUM;
			}

			ImGui::TreePop();
		}
		ImGui::Separator();

		ImGui::End();
	}
}