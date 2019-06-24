#pragma once

namespace GameL
{
	//継承抽象用　シーンクラス
	class CScene
	{
		public:
			CScene() {}
			virtual ~CScene() {};
			virtual void Initialize() = 0;
			virtual void Scene() = 0;
			virtual void SceneChange(CScene* prev_scene,CScene* next_scene) { }
		private:
	};

	//シーンマネージャー
	typedef class SceneManagerClass
	{
		public:
			SceneManagerClass();
			~SceneManagerClass();
			void Initialize();						//初期化
			void Delete();
			void SetScene(CScene* scene);	//新しいシーンをセットして、古いシーンを破棄する
			void GameLoop();					//登録しているシーンを実行する
			CScene* Getcene();				//登録しているシーンの情報を取得用

		private:
			CScene* m_new_scene;
			CScene* m_current_scene;
			unsigned long m_time_start;
			unsigned long m_time_end;
			unsigned long m_time;
			int m_time_count;

			void SceneAction();
			void SceneDraw();
			void SceneSwap();			//シーン切り替え
	}Scene;
}
