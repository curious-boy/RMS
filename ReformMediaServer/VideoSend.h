// VideoSend.h: interface for the CVideoSend class.
//
//////////////////////////////////////////////////////////////////////


#pragma once
//#include <afxtempl.h>
//#include "VideoSend.h"
#include "ReformMediaServerDef.H"

class CFileIndex;

class CVideoSend
{
public:// ��ͬ����
	CVideoSend();
	virtual ~CVideoSend();

	BOOL	SendVideo(char *pDate, int length);
	BOOL	AcceptSock(SOCKET s, int num);
	BOOL	InitSock();
	BOOL	GetPeerName(ATL::CString& rPeerAddress, UINT& rPeerPort);
	BOOL	SetSendType(int type);
	
	BOOL	OnSendFile();
	BOOL	OnSendJpgFile();
	BOOL	End();
	static	DWORD WINAPI NetThread( LPVOID lpParameter);

//public:// ���粥�Ų��֣�
	BOOL	Init(FIND_INFO info);
	BOOL	StartNetPlay(_int64 startpos);//iStartTimeΪ¼��ľ���ʱ�䣡����
	BOOL	PauseNetPlay();
	BOOL	ContinueNetPlay();

	CFileIndex *m_pFileIndex;
	int			m_FileIndex;
	int			m_StampIndex;

	FIND_INFO	m_findinfo;

    //char	m_csAdd[20];				// �ͻ���IP��ַ
	CString				m_csAdd;		// �ͻ���IP��ַ
	unsigned int 		m_Port;			// �ͻ��˶˿ں�
//	int		TokenID;

	SOCKET				m_Socket;
	int					m_sockid;		// ��manager�����е�λ��
	int					m_SendType;		// �������ͣ�����

	HANDLE 		m_hNetThread;			// �������粥�š����ش�����߳̾��
	HANDLE		m_hNetKill;				// �������粥�š����ش����߳̽������¼�
	HANDLE		m_DataEvent ;			// �������粥�š����ش����߳���ͣ�������¼�
	ATL::CString  m_Filepath;
	HANDLE		m_NetFile;
	int			m_ReadFilePos;
	DWORD			m_FileLength;
	int			m_curFileLength;
	BOOL		m_NetActive;

	char	m_Head[1024];
	int		m_HeadLength;
	int		m_Speed;
	char	m_JpgSendMem[512*1024];
	int		m_JpgSendMemLength;
	
	DWORD _left_length;
	char _readbuf[BLOCKSIZE_FILE];
	DWORD _buf_size;
	DWORD _buf_left;
	BOOL StartNetPlayOne();
	BOOL OnSendOneFile();
	bool onerun;
	static	DWORD WINAPI NetOneThread( LPVOID lpParameter);
};

