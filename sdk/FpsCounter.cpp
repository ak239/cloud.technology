#include "FpsCounter.h"

double FpsCounter::calculateFps()
{
	//  Increase frame count
	m_FrameCount++;

	//  Get the number of milliseconds since glutInit called
	//  (or first call to glutGet(GLUT ELAPSED TIME)).
	m_CurrentTime = glutGet(GLUT_ELAPSED_TIME);

	//  Calculate time passed
	int timeInterval = m_CurrentTime - m_PreviousTime;

	if(timeInterval > 1000)
	{
		//  calculate the number of frames per second
		m_Fps = m_FrameCount / (timeInterval / 1000.0f);

		//  Set time
		m_PreviousTime = m_CurrentTime;

		//  Reset frame count
		m_FrameCount = 0;
	}
	return m_Fps;
}