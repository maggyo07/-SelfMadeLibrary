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
//����HitBox�����I�u�W�F�N�g��ݒ肷��
void HitBoxClass::SetObj(void* obj)
{
	if (this == nullptr) return;
	m_my_obj = obj;
}
//�ʒu���E�T�C�Y����ݒ肷��
void HitBoxClass::SetPos(float pos_x, float pos_y, float width, float height)
{
	if (this == nullptr) return;
	m_hit_box.pos_x = pos_x;
	m_hit_box.pos_y = pos_y;
	m_hit_box.width = width;
	m_hit_box.height = height;

}
//�ʒu����ݒ肷��
void HitBoxClass::SetPos(float pos_x, float pos_y)
{
	if (this == nullptr) return;
	m_hit_box.pos_x = pos_x;
	m_hit_box.pos_y = pos_y;
}
//�����E���O�E�|�C���g����ݒ肷��
void HitBoxClass::SetStatus(int element, int name, int point)
{
	if (this == nullptr) return;
	m_hit_status.element = element;
	m_hit_status.name = name;
	m_hit_status.point = point;
}
//���G�t���O��ݒ肷��
void HitBoxClass::SetInvincible(bool invincible)
{
	if (this == nullptr) return;
	m_hit_status.invincible_flag = invincible;
}
//�������Ă��鑊��̐���Ԃ�
int	HitBoxClass::GetHitCount()
{
	if (this == nullptr) return NULL;
	return m_hit_count;
}
//���G��Ԃ�Ԃ�
bool HitBoxClass::GetInvincibility()
{
	if (this == nullptr) return NULL;
	return m_hit_status.invincible_flag;
}
//����HIT_BOX�����I�u�W�F�N�g�A�h���X��Ԃ�
void* HitBoxClass::GetMyObj()
{
	if (this == nullptr) return nullptr;
	return m_my_obj;
}
//�X�e�[�^�X�̏�Ԃ�Ԃ�
HIT_STATUS*	HitBoxClass::GetStatus()
{
	if (this == nullptr) return nullptr;
	return &m_hit_status;
}
//HIT_BOX�̏���Ԃ�
HIT_BOX* HitBoxClass::GetHitBox()
{
	if (this == nullptr) return nullptr;
	return &m_hit_box;
}


//�������Ă��鑊��̒����疼�O�������Ɍ������A�q�b�g�����I�u�W�F�N�g����1�Ԃ�
void* HitBoxClass::CheckObjNameHit(int obj_name)
{
	if (this == nullptr) return nullptr;
	for (int hit_count = 0; hit_count < m_hit_count; hit_count++)
		if (m_hit_data[hit_count].obj != nullptr)
			if (m_hit_data[hit_count].name == obj_name)
				return m_hit_data[hit_count].obj;
	return nullptr;
}
//�������Ă��鑊��̒����疼�O�E�|�C���g�������Ɍ������A�q�b�g�����I�u�W�F�N�g����1�Ԃ�
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
//�������Ă��鑊��̒����瑮���������Ɍ������A�q�b�g�������ǂ�����Ԃ�
bool HitBoxClass::CheckElementHit(int element)
{
	if (this == nullptr) return NULL;
	for (int hit_count = 0; hit_count < m_hit_count; hit_count++)
		if (m_hit_data[hit_count].obj != nullptr)
			if (m_hit_data[hit_count].element == element)
				return true;
	return false;
}
//�������Ă��鑊��̒����瑮���E�|�C���g�������Ɍ������A�q�b�g�������ǂ�����Ԃ�
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

