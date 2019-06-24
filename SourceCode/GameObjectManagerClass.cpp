#include "GameObjectManagerClass.h"

#include "AccessController.h"

using namespace GameL;
using System::ObjManager;

//�\�[�g�p �`��D�揇�ʂ��r����(�~��)
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
//�e�̏��Ǝq�̏����X�V����
void GameObject::UpDataParentAndChilds()
{
	//�q�̏����X�V����
	auto itr = m_childs->begin();
	while (itr != m_childs->end())
	{
		//���łɎQ�Ɛ�̃��������������Ă�����폜
		if ((*itr).expired())
		{
			(*itr).reset();
			itr = m_childs->erase(itr);
		}
		else
			itr++;
	}
}
//�e�q�̏������S�ɍ폜����
void GameObject::DeleteParentAndChilds()
{
	//�폜����O�ɏ����X�V����
	UpDataParentAndChilds();
	//�e�̏����폜����
	m_parent.reset();

	//�q�̏����폜����
	auto itr = m_childs->begin();
	while (itr != m_childs->end())
	{
		itr->lock().get()->Delete();
		itr->lock().get()->DeleteParentAndChilds();
		itr = m_childs->erase(itr);
	}
	m_childs->clear();
}
//�`��D�揇�ʕύX
void GameObject::SetDrawOrder(unsigned int draw_order)
{
	//�l��ύX����
	m_draw_order = draw_order;
	//���X�g����ёւ���
	ObjManager().ResetDrawOrder();
}

//�ݒ肵�Ă���`��D�揇�ʎ擾�p
unsigned int GameObject::GetDrawOrder()
{
	return m_draw_order;
}

//�I�u�W�F�N�g���OID�ύX�p
void GameObject::SetName(int name)
{
	m_name = name;
}

//�I�u�W�F�N�g���OID�擾�p
int GameObject::GetName()
{
	return m_name;
}

//�I�u�W�F�N�g�̃A�N�e�B�u��Ԃ�ύX
void GameObject::SetActive(bool active)
{
	//���݂̃A�N�e�B�u�ƈ����̃A�N�e�B�u���Ⴄ��������
	if (active != m_active)
	{
		//�A�N�e�B�u�ɂȂ����u�ԂȂ̂Ő�p���\�b�h���Ă�
		if (active)
			this->OnEnable();
		//��A�N�e�B�u�ɂȂ����u�ԂȂ̂Ő�p���\�b�h���Ă�
		else
			this->OnDisable();
		//�A�N�e�B�u���X�V����
		m_active = active;
	}
}

