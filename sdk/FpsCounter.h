#pragma once

#include "gl.h"

class FpsCounter {
public:
	FpsCounter() : m_FrameCount(0), m_CurrentTime(0), m_PreviousTime(0), m_Fps(0.0)  { }

	double calculateFps();
private:
	unsigned int m_FrameCount;
	int m_CurrentTime;
	int m_PreviousTime;
	double m_Fps;
};