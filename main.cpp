// usb_enum_demo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "impl.h"


int _tmain(int argc, _TCHAR* argv[])
{
    ULONG ulDeviceCnt = 0;

    Impl impl;
    impl.Init();
    impl.EnumerateHostControllers(&ulDeviceCnt);
	return 0;
}

