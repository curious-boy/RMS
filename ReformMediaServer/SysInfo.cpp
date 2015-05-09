#include "StdHeader.h"
#include "SysInfo.h"

// ���ڻ�ȡ��д��FileServer.ini�����ļ�
CSysInfo::CSysInfo(void)
{
	char curpath[150];
    
	//GetCurrentDirectory( 150, curpath );
	GetModuleFileName( NULL, curpath, MAX_PATH);
	(_tcsrchr(curpath, _T('\\')))[1] = 0;
	sprintf( m_XMLFilePath, "%s\\%s", curpath, "CameraCfg.xml" );
	
	ReadLoacl();
	WriteLoacl();
	//if( !ReadLoacl() )
	//{
	//	m_FilePort		= 8000;
	//	m_RtspPort		= 554;
	//	m_ListenDvrPort	= 8002;
	//	m_bLog			= 1;
	//	m_PushPort		=8004;
	//	WriteLoacl();
	//}
}

CSysInfo::~CSysInfo(void)
{
//	WriteLoacl();
}

// ��ϵͳ������Ϣд���ļ��б���
BOOL CSysInfo::WriteLoacl()
{   
	char path[MAX_PATH];         
	//GetCurrentDirectory(MAX_PATH, path);
	GetModuleFileName( NULL, path, MAX_PATH);
	(_tcsrchr(path, _T('\\')))[1] = 0;
	CString curpath;
	curpath.Format("%s", path);
	curpath = curpath + "\\FileServer.ini";
	DeleteFile(curpath);
    
	CString strport;
	strport.Format("%d", m_FilePort);
	BOOL bWrite = ::WritePrivateProfileString("FileServer", "FilePort", strport,curpath);
	if(!bWrite)
		return FALSE;

	strport.Format("%d", m_RtspPort);
	bWrite = ::WritePrivateProfileString("FileServer","RtspPort", strport, curpath);
	if(!bWrite)
		return FALSE;

	strport.Format("%d", m_bLog);
	bWrite = ::WritePrivateProfileString("FileServer","log", strport, curpath);
	if(!bWrite)
		return FALSE;

	strport.Format("%d", m_PushPort);
	bWrite = ::WritePrivateProfileString("FileServer","PushPort", strport, curpath);
	if(!bWrite)
		return FALSE;

	return TRUE;
}

BOOL CSysInfo::ReadLoacl()    //���ļ���ȡϵͳ������Ϣ
{
	char path[MAX_PATH];                 
	//GetCurrentDirectory(MAX_PATH,path);
	GetModuleFileName( NULL, path, MAX_PATH);
	(_tcsrchr(path, _T('\\')))[1] = 0;
	CString curpath;                                  
	curpath.Format("%s",path);
	curpath = curpath + "\\FileServer.ini";

	m_FilePort = (UINT)::GetPrivateProfileInt("FileServer","FilePort", 8000, curpath);              
	if(m_FilePort == 0)           
		return FALSE;

	m_RtspPort = (UINT)::GetPrivateProfileInt("FileServer","RtspPort", 554, curpath);              
	if(m_RtspPort == 0)           
		return FALSE;

	m_bLog = (int)::GetPrivateProfileInt("FileServer","log", 1, curpath);

	m_PushPort = (int)::GetPrivateProfileInt("FileServer","PushPort", 8004, curpath); 

	return TRUE;
}