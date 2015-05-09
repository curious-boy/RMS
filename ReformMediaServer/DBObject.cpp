#include "DBObject.h"
#include "CppSQLite3.h"
#include "Log.h"

CDBObject g_dbobj;

//UTF-8תUnicode
std::wstring Utf82Unicode(const std::string& utf8string)
{
	int widesize = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (widesize == 0)
	{
		throw std::exception("Error in conversion.");
	}

	std::vector<wchar_t> resultstring(widesize);

	int convresult = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, &resultstring[0], widesize);

	if (convresult != widesize)
	{
		throw std::exception("La falla!");
	}

	return std::wstring(&resultstring[0]);
}

//unicode תΪ ascii
string WideByte2Acsi(wstring& wstrcode)
{
	int asciisize = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, NULL, 0, NULL, NULL);
	if (asciisize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (asciisize == 0)
	{
		throw std::exception("Error in conversion.");
	}
	std::vector<char> resultstring(asciisize);
	int convresult =::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, &resultstring[0], asciisize, NULL, NULL);

	if (convresult != asciisize)
	{
		throw std::exception("La falla!");
	}

	return std::string(&resultstring[0]);
}

//utf-8 ת ascii
string UTF_82ASCII(string& strUtf8Code)
{
	string strRet("");


	//�Ȱ� utf8 תΪ unicode 
	wstring wstr = Utf82Unicode(strUtf8Code);

	//���� unicode תΪ ascii
	strRet = WideByte2Acsi(wstr);


	return strRet;
}

//ascii ת Unicode
wstring Acsi2WideByte(string& strascii)
{
	int widesize = MultiByteToWideChar (CP_ACP, 0, (char*)strascii.c_str(), -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (widesize == 0)
	{
		throw std::exception("Error in conversion.");
	}
	std::vector<wchar_t> resultstring(widesize);
	int convresult = MultiByteToWideChar (CP_ACP, 0, (char*)strascii.c_str(), -1, &resultstring[0], widesize);

	if (convresult != widesize)
	{
		throw std::exception("La falla!");
	}

	return std::wstring(&resultstring[0]);
}

//Unicode ת Utf8
std::string Unicode2Utf8(const std::wstring& widestring)
{
	int utf8size = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, NULL, 0, NULL, NULL);
	if (utf8size == 0)
	{
		throw std::exception("Error in conversion.");
	}

	std::vector<char> resultstring(utf8size);

	int convresult = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, &resultstring[0], utf8size, NULL, NULL);

	if (convresult != utf8size)
	{
		throw std::exception("La falla!");
	}

	return std::string(&resultstring[0]);
}

//ascii ת Utf8
string ASCII2UTF_8(string& strAsciiCode)
{
	string strRet("");
	
	//�Ȱ� ascii תΪ unicode 
	wstring wstr = Acsi2WideByte(strAsciiCode);

	//���� unicode תΪ utf8
	strRet = Unicode2Utf8(wstr);

	return strRet;
}

///////////////////////////////////////////////////////////////////////

// ����ļ��Ƿ����
BOOL IsFileExist(string strFile)
{
	if(strFile.empty())
		return FALSE;

	WIN32_FIND_DATA fd;
	memset(&fd, 0, sizeof(WIN32_FIND_DATA));

	HANDLE hFile = FindFirstFile(strFile.c_str(), &fd);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	FindClose(hFile);
	return TRUE;
}

CDBObject::CDBObject(void)
{
	InitializeCriticalSection(&m_CriticalSectionDevice);
	InitializeCriticalSection(&m_CriticalSectionRecord);
}

CDBObject::~CDBObject(void)
{
	ClearData();
	DeleteCriticalSection(&m_CriticalSectionDevice);
	DeleteCriticalSection(&m_CriticalSectionRecord);
}

