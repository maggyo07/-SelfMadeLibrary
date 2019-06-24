#include "GameObjectManagerClass.h"

#include "AccessController.h"

using namespace GameL;
using System::ObjManager;

//ソート用 描画優先順位を比較する(降順)
int DrawOrderCompare(const shared_ptr<GameObject> x, const shared_ptr<GameObject> y)
{
	return x.get()->GetDrawOrder() < y.get()->GetDrawOrder();
}

#pragma region GameObject
GameObject::GameObject()
	:m_draw_order(0), m_name(0), m_active(true)
	, m_delete_flag(false), m_childs(new list<weak_ptr<GameObject>>)
{
	
}

GameObject::~GameObject()
{
	delete m_childs;
}
//親の情報と子の情報を更新する
void GameObject::UpDataParentAndChilds()
{
	//子の情報を更新する
	auto itr = m_childs->begin();
	while (itr != m_childs->end())
	{
		//すでに参照先のメモリが解放されていたら削除
		if ((*itr).expired())
		{
			(*itr).reset();
			itr = m_childs->erase(itr);
		}
		else
			itr++;
	}
}
//親子の情報を完全に削除する
void GameObject::DeleteParentAndChilds()
{
	//削除する前に情報を更新する
	UpDataParentAndChilds();
	//親の情報を削除する
	m_parent.reset();

	//子の情報を削除する
	auto itr = m_childs->begin();
	while (itr != m_childs->end())
	{
		itr->lock().get()->Delete();
		itr->lock().get()->DeleteParentAndChilds();
		itr = m_childs->erase(itr);
	}
	m_childs->clear();
}
//描画優先順位変更
void GameObject::SetDrawOrder(unsigned int draw_order)
{
	//値を変更する
	m_draw_order = draw_order;
	//リストを並び替える
	ObjManager().ResetDrawOrder();
}

//設定している描画優先順位取得用
unsigned int GameObject::GetDrawOrder()
{
	return m_draw_order;
}

//オブジェクト名前ID変更用
void GameObject::SetName(int name)
{
	m_name = name;
}

//オブジェクト名前ID取得用
int GameObject::GetName()
{
	return m_name;
}

//オブジェクトのアクティブ状態を変更
void GameObject::SetActive(bool active)
{
	//現在のアクティブと引数のアクティブが違うかったら
	if (active != m_active)
	{
		//アクティブになった瞬間なので専用メソッドを呼ぶ
		if (active)
			this->OnEnable();
		//非アクティブになった瞬間なので専用メソッドを呼ぶ
		else
			this->OnDisable();
		//アクティブを更新する
		m_active = active;
	}
}

//オブジェクトのアクティブ状態を取得用
bool GameObject::GetActive()
{
	return m_active;
}
//オブジェクトを削除する
void GameObject::SetDeleteFlag(bool delete_flag)
{
	m_delete_flag = delete_flag;
}
//オブジェクトの削除フラグを取得する
bool GameObject::GetDeleteFlag()
{
	return m_delete_flag;
}
//子要素を取得する
GameObject* GameObject::GetChild(int id)
{
	//子要素がなければ終了
	if ((int)m_childs->size() <= id) return nullptr;

	//引数id番目の子の情報を返す
	int count = 0;
	for (auto itr = m_childs->begin(); itr != m_childs->end(); itr++)
	{
		if (count == id)
			return itr->lock().get();
		count++;
	}
	return nullptr;
}
//子要素全てを取得する
GameObject** GameObject::GetChilds(int *out_count)
{
	//子要素がなければ終了
	if ((int)m_childs->size() <= 0)return nullptr;

	//子の数を取得
	*out_count = (int)m_childs->size();

	//子のオブジェクト情報格納用
	GameObject** childs = new GameObject*[(int)m_childs->size()];

	//子のオブジェクト情報格納
	int child_count = 0;
	for each (auto child in *m_childs)
	{
		childs[child_count] = child.lock().get();
		child_count++;
	}

	return childs;
}
//子要素を設定する
void GameObject::SetChild(const weak_ptr<GameObject> child)
{
	//子が自身または無い場合は終了する
	if (child.lock().get() == this || child.lock().get() == nullptr) return;

	//すでに登録済みなら登録せずに終了する
	for each (auto obj in *m_childs)
		if (obj.lock().get() == child.lock().get())
			return;

	m_childs->push_back(child);
}
//親情報を取得する
GameObject* GameObject::GetParent()
{
	return m_parent.lock().get();
}
//親を設定する
void GameObject::SetParent(GameObject* parent)
{
	//親の情報が自身またわ無い場合は終了する
	if (parent == this || parent == nullptr) return;

	//親情報を設定する
	m_parent = ObjManager().GetObj(parent);

	//親の子になったので設定する
	m_parent.lock().get()->SetChild(ObjManager().GetObj(this));
}
//親のアクティブ状態を取得する　＊親が存在しない場合trueを返す
bool GameObject::GetParentActive()
{
	//親が存在しなければtrueを返す
	if (m_parent.lock().get() == nullptr) return true;

	//親のアクティブ状態チェック
	if (m_parent.lock().get()->GetActive())
	{
		//親の親のアクティブ状態チェック
		if (m_parent.lock().get()->GetParentActive())
			return true;
	}

	return false;
}
#pragma endregion

