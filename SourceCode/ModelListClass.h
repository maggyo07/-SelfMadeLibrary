#pragma once

#include <D3DX10math.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <list>
#include <memory>

using namespace std;

constexpr auto SEARCH_LENGTH_MAX{ 50 };

class ModelListClass
{
private:
	struct ModelInfoType
	{
		D3DXVECTOR4 color;
		D3DXVECTOR3 position;
	};
public:
	ModelListClass();
	bool Initialize();
	void Delete();

	int GetModelCount();
	void GetData(int index, D3DXVECTOR3& position, D3DXVECTOR4& color);
	void TestRandomBllCreation(int num);	//ランダムな位置と色の玉を作成する(テスト用)
	bool LoadModel(const wchar_t*);
	/*
	読み込み済みファイルから指定も文字列を探索し、そこまで読み込む
	引数1 fin			:読み込み済みファイル
	引数2 search_str	:探索文字列(この文字列があるかどうかを探索する)
	戻り値 bool	:true＝成功	false＝失敗
	*/
	bool StringSearchFromFile(ifstream& fin, const char* search_str);
private:
	list<shared_ptr<ModelInfoType>>* m_model_list;

};