void CDBObject::ClearData()
{
	for(IterRFCamera iter = m_mapRFCamera.begin(); iter != m_mapRFCamera.end(); ++iter)
		delete (LPRFCamera)iter->second;

	for(IterMapQueryRecordResult iterRecord = m_mapRecord.begin(); iterRecord != m_mapRecord.end(); ++iterRecord)
		delete (LpQueryRecordResult)iterRecord->second;

	m_mapRFCamera.clear();
	m_mapRecord.clear();
}

void CDBObject::CheckDataBase()
{
	CppSQLite3DB database;
	// database.checkDB(gszFile);
	database.open(g_szFile);
}

void CreeateSqliteDb()
{
	//CppSQLite3DB db;

	//// ����Ƿ����
	//if(!IsFileExist(g_szFile))
	//{
	//	// ��������
	//	db.open(gszFile);

	// db.tableExists(TEXT("Camera")

	//	// ��������
	//	//db.setKey("rf123", 5);

	//	// ����Camera�豸��
	//	db.execDML("CREATE TABLE IF NOT EXISTS Camera (								\
	//										Id INTEGER PRIMARY KEY AUTOINCREMENT,	\
	//										CmeraId		varchar NOT NULL,			\
	//										CameraType	varchar NOT NULL,			\
	//										CameraName	varchar NOT NULL,			\
	//										CameraMode	varchar NOT NULL,			\
	//										Provider	varchar NOT NULL,			\
	//										Codec		varchar NOT NULL,			\
	//										Login		varchar NOT NULL,			\
	//										Password	varchar,					\
	//										IP			varchar NOT NULL,			\
	//										Port		varchar NOT NULL,			\
	//										ChannelId	varchar NOT NULL,			\
	//										IsMultiCast	bool NOT NULL,				\
	//										MultiCastAddr varchar,					\
	//										MultiCastPort varchar,					\
	//										RecordType    varchar					\
	//										);");
	//	// д���ʼ��������
	//	int nRet2 = db.execDML("begin transaction;");

	//	char szSql[256] = {0};
	//	string strSql;

	//	sprintf_s(szSql, "INSERT INTO Camera VALUES(NULL, %s, 'DIGITAL', '%s', '%s','%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', 'INVAILID_RECORD')",
	//		"01", "��԰ɽ��", "DVR", "��", "H264", "admin", "admin", "183.63.214.81", "37776", "1", "true", "233.0.0.4", "5152");
	//	strSql = szSql;
	//	nRet2 = db.execDML(ASCII2UTF_8(strSql).c_str());

	//	memset(szSql, 0, 256);
	//	sprintf_s(szSql, "INSERT INTO Camera VALUES(NULL, %s, 'DIGITAL', '%s', '%s','%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', 'INVAILID_RECORD')",
	//		"02", "��԰ɽ��", "DVR", "��", "H264", "admin", "admin", "183.63.214.81", "37776", "2", "true", "233.0.0.4", "5153");
	//	strSql = szSql;
	//	nRet2 = db.execDML(ASCII2UTF_8(strSql).c_str());

	//	memset(szSql, 0, 256);
	//	sprintf_s(szSql, "INSERT INTO Camera VALUES(NULL, %s, 'DIGITAL', '%s', '%s','%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', 'INVAILID_RECORD')",
	//		"03", "�弶ǰ��", "DVR", "��", "H264", "admin", "lfad", "125.93.51.246", "37776", "1", "true", "233.0.0.4", "5154");
	//	strSql = szSql;
	//	nRet2 = db.execDML(ASCII2UTF_8(strSql).c_str());

	//	memset(szSql, 0, 256);
	//	sprintf_s(szSql, "INSERT INTO Camera VALUES(NULL, %s, 'DIGITAL', '%s', '%s','%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', 'INVAILID_RECORD')",
	//		"04", "�弶ǰ��", "DVR", "��", "H264", "admin", "lfad", "125.93.51.246", "37776", "2", "true", "233.0.0.4", "5155");
	//	strSql = szSql;
	//	nRet2 = db.execDML(ASCII2UTF_8(strSql).c_str());

	//	// �ύ����
	//	nRet2 = db.execDML("commit transaction;");
	//	nRet2 = 0;
	//}
}

