#pragma once
#include"Physical_RW.h"

//�����ַ��д��ȫ�־��
static HANDLE hPhysicalhandle = NULL;

//��������
NTSTATUS ReadPhysicalAddress(PVOID MapAddress, DWORD64 Physicaladdress);
NTSTATUS WritePhysicalMemory(DWORD64 PhysicalAddress, DWORD32 WriteData);
NTSTATUS GetPhysicalHandle();


//�ṹ������
typedef struct _WritePhysicalMemoryStruct
{
	DWORD64 PhysicalAddress;	//�����ַ
	DWORD32 WriteData;			//д�������
}WritePhysicalMemoryStruct, *PWritePhysicalMemoryStruct;