#pragma region GameObjectManagerClass
GameObjectManagerClass::GameObjectManagerClass():m_list_data(new list<shared_ptr<GameObject>>)
{

}
//初期化
void GameObjectManagerClass::Initialize()
{

}

//削除
void GameObjectManagerClass::Delete()
{
	ListDeleteSceneObj();
	delete m_list_data;
}

//オブジェクトの登録
void GameObjectManagerClass::InsertObj(GameObject* obj, int name, unsigned int draw_order)
{
	//ポリモーフィズムが崩壊している
	//直したい・・・
	if (obj == nullptr)
		return;

	//オブジェクトの名前設定
	obj->SetName(name);

	//オブジェクトの初期化メソッド実行
	obj->Initialize();

	//データ登録
	unique_ptr<GameObject> ptr_obj(obj);
	m_list_data->push_front(move(ptr_obj));

	//オブジェクトの描画優先順位設定
	obj->SetDrawOrder(draw_order);

	return;
}

//オブジェクトリスト削除命令
void GameObjectManagerClass::ListDeleteSceneObj()
{
	auto itr = m_list_data->begin();
	while (itr != m_list_data->end())
	{
		//親子の情報を削除
		(*itr)->DeleteParentAndChilds();
		itr = m_list_data->erase(itr);
	}
	m_list_data->clear();
}

//リストに登録している全てのオブジェクトのアクションメソッド実行
void GameObjectManagerClass::ObjsAction()
{
	//削除実行
	auto itr = m_list_data->begin();
	while (itr != m_list_data->end())
	{
		if (itr->get()->GetDeleteFlag() == true)
		{
			(*itr)->Delete();
			//親子情報を削除する
			(*itr)->DeleteParentAndChilds();
			itr = m_list_data->erase(itr);
		}
		else
			itr++;
	}

	//各オブジェクトの子要素と親の情報を更新する
	for each (auto obj in *m_list_data)
	{
		obj->UpDataParentAndChilds();
	}

	//アクション実行
	for each (shared_ptr<GameObject> obj in *m_list_data)
	{
		//状態状態ならアクションメソッド実行
		if (obj->GetActive() == true && obj->GetParentActive() == true)
			obj->Action();
	}
}

//リストに登録している全てのオブジェクトの描画メソッド実行
void GameObjectManagerClass::ObjsDraw()
{
	for each (shared_ptr<GameObject> obj in *m_list_data)
	{
		//状態状態なら描画メソッド実行
		if (obj->GetActive() == true && obj->GetParentActive() == true)
			obj->Draw();
	}
}

//描画優先順位を変更(リストを描画優先順位の降順に並び替え)
void GameObjectManagerClass::ResetDrawOrder()
{
	//リストに２つ以上情報があればソートをする
	if (m_list_data->size() >= 2)
	{
		m_list_data->sort(DrawOrderCompare);
	}
}

//名前情報からオブジェクト情報を１つ取得
//引数2 activ_obj_flag	:アクティブ状態がOFFのオブジェクトも含むかどうか
GameObject* GameObjectManagerClass::GetObj(int name, bool activ_obj_flag)
{
	for (auto itr = m_list_data->begin(); itr != m_list_data->end(); itr++)
	{
		//引数のアクティブフラグがON　か
		//アクティブ状態なら取得できる
		if (activ_obj_flag || (*itr)->GetActive() == true)
		{
			//名前が一致した最初のオブジェクト情報を渡す
			if ((*itr)->GetName() == name)
				return (*itr).get();
		}
	}

	return nullptr;
}
//
shared_ptr<GameObject> GameObjectManagerClass::GetObj(GameObject* obj)
{
	for each (auto itr in *m_list_data)
	{
		if (itr.get() == obj)
		{
			return itr;
		}
	}
	return nullptr;
}
//名前情報からオブジェクト情報を複数取得
//引数3 activ_obj_flag	:アクティブ状態がOFFのオブジェクトも含むかどうか
GameObject** GameObjectManagerClass::GetObjs(int name, int *out_objs_count, bool activ_obj_flag)
{
	GameObject** objs = nullptr;

	int obj_count = 0;

	//名前が一致するオブジェクトがいくつ存在するかを数える
	for (auto itr = m_list_data->begin(); itr != m_list_data->end(); itr++)
	{
		//引数のアクティブフラグがON　か
		//アクティブ状態なら取得できる
		if (activ_obj_flag || (*itr)->GetActive() == true)
		{
			//名前が一致したならカウントを加算する
			if ((*itr)->GetName() == name)
				obj_count++;
		}
	}

	//いくつ同名のオブジェクトがあるか判明したのでメモリをセットする
	objs = new GameObject*[obj_count];
	//オブジェクトがいくつあったのかが判明したので値をせっとする
	*out_objs_count = obj_count;
	//オブジェクトカウントを使い回したいので初期化
	obj_count = 0;

	//後はもう一度同名のオブジェクトを検索して格納する
	for (auto itr = m_list_data->begin(); itr != m_list_data->end(); itr++)
	{
		//引数のアクティブフラグがON　か
		//アクティブ状態なら取得できる
		if (activ_obj_flag || (*itr)->GetActive() == true)
		{
			//名前が一致しら、格納する
			if ((*itr)->GetName() == name)
				objs[obj_count++] = (*itr).get();
		}
	}

	//全てが完了したので渡す
	return objs;
}
#pragma endregion