void CDBObject::Init()
{
	
	string db_path=GetAppPath() +"\\"+"rfserv.db";
	strcpy_s(g_szFile, db_path.c_str());
	g_pLog->WriteLog("���ݿ�:%s\n",g_szFile);
	ClearData();

	//GetDevice();

	SYSTEMTIME loctime;
		GetLocalTime(&loctime);
		char tstart[18]={0};
		sprintf_s(tstart, "%.4d%.2d%.2d000000000", loctime.wYear, loctime.wMonth, loctime.wDay);
		char tend[18]={0};
		sprintf_s(tend, "%.4d%.2d%.2d235959000", loctime.wYear, loctime.wMonth, loctime.wDay);
	GetVideoRecords(tstart, tend, "");
}

void CDBObject::GetDevice()
{
	string		strCameraType;			// ����ͷ����
	string		strCameraName;			// ����ͷ����
	string		strMode;				// ����ͷ����(DVR��NVR��IPC������)
	string		strProvider;			// �豸����(�󻪡���������˼������)
	string		strCodec;				// �����ʽ
	string		strLogin;				// ���ӵ��豸ʱ��¼���û���
	string		strPassword;			// ��¼����
	string		strIp;					// �豸��IP��ַ
	string		strPort;				// �豸���Ӷ˿ں�
	string		strCnlId;				// �豸����ͨ����
	string		IsMultiCast;			// �Ƿ�㲥����
	string		strMultiCastAddress;	// �㲥��ַ
	string		strMultiCastPort;		// �㲥�˿�
	string		strRecordType;			// ¼������

	string strSql = "SELECT * FROM [CAMERA]";
	EnterCriticalSection(&m_CriticalSectionDevice);

	try
	{
		CppSQLite3DB database;
		database.open(g_szFile);

		//int result = database.setKey( "pwd", 3 );		// �������
		//result = database.resetKey( "sqlite3", 7 );	// �޸�����

		LPRFCamera lpCamera = NULL;
		CppSQLite3Query query = database.execQuery(strSql.c_str());
        while (!query.eof())
        {
			lpCamera				= new RFCamera();
			lpCamera->nId			= query.getIntField(0);
            lpCamera->strCameraId	= query.getStringField(1);

			strCameraType			= query.getStringField(2);
            strCameraName			= query.getStringField(3);
            strMode					= query.getStringField(4);
			strProvider				= query.getStringField(5);
			strCodec				= query.getStringField(6);
			strLogin				= query.getStringField(7);
			strPassword				= query.getStringField(8);
			strIp					= query.getStringField(9);
			strPort					= query.getStringField(10);
			strCnlId				= query.getStringField(11);
			string strMul			= query.getStringField(12);
			strMultiCastAddress		= query.getStringField(13);
			strMultiCastPort		= query.getStringField(14);
			strRecordType			= query.getStringField(15);

            lpCamera->strCameraType			= UTF_82ASCII(strCameraType);
            lpCamera->strCameraName			= UTF_82ASCII(strCameraName);
            lpCamera->strMode				= UTF_82ASCII(strMode);
			lpCamera->strProvider			= UTF_82ASCII(strProvider);
			lpCamera->strCodec				= UTF_82ASCII(strCodec);
			lpCamera->strLogin				= UTF_82ASCII(strLogin);
			lpCamera->strPassword			= UTF_82ASCII(strPassword);
			lpCamera->strIp					= UTF_82ASCII(strIp);
			lpCamera->strPort				= UTF_82ASCII(strPort);
			lpCamera->strCnlId				= UTF_82ASCII(strCnlId);
			lpCamera->strMultiCast			= UTF_82ASCII(strMul);
			lpCamera->strMultiCastAddress	= UTF_82ASCII(strMultiCastAddress);
			lpCamera->strMultiCastPort		= UTF_82ASCII(strMultiCastPort);
			lpCamera->strRecordType			= UTF_82ASCII(strRecordType);

            query.nextRow();
			m_mapRFCamera.insert(ValuetRFCamera(lpCamera->nId, lpCamera));
        }
	}
	catch(...)
	{
		// 
	}
	
	//char szContent[1024] = {'\0'};
	//sprintf_s(szContent, 1024, "�������豸��:%d", m_mapDevice.size());
	//SendMessage(AfxGetApp()->GetMainWnd()->m_hWnd, WM_SHOW_EVENT_LOG, 0, (LPARAM)szContent);
	//g_pLog->WriteLogFile(szContent);

	LeaveCriticalSection(&m_CriticalSectionDevice);
}

