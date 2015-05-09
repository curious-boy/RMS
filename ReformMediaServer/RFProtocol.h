#ifndef __RF_PROCOTOL_H__
#define __RF_PROCOTOL_H__


#define PACKET_MAX_SIZE				1024	// �������ݰ�����󳤶�

#define A_PROTOCOL_HEAD				0xABCD	// Э��ͷ��־
#define REGISTER_COMMAND			0x9001	// ע��
#define RESP_REGISTER_COMMAND		0x9002	// ע���Ӧ
#define UPDATE_DEVICE_COMMAND		0x9003	// �����豸
#define RESP_UPDATE_DEVICE_COMMAND	0x9004	// �����豸��Ӧ
#define UPDATE_CONFIG_COMMAND		0x9005	// ��������
#define RESP_UPDATE_CONFIG_COMMAND	0x9006	// �������û�Ӧ
#define HANDSHAKE_COMMAND			0x9007	// ����
#define RESP_HANDSHAKE_COMMAND		0x9008	// ���ֻ�Ӧ

#define DEVICE_UPDATE_COMMAND		0x9009	// ���ݸ���ָ��

#define DEVICE_REQUEST_COMMAND		0x9010	// ��������ָ��
#define RESP_DEVICE_REQUEST_COMMAND	0x9011	// ���ݸ��»�Ӧָ��

#define VIDEO_RECORD_REQUEST_COMMAND		0x9012	// ��������ָ��
#define RESP_VIDEO_RECORD_REQUEST_COMMAND	0x9013	// ���ݸ��»�Ӧָ��

#define VIDEO_RECORD_REQUEST_WITH_ICON_COMMAND		0x9014	// ��ȡ����ͼ���¼����Ϣ

// Ӧ�ò��ͷ
#pragma pack(1)
typedef struct tagRFAppComand
{
	unsigned short  nPacketID;		// (0xAB,0xCD)
	unsigned short  nPacketNo;		// �����    
	unsigned short  nCommand;		// ָ��
	unsigned int  nDataSize;		// ���ݳ���

	tagRFAppComand()
	{
		*((unsigned char *)&nPacketID)		= 0xAB;
		*((unsigned char *)&nPacketID + 1)	= 0xCD;
	}

	int GetPacketTotalSize()
	{
		return sizeof(tagRFAppComand) + nDataSize;
	}
} RFAppComand, *LpRFAppComand;
#pragma pack()

// �����豸ָ���Ӧ��ʽ
typedef struct tagGetDeviceListCommandAck
{
	int			nTotalCount;	// �б�����
	int			nCurrCount;		// �豸�б�����
} GetDeviceListCommandAck, *LpGetDeviceListCommandAck;

typedef struct tagQueryVideoReocrdCommand
{
	char		szStartTime[20];
	char		szEndTime[20];
	char		szKey[20];
} QueryVideoReocrdCommand, *LpQueryVideoReocrdCommand ;

// ����¼���б�ָ���Ӧ��ʽ
typedef struct tagQueryVideoRecordCommandAck
{
	int			nTotalCount;	// �б�����
	int			nCurrCount;		// �豸�б�����
} QueryVideoRecordCommandAck, *LpQueryVideoRecordCommandAck;

// ��ѯ�豸�б�
typedef struct tagDeviceObject
{
	char	szRtspString[300];
	char	szDVRName[20];
	char	szChannelName[20];
	char	szInnerName[20];
	char	szProvider[20];

} DeviceObject, *LpDeviceObject;

// ��ѯ¼���ļ��б�
typedef struct tagQueryVideoRecord
{
	// ��ţ�¼���ļ����ƣ���ȫ·������ʼʱ�䣬����ʱ�䣬dvr���ƣ�dvr��ip��ַ�� ͨ���ţ�����ͷid�� �ļ���С
	char		szFileName[50];
	char		szPath[200];
	char		szStartTime[20];
	char		szEndTime[20];
	char		szDVRName[20];
	char		szDVRAddr[20];
	char		szChannelName[20];
	int			nDeviceId;
	int			Id;
	int			nFileSize;
} QueryVideoRecord, *LpQueryVideoRecord;

typedef struct _QuerayVRResource
{
	char file_name[50];
	char url[200];
	char start_time[20];
	char end_time[20];
	int file_size;
}QueryVRResource, *LPQueryVRResource;

