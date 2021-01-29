
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "mouse.h" 
#include "game_object.h"
#include "light.h"
#include "scene.h"
#include "shadow_scene.h"
#include "shader.h"
#include "atmosphere.h"
#include "myimGui.h"
#include "camera_light.h"

#include "model.h"

//===========================================
//�O���[�o���ϐ�
//===========================================
CScene *CManager::m_Scene = NULL;
CScene *CManager::m_ShadowScene = NULL;
CPolygon* g_Polygon;

//===========================================
//������
//===========================================
void CManager::Init()
{
	CRenderer::Init();
	CInput::Init();
	CMouse::Init();
	CMyImGui::Init();
	CShader::ToonTextureLoad();
	CLight::SetDefaultLight();
	CAtmosphere::Init();

	m_Scene = new CScene;
	m_Scene->Init();

	m_ShadowScene = new CShadowScene;
	m_ShadowScene->Init();

	g_Polygon = new CPolygon;
	g_Polygon->Init();
}

//===========================================
//�I��
//===========================================
void CManager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;

	m_ShadowScene->Uninit();
	delete m_ShadowScene;

	g_Polygon->Uninit();
	delete g_Polygon;

	CInput::Uninit();
	CMouse::Uninit();
	CRenderer::Uninit();
	CMyImGui::Uninit();
	CShader::ToonTextureUnload();
	CAtmosphere::Uninit();
}

//===========================================
//�X�V
//===========================================
void CManager::Update()
{
	CInput::Update();
	CMouse::Update();
	CAtmosphere::Update();

	m_ShadowScene->Update();
	m_Scene->Update();
	g_Polygon->Update();
}

//===========================================
//�`��
//===========================================
void CManager::Draw()
{
	//�ϐ��錾
	GaussBlurParam gauss;
	float w = (float)SCREEN_WIDTH;
	float h = (float)SCREEN_HEIGHT;
	CShader* gauss_x = new CShader("gaussVS.cso", "gaussXPS.cso");
	CShader* gauss_y = new CShader("gaussVS.cso", "gaussYPS.cso");
	CShader* unlitTexture = new CShader("unlitTextureVS.cso", "unlitTexturePS.cso");

	CMyImGui::Begin();

	//�[�x�e�N�X�`���쐬
	CRenderer::BeginDepth();
	m_ShadowScene->Draw();

	//1�p�X��
	CRenderer::BeginRenderTargetViewSwitch(0);
	// �p�����[�^�ݒ�
	gauss = CRenderer::CalcBlurParam(32.0f, w, h);
	CRenderer::SetGaussBuler(gauss);

	g_Polygon->FullScreenDraw(CRenderer::GetShadowTexture(), gauss_y);

	//2�p�X��
	CRenderer::BeginRenderTargetViewSwitch(1);

	g_Polygon->FullScreenDraw(CRenderer::GetSRV(0), gauss_x);

	//�ŏI�p�X
	CRenderer::Begin();
	m_Scene->Draw();
	g_Polygon->Draw(CRenderer::GetSRV(1), unlitTexture);
	CAtmosphere::Draw();

	//m_ShadowScene->GetGameObject<CLightCamera>(Layer_Camera)->ImGuiDraw();

	CMyImGui::End();
	CRenderer::End();

	delete gauss_x;
	delete gauss_y;
	delete unlitTexture;
}
