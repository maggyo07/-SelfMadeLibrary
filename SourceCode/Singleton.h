#pragma once
#include <vector>
#include <mutex>
#include <assert.h>

using namespace std;
class SingletonFinalizer
{
public:
	//�֐��擾�p�^
	typedef void(*FinalizerFunc)();

	//�֐���ǉ�
	static void addFinalizer(FinalizerFunc func);
	//�o�^���ꂽ�֐����ĂсA�֐��i�[�p�ϐ���j��
	static void Finalize();
private:
	static vector<FinalizerFunc>* gFinalizers;//�֐��i�[�p
};

template <typename T>
class Singleton final
{
public:
	//�C���X�^���X���擾�p�֐�
	//�����g��cpp�ŏ����������������킩��Ȃ�
	static T& GetInstance()
	{
		//��x�����Ăяo��
		std::call_once(initFlag, Create);
		//�����Ɛ�������Ă��邩���`�F�b�N
		assert(instance);
		//�C���X�^���X��Ԃ�
		return *instance;
	}

private:
	//���������Z�b�g���A�j���֐����ĂԂ悤�ɂ���
	static void Create() 
	{
		instance = new T;//�C���X�^���X�̍쐬
		SingletonFinalizer::addFinalizer(&Singleton<T>::Destroy);
	}

	//�j������
	static void Destroy() 
	{
		//�C���X�^���X��������ΏI��
		if (!instance)
			return;
		//�C���X�^���X��j��
		delete instance;
		instance = nullptr;
	}
	static once_flag initFlag;	//�C���X�^���X�̍쐬��1��ɂ��邽�߂̐���p
	static T* instance;			//�C���X�^���X
};

template <typename T> once_flag Singleton<T>::initFlag;//�C���X�^���X�̍쐬��1��ɂ��邽�߂̐���p
template <typename T> T* Singleton<T>::instance = nullptr;//�C���X�^���X