//�I�u�W�F�N�g�̃A�N�e�B�u��Ԃ��擾�p
bool GameObject::GetActive()
{
	return m_active;
}
//�I�u�W�F�N�g���폜����
void GameObject::SetDeleteFlag(bool delete_flag)
{
	m_delete_flag = delete_flag;
}
//�I�u�W�F�N�g�̍폜�t���O���擾����
bool GameObject::GetDeleteFlag()
{
	return m_delete_flag;
}
//�q�v�f���擾����
GameObject* GameObject::GetChild(int id)
{
	//�q�v�f���Ȃ���ΏI��
	if ((int)m_childs->size() <= id) return nullptr;

	//����id�Ԗڂ̎q�̏���Ԃ�
	int count = 0;
	for (auto itr = m_childs->begin(); itr != m_childs->end(); itr++)
	{
		if (count == id)
			return itr->lock().get();
		count++;
	}
	return nullptr;
}
//�q�v�f�S�Ă��擾����
GameObject** GameObject::GetChilds(int *out_count)
{
	//�q�v�f���Ȃ���ΏI��
	if ((int)m_childs->size() <= 0)return nullptr;

	//�q�̐����擾
	*out_count = (int)m_childs->size();

	//�q�̃I�u�W�F�N�g���i�[�p
	GameObject** childs = new GameObject*[(int)m_childs->size()];

	//�q�̃I�u�W�F�N�g���i�[
	int child_count = 0;
	for each (auto child in *m_childs)
	{
		childs[child_count] = child.lock().get();
		child_count++;
	}

	return childs;
}
//�q�v�f��ݒ肷��
void GameObject::SetChild(const weak_ptr<GameObject> child)
{
	//�q�����g�܂��͖����ꍇ�͏I������
	if (child.lock().get() == this || child.lock().get() == nullptr) return;

	//���łɓo�^�ς݂Ȃ�o�^�����ɏI������
	for each (auto obj in *m_childs)
		if (obj.lock().get() == child.lock().get())
			return;

	m_childs->push_back(child);
}
//�e�����擾����
GameObject* GameObject::GetParent()
{
	return m_parent.lock().get();
}
//�e��ݒ肷��
void GameObject::SetParent(GameObject* parent)
{
	//�e�̏�񂪎��g�܂��햳���ꍇ�͏I������
	if (parent == this || parent == nullptr) return;

	//�e����ݒ肷��
	m_parent = ObjManager().GetObj(parent);

	//�e�̎q�ɂȂ����̂Őݒ肷��
	m_parent.lock().get()->SetChild(ObjManager().GetObj(this));
}
//�e�̃A�N�e�B�u��Ԃ��擾����@���e�����݂��Ȃ��ꍇtrue��Ԃ�
bool GameObject::GetParentActive()
{
	//�e�����݂��Ȃ����true��Ԃ�
	if (m_parent.lock().get() == nullptr) return true;

	//�e�̃A�N�e�B�u��ԃ`�F�b�N
	if (m_parent.lock().get()->GetActive())
	{
		//�e�̐e�̃A�N�e�B�u��ԃ`�F�b�N
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
//������
void GameObjectManagerClass::Initialize()
{

}

//�폜
void GameObjectManagerClass::Delete()
{
	ListDeleteSceneObj();
	delete m_list_data;
}

//�I�u�W�F�N�g�̓o�^
void GameObjectManagerClass::InsertObj(GameObject* obj, int name, unsigned int draw_order)
{
	//�|�����[�t�B�Y�������󂵂Ă���
	//���������E�E�E
	if (obj == nullptr)
		return;

	//�I�u�W�F�N�g�̖��O�ݒ�
	obj->SetName(name);

	//�I�u�W�F�N�g�̏��������\�b�h���s
	obj->Initialize();

	//�f�[�^�o�^
	unique_ptr<GameObject> ptr_obj(obj);
	m_list_data->push_front(move(ptr_obj));

	//�I�u�W�F�N�g�̕`��D�揇�ʐݒ�
	obj->SetDrawOrder(draw_order);

	return;
}

//�I�u�W�F�N�g���X�g�폜����
void GameObjectManagerClass::ListDeleteSceneObj()
{
	auto itr = m_list_data->begin();
	while (itr != m_list_data->end())
	{
		//�e�q�̏����폜
		(*itr)->DeleteParentAndChilds();
		itr = m_list_data->erase(itr);
	}
	m_list_data->clear();
}

//���X�g�ɓo�^���Ă���S�ẴI�u�W�F�N�g�̃A�N�V�������\�b�h���s
void GameObjectManagerClass::ObjsAction()
{
	//�폜���s
	auto itr = m_list_data->begin();
	while (itr != m_list_data->end())
	{
		if (itr->get()->GetDeleteFlag() == true)
		{
			(*itr)->Delete();
			//�e�q�����폜����
			(*itr)->DeleteParentAndChilds();
			itr = m_list_data->erase(itr);
		}
		else
			itr++;
	}

	//�e�I�u�W�F�N�g�̎q�v�f�Ɛe�̏����X�V����
	for each (auto obj in *m_list_data)
	{
		obj->UpDataParentAndChilds();
	}

	//�A�N�V�������s
	for each (shared_ptr<GameObject> obj in *m_list_data)
	{
		//��ԏ�ԂȂ�A�N�V�������\�b�h���s
		if (obj->GetActive() == true && obj->GetParentActive() == true)
			obj->Action();
	}
}

//���X�g�ɓo�^���Ă���S�ẴI�u�W�F�N�g�̕`�惁�\�b�h���s
void GameObjectManagerClass::ObjsDraw()
{
	for each (shared_ptr<GameObject> obj in *m_list_data)
	{
		//��ԏ�ԂȂ�`�惁�\�b�h���s
		if (obj->GetActive() == true && obj->GetParentActive() == true)
			obj->Draw();
	}
}

//�`��D�揇�ʂ�ύX(���X�g��`��D�揇�ʂ̍~���ɕ��ёւ�)
void GameObjectManagerClass::ResetDrawOrder()
{
	//���X�g�ɂQ�ȏ��񂪂���΃\�[�g������
	if (m_list_data->size() >= 2)
	{
		m_list_data->sort(DrawOrderCompare);
	}
}

//���O��񂩂�I�u�W�F�N�g�����P�擾
//����2 activ_obj_flag	:�A�N�e�B�u��Ԃ�OFF�̃I�u�W�F�N�g���܂ނ��ǂ���
GameObject* GameObjectManagerClass::GetObj(int name, bool activ_obj_flag)
{
	for (auto itr = m_list_data->begin(); itr != m_list_data->end(); itr++)
	{
		//�����̃A�N�e�B�u�t���O��ON�@��
		//�A�N�e�B�u��ԂȂ�擾�ł���
		if (activ_obj_flag || (*itr)->GetActive() == true)
		{
			//���O����v�����ŏ��̃I�u�W�F�N�g����n��
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
//���O��񂩂�I�u�W�F�N�g���𕡐��擾
//����3 activ_obj_flag	:�A�N�e�B�u��Ԃ�OFF�̃I�u�W�F�N�g���܂ނ��ǂ���
GameObject** GameObjectManagerClass::GetObjs(int name, int *out_objs_count, bool activ_obj_flag)
{
	GameObject** objs = nullptr;

	int obj_count = 0;

	//���O����v����I�u�W�F�N�g���������݂��邩�𐔂���
	for (auto itr = m_list_data->begin(); itr != m_list_data->end(); itr++)
	{
		//�����̃A�N�e�B�u�t���O��ON�@��
		//�A�N�e�B�u��ԂȂ�擾�ł���
		if (activ_obj_flag || (*itr)->GetActive() == true)
		{
			//���O����v�����Ȃ�J�E���g�����Z����
			if ((*itr)->GetName() == name)
				obj_count++;
		}
	}

	//���������̃I�u�W�F�N�g�����邩���������̂Ń��������Z�b�g����
	objs = new GameObject*[obj_count];
	//�I�u�W�F�N�g�������������̂������������̂Œl�������Ƃ���
	*out_objs_count = obj_count;
	//�I�u�W�F�N�g�J�E���g���g���񂵂����̂ŏ�����
	obj_count = 0;

	//��͂�����x�����̃I�u�W�F�N�g���������Ċi�[����
	for (auto itr = m_list_data->begin(); itr != m_list_data->end(); itr++)
	{
		//�����̃A�N�e�B�u�t���O��ON�@��
		//�A�N�e�B�u��ԂȂ�擾�ł���
		if (activ_obj_flag || (*itr)->GetActive() == true)
		{
			//���O����v����A�i�[����
			if ((*itr)->GetName() == name)
				objs[obj_count++] = (*itr).get();
		}
	}

	//�S�Ă����������̂œn��
	return objs;
}
#pragma endregion