void CDBObject::GetVideoRecords(string time, string timeEnd, string strkey)
{
	char szSql[256] =  {0};
	string sql_operator="";
	char sql_bytime[256] =  {0};
	char sql_byname[256] = {0};
	if(time.empty())
	{
		if(!timeEnd.empty())
			time = timeEnd;
	}
	else if(timeEnd.empty())
	{
		timeEnd = time;
	}
	if(!time.empty())
	{
		sprintf_s(sql_bytime,"(NOT((%s>=EndTime) OR (%s<=StartTime)))",time.c_str(), timeEnd.c_str());
	}

	if(!strkey.empty())
	{
		sprintf_s(sql_byname,"%s DvrName like '%%%s%%' ", sql_bytime[0]!='\0'?"AND":"", strkey.c_str());
		sql_operator = "where";
	}
	else if(sql_bytime[0]!='\0')
		sql_operator = "where";

	sprintf_s(szSql, sizeof(szSql),"SELECT * FROM [Record] %s %s %s", sql_operator.c_str(), sql_bytime, sql_byname);
	g_pLog->WriteLog("��ѯ¼��[%s]\n",szSql);
	string strSql = szSql;
	strSql= ASCII2UTF_8(strSql);
	//EnterCriticalSection(&m_CriticalSectionRecord);
	
	string		strRecordName;			// ¼���ļ�������
	string		strFolder;				// ¼���ļ����ڵ��ļ���·��
	//bool		bIsRecording;			// ¼���ļ��Ƿ���¼���У����¼���ڽ��У���ʱ��ȡ��¼���ļ������ݻ�ʧ�ܣ�
	string		tStart;					// ¼��ʼʱ��
	string		tEnd;					// ¼�����ʱ��
	string		strDVRName;				// ¼���ļ�����DVR�����Ʊ�ʶ
	string		strDVRAddr;				// ¼���ļ�����DVR��IP��ַ
	string		strDVRPort;				// ¼���ļ�����DVR�Ķ˿ں�
	string		strDVRChannel;			// ¼���ļ�����DVR��ͨ����
	string		strProvider;
	try
	{
		CppSQLite3DB database;
		database.open(g_szFile);
		
		g_pLog->WriteLog("�����ݿ�ɹ�%s\n",g_szFile);

		LpQueryRecordResult LpRecordResult = NULL;
		CppSQLite3Query query = database.execQuery(strSql.c_str());
		int recordCount=0;
		while (!query.eof())
		{
			LpRecordResult				= new QueryRecordResult();
			LpRecordResult->nRecordId	= query.getIntField(0);
			LpRecordResult->nDeviceid	= query.getIntField(1);

			strDVRName				= query.getStringField(2);
			strProvider					= query.getStringField(3);
			strDVRChannel				= query.getStringField(4);
			strDVRPort					= query.getStringField(5);
			strDVRAddr					= query.getStringField(6);
			strRecordName				= query.getStringField(7);
			strFolder					= query.getStringField(8);
			//bIsRecording				= query.getStringField(10);
			tStart						= query.getStringField(11);
			tEnd						= query.getStringField(12);

			LpRecordResult->strRecordName		= UTF_82ASCII(strRecordName);	// ¼���ļ�������
			LpRecordResult->strFolder			= UTF_82ASCII(strFolder);		// ¼���ļ����ڵ��ļ���·��
			LpRecordResult->bIsRecording		= false;						// ¼���ļ��Ƿ���¼����
			LpRecordResult->tStart				= UTF_82ASCII(tStart);			// ¼��ʼʱ��
			LpRecordResult->tEnd				= UTF_82ASCII(tEnd);			// ¼�����ʱ��
			LpRecordResult->strDVRName			= UTF_82ASCII(strDVRName);		// ¼���ļ�����DVR�����Ʊ�ʶ
			LpRecordResult->strDVRAddr			= UTF_82ASCII(strDVRAddr);		// ¼���ļ�����DVR��IP��ַ
			LpRecordResult->strDVRPort			= UTF_82ASCII(strDVRPort);		// ¼���ļ�����DVR�Ķ˿ں�
			LpRecordResult->strDVRChannel		= UTF_82ASCII(strDVRChannel);	// ¼���ļ�����DVR��ͨ����
			LpRecordResult->strProvider = UTF_82ASCII(strProvider);

			query.nextRow();
			m_mapRecord.insert(ValueMapQueryRecordResult(LpRecordResult->nRecordId, LpRecordResult));
			recordCount++;
		}
		query.finalize();
		g_pLog->WriteLog("��ѯ¼���¼��:%d\n",recordCount);
	}
	catch(CppSQLite3Exception& ex)
	{
		g_pLog->WriteLog("��ѯ(%s)¼��ʧ��:%d;%s\n",g_szFile, ex.errorCode(), ex.errorMessage());
	}
	catch(...)
	{
		g_pLog->WriteLog("��ѯ¼��ʧ��\n");
		// 
	}

	//LeaveCriticalSection(&m_CriticalSectionRecord);
}