//�������Ă��鑊��̒����疼�O�������Ɍ������A�q�b�g�����S��(�ő吔)��HIT_DATA����Ԃ�
HIT_DATA** HitBoxClass::SearchObjNameHit(int obj_name)
{
	if (this == nullptr) return nullptr;
	//�������Ă��鑊��̏��������̏����ɕ��ёւ�
	SortHitBox();
	//�T��������̂ŏ��i�[�p�ϐ��̃��������Z�b�g����
	memset(&(*m_hit_search_data), 0x00, sizeof(HIT_DATA*)*MAX_HITS);
	//�T���Ńq�b�g������
	int search_count = 0;
	for (int hit_count = 0; hit_count < m_hit_count; hit_count++)
		if (m_hit_data[hit_count].obj != nullptr)
			if (m_hit_data[hit_count].name == obj_name)
				m_hit_search_data[search_count++] = &m_hit_data[hit_count];

	//�T���q�b�g���Ȃ�������nullptr��Ԃ�
	if (search_count == 0)
		return nullptr;
	else
		return m_hit_search_data;
}
//�������Ă��鑊��̒����疼�O�E�|�C���g�������Ɍ������A�q�b�g�����S��(�ő吔)��HIT_DATA����Ԃ�
HIT_DATA** HitBoxClass::SearchObjNameHit(int obj_name, int point)
{
	if (this == nullptr) return nullptr;
	//�������Ă��鑊��̏��������̏����ɕ��ёւ�
	SortHitBox();
	//�T��������̂ŏ��i�[�p�ϐ��̃��������Z�b�g����
	memset(&(*m_hit_search_data), 0x00, sizeof(HIT_DATA*)*MAX_HITS);
	//�T���Ńq�b�g������
	int search_count = 0;
	for (int hit_count = 0; hit_count < m_hit_count; hit_count++)
		if (m_hit_data[hit_count].obj != nullptr)
			if (m_hit_data[hit_count].name == obj_name)
				if (m_hit_data[hit_count].point == point)
					m_hit_search_data[search_count++] = &m_hit_data[hit_count];

	//�T���q�b�g���Ȃ�������nullptr��Ԃ�
	if (search_count == 0)
		return nullptr;
	else
		return m_hit_search_data;
}
//�������Ă��鑊��̒����瑮���������Ɍ������A�q�b�g�����S��(�ő吔)��HIT_DATA����Ԃ�
HIT_DATA** HitBoxClass::SearchElementHit(int element)
{
	if (this == nullptr) return nullptr;
	//�������Ă��鑊��̏��������̏����ɕ��ёւ�
	SortHitBox();
	//�T��������̂ŏ��i�[�p�ϐ��̃��������Z�b�g����
	memset(&(*m_hit_search_data), 0x00, sizeof(HIT_DATA*)*MAX_HITS);
	//�T���Ńq�b�g������
	int search_count = 0;
	for (int hit_count = 0; hit_count < m_hit_count; hit_count++)
		if (m_hit_data[hit_count].obj != nullptr)
			if (m_hit_data[hit_count].element == element)
				m_hit_search_data[search_count++] = &m_hit_data[hit_count];

	//�T���q�b�g���Ȃ�������nullptr��Ԃ�
	if (search_count == 0)
		return nullptr;
	else
		return m_hit_search_data;
}
//�������Ă��鑊��̒����瑮���E�|�C���g�������Ɍ������A�q�b�g�����S��(�ő吔)��HIT_DATA����Ԃ�
HIT_DATA** HitBoxClass::SearchElementHit(int element, int point)
{
	if (this == nullptr) return nullptr;
	//�������Ă��鑊��̏��������̏����ɕ��ёւ�
	SortHitBox();
	//�T��������̂ŏ��i�[�p�ϐ��̃��������Z�b�g����
	memset(&(*m_hit_search_data), 0x00, sizeof(HIT_DATA*)*MAX_HITS);
	//�T���Ńq�b�g������
	int search_count = 0;
	for (int hit_count = 0; hit_count < m_hit_count; hit_count++)
		if (m_hit_data[hit_count].obj != nullptr)
			if (m_hit_data[hit_count].element == element)
				if (m_hit_data[hit_count].point == point)
					m_hit_search_data[search_count++] = &m_hit_data[hit_count];

	//�T���q�b�g���Ȃ�������nullptr��Ԃ�
	if (search_count == 0)
		return nullptr;
	else
		return m_hit_search_data;
}

