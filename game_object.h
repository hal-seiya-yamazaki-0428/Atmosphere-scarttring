#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

class CGameObject
{
protected:
	bool		m_Destroy;
	int			m_Layer;
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Rotation;
	D3DXVECTOR3 m_Scale;
	D3DXVECTOR3 m_vecFront;
	D3DXVECTOR3 m_vecRight;
	D3DXVECTOR3 m_vecUp;
	std::string m_Name;

public:
	CGameObject() 
	{ 
		m_Destroy = false;
		m_Layer = 1;	//zソート用
		m_Name = "Object";
	}
	virtual ~CGameObject() {}
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void ShadowShaderDraw() {};
	virtual void DefaultShaderDraw() {};

	//Zソート用オーバーロード演算子
	bool operator<(const CGameObject &rhs) const
	{
		//昇順
		return m_Layer < rhs.m_Layer;
	}

	//ゲット関数
	D3DXVECTOR3 GetPosition() { return m_Position; }
	D3DXVECTOR3 GetRotation() { return m_Rotation; }
	D3DXVECTOR3 GetScale() { return m_Scale; }
	D3DXVECTOR3 GetVecFront() { return m_vecFront; }
	D3DXVECTOR3 GetVecRight() { return m_vecRight; }
	D3DXVECTOR3 GetVecUp() { return m_vecUp; }
	std::string GetName() { return m_Name; }

	D3DXVECTOR3 GetFront()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
		D3DXVECTOR3 front;
		front.x = rot._31;
		front.y = rot._32;
		front.z = rot._33;
		return front;
	}

	//セット関数
	void SetPosition(D3DXVECTOR3 pos) { m_Position = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_Rotation = rot; }
	void SetScale(D3DXVECTOR3 scale) { m_Scale = scale; }

	void SetDestroy() { m_Destroy = true; }

	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}
};

#endif