BOOL CDBObject::UpdateDevice(list<string> sqlList)
{
	if(sqlList.empty())
		return FALSE;

	CppSQLite3DB db;
	db.open(g_szFile);
	int nRet2 = db.execDML("begin transaction;");

	list<string>::iterator iter;
	for(iter = sqlList.begin(); iter != sqlList.end(); ++iter)
	{
		string strSql = sqlList.front();
		nRet2 = db.execDML(ASCII2UTF_8(strSql).c_str());
		sqlList.pop_front();
	}

	nRet2 = db.execDML("commit transaction;");

	return ( nRet2 == SQLITE_OK );
}

BOOL CDBObject::UpdateData(LPDeviceUnitData lpDeviceData)
{
	if(lpDeviceData == NULL)
		return FALSE;

	// ���¼�������
	Init();

	//EnterCriticalSection(&m_CriticalSection);
	//if(lpDeviceData->nUpdateType == INSERT_DATA)
	//{// ����¼Ӷ������ݣ�ÿһ����Ҫ����
	//	for(IterRFCamera iter = m_mapRFCamera.begin(); iter != m_mapRFCamera.end(); ++iter)
	//	{
	//		(LPRFCamera)iter->second;
	//	}
	//}
	//else if(lpDeviceData->nUpdateType == UPDATE_DATA)
	//{
	//	
	//}
	//else if(lpDeviceData->nUpdateType == DELETE_DATA)
	//{
	//	
	//}
	//else
	//{
	//	LeaveCriticalSection(&m_CriticalSection);
	//	return FALSE;
	//}

	//LeaveCriticalSection(&m_CriticalSection);

	return TRUE;
}

void CDBObject::GetDeviceList(MapRFCamera& mapRfCamera)
{
	EnterCriticalSection(&m_CriticalSectionDevice);
	mapRfCamera = m_mapRFCamera;
	LeaveCriticalSection(&m_CriticalSectionDevice);
}

// ¼���������¼���
BOOL CDBObject::QueryRecord(MapQueryRecordResult& mapRecord, string time, string timeEnd, string strkey)
{
	EnterCriticalSection(&m_CriticalSectionRecord);
	for(IterMapQueryRecordResult iterDevice = m_mapRecord.begin(); iterDevice != m_mapRecord.end(); iterDevice++)
	{
		delete (LpQueryRecordResult)iterDevice->second;
	}
	m_mapRecord.clear();

	LeaveCriticalSection(&m_CriticalSectionRecord);
	//string strKeyUtf8 = ASCII2UTF_8(strkey);
	string strKeyUtf8 = strkey;
	GetVideoRecords(time, timeEnd, strKeyUtf8);
	EnterCriticalSection(&m_CriticalSectionRecord);
	mapRecord=m_mapRecord;
	LeaveCriticalSection(&m_CriticalSectionRecord);
	return TRUE;
}

