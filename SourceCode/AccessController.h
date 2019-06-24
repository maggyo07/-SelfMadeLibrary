#pragma once

#include "Singleton.h"

#include "SystemClass.h"
#include "InputClass.h"
#include "AudioClass.h"
#include "TimeClass.h"
#include "FpsClass.h"
#include "CPUClass.h"
#include "D3DClass.h"
#include "TextureClass.h"
#include "ModelListClass.h"
#include "GameObjectManagerClass.h"
#include "HitBoxManager.h"

using namespace GameL;

namespace System
{
	inline SystemClass& System() { return Singleton<SystemClass>().GetInstance(); }
	inline InputClass& Input() { return Singleton<InputClass>().GetInstance(); }
	inline AudioClass& Audio() { return Singleton<AudioClass>().GetInstance(); }
	inline TimeClass& Time() { return Singleton<TimeClass>().GetInstance(); }
	inline FPSClass& FPS() { return Singleton<FPSClass>().GetInstance(); }
	inline CPUClass& CPU() { return Singleton<CPUClass>().GetInstance(); }
	inline GameObjectManagerClass& ObjManager() { return Singleton<GameObjectManagerClass>().GetInstance(); }
	inline HitBoxManagerClass& HitBoxManager() { return Singleton<HitBoxManagerClass>().GetInstance(); }
}

namespace Graphics
{
	inline D3DClass& D3D() { return Singleton<D3DClass>().GetInstance(); }
	inline DrawTexture2DClass& DrawTexture2D() { return Singleton<DrawTexture2DClass>().GetInstance(); }
	inline ModelListClass& ModelList() { return Singleton<ModelListClass>().GetInstance(); }
}