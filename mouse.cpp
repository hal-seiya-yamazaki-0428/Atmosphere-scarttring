
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "input.h"

#include "mouse.h"

//===========================================
//グローバル変数
//===========================================
POINT CMouse::m_MousePos;

//===========================================
//初期化
//===========================================
void CMouse::Init()
{
	m_MousePos.x = m_MousePos.y = 0.0f;
}

//===========================================
//終了
//===========================================
void CMouse::Uninit()
{

}

//===========================================
//更新
//===========================================
void CMouse::Update()
{
	//マウス座標をクライアント座標に変換
	GetCursorPos(&m_MousePos);
	ScreenToClient(GetWindow(), &m_MousePos);

	//画面外座標制限
	m_MousePos.x = std::max((LONG)0.0f, m_MousePos.x);			//画面左制限
	m_MousePos.x = std::min((LONG)SCREEN_WIDTH, m_MousePos.x);	//画面右制限
	m_MousePos.y = std::max((LONG)0.0f, m_MousePos.y);			//画面上制限
	m_MousePos.y = std::min((LONG)SCREEN_HEIGHT, m_MousePos.y);	//画面下制限

}

//===========================================
// スクリーン座標をワールド座標に変換
//===========================================
void CMouse::GetScreenToWorld(D3DXVECTOR3* pout, int Sx, int Sy, float fZ)
{
	//カメラの行列取得
	D3DXMATRIX View, Prj;
	CCamera *camera = CManager::GetScene()->GetGameObject<CCamera>(Layer_Camera);
	View = camera->GetViewMtx();
	Prj = camera->GetProjectionMtx();
	// 各行列の逆行列を算出
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, &View);
	D3DXMatrixInverse(&InvPrj, NULL, &Prj);
	D3DXMatrixIdentity(&VP);
	VP._11 = SCREEN_WIDTH / 2.0f; VP._22 = -SCREEN_HEIGHT / 2.0f;
	VP._41 = SCREEN_WIDTH / 2.0f; VP._42 = SCREEN_HEIGHT / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// 逆変換
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(pout, &D3DXVECTOR3(Sx, Sy, fZ), &tmp);
}

//===========================================
// XZ平面とスクリーン座標の交点算出関数
//===========================================
void CMouse::GetScreenToXZ(D3DXVECTOR3* pout, int Sx, int Sy)
{
	D3DXVECTOR3 nearpos;
	D3DXVECTOR3 farpos;
	D3DXVECTOR3 ray;
	GetScreenToWorld(&nearpos, Sx, Sy, 0.0f);
	GetScreenToWorld(&farpos, Sx, Sy, 1.0f);
	ray = farpos - nearpos;
	D3DXVec3Normalize(&ray, &ray);

	// 床との交差が起きている場合は交点を
	// 起きていない場合は遠くの壁との交点を出力
	if (ray.y <= 0) {
		// 床交点
		float Lray = D3DXVec3Dot(&ray, &D3DXVECTOR3(0, 1, 0));
		float LP0 = D3DXVec3Dot(&(-nearpos), &D3DXVECTOR3(0, 1, 0));
		*pout = nearpos + (LP0 / Lray)*ray;
	}
	else {
		*pout = farpos;
	}
}

//===========================================
//直線が球に当たっているかどうか　
//===========================================
bool CMouse::GetLineHit(D3DXVECTOR3 center, float radius, int Sx, int Sy)
{
	D3DXVECTOR3 start;
	D3DXVECTOR3 end;
	GetScreenToWorld(&start, Sx, Sy, 0.0f);
	GetScreenToWorld(&end, Sx, Sy, 1.0f);

	//ベクトルの用意
	D3DXVECTOR3 start_center = center - start;	//始点から円
	D3DXVECTOR3 end_center = center - end;		//終点から円
	D3DXVECTOR3 start_end = end - start;		//始点から終点
	D3DXVECTOR3 start_end_normal;				//正規化用
	D3DXVec3Normalize(&start_end_normal, &start_end);

	D3DXVECTOR3 cross;
	D3DXVec3LengthSq(D3DXVec3Cross(&cross, &start_center, &start_end_normal));
	float length = D3DXVec3LengthSq(&cross);
	
	//垂線が半径より短い場合
	if (fabsf(length) <= radius)
	{
		float dot_len = D3DXVec3Dot(&start_center, &start_end);
		float dot_len2 = D3DXVec3Dot(&end_center, &start_end);

		//内積の掛け算結果
		if (dot_len * dot_len2 <= 0.0f)
		{
			return true;
		}
		//始点か終点が半径より短いか
		else if (D3DXVec3LengthSq(&start_center) < radius || D3DXVec3LengthSq(&end_center) < radius)
		{
			return true;
		}
	}
	return false;
}