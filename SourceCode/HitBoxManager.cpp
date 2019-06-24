#include "HitBoxManager.h"

#pragma region HitBoxClass
HitBoxClass::HitBoxClass()
{
	m_my_obj = nullptr;
	m_hit_data = new HIT_DATA[MAX_HITS];
	m_hit_search_data = new HIT_DATA*[MAX_HITS];
	m_hit_count = 0;
	memset(&m_hit_status, 0x00, sizeof(HIT_STATUS));
}
HitBoxClass::~HitBoxClass()
{
	SAFE_DELETE_ARRAY(m_hit_data);
	SAFE_DELETE_ARRAY(m_hit_search_data);
}
//このHitBoxを持つオブジェクトを設定する
void HitBoxClass::SetObj(void* obj)
{
	if (this == nullptr) return;
	m_my_obj = obj;
}
//位置情報・サイズ情報を設定する
void HitBoxClass::SetPos(float pos_x, float pos_y, float width, float height)
{
	if (this == nullptr) return;
	m_hit_box.pos_x = pos_x;
	m_hit_box.pos_y = pos_y;
	m_hit_box.width = width;
	m_hit_box.height = height;

}
//位置情報を設定する
void HitBoxClass::SetPos(float pos_x, float pos_y)
{
	if (this == nullptr) return;
	m_hit_box.pos_x = pos_x;
	m_hit_box.pos_y = pos_y;
}
//属性・名前・ポイント情報を設定する
void HitBoxClass::SetStatus(int element, int name, int point)
{
	if (this == nullptr) return;
	m_hit_status.element = element;
	m_hit_status.name = name;
	m_hit_status.point = point;
}
//無敵フラグを設定する
void HitBoxClass::SetInvincible(bool invincible)
{
	if (this == nullptr) return;
	m_hit_status.invincible_flag = invincible;
}
//当たっている相手の数を返す
int	HitBoxClass::GetHitCount()
{
	if (this == nullptr) return NULL;
	return m_hit_count;
}
//無敵状態を返す
bool HitBoxClass::GetInvincibility()
{
	if (this == nullptr) return NULL;
	return m_hit_status.invincible_flag;
}
//このHIT_BOXを持つオブジェクトアドレスを返す
void* HitBoxClass::GetMyObj()
{
	if (this == nullptr) return nullptr;
	return m_my_obj;
}
//ステータスの状態を返す
HIT_STATUS*	HitBoxClass::GetStatus()
{
	if (this == nullptr) return nullptr;
	return &m_hit_status;
}
//HIT_BOXの情報を返す
HIT_BOX* HitBoxClass::GetHitBox()
{
	if (this == nullptr) return nullptr;
	return &m_hit_box;
}


//当たっている相手の中から名前情報を元に検索し、ヒットしたオブジェクト情報を1つ返す
void* HitBoxClass::CheckObjNameHit(int obj_name)
{
	if (this == nullptr) return nullptr;
	for (int hit_count = 0; hit_count < m_hit_count; hit_count++)
		if (m_hit_data[hit_count].obj != nullptr)
			if (m_hit_data[hit_count].name == obj_name)
				return m_hit_data[hit_count].obj;
	return nullptr;
}
//当たっている相手の中から名前・ポイント情報を元に検索し、ヒットしたオブジェクト情報を1つ返す
void* HitBoxClass::CheckObjNameHit(int obj_name, int point)
{
	if (this == nullptr) return nullptr;
	for (int hit_count = 0; hit_count < m_hit_count; hit_count++)
		if (m_hit_data[hit_count].obj != nullptr)
			if (m_hit_data[hit_count].name == obj_name)
				if (m_hit_data[hit_count].point == point)
					return m_hit_data[hit_count].obj;
	return nullptr;
}
//当たっている相手の中から属性情報を元に検索し、ヒットしたかどうかを返す
bool HitBoxClass::CheckElementHit(int element)
{
	if (this == nullptr) return NULL;
	for (int hit_count = 0; hit_count < m_hit_count; hit_count++)
		if (m_hit_data[hit_count].obj != nullptr)
			if (m_hit_data[hit_count].element == element)
				return true;
	return false;
}
//当たっている相手の中から属性・ポイント情報を元に検索し、ヒットしたかどうかを返す
bool HitBoxClass::CheckElementHit(int element, int point)
{
	if (this == nullptr) NULL;
	for (int hit_count = 0; hit_count < m_hit_count; hit_count++)
		if (m_hit_data[hit_count].obj != nullptr)
			if (m_hit_data[hit_count].element == element)
				if (m_hit_data[hit_count].point == point)
					return true;
	return false;
}

