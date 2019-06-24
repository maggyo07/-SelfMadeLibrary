#include "Singleton.h"

vector<SingletonFinalizer::FinalizerFunc>* SingletonFinalizer::gFinalizers;//�֐��i�[�p

#pragma region SingletonFinalizer
//�֐���ǉ�
void SingletonFinalizer::addFinalizer(FinalizerFunc func)
{
	//�֐��i�[�p�ϐ����쐬����
	if (!gFinalizers)
		gFinalizers = new vector<FinalizerFunc>;
	//�֐���ǉ�
	gFinalizers->push_back(func);
}
//�o�^���ꂽ�֐����ĂсA�֐��i�[�p�ϐ���j��
void SingletonFinalizer::Finalize()
{
	//�֐��i�[�p�ϐ����쐬����Ă��Ȃ���ΏI��
	if (!gFinalizers)
		return;

	//�֐������ׂČĂ�
	for (auto itr = gFinalizers->begin(); itr < gFinalizers->end(); itr++)
		(*itr)();

	//�֐��i�[�p�ϐ���j��
	gFinalizers->clear();
	delete gFinalizers;
	gFinalizers = nullptr;
}
#pragma endregion

#pragma region Singleton
//�C���X�^���X���擾�p�֐�
//template<typename T>
//T& Singleton<T>::GetInstance()
//{
//	//��x�����Ăяo��
//	std::call_once(initFlag, Create);
//	//�����Ɛ�������Ă��邩���`�F�b�N
//	assert(instance);
//	//�C���X�^���X��Ԃ�
//	return *instance;
//}

////���������Z�b�g���A�j���֐����ĂԂ悤�ɂ���
//template<typename T>
//void Singleton<T>::Create()
//{
//	instance = new T;//�C���X�^���X�̍쐬
//	SingletonFinalizer::addFinalizer(&Singleton<T>::Destroy);
//}
//
////�j������
//template<typename T>
//void Singleton<T>::Destroy()
//{
//	//�C���X�^���X��������ΏI��
//	if (!instance)
//		return;
//	//�C���X�^���X��j��
//	delete instance;
//	instance = nullptr;
//}
#pragma endregion
