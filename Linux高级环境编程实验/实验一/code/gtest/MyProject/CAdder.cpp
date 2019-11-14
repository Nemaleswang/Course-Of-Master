#include "CAdder.h"

CAdder::CAdder()
{
    m_LeftPod = 0;
    m_RightPod = 0;
}

CAdder::CAdder(int LeftPod, int RightPod)
{
    m_LeftPod = LeftPod;
    m_RightPod = RightPod;
}

CAdder::~CAdder()
{
}

int CAdder::Add(int LeftPod, int RightPod)
{
    return LeftPod + RightPod;
}

int CAdder::Add()
{
    return m_LeftPod + m_RightPod;
}

