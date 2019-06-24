#pragma once

namespace GameL
{
	//�p�����ۗp�@�V�[���N���X
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

	//�V�[���}�l�[�W���[
	typedef class SceneManagerClass
	{
		public:
			SceneManagerClass();
			~SceneManagerClass();
			void Initialize();						//������
			void Delete();
			void SetScene(CScene* scene);	//�V�����V�[�����Z�b�g���āA�Â��V�[����j������
			void GameLoop();					//�o�^���Ă���V�[�������s����
			CScene* Getcene();				//�o�^���Ă���V�[���̏����擾�p

		private:
			CScene* m_new_scene;
			CScene* m_current_scene;
			unsigned long m_time_start;
			unsigned long m_time_end;
			unsigned long m_time;
			int m_time_count;

			void SceneAction();
			void SceneDraw();
			void SceneSwap();			//�V�[���؂�ւ�
	}Scene;
}
