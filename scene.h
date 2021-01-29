#ifndef _SCENE_H_
#define _SCENE_H_

#include <list>
#include "main.h"
#include "game_object.h"
#include "sprite.h"
#include "camera.h"
#include "mesh_field.h"
#include "sky_dome.h"
#include "polygon.h"
#include "torus.h"
#include "piller.h"
#include <typeinfo>

typedef enum
{
	Layer_Camera,
	Layer_3d_Object,
	Layer_Translucent,
	Layer_2d_Object,
	Layer_Max
}SCENE_LAYER;

class CScene
{
protected:
	std::list<CGameObject *> m_GameObject[Layer_Max];
public:
	CScene() {}
	virtual ~CScene() {}

	virtual void Init()
	{
		AddGameObject<CCamera>(Layer_Camera);
		AddGameObject<CMeshField>(Layer_3d_Object)->SetPosition(D3DXVECTOR3(0.0f, -1.0f, 0.0f));
		AddGameObject<CSkyDome>(Layer_3d_Object);
		AddGameObject<CTorus>(Layer_3d_Object)->SetPosition(D3DXVECTOR3(-5.0f, 2.0f, 7.5f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		AddGameObject<CTorus>(Layer_3d_Object)->SetPosition(D3DXVECTOR3(0.0f, 0.5f, -3.0f), D3DXVECTOR3(0.0f, 0.0f, -0.01f));
		AddGameObject<CTorus>(Layer_3d_Object)->SetPosition(D3DXVECTOR3(5.0f, 2.0f, 5.0f), D3DXVECTOR3(0.05f, 0.05f, 0.0f));
		AddGameObject<CPiller>(Layer_3d_Object);
	}

	virtual void Uninit()
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

	virtual void Update()
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
	virtual void Draw()
	{
		for (int i = 0; i < Layer_Max; i++)
		{
			for (CGameObject *object : m_GameObject[i])
			{
				object->DefaultShaderDraw();
				object->Draw();
			}
		}
	}

	//オブジェクトの追加
	template <typename T>
	T *AddGameObject(int layer)
	{
		T *gameObject = new T();
		m_GameObject[layer].push_back(gameObject);
		gameObject->Init();

		return gameObject;
	}

	//単体のオブジェクトの取得
	template <typename T>
	T *GetGameObject(int layer)
	{
		for (CGameObject *object : m_GameObject[layer])
		{
			if (typeid(*object) == typeid(T))
			{
				return (T*)object;
			}
		}
		return NULL;
	}

	//複数のオブジェクトの取得
	template <typename T>
	std::vector<T*> GetGameObjects(int layer)
	{
		std::vector<T*> objects;
		for (CGameObject *object : m_GameObject[layer])
		{
			if (typeid(*object) == typeid(T))
			{
				objects.push_back((T*)object);
			}
		}
		return objects;
	}
};

#endif