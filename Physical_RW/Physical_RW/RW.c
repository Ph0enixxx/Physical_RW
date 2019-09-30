#include"RW.h"

/*
function
	��ȡ�����ַ����СΪFF
argv
	MapAddress�������ַӳ������ĵ�ַ
	Physicaladdress��ָ����ȡ�������ַ
return
	����״̬
*/
NTSTATUS ReadPhysicalAddress(PVOID MapAddress, DWORD64 Physicaladdress)
{
	NTSTATUS status;
	PVOID BaseAddress = NULL; // ӳ������ַ
	DWORD32 offset;
	LARGE_INTEGER SectionOffset; //��������ַ
	SIZE_T size = 0x2000; //ӳ���С

	//���ں˶���
	status = GetPhysicalHandle();
	if (status < 0)
	{
		status = FALSE;
		goto Leave;
	}

	//��ȡ�����ĵ�ַ��4K���룬����ƫ���Լ�����һ�¼���
	offset = Physicaladdress & 0xFFF;	//ȡ��12λ��Ϊƫ��ʹ��
	SectionOffset.QuadPart = (ULONGLONG)(Physicaladdress);

	// ӳ�������ڴ��ַ����ǰ���̵����ַ�ռ�
	status = ZwMapViewOfSection(
		hPhysicalhandle,
		NtCurrentProcess(),
		(PVOID *)&BaseAddress,
		0,
		size,
		&SectionOffset,
		&size,
		ViewShare,
		MEM_TOP_DOWN,
		PAGE_READWRITE);

	if (status < 0)
	{
		status = FALSE;
		goto Leave;
	}

	//��ȡӳ�����������
	memmove_s(MapAddress, 0x100, (PVOID)((DWORD64)BaseAddress + offset), 0x100);

	// ��ɲ�����ȡ����ַӳ��
	status = ZwUnmapViewOfSection(NtCurrentProcess(), BaseAddress);

	if (status < 0)
	{
		status = FALSE;
		goto Leave;
	}

Leave:
	if (hPhysicalhandle != NULL)
	{
		ZwClose(hPhysicalhandle);
	}

	return status;
}

/*
function
	д�����ַ����СΪDWORD
argv
	PhysicalAddress�������ַ
	WriteData��д������
return
	����״̬
*/
NTSTATUS WritePhysicalMemory(DWORD64 PhysicalAddress, DWORD32 WriteData)
{
	NTSTATUS status;
	PVOID BaseAddress = NULL; // ӳ������ַ
	DWORD32 offset;
	LARGE_INTEGER SectionOffset; //��������ַ
	SIZE_T size = 0x2000; //ӳ���С

	//���ں˶���
	status = GetPhysicalHandle();
	if (status < 0)
	{
		status = FALSE;
		goto Leave;
	}

	offset = PhysicalAddress & 0xFFF;		//ȡ��12λ��Ϊƫ��ʹ��

	SectionOffset.QuadPart = (ULONGLONG)(PhysicalAddress);

	// ӳ�������ڴ��ַ����ǰ���̵����ַ�ռ�
	status = ZwMapViewOfSection(
		hPhysicalhandle,
		NtCurrentProcess(),
		(PVOID *)&BaseAddress,
		0,
		size,
		&SectionOffset,
		&size,
		ViewShare,
		MEM_TOP_DOWN,
		PAGE_READWRITE);

	if (status < 0)
	{
		status = FALSE;
		goto Leave;
	}

	memmove_s((PVOID)((DWORD64)BaseAddress + offset), sizeof(DWORD32), &WriteData, sizeof(DWORD32));

	status = ZwUnmapViewOfSection(NtCurrentProcess(), BaseAddress);

	if (status < 0)
	{
		status = FALSE;
	}

Leave:
	if (hPhysicalhandle != NULL)
	{
		ZwClose(hPhysicalhandle);
	}

	return status;
}

/*
function
	���ں˶���
argv
	NULL
return
	���ص���״̬
*/
NTSTATUS GetPhysicalHandle()
{
	NTSTATUS status;
	UNICODE_STRING PhysicalMemoryString;
	OBJECT_ATTRIBUTES attributes;

	WCHAR PhysicalMemoryName[] = L"\\Device\\PhysicalMemory";
	RtlInitUnicodeString(&PhysicalMemoryString, PhysicalMemoryName);
	InitializeObjectAttributes(&attributes, &PhysicalMemoryString, 0, NULL, NULL);
	status = ZwOpenSection(&hPhysicalhandle, SECTION_MAP_READ | SECTION_MAP_WRITE, &attributes);

	return status;
}