int CDBObject::InsertReocord(const char* pProvider, const char* pDVRName, string pId,
	int pChannelId, const char* pDVRAddr, int state, int type, const char* path, const char* file, const char* startTime, const char* endTime)
{

	char szSql[MAX_PATH] = {0};
	sprintf_s(szSql, "insert into Record values(NULL, '%s', '%s', '%s' , '%d', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s') ",
		pId.c_str(), pDVRName, pProvider, pChannelId, pId.c_str(), pDVRAddr, file, path, "", "", startTime, endTime);


	CppSQLite3DB db;
	db.open(g_szFile);
	int nRet2 = db.execDML("begin transaction;");
	string strSql = szSql;
	nRet2 = db.execDML(ASCII2UTF_8(strSql).c_str());
	nRet2 = db.execDML("commit transaction;");
	
	return (int)(db.lastRowId());
}

BOOL CDBObject::UpdateRecord(int nId)
{
	char szTime[20] = {0};
	SYSTEMTIME loctime;
	GetLocalTime(&loctime);	
	sprintf_s(szTime, "%.4d%.2d%.2d%.2d%.2d%.2d%", loctime.wYear, loctime.wMonth, loctime.wDay, loctime.wHour, loctime.wMinute, loctime.wSecond);

	char szSql[MAX_PATH] = {0};
	sprintf_s(szSql, "update Record set EndTime = '%s' where id=%d", szTime, nId - 1);

	CppSQLite3DB db;
	db.open(g_szFile);
	int nRet2 = db.execDML("begin transaction;");
	string strSql = szSql;
	nRet2 = db.execDML(ASCII2UTF_8(strSql).c_str());
	nRet2 = db.execDML("commit transaction;");
	
	return TRUE;
}

BOOL CDBObject::DeleteRecord(int nId)
{
	char szSql[MAX_PATH] = {0};
	sprintf_s(szSql, "delete from Record where id=%d", nId);

	CppSQLite3DB db;
	db.open(g_szFile);
	int nRet2 = db.execDML("begin transaction;");
	string strSql = szSql;
	nRet2 = db.execDML(ASCII2UTF_8(strSql).c_str());
	nRet2 = db.execDML("commit transaction;");
	
	return TRUE;
}

int CDBObject::getFirstFile(char* name)
{
	try
	{
		if(name != NULL)
		{
			char szSql[]= {"SELECT * from Record"};
			CppSQLite3DB database;
			database.open(g_szFile);
			CppSQLite3Query query = database.execQuery(szSql);
			if(!query.eof())
			{
				string path = query.getStringField(8);
				string file= query.getStringField(7);
				sprintf(name, "%s\\%s", UTF_82ASCII(path).c_str(), UTF_82ASCII(file).c_str());
				return query.getIntField(0);
			}
		}
	}
	catch(...)
	{
		g_pLog->WriteLog("���ݿ��ѯ�쳣 getFirstFile\n");
	}
	return -1;
}

BOOL CDBObject::isFileInRecord(const char* file)
{
	try
	{
		char szSql[MAX_PATH]= {};
		sprintf_s(szSql, "SELECT * from Record Where FileName = '%s'", file);
		string strSql = szSql;
		strSql= ASCII2UTF_8(strSql);
		CppSQLite3DB database;
		database.open(g_szFile);
		CppSQLite3Query query = database.execQuery(strSql.c_str());
		if(!query.eof())
		{
			return TRUE;
		}
	}
	catch(...)
	{
		g_pLog->WriteLog("���ݿ��ѯ�쳣 getFirstFile\n");
	}
	return FALSE;
}