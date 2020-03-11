#ifndef CADDER_H
#define CADDER_H

class CAdder
{
public:
    CAdder();

    CAdder(int LeftPod, int RightPod);

    virtual ~CAdder();

public:
    virtual int Add(int LeftPod, int RightPod);

    virtual int Add();

private:
    int m_LeftPod;
    int m_RightPod;
};

#endif