//当たっている相手の中から名前情報を元に検索し、ヒットした全て(最大数)のHIT_DATA情報を返す
HIT_DATA** HitBoxClass::SearchObjNameHit(int obj_name)
{
	if (this == nullptr) return nullptr;
	//当たっている相手の情報を距離の昇順に並び替え
	SortHitBox();
	//探索をするので情報格納用変数のメモリをセットする
	memset(&(*m_hit_search_data), 0x00, sizeof(HIT_DATA*)*MAX_HITS);
	//探索でヒットした数
	int search_count = 0;
	for (int hit_count = 0; hit_count < m_hit_count; hit_count++)
		if (m_hit_data[hit_count].obj != nullptr)
			if (m_hit_data[hit_count].name == obj_name)
				m_hit_search_data[search_count++] = &m_hit_data[hit_count];

	//探索ヒットしなかったらnullptrを返す
	if (search_count == 0)
		return nullptr;
	else
		return m_hit_search_data;
}
//当たっている相手の中から名前・ポイント情報を元に検索し、ヒットした全て(最大数)のHIT_DATA情報を返す
HIT_DATA** HitBoxClass::SearchObjNameHit(int obj_name, int point)
{
	if (this == nullptr) return nullptr;
	//当たっている相手の情報を距離の昇順に並び替え
	SortHitBox();
	//探索をするので情報格納用変数のメモリをセットする
	memset(&(*m_hit_search_data), 0x00, sizeof(HIT_DATA*)*MAX_HITS);
	//探索でヒットした数
	int search_count = 0;
	for (int hit_count = 0; hit_count < m_hit_count; hit_count++)
		if (m_hit_data[hit_count].obj != nullptr)
			if (m_hit_data[hit_count].name == obj_name)
				if (m_hit_data[hit_count].point == point)
					m_hit_search_data[search_count++] = &m_hit_data[hit_count];

	//探索ヒットしなかったらnullptrを返す
	if (search_count == 0)
		return nullptr;
	else
		return m_hit_search_data;
}
//当たっている相手の中から属性情報を元に検索し、ヒットした全て(最大数)のHIT_DATA情報を返す
HIT_DATA** HitBoxClass::SearchElementHit(int element)
{
	if (this == nullptr) return nullptr;
	//当たっている相手の情報を距離の昇順に並び替え
	SortHitBox();
	//探索をするので情報格納用変数のメモリをセットする
	memset(&(*m_hit_search_data), 0x00, sizeof(HIT_DATA*)*MAX_HITS);
	//探索でヒットした数
	int search_count = 0;
	for (int hit_count = 0; hit_count < m_hit_count; hit_count++)
		if (m_hit_data[hit_count].obj != nullptr)
			if (m_hit_data[hit_count].element == element)
				m_hit_search_data[search_count++] = &m_hit_data[hit_count];

	//探索ヒットしなかったらnullptrを返す
	if (search_count == 0)
		return nullptr;
	else
		return m_hit_search_data;
}
//当たっている相手の中から属性・ポイント情報を元に検索し、ヒットした全て(最大数)のHIT_DATA情報を返す
HIT_DATA** HitBoxClass::SearchElementHit(int element, int point)
{
	if (this == nullptr) return nullptr;
	//当たっている相手の情報を距離の昇順に並び替え
	SortHitBox();
	//探索をするので情報格納用変数のメモリをセットする
	memset(&(*m_hit_search_data), 0x00, sizeof(HIT_DATA*)*MAX_HITS);
	//探索でヒットした数
	int search_count = 0;
	for (int hit_count = 0; hit_count < m_hit_count; hit_count++)
		if (m_hit_data[hit_count].obj != nullptr)
			if (m_hit_data[hit_count].element == element)
				if (m_hit_data[hit_count].point == point)
					m_hit_search_data[search_count++] = &m_hit_data[hit_count];

	//探索ヒットしなかったらnullptrを返す
	if (search_count == 0)
		return nullptr;
	else
		return m_hit_search_data;
}

//当たっている相手の情報を距離の昇順で並び替え
void HitBoxClass::SortHitBox()
{
	if (this == nullptr) return;
	for (int front = 0; front < m_hit_count; front++)	//前
	{
		for (int rear = front + 1; rear < m_hit_count; rear++)	//後
		{
			//前の距離を取得
			float front_distance = (m_hit_data[front].obj == nullptr) ? 9999.0f : m_hit_data[front].distance;
			//後の距離を取得
			float rear_distance = (m_hit_data[rear].obj == nullptr) ? 9999.0f : m_hit_data[rear].distance;

			//前の距離より後の距離が短ければ並び替えを行う
			if (front_distance > rear_distance)
			{
				HIT_DATA hit_data;
				hit_data = m_hit_data[front];
				m_hit_data[front] = m_hit_data[rear];
				m_hit_data[rear] = hit_data;
			}
		}
	}
}
#pragma endregion


