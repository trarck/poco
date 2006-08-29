//
// ICMPPacket.h
//
// $Id: //poco/1.2/Net/include/Poco/Net/ICMPPacket.h#1 $
//
// Library: Net
// Package: ICMP
// Module:  ICMPPacket
//
// Definition of the ICMPPacket class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Net_ICMPPacket_INCLUDED
#define Net_ICMPPacket_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Net/Socket.h"
#include "Poco/Net/ICMPPacketImpl.h"


namespace Poco {
namespace Net {


class Net_API ICMPPacket
	/// This class is the ICMP packet abstraction. 
{
public:
	ICMPPacket(IPAddress::Family family, int dataSize = 48);
		/// Creates an ICMPPacket of specified family.

	~ICMPPacket();
		/// Destroys the ICMPPacket.

	const Poco::UInt8* packet();
		/// Returns raw ICMP packet. ICMP header and data are included in the returned packet.

	int packetSize() const;
		/// Returns the total length of packet (header + data);

	Poco::UInt16 sequence() const;
		/// Returns the most recent sequence number generated.

	void setDataSize(int dataSize);
		/// Sets data size.

	int getDataSize() const;
		/// Returns data size.

	int maxPacketSize() const;
		/// Returns the total length of packet (header + data);

	struct timeval time(Poco::UInt8* buffer = 0, int length = 0) const;
		/// Returns current epoch time if either buffer or length are equal to zero.
		/// Otherwise, it extracts the time value from the supplied buffer and 
		/// returns the extracted value.
		/// 
		/// Supplied buffer includes IP header, ICMP header and data.

	bool validReplyID(Poco::UInt8* buffer, int length) const;
		/// Returns true if the extracted id is recognized 
		/// (equals the process id).
		///	
		/// Supplied buffer includes IP header, ICMP header and data.

	std::string errorDescription(Poco::UInt8* buffer, int length);
		/// Returns error description string.
		/// If supplied buffer contains an ICMP echo reply packet, an
		/// empty string is returned indicating the absence of error.
		///	
		/// Supplied buffer includes IP header, ICMP header and data.

	std::string typeDescription(int typeId);
		/// Returns the description of the packet type.

private:
	ICMPPacketImpl* _pImpl;
};


} } // namespace Poco::Net


#endif // Net_ICMPPacket_INCLUDED