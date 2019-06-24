#pragma once
#include <memory>
#include <list>
#include <algorithm>

using namespace std;

namespace GameL
{
	//�p�����ۗp�@�I�u�W�F�N�g�N���X
	class GameObject
	{
		public:
			GameObject();
			virtual ~GameObject();
			virtual void Initialize() = 0;	//�������֐�
			virtual void Action() = 0;	//�����֐�
			virtual void Draw() = 0;	//�`��֐�
			virtual void Delete() = 0;	//�폜�֐�
			virtual void OnEnable() {};	//�A�N�e�B�u�ɂȂ����u�ԂɌĂ΂��
			virtual void OnDisable() {};//��A�N�e�B�u�ɂȂ����u�ԂɌĂ΂��
			void		 UpDataParentAndChilds();			   //�e�̏��Ǝq�̏����X�V����
			void		 DeleteParentAndChilds();			   //�e�q�̏������S�ɍ폜����
			void		 SetDrawOrder(unsigned int draw_order);//�`��D�揇�ʂ�ݒ肷��
			unsigned int GetDrawOrder();					   //�ݒ肵�Ă���`��D�揇�ʎ擾�p
			void		 SetName(int name);					   //�I�u�W�F�N�g���OID�ύX�p
			int			 GetName();							   //�I�u�W�F�N�g���OID�擾�p
			void		 SetActive(bool active);			   //�I�u�W�F�N�g�̃A�N�e�B�u��Ԃ�ύX
			bool		 GetActive();						   //�I�u�W�F�N�g�̃A�N�e�B�u��Ԃ��擾�p
			void		 SetDeleteFlag(bool delete_flag);	   //�I�u�W�F�N�g���폜����
			bool		 GetDeleteFlag();					   //�I�u�W�F�N�g�̍폜�t���O���擾����
			GameObject*	 GetChild(int id);					   //�q�v�f���擾����
			GameObject** GetChilds(int *out_count);			   //�q�v�f�S�Ă��擾����
			void		 SetChild(const weak_ptr<GameObject> child); //�q�v�f��ݒ肷��
			GameObject*	 GetParent();						   //�e�����擾����
			void		 SetParent(GameObject* parent);			   //�e��ݒ肷��
			bool		 GetParentActive();					   //�e�̃A�N�e�B�u��Ԃ��擾����@���e�����݂��Ȃ��ꍇtrue��Ԃ�
			
		private:
			bool			m_active;		//�I�u�W�F�N�g�̃A�N�e�B�u���(Action&Draw�����s���邩�ǂ���)
			bool			m_delete_flag;	//�I�u�W�F�N�g���폜���邩�ǂ���(����Action���s���O��ON�Ȃ�폜����)
			unsigned int	m_draw_order;	//�`��D�揇��
			int				m_name;			//�I�u�W�F�N�g�̖��OID
			weak_ptr<GameObject>	m_parent;		//�e�̏��
			list<weak_ptr<GameObject>>* m_childs;	//�q�̏��
	};

	//�I�u�W�F�N�g�}�l�[�W���[
	class GameObjectManagerClass
	{
		public:
			GameObjectManagerClass();
			void Initialize();					//�������֐�
			void Delete();				//�폜�֐�
			void InsertObj(GameObject* obj,int name,unsigned int draw_order);	//�I�u�W�F�N�g�̓o�^
			void ListDeleteSceneObj();	//�I�u�W�F�N�g���X�g�폜����
			void ObjsAction();			//���X�g�ɓo�^���Ă���S�ẴI�u�W�F�N�g�̃A�N�V�������\�b�h���s
			void ObjsDraw();				//���X�g�ɓo�^���Ă���S�ẴI�u�W�F�N�g�̕`�惁�\�b�h���s
			void ResetDrawOrder();		//�`��D�揇�ʂ�ύX(���X�g��`��D�揇�ʂ̍~���ɕ��ёւ�)

			GameObject* GetObj(int name,bool activ_obj_flag = false);		//���O��񂩂�I�u�W�F�N�g�����P�擾
			shared_ptr<GameObject> GetObj(GameObject*);
			GameObject** GetObjs(int name, int *out_objs_count, bool activ_obj_flag = false);	//���O��񂩂�I�u�W�F�N�g���𕡐��擾
		private:
			list<shared_ptr<GameObject>>* m_list_data;	//�Q�[�����s���X�g
	};

}