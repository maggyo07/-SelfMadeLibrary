#pragma once
#include <memory>
#include <list>
#include <algorithm>

using namespace std;

namespace GameL
{
	//継承抽象用　オブジェクトクラス
	class GameObject
	{
		public:
			GameObject();
			virtual ~GameObject();
			virtual void Initialize() = 0;	//初期化関数
			virtual void Action() = 0;	//処理関数
			virtual void Draw() = 0;	//描画関数
			virtual void Delete() = 0;	//削除関数
			virtual void OnEnable() {};	//アクティブになった瞬間に呼ばれる
			virtual void OnDisable() {};//非アクティブになった瞬間に呼ばれる
			void		 UpDataParentAndChilds();			   //親の情報と子の情報を更新する
			void		 DeleteParentAndChilds();			   //親子の情報を完全に削除する
			void		 SetDrawOrder(unsigned int draw_order);//描画優先順位を設定する
			unsigned int GetDrawOrder();					   //設定している描画優先順位取得用
			void		 SetName(int name);					   //オブジェクト名前ID変更用
			int			 GetName();							   //オブジェクト名前ID取得用
			void		 SetActive(bool active);			   //オブジェクトのアクティブ状態を変更
			bool		 GetActive();						   //オブジェクトのアクティブ状態を取得用
			void		 SetDeleteFlag(bool delete_flag);	   //オブジェクトを削除する
			bool		 GetDeleteFlag();					   //オブジェクトの削除フラグを取得する
			GameObject*	 GetChild(int id);					   //子要素を取得する
			GameObject** GetChilds(int *out_count);			   //子要素全てを取得する
			void		 SetChild(const weak_ptr<GameObject> child); //子要素を設定する
			GameObject*	 GetParent();						   //親情報を取得する
			void		 SetParent(GameObject* parent);			   //親を設定する
			bool		 GetParentActive();					   //親のアクティブ状態を取得する　＊親が存在しない場合trueを返す
			
		private:
			bool			m_active;		//オブジェクトのアクティブ状態(Action&Drawを実行するかどうか)
			bool			m_delete_flag;	//オブジェクトを削除するかどうか(仮にAction実行直前にONなら削除する)
			unsigned int	m_draw_order;	//描画優先順番
			int				m_name;			//オブジェクトの名前ID
			weak_ptr<GameObject>	m_parent;		//親の情報
			list<weak_ptr<GameObject>>* m_childs;	//子の情報
	};

	//オブジェクトマネージャー
	class GameObjectManagerClass
	{
		public:
			GameObjectManagerClass();
			void Initialize();					//初期化関数
			void Delete();				//削除関数
			void InsertObj(GameObject* obj,int name,unsigned int draw_order);	//オブジェクトの登録
			void ListDeleteSceneObj();	//オブジェクトリスト削除命令
			void ObjsAction();			//リストに登録している全てのオブジェクトのアクションメソッド実行
			void ObjsDraw();				//リストに登録している全てのオブジェクトの描画メソッド実行
			void ResetDrawOrder();		//描画優先順位を変更(リストを描画優先順位の降順に並び替え)

			GameObject* GetObj(int name,bool activ_obj_flag = false);		//名前情報からオブジェクト情報を１つ取得
			shared_ptr<GameObject> GetObj(GameObject*);
			GameObject** GetObjs(int name, int *out_objs_count, bool activ_obj_flag = false);	//名前情報からオブジェクト情報を複数取得
		private:
			list<shared_ptr<GameObject>>* m_list_data;	//ゲーム実行リスト
	};

}