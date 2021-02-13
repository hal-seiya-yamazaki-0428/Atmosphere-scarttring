
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
#include "sprite.h"
#include "atmosphere.h"
#include "myimGui.h"
#include "camera_light.h"

#include "model.h"
#include "polygon.h"
#include "gauss.h"
#include "shadow.h"

//===========================================
//グローバル変数
//===========================================
CScene *CManager::m_Scene = NULL;
CScene *CManager::m_ShadowScene = NULL;
CPolygon* g_Polygon;

//===========================================
//初期化
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
	CGauss::Init();
	CShadow::Init();

	m_Scene = new CScene;
	m_Scene->Init();

	m_ShadowScene = new CShadowScene;
	m_ShadowScene->Init();

	g_Polygon = new CPolygon;
	g_Polygon->Init();

}

//===========================================
//終了
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
	CGauss::Uninit();
	CShadow::Uninit();
}

//===========================================
//更新
//===========================================
void CManager::Update()
{
	CInput::Update();
	CMouse::Update();
	CAtmosphere::Update();
	CGauss::Update();
	CShadow::Update();

	m_ShadowScene->Update();
	m_Scene->Update();
	g_Polygon->Update();
}

//===========================================
//描画
//===========================================
void CManager::Draw()
{
	//変数宣言
	CShader* unlitTexture = new CShader("unlitTextureVS.cso", "unlitTexturePS.cso");

	CMyImGui::Begin();

	//深度テクスチャ作成
	CShadow::Create();
	m_ShadowScene->Draw();

	//ガウステクスチャ
	auto tex = CGauss::Create(CShadow::GetShadowTexture());

	//最終パス
	CRenderer::Begin();

	m_Scene->Draw();
	//g_Polygon->Draw(tex, unlitTexture);
	CAtmosphere::Draw();

	//m_ShadowScene->GetGameObject<CLightCamera>(Layer_Camera)->ImGuiDraw();

	CMyImGui::End();
	CRenderer::End();

	delete unlitTexture;
}
