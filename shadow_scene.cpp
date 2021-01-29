
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "shadow_scene.h"
#include "camera_light.h"
#include "torus.h"
#include "piller.h"

void CShadowScene::Init()
{
	AddGameObject<CLightCamera>(Layer_Camera)->SetPosition(D3DXVECTOR3(-10.0f, 10.0f, 0.0f));
	AddGameObject<CMeshField>(Layer_3d_Object)->SetPosition(D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	AddGameObject<CTorus>(Layer_3d_Object)->SetPosition(D3DXVECTOR3(-5.0f, 2.0f, 7.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	AddGameObject<CTorus>(Layer_3d_Object)->SetPosition(D3DXVECTOR3(0.0f, 0.5f, -3.0f), D3DXVECTOR3(0.0f, 0.0f, -0.01f));
	AddGameObject<CTorus>(Layer_3d_Object)->SetPosition(D3DXVECTOR3(5.0f, 2.0f, 5.0f), D3DXVECTOR3(0.05f, 0.05f, 0.0f));
	AddGameObject<CPiller>(Layer_3d_Object);
}

void CShadowScene::Uninit()
{
	for (int i = 0; i < Layer_Max; i++)
	{
		for (CGameObject *object : m_GameObject[i])
		{
			object->Uninit();
			delete object;
		}
		//リスト構造のクリア
		m_GameObject[i].clear();
	}
}

void CShadowScene::Update()
{
	for (int i = 0; i < Layer_Max; i++)
	{
		for (CGameObject *object : m_GameObject[i])
		{
			object->Update();
		}
		//リストを走査して引数の関数がtrueでリストから削除
		m_GameObject[i].remove_if([](CGameObject *object) { return object->Destroy(); });
	}
}

void CShadowScene::Draw()
{
	for (int i = 0; i < Layer_Max; i++)
	{
		for (CGameObject *object : m_GameObject[i])
		{
			object->ShadowShaderDraw();
			object->Draw();
		}
	}
}