typedef struct _QuerayVRResourceWithIcon
{
	char file_name[50];
	char url[200];
	char start_time[20];
	char end_time[20];
	int file_size;
	int icon_size;
	char* icon_data;
}QueryVRResourceWithIcon, *LPQueryVRResourceWithIcon;

// ͨ�û�Ӧָ���ʽ
typedef struct _CommonPackAck
{
	unsigned short nResult;	// ��� 0 �ɹ� ������������
} CommonPackAck, * LPCommonPackAck;

// ע���
typedef struct _Register
{
	unsigned short nDeviceType;		// �豸����
	unsigned int   nDeviceAddr;		// �豸��ַ
	unsigned int   nSoftVer;		// ����汾
	char szUser[20];
	char szPass[20];
} Register, * LPRegister;

// ע�����Ӧ
typedef struct _RegisterAck
{
	unsigned short nResult;			// ע����
	unsigned int   nDeviceAddr;		// ������豸��ַ
} RegisterAck, * LPRegisterAck;

// ���ֻ�Ӧ
typedef struct _HandShakeAck
{
	unsigned short nStatus;	// ״̬ 0 ���� 1 �쳣
} HandShakeAck, * LPHandShakeAck;

// ���ݸ���ָ��
typedef struct _DeviceUpdate
{
	int flag;
}DeviceUpdate, *LpDeviceUpdate;

// �豸���ݰ�
typedef struct _DevicePack
{
	unsigned char byCount;		// ����
} DevicePack, *LPDevicePack;

// ���ݲ�������
typedef enum _UpdateType
{
	SELECT_DATA = 0,		// ��ȡ
	INSERT_DATA = 1,		// ����
	UPDATE_DATA = 2,		// �޸�
	DELETE_DATA = 3			// ɾ��
} UpdateType;

// Ӧ�ó�������
typedef enum _AppType
{
    APP_CENTER_SERVER,
    APP_DEVICE_SERVER,
    APP_RECORD_SERVER,
    APP_STREAM_SERVER,
    APP_ALARM_SERVER,
    APP_CLIENT_CLIENT,
    APP_MANAGER_CLIENT,
    APP_MATRIX_CLIENT,
    APP_ACTIVEX_CLIENT,
} AppType;

// �豸����
typedef struct _DeviceUnitData
{
	int			nId;					// ���ݿ��Ӧ�ı��
	int			nUpdateType;			// ���ݲ�������(����=0������=1��ɾ��=2,,,)
	char		szCameraId[20];			// ϵͳ���ɵ�����ͷ�豸���
	char		szCameraType[20];		// ����ͷ����
	char		szCameraName[20];		// ����ͷ����
	char		szMode[20];				// ����ͷ����(DVR��NVR��IPC������)
	char		szProvider[20];			// �豸����(�󻪡���������˼������)
	char		szCodec[20];			// �����ʽ
	char		szLogin[20];			// ���ӵ��豸ʱ��¼���û���
	char		szPassword[20];			// ��¼����
	char		szIp[20];				// �豸��IP��ַ
	char		szPort[20];				// �豸���Ӷ˿ں�
	char		szCnlId[20];			// �豸����ͨ����

	char		szMultiCast[20];		// �Ƿ�㲥����
	char		szMultiCastAddress[20];	// �㲥��ַ
	char		szMultiCastPort[20];	// �㲥�˿�
	char		szRecordType[20];		// 
} DeviceUnitData, *LPDeviceUnitData;

// ʱ���
typedef struct _RecordTime
{
	unsigned short    nYear;
	unsigned char     nMonth;
	unsigned char     nDay;
	unsigned char     nSer;
	unsigned char     nHour;
	unsigned char     nMinute;
	unsigned char     nSecond;
} RecordTime, * LPRecordTime;

// ¼�������ݿ��е��ֶ�
typedef struct _RecordData
{
	int		nId;					// ���ݿ��еı��
	int		nDeviceId;				// �豸���
	char	szDVRName[20];			// DVR����
	char	szChannelName[20];		// ͨ����
	char	szInnerName[20];		// �ڲ�����
	char	szIp[20];				// �豸��IP��ַ
	char	szFileName[50];			// �ļ�����
	char	szPath[256];			// �ļ�����·��
	int		nRecordType;			// ¼������(�澯���ճ���)
	int		nState;					// ¼���ļ�״̬(¼���С�¼�ꡢ�д���)
	RecordTime	tStart;				// ¼��ʼʱ��
	RecordTime	tEnd;				// ¼�����ʱ��	

} RecordData, *LpRecordData;

#endif