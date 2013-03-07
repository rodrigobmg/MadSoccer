
#include "stdafx.h"
#include "Packet.h"


using namespace network;

// Disconnect 패킷을 리턴한다.
CPacket network::DisconnectPacket()
{
	CPacket packet;
	packet << 0;
	packet << (int)PACKETID_DISCONNECT;
	return packet;
}


CPacket::CPacket():
	m_WriteIdx(0)
,	m_ReadIdx(0)
{
}

CPacket::CPacket(netid senderId, char *buf256) :
	m_SenderId(senderId)
,	m_WriteIdx(0)
,	m_ReadIdx(0)
{
	if (buf256)
		memcpy( m_Data, buf256, MAX_PACKETSIZE);
}


//------------------------------------------------------------------------
// 패킷의 프로토콜 정보를 리턴한다.
//------------------------------------------------------------------------
int	CPacket::GetProtocolId() const
{
	const int id = *(int*)m_Data;
	return id;
}


//------------------------------------------------------------------------
// 패킷의 패킷 아이디 정보를 리턴한다.
//------------------------------------------------------------------------
int	CPacket::GetPacketId() const 
{
	const int id = *(int*)(m_Data+sizeof(int));
	return id;
}


//CPacket& CPacket::operator<<(const _variant_t &rhs)
//{
//	const int type = rhs.vt;
//	switch (type)
//	{
//	case VT_I2: Append(rhs.iVal); break;
//	case VT_I4: Append(rhs.lVal); break;
//	case VT_R4: Append(rhs.fltVal); break;
//	case VT_R8: Append(rhs.dblVal); break;
//
//	case VT_BSTR:
//		{
//			tstring str = (LPCTSTR) (_bstr_t)rhs.bstrVal;
//#ifdef _UNICODE
//			std::string s = common::wstr2str(str);
//			operator<<(s);
//#else
//			operator<<(str);
//#endif
//		}
//		break;
//
//	case VT_DECIMAL:
//	case VT_I1:
//	case VT_UI1:
//	case VT_UI2:
//	case VT_UI4:
//		break;
//
//	case VT_INT: Append(rhs.intVal); break;
//	case VT_UINT: Append(rhs.uintVal); break;
//	default:
//		{
//			error::ErrorLog( 
//				common::format("CPacket::operator<< %d 에 해당하는 타입의 Append는 없다.", type) );
//			assert(0);
//		}
//		break;
//	}
//	return *this;
//}


//		VT_EMPTY	= 0,
//			VT_NULL	= 1,
// 			VT_CY	= 6,
// 			VT_DATE	= 7,

// 			VT_DISPATCH	= 9,
// 			VT_ERROR	= 10,
// 			VT_BOOL	= 11,
// 			VT_VARIANT	= 12,
// 			VT_UNKNOWN	= 13,
// 			VT_I8	= 20,
// 			VT_UI8	= 21,


// 			VT_VOID	= 24,
// 			VT_HRESULT	= 25,
// 			VT_PTR	= 26,
// 			VT_SAFEARRAY	= 27,
// 			VT_CARRAY	= 28,
// 			VT_USERDEFINED	= 29,
// 			VT_LPSTR	= 30,
// 			VT_LPWSTR	= 31,
// 			VT_RECORD	= 36,
// 			VT_INT_PTR	= 37,
// 			VT_UINT_PTR	= 38,
// 			VT_FILETIME	= 64,
// 			VT_BLOB	= 65,
// 			VT_STREAM	= 66,
// 			VT_STORAGE	= 67,
// 			VT_STREAMED_OBJECT	= 68,
// 			VT_STORED_OBJECT	= 69,
// 			VT_BLOB_OBJECT	= 70,
// 			VT_CF	= 71,
// 			VT_CLSID	= 72,
// 			VT_VERSIONED_STREAM	= 73,
// 			VT_BSTR_BLOB	= 0xfff,
// 			VT_VECTOR	= 0x1000,
// 			VT_ARRAY	= 0x2000,
// 			VT_BYREF	= 0x4000,
// 			VT_RESERVED	= 0x8000,
// 			VT_ILLEGAL	= 0xffff,
// 			VT_ILLEGALMASKED	= 0xfff,
// 			VT_TYPEMASK	= 0xfff


////------------------------------------------------------------------------
//// 인자로 넘어온 varType 타입으로 패킷에 있는 데이타를 가져와 리턴한다.
////------------------------------------------------------------------------
//_variant_t CPacket::GetVariant(const _variant_t &varType)
//{
//	_variant_t var = varType;
//	switch (varType.vt)
//	{
//	case VT_I2: *this >>(var.iVal); break;
//	case VT_I4: *this  >>(var.lVal); break;
//	case VT_R4: *this  >>(var.fltVal); break;
//	case VT_R8: *this  >>(var.dblVal); break;
//
//	case VT_BSTR:
//		{
//			std::string str;
//			*this  >>(str);
// #ifdef _UNICODE
// 			var.bstrVal = (_bstr_t)common::str2wstr(str).c_str();
// #else
// 			var.bstrVal = (_bstr_t)str.c_str();
// #endif
//		}
//		break;
//
//	case VT_DECIMAL:
//	case VT_I1:
//	case VT_UI1:
//	case VT_UI2:
//	case VT_UI4:
//		break;
//
//	case VT_INT: *this  >>(var.intVal); break;
//	case VT_UINT: *this >>(var.uintVal); break;
//	default:
//		break;
//	}
//	return var;
//}
