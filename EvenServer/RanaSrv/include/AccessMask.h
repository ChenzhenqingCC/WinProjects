#ifndef __RANASRV_ACCESSMASK_H
#define __RANASRV_ACCESSMASK_H
#include <Zoic/include/Types.h>

const DWORD RSAM_QUERY				=0x0001;	//查询权限
const DWORD RSAM_SCRIPT_UPLOAD		=0x0002;	//上传脚本权限
const DWORD RSAM_SCRIPT_DOWNLOAD	=0x0004;	//下载脚本权限
const DWORD RSAM_SPACE_RELOAD		=0x0008;	//重新装载脚本权限
const DWORD RSAM_SPACE_MANTAIN		=0x0010;	//Rana空间维护权限
const DWORD RSAM_TRIGGER_MANTAIN	=0x0020;	//触发器维护权限
const DWORD RSAM_TRIGGER_CREATE		=0x0040;	//触发器增加权限
const DWORD RSAM_TRIGGER_DELETE		=0x0080;	//触发器删除权限
const DWORD RSAM_TRIGGER_MODIFY		=0x0100;	//触发器修改权限
const DWORD RSAM_SPACE_SAVE			=0x0200;	//Rana空间保存权限
const DWORD RSAM_SCRIPT_EXECUTE		=0x0400;	//执行脚本权限
const DWORD RSAM_EVENT_MANTAIN		=0x0800;	//事件维护权限
const DWORD RSAM_EVENT_CREATE		=0x1000;	//事件增加权限
const DWORD RSAM_EVENT_DELETE		=0x2000;	//事件删除权限
const DWORD RSAM_SPACE_ALL			=0xFFFF;	//全部权限

#endif
