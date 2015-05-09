#pragma once
#include "ReformMediaServerDef.H"
#include "StdHeader.h"
#include "RFProtocol.h"

// ���ݿ������
class CDBObject
{
public:
	CDBObject(void);
	~CDBObject(void);

public:
	void Init();
	void CheckDataBase();

	// �豸����
	BOOL UpdateDevice(list<string> sqlList);
	BOOL UpdateData(LPDeviceUnitData lpDeviceData);
	void GetDeviceList(MapRFCamera& mapRfCamera);

	// ¼���������¼���
	BOOL QueryRecord( MapQueryRecordResult& mapRecord, string time = "", string timeEnd = "", string strkey = "");		// ��ʱ�䡢���Ƽ���
	int InsertReocord(const char* pProvider, const char* pDVRName, string pId,	int pChannelId,
		const char* pDVRAddr, int state, int type, const char* path, const char* file, const char* startTime, const char* endTime = NULL);
	BOOL UpdateRecord(int nId);
	BOOL DeleteRecord(int nId);

	int getFirstFile(char* name);
	BOOL isFileInRecord(const char* file);

private:
	void ClearData();
	void GetDevice();
	void GetVideoRecords(string time, string timeEnd, string strkey);

	char g_szFile[MAX_PATH];
private:

	MapRFCamera m_mapRFCamera;
	MapQueryRecordResult m_mapRecord;

	CRITICAL_SECTION m_CriticalSectionDevice;
	CRITICAL_SECTION m_CriticalSectionRecord;
};

extern CDBObject g_dbobj;