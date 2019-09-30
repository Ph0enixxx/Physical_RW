#include<Windows.h>
#include<stdio.h>

#define IOCTL_WritePhysicalMemory (DWORD32)CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)		//д

//�ṹ������
typedef struct _WritePhysicalMemoryStruct
{
	DWORD64 PhysicalAddress;	//�����ַ
	DWORD32 WriteData;			//д�������
}WritePhysicalMemoryStruct, *PWritePhysicalMemoryStruct;

int main()
{
	HANDLE device;
	DWORD BytesReturned;

	//�򿪾��
	device = CreateFile(L"\\\\.\\Physical_RW", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, NULL);
	if (device == INVALID_HANDLE_VALUE)
	{
		goto Leave;
	}

	while (1)
	{
		//д�����ڴ棬�ĸ��ֽ�
		PWritePhysicalMemoryStruct WritePhysical_dword = VirtualAlloc(NULL, sizeof(WritePhysicalMemoryStruct), MEM_COMMIT | MEM_TOP_DOWN, PAGE_READWRITE);
		printf_s("���������ַ��д���ֵ(16����):");
		scanf_s("%llx %x", &WritePhysical_dword->PhysicalAddress, &WritePhysical_dword->WriteData);

		if (DeviceIoControl(device, IOCTL_WritePhysicalMemory, (LPVOID)WritePhysical_dword, sizeof(WritePhysicalMemoryStruct), NULL, 0, &BytesReturned, NULL))
		{
			printf_s("д��ɹ�\n");
		}
		WritePhysical_dword->PhysicalAddress += 4;
		

		//scanf_s("%llx %x", &WritePhysical_dword->PhysicalAddress, &WritePhysical_dword->WriteData);

		VirtualFree(WritePhysical_dword, 0, MEM_RELEASE);
	}

Leave:

	return 0;
}