#pragma region HitBoxManagerClass
HitBoxManagerClass::HitBoxManagerClass()	:m_list_hitbox(new list<shared_ptr<HitBox>>)
{

}
//初期化
void HitBoxManagerClass::Initialize()
{
	
}
//削除
void HitBoxManagerClass::Delete()
{
	DeleteAllListData();
	delete m_list_hitbox;
}
//HitBoxの作成・登録
void HitBoxManagerClass::SetHitBox(void* obj, float pos_x, float pos_y, float width, float height, int element, int name, int point)
{
	//データの作成
	shared_ptr<HitBox> ptr_box(new HitBox());
	ptr_box->SetPos(pos_x, pos_y, width, height);	//位置・幅・高さを設定する
	ptr_box->SetStatus(element, name, point);		//属性・名前・ポイントを設定する
	ptr_box->SetObj(obj);						//オブジェクト情報を設定する
	ptr_box->SetInvincible(false);				//無敵情報を設定する
	m_list_hitbox->push_front(ptr_box);			//データの登録
}
//自身のアドレスを持つHitBoxを返す
HitBox* HitBoxManagerClass::GetHitBox(void* obj)
{
	for each (shared_ptr<HitBox> itr in *m_list_hitbox)
	{
		if (itr->GetMyObj() == obj)
			return itr.get();
	}
	return nullptr;
}
//自身のアドレスを持つHitBoxの削除
void	HitBoxManagerClass::DeleteHitBox(void* obj)
{
	for (auto itr = m_list_hitbox->begin(); itr != m_list_hitbox->end(); itr++)
	{
		if ((*itr).get()->GetMyObj() == obj)
		{
			m_list_hitbox->erase(itr);	//データ削除
			return;
		}
	}
}
//衝突判定
void HitBoxManagerClass::AllHitCheck()
{
	//処理は仮です
	for each(shared_ptr<HitBox> itr_my in *m_list_hitbox)
	{
		bool hit;

		//前回の内容を破棄
		itr_my->m_hit_count = 0;
		memset(&(*itr_my->m_hit_data), 0x00, sizeof(HIT_DATA)*MAX_HITS);

		if (itr_my->GetStatus()->invincible_flag == true)
		{
			continue;
		}

		for each(shared_ptr<HitBox> itr in *m_list_hitbox)
		{

			if (itr->GetStatus()->element != itr_my->m_hit_status.element || itr_my->GetStatus()->element == 0)
			{
				if (itr->GetStatus()->invincible_flag == true)
				{
					;
				}
				else if (itr != itr_my)
				{
					if (itr_my->m_hit_count > MAX_HITS - 1)
					{
						continue;
					}
					hit = Hit(&itr_my->m_hit_box, &itr->m_hit_box, &itr_my->m_hit_data[itr_my->m_hit_count]);
					if (hit == true)
					{
						itr_my->m_hit_data[itr_my->m_hit_count].element = itr->GetStatus()->element;
						itr_my->m_hit_data[itr_my->m_hit_count].point = itr->GetStatus()->point;
						itr_my->m_hit_data[itr_my->m_hit_count].name = itr->GetStatus()->name;
						itr_my->m_hit_data[itr_my->m_hit_count].obj = itr->GetMyObj();
						itr_my->m_hit_count++;

					}
				}
			}
		}
	}
}
//衝突判定メソッド
//aがbのどこに当たっているか
bool HitBoxManagerClass::Hit(HIT_BOX* a, HIT_BOX* b, HIT_DATA* hit)
{
	//処理は仮です

	//衝突判定
	if (
		(a->pos_x + a->width > b->pos_x) && (a->pos_x < b->pos_x + b->width) &&
		(a->pos_y + a->height > b->pos_y) && (a->pos_y < b->pos_y + b->height)
		)
	{
		//どこに当たっているかチェックを行い角度を返す
		float pa_x, pa_y, pb_x, pb_y;
		float angle;
		pa_x = a->pos_x + (a->width / 2.0f);	pa_y = a->pos_y + (a->height / 2.0f);
		pb_x = b->pos_x + (b->width / 2.0f);	pb_y = b->pos_y + (b->height / 2.0f);

		angle = atan2(pb_y - pa_y, pb_x - pa_x);
		hit->angle = angle * 180.0f / 3.14f;

		if (hit->angle <= 0.0f)
			hit->angle = abs(angle * 180.0f / 3.14f);
		else
			hit->angle = 360.0f - abs(angle * 180.0f / 3.14f);

		//三平方の定理から距離を図る
		hit->distance = sqrt((float)(pb_x - pa_x)*(pb_x - pa_x) + (pb_y - pa_y)*(pb_y - pa_y));
		return true;
	}

	return false;
}
//衝突判定リストを破棄
void	HitBoxManagerClass::DeleteAllListData()
{
	m_list_hitbox->clear();
}
//デバック用　当たり判定描画
void	HitBoxManagerClass::DrawHitBox()
{
	//HitBox描画時の色情報
	//				　R　　G  　B　　A
	float col[4] = { 0.5f,0.0f,0.0f,0.2f*255.0f };

	//登録しているHitBox分回す
	for each (shared_ptr<HitBox> itr in *m_list_hitbox)
	{
		switch (itr->GetStatus()->element)
		{

		}
		for (int i = 0; i < 3; i++)
			col[i] *= 255;
	}
}
#pragma endregion