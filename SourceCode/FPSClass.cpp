#include "FPSClass.h"

FPSClass::FPSClass()	:m_fps(0),m_count(0),m_start_time(timeGetTime())
{

}
void FPSClass::Initialize()
{

}
void FPSClass::Delete()
{

}

void FPSClass::Frame()
{
	m_count++;

	if (timeGetTime() >= (m_start_time + 1000))
	{
		m_fps = m_count;
		m_count = 0;

		m_start_time = timeGetTime();
	}
}

int FPSClass::GetFps()
{
	return m_fps;
}