//�������Ă��鑊��̏��������̏����ŕ��ёւ�
void HitBoxClass::SortHitBox()
{
	if (this == nullptr) return;
	for (int front = 0; front < m_hit_count; front++)	//�O
	{
		for (int rear = front + 1; rear < m_hit_count; rear++)	//��
		{
			//�O�̋������擾
			float front_distance = (m_hit_data[front].obj == nullptr) ? 9999.0f : m_hit_data[front].distance;
			//��̋������擾
			float rear_distance = (m_hit_data[rear].obj == nullptr) ? 9999.0f : m_hit_data[rear].distance;

			//�O�̋�������̋������Z����Ε��ёւ����s��
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
//������
void HitBoxManagerClass::Initialize()
{
	
}
//�폜
void HitBoxManagerClass::Delete()
{
	DeleteAllListData();
	delete m_list_hitbox;
}
//HitBox�̍쐬�E�o�^
void HitBoxManagerClass::SetHitBox(void* obj, float pos_x, float pos_y, float width, float height, int element, int name, int point)
{
	//�f�[�^�̍쐬
	shared_ptr<HitBox> ptr_box(new HitBox());
	ptr_box->SetPos(pos_x, pos_y, width, height);	//�ʒu�E���E������ݒ肷��
	ptr_box->SetStatus(element, name, point);		//�����E���O�E�|�C���g��ݒ肷��
	ptr_box->SetObj(obj);						//�I�u�W�F�N�g����ݒ肷��
	ptr_box->SetInvincible(false);				//���G����ݒ肷��
	m_list_hitbox->push_front(ptr_box);			//�f�[�^�̓o�^
}
//���g�̃A�h���X������HitBox��Ԃ�
HitBox* HitBoxManagerClass::GetHitBox(void* obj)
{
	for each (shared_ptr<HitBox> itr in *m_list_hitbox)
	{
		if (itr->GetMyObj() == obj)
			return itr.get();
	}
	return nullptr;
}
//���g�̃A�h���X������HitBox�̍폜
void	HitBoxManagerClass::DeleteHitBox(void* obj)
{
	for (auto itr = m_list_hitbox->begin(); itr != m_list_hitbox->end(); itr++)
	{
		if ((*itr).get()->GetMyObj() == obj)
		{
			m_list_hitbox->erase(itr);	//�f�[�^�폜
			return;
		}
	}
}
//�Փ˔���
void HitBoxManagerClass::AllHitCheck()
{
	//�����͉��ł�
	for each(shared_ptr<HitBox> itr_my in *m_list_hitbox)
	{
		bool hit;

		//�O��̓��e��j��
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
//�Փ˔��胁�\�b�h
//a��b�̂ǂ��ɓ������Ă��邩
bool HitBoxManagerClass::Hit(HIT_BOX* a, HIT_BOX* b, HIT_DATA* hit)
{
	//�����͉��ł�

	//�Փ˔���
	if (
		(a->pos_x + a->width > b->pos_x) && (a->pos_x < b->pos_x + b->width) &&
		(a->pos_y + a->height > b->pos_y) && (a->pos_y < b->pos_y + b->height)
		)
	{
		//�ǂ��ɓ������Ă��邩�`�F�b�N���s���p�x��Ԃ�
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

		//�O�����̒藝���狗����}��
		hit->distance = sqrt((float)(pb_x - pa_x)*(pb_x - pa_x) + (pb_y - pa_y)*(pb_y - pa_y));
		return true;
	}

	return false;
}
//�Փ˔��胊�X�g��j��
void	HitBoxManagerClass::DeleteAllListData()
{
	m_list_hitbox->clear();
}
//�f�o�b�N�p�@�����蔻��`��
void	HitBoxManagerClass::DrawHitBox()
{
	//HitBox�`�掞�̐F���
	//				�@R�@�@G  �@B�@�@A
	float col[4] = { 0.5f,0.0f,0.0f,0.2f*255.0f };

	//�o�^���Ă���HitBox����
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