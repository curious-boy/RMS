#ifndef _MEDIA_PROTOCAL_H_
#define _MEDIA_PROTOCAL_H_

#pragma pack(1)
struct PacketHeader
{
	unsigned short  nPacketID;		// (0xAB,0xCD)
	unsigned short  nPacketNo;		// �����    
	unsigned short  nCommand;		// ָ��
	unsigned int  nDataSize;		// ���ݳ���
};
#pragma pack()


#endif _MEDIA_PROTOCAL_H_