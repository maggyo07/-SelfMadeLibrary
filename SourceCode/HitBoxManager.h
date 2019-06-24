#pragma once

#include <memory>
#include <list>

using namespace std;

//ポインタを安全に削除をする
#define SAFE_DELETE_ARRAY(p) {if(p) {delete[] (p); (p) = nullptr;}}

//当たり判定の複数同時時判定量
#define MAX_HITS	(10)

//------衝突状態構造体------
struct HIT_BOX
{
	float pos_x;	//位置X
	float pos_y;	//位置Y
	float width;	//幅
	float height;	//高さ
	float angle;	//角度
};
struct HIT_STATUS
{
	int element;			//属性
	int name;				//オブジェクトネーム
	bool invincible_flag;	//無敵フラグ(Hit判定無効)
	int point;				//ポイント(検索などで使用する)
};
//当たった相手の情報格納用
struct HIT_DATA
{
	int	element;	//当たった相手の属性情報
	int name;		//当たった相手のオブジェクトネーム
	int point;		//ポイント(検索などで使用する)
	float distance;	//距離
	float angle;	//当たっている角度
	void* obj;		//当たっているオブジェクトアドレス
};
//--------------------------
//-----衝突判定用クラス-----
typedef class HitBoxClass
{
	friend class HitBoxManagerClass;
public:
	HitBoxClass();
	~HitBoxClass();
	void SetObj(void* obj);								//このHitBoxを持つオブジェクトを設定する
	void SetPos(float pos_x, float pos_y,float width , float height);	//位置情報・サイズ情報を設定する
	void SetPos(float pos_x, float pos_y);				//位置情報を設定する
	void SetStatus(int element, int name, int point);	//属性・名前・ポイント情報を設定する
	void SetElement(int element) { SetStatus(element, m_hit_status.name, m_hit_status.point); }
	void SetInvincible(bool invincible);				//無敵フラグを設定する

	int			GetHitCount();							//当たっている相手の数を返す
	bool		GetInvincibility();						//無敵状態を返す
	void*		GetMyObj();								//このHit_BOXを持つオブジェクトアドレスを返す
	HIT_STATUS*	GetStatus();							//ステータスの状態を返す
	HIT_BOX*	GetHitBox();							//位置・高さ・幅の情報を返す

	void*	CheckObjNameHit(int obj_name);				//当たっている相手の中から名前情報を元に検索し、ヒットしたオブジェクト情報を1つ返す
	void*	CheckObjNameHit(int obj_name, int point);	//当たっている相手の中から名前・ポイント情報を元に検索し、ヒットしたオブジェクト情報を1つ返す
	bool	CheckElementHit(int element);				//当たっている相手の中から属性情報を元に検索し、ヒットしたかどうかを返す
	bool	CheckElementHit(int element, int point);	//当たっている相手の中から属性・ポイント情報を元に検索し、ヒットしたかどうかを返す
		
	HIT_DATA** SearchObjNameHit(int obj_name);				//当たっている相手の中から名前情報を元に検索し、ヒットした全て(最大数)のHIT_DATA情報を返す
	HIT_DATA** SearchObjNameHit(int obj_name, int point);	//当たっている相手の中から名前・ポイント情報を元に検索し、ヒットした全て(最大数)のHIT_DATA情報を返す
	HIT_DATA** SearchElementHit(int element);				//当たっている相手の中から属性情報を元に検索し、ヒットした全て(最大数)のHIT_DATA情報を返す
	HIT_DATA** SearchElementHit(int element, int point);	//当たっている相手の中から属性・ポイント情報を元に検索し、ヒットした全て(最大数)のHIT_DATA情報を返す

	private:
		void SortHitBox();					//当たっている相手の情報を距離の昇順で並び替え
		HIT_STATUS	m_hit_status;			//当たり判定の状態
		HIT_BOX		m_hit_box;				//当たり判定の座標幅
		HIT_DATA*	m_hit_data;				//当たっている相手の情報
		HIT_DATA**	m_hit_search_data;		//当たっている相手の中で探索後の情報
		void*		m_my_obj;				//このHitBox持つオブジェクトアドレス
		int			m_hit_count;			//ヒットしているカウント数
}HitBox;
//--------------------------

//衝突判定マネージャー
typedef class HitBoxManagerClass
{
	public:
		HitBoxManagerClass();
		void		Initialize();										//初期化
		void		Delete();									//削除
		void		SetHitBox(void* obj, float pos_x, float pos_y, float width, float height,int element,int name,int point);//HitBoxの作成・登録
		HitBox*	GetHitBox(void* obj);						//自身のアドレスを持つHitBoxを返す
		void		DeleteHitBox(void* obj);					//自身のアドレスを持つHitBoxの削除
		void		AllHitCheck();								//衝突判定
		void		DeleteAllListData();						//衝突判定リストを破棄
		void		DrawHitBox();								//デバック用　当たり判定描画
	private:
		bool Hit(HIT_BOX* a, HIT_BOX* b, HIT_DATA* hit);		//aがbのどこに当たっているか
		list<shared_ptr<HitBox>>* m_list_hitbox;			//衝突判定リスト
}Hits;
