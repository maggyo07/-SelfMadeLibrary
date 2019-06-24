#include "ModelListClass.h"

#include <fstream>

ModelListClass::ModelListClass()	:m_model_list(new list<shared_ptr< ModelInfoType >>)
{

}
bool ModelListClass::Initialize()
{
	return true;
}

void ModelListClass::Delete()
{
	if (m_model_list)
	{
		m_model_list->clear();
		delete m_model_list;
	}
}

int ModelListClass::GetModelCount()
{
	return m_model_list->size();
}

void ModelListClass::GetData(int index,D3DXVECTOR3& position, D3DXVECTOR4& color)
{
	//indexの値がおかしい
	if (index < 0 || index >= m_model_list->size())
		return;

	auto itr = m_model_list->begin();
	for (; index != 0;index--)
		itr++;

	if (position != nullptr)
		position = itr->get()->position;

	if (color != nullptr)
		color = itr->get()->color;
}
//ランダムな位置と色の玉を作成する(テスト用)
void ModelListClass::TestRandomBllCreation(int num)
{
	//ランダムジェネレータに現在の時刻をシードする
	srand((unsigned int)time(NULL));

	//指定数球を作成する
	for (int count = 0; count < num; count++)
	{
		//リストに追加する変数
		shared_ptr<ModelInfoType> add_model_info(new ModelInfoType);

		//モデルの色をランダムに生成する
		add_model_info.get()->color.x = (float)rand() / RAND_MAX;	//赤
		add_model_info.get()->color.y = (float)rand() / RAND_MAX;	//緑
		add_model_info.get()->color.z = (float)rand() / RAND_MAX;	//青
		add_model_info.get()->color.w = 1.0f;						//透過

		//モードに対してビューアの前にランダムな位置を生成する
		add_model_info.get()->position.x = (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f;
		add_model_info.get()->position.y = (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f;
		add_model_info.get()->position.z = ((((float)rand() - (float)rand()) / RAND_MAX) * 10.0f) + 0.5f;

		//リストに追加
		m_model_list->push_back(move(add_model_info));
	}		 
}

bool ModelListClass::LoadModel(const wchar_t* name)
{
	//開発中・・・

	//参照サイト
	//https://qiita.com/gshirato/items/87586421de0ec37e46a6
	//http://help.autodesk.com/view/FBX/2017/ENU/?guid=__files_GUID_274163DA_9E89_4DCC_8AF6_10B0C498582E_htm
	//https://hexed.it/
	//ファイルを開く用
	ifstream fin;

	//ファイルを開く
	fin.open(name,ios::binary | ios::in);

	//ファイルが見つからなければ終了
	if (fin.fail())
		return false;
	
	//ヘッダーを読み込み(最初の27バイト)-----------------
	char magic_number[21];	//マジックナンバー
	char padding[2];		//使いみちは不明だが全ての確認済みファイルでこの並び
	UINT fbx_version;			//バージョン
	fin.read(magic_number, sizeof(magic_number));
	fin.read(padding, sizeof(padding));
	fin.read((char*)&fbx_version, sizeof(fbx_version));

	//マジックナンバーを見てバイナリデータかどうかを確認
	if (!strstr(magic_number, "Binary"))
		return false;
	//---------------------------------------------------
	//-----------FBXHeaderExtension----------------------
	if (StringSearchFromFile(fin, "FBXHeaderExtension"))
	{
		//ヘッダーバージョン
		UINT heder_version;
		//バージョン
		UINT version;
		if (StringSearchFromFile(fin, "FBXHeaderVersionI"))
			fin.read((char*)&heder_version, sizeof(heder_version));
		if (StringSearchFromFile(fin, "FBXVersionI"))
			fin.read((char*)&version, sizeof(version));
		if (StringSearchFromFile(fin, "CreationTimeStamp"))
		{

		}
	}
	

	//---------------------------------------------------
	char aa = 0x49;

	char xx[100];
	fin.read(xx, 100);
	char c;
	while (fin.get(c))
	{
		c;
	}

	fin.close();
	return true;
}

/*
読み込み済みファイルから指定も文字列を探索し、そこまで読み込む
引数1 fin			:読み込み済みファイル
引数2 search_str	:探索文字列(この文字列があるかどうかを探索する)
戻り値 bool	:true＝成功	false＝失敗
*/
bool ModelListClass::StringSearchFromFile(ifstream& fin, const char* search_str)
{
	//読み込まれていない
	if (!fin)
		return false;
	//ファイルから取得する文字列格納用(一度に取れる文字数50)
	char fin_str[SEARCH_LENGTH_MAX];
	
	//長さ
	int length = strlen(search_str);

	//探索文字列の数が最大値を超えていたり、
	//０以下であれば終了
	if (length > SEARCH_LENGTH_MAX || length <= 0)
		return false;

	//
	while (fin.get(fin_str[0]))
	{
		if (fin_str[0] == search_str[0])
		{
			fin.read(&fin_str[1], length - 1);

			if (strstr(fin_str, search_str))
				return true;
		}
	}

	return false;
}
