#ifndef __RANASRV_ACCESSMASK_H
#define __RANASRV_ACCESSMASK_H
#include <Zoic/include/Types.h>

const DWORD RSAM_QUERY				=0x0001;	//��ѯȨ��
const DWORD RSAM_SCRIPT_UPLOAD		=0x0002;	//�ϴ��ű�Ȩ��
const DWORD RSAM_SCRIPT_DOWNLOAD	=0x0004;	//���ؽű�Ȩ��
const DWORD RSAM_SPACE_RELOAD		=0x0008;	//����װ�ؽű�Ȩ��
const DWORD RSAM_SPACE_MANTAIN		=0x0010;	//Rana�ռ�ά��Ȩ��
const DWORD RSAM_TRIGGER_MANTAIN	=0x0020;	//������ά��Ȩ��
const DWORD RSAM_TRIGGER_CREATE		=0x0040;	//����������Ȩ��
const DWORD RSAM_TRIGGER_DELETE		=0x0080;	//������ɾ��Ȩ��
const DWORD RSAM_TRIGGER_MODIFY		=0x0100;	//�������޸�Ȩ��
const DWORD RSAM_SPACE_SAVE			=0x0200;	//Rana�ռ䱣��Ȩ��
const DWORD RSAM_SCRIPT_EXECUTE		=0x0400;	//ִ�нű�Ȩ��
const DWORD RSAM_EVENT_MANTAIN		=0x0800;	//�¼�ά��Ȩ��
const DWORD RSAM_EVENT_CREATE		=0x1000;	//�¼�����Ȩ��
const DWORD RSAM_EVENT_DELETE		=0x2000;	//�¼�ɾ��Ȩ��
const DWORD RSAM_SPACE_ALL			=0xFFFF;	//ȫ��Ȩ��

#endif
