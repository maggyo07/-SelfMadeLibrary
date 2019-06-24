#include "Singleton.h"

vector<SingletonFinalizer::FinalizerFunc>* SingletonFinalizer::gFinalizers;//関数格納用

#pragma region SingletonFinalizer
//関数を追加
void SingletonFinalizer::addFinalizer(FinalizerFunc func)
{
	//関数格納用変数を作成する
	if (!gFinalizers)
		gFinalizers = new vector<FinalizerFunc>;
	//関数を追加
	gFinalizers->push_back(func);
}
//登録された関数を呼び、関数格納用変数を破棄
void SingletonFinalizer::Finalize()
{
	//関数格納用変数が作成されていなければ終了
	if (!gFinalizers)
		return;

	//関数をすべて呼ぶ
	for (auto itr = gFinalizers->begin(); itr < gFinalizers->end(); itr++)
		(*itr)();

	//関数格納用変数を破棄
	gFinalizers->clear();
	delete gFinalizers;
	gFinalizers = nullptr;
}
#pragma endregion

#pragma region Singleton
//インスタンス情報取得用関数
//template<typename T>
//T& Singleton<T>::GetInstance()
//{
//	//一度だけ呼び出す
//	std::call_once(initFlag, Create);
//	//ちゃんと生成されているかをチェック
//	assert(instance);
//	//インスタンスを返す
//	return *instance;
//}

////メモリをセットし、破棄関数を呼ぶようにする
//template<typename T>
//void Singleton<T>::Create()
//{
//	instance = new T;//インスタンスの作成
//	SingletonFinalizer::addFinalizer(&Singleton<T>::Destroy);
//}
//
////破棄する
//template<typename T>
//void Singleton<T>::Destroy()
//{
//	//インスタンスが無ければ終了
//	if (!instance)
//		return;
//	//インスタンスを破棄
//	delete instance;
//	instance = nullptr;
//}
#pragma endregion
