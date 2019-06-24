#pragma once
#include <vector>
#include <mutex>
#include <assert.h>

using namespace std;
class SingletonFinalizer
{
public:
	//関数取得用型
	typedef void(*FinalizerFunc)();

	//関数を追加
	static void addFinalizer(FinalizerFunc func);
	//登録された関数を呼び、関数格納用変数を破棄
	static void Finalize();
private:
	static vector<FinalizerFunc>* gFinalizers;//関数格納用
};

template <typename T>
class Singleton final
{
public:
	//インスタンス情報取得用関数
	//※中身をcppで書きたいがやり方がわからない
	static T& GetInstance()
	{
		//一度だけ呼び出す
		std::call_once(initFlag, Create);
		//ちゃんと生成されているかをチェック
		assert(instance);
		//インスタンスを返す
		return *instance;
	}

private:
	//メモリをセットし、破棄関数を呼ぶようにする
	static void Create() 
	{
		instance = new T;//インスタンスの作成
		SingletonFinalizer::addFinalizer(&Singleton<T>::Destroy);
	}

	//破棄する
	static void Destroy() 
	{
		//インスタンスが無ければ終了
		if (!instance)
			return;
		//インスタンスを破棄
		delete instance;
		instance = nullptr;
	}
	static once_flag initFlag;	//インスタンスの作成を1回にするための制御用
	static T* instance;			//インスタンス
};

template <typename T> once_flag Singleton<T>::initFlag;//インスタンスの作成を1回にするための制御用
template <typename T> T* Singleton<T>::instance = nullptr;//インスタンス