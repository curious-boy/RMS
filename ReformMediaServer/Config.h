#pragma once

#include "RFProtocol.h"
#include "ReformMediaServerDef.H"
#include "xml\tinyxml.h"
#include <string>

#define RECORD_SETTINGS_FILE_NAME		"RecordCfg.xml"
#define CAMERA_SETTINGS_FILE_NAME		"CameraCfg.xml"
#define PTZCONTROL_SETTINGS_FILE_NAME	"PtzControlCfg.xml"

using namespace std;

// ������
class CConfig
{
public:
	CConfig(void);
	~CConfig(void);

	bool Init();											// ��ʼ����������������
	bool Save();											// �������ݵ��ļ�

	bool get_record_status(string pId);
	bool get_record_path(string pId, char * path);
	bool get_record_path(int size, string pId, char * path);
	// ¼����
	bool AddRecord(LPRFRecordSetting lpRecord);					// ����¼�����
	bool UpdateRecord(LPRFRecordSetting lpRecord);					// ����¼�����
	bool DeleteRecord(LPRFRecordSetting lpRecord);					// ɾ��¼�����
	bool QueryRecords(MapRecordSettings & recordsettings);	// ��ȡ���в���	
	bool GetRecordSetting(string pId, char * path);			// ��ѯĳ��ͨ���Ƿ�¼��ʹ洢·��

	bool SetRecordState(bool bRecord, string strFileName);	// �޸�channel�е�¼��״̬λ���Կ���¼�����,���Ҹ������ݿ��е�¼����־
	bool UpdateRecordState(int recordId, string strFile);	// ����¼��״̬
	bool QueryRecordByName(string strQueryName, MapQueryRecordResult &queryResult);		// ��ѯ¼�񷵻صĽ����
	bool IsRecordFinished(string strRecordName);			// ��ѯָ�����ֵ�¼���Ƿ��Ѿ�������ֻ�н�����¼����ܲ鿴�����ص�

	// ����ͷ����
	bool AddCamera(	LPRFCamera lpCamera);			// �������ͷ(�����ڴ桢���浽���ݿ⡢֪ͨ���пͻ������¼���)
	bool UpdateCamera(LPRFCamera lpCamera);			// ��������ͷ��Ϣ(�����ڴ桢���浽���ݿ⡢֪ͨ���пͻ������¼���)
	bool DeleteCamera(LPRFCamera lpCamera);			// ɾ������ͷ(�����ڴ桢���浽���ݿ⡢֪ͨ���пͻ������¼���)
	bool QueryCameras(MapRFCamera &cameras);		// ��ѯ��������ͷ
	
//private:
public:

	// ����¼��״̬�б��߳�
	static DWORD WINAPI RecordSettingThread( LPVOID lpParameter);

	bool LoadCameras();								// ��������ͷ�����б�
	bool LoadRecordSettings();						// ����¼�����
	bool LoadPTZControlRuler();						// ������̨���Ʋ���
	bool SaveRecordSettings();						// ����¼�����
	bool SaveCameras();								// �����豸�б�
	bool SavePTZControlRuler();						// ������̨���Ʋ���

	bool GetRecordType(RF_RECORD_TYPE type, string& strType);													// ��ȡ¼�������ַ���
	bool GetRecordTime( string strStart,  string strEnd, RFRecordTime& value );									// ��ȡʱ��ֵ
	bool GetChildText( TiXmlElement* pElement ,string strElementName, int& value );								// ��ȡ�ӽڵ�����ֵ
	bool GetChildText( TiXmlElement* pElement ,string strElementName, string& value );							// ��ȡ�ӽڵ��ı�ֵ
	bool GetChildAttribute( TiXmlElement* pElement ,string strElementName, string strAttribute, int& value );	// ��ȡ�ӽڵ�����ֵ
	bool GetChildAttribute( TiXmlElement* pElement ,string strElementName, string strAttribute, string& value );// ��ȡ�ӽڵ�����ֵ
	bool GetRecordType( TiXmlElement* pElement ,string strElementName, RF_RECORD_TYPE& value );					// ��ȡ¼������

	string getRecordPaths()
	{
		return m_RecordPath;
	}

private:

	bool					m_bExit;				// �Ƿ��˳�����
	HANDLE					m_hExitEvent;			// �˳��¼�
	HANDLE					m_hSettingChangeEvent;	// ���ø����¼�
	bool					m_bRecordLog;			// �Ƿ��¼¼����־
	bool					m_bCameraLog;			// �Ƿ��¼�豸������־

	// ����ͷ����
	MapRFCamera				m_mapCameras;	// ����ͷ�б�

	// ¼����
	string					m_strFile;				// ¼�������ļ�·��
	MapRecordSettings		m_mapRecordSettings;	// ����¼������

	int						m_nMaxId;				// ����id���
	string m_RecordPath;
};

extern CConfig g_xmlConfig;