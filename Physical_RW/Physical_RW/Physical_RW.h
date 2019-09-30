#pragma once
#include<ntifs.h>
#include"RW.h"

//IOCTL������
#define IOCTL_ReadPhysicalMemory (DWORD32)CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)		//��
#define IOCTL_WritePhysicalMemory (DWORD32)CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)		//д

//�������ӵȳ�ʼ��
UNICODE_STRING DeviceName;
UNICODE_STRING SymbolLink;
PDEVICE_OBJECT Device;

//��������
VOID DriverUnload(PDRIVER_OBJECT pDriverObject);
NTSTATUS MajorDispatch(PDEVICE_OBJECT device, PIRP Irp);
NTSTATUS DeviceControlDispatch(PDEVICE_OBJECT deviceObject, PIRP Irp);
