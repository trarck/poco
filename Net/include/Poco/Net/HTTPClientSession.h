//
// HTTPClientSession.h
//
// $Id: //poco/1.2/Net/include/Poco/Net/HTTPClientSession.h#1 $
//
// Library: Net
// Package: HTTPClient
// Module:  HTTPClientSession
//
// Definition of the HTTPClientSession class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
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


#ifndef Net_HTTPClientSession_INCLUDED
#define Net_HTTPClientSession_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPSession.h"
#include "Poco/Net/SocketAddress.h"
#include <istream>
#include <ostream>


namespace Poco {
namespace Net {


class HTTPRequest;
class HTTPResponse;


class Net_API HTTPClientSession: public HTTPSession
	/// This class implements the client-side of
	/// a HTTP session.
	///
	/// To send a HTTP request to a HTTP server, first
	/// instantiate a HTTPClientSession object and
	/// specify the server's host name and port number.
	///
	/// Then create a HTTPRequest object, fill it accordingly,
	/// and pass it as argument to the sendRequst() method.
	///
	/// sendRequest() will return an output stream that can
	/// be used to send the request body, if there is any.
	///
	/// After you are done sending the request body, create
	/// a HTTPResponse object and pass it to receiveResponse().
	///
	/// This will return an input stream that can be used to
	/// read the response body.
	///
	/// See RFC 2616 <http://www.faqs.org/rfcs/rfc2616.html> for more
	/// information about the HTTP protocol.
{
public:
	HTTPClientSession();
		/// Creates an unconnected HTTPClientSession.

	explicit HTTPClientSession(const StreamSocket& socket);
		/// Creates a HTTPClientSession using the given socket.
		/// The socket must not be connected. The session
		/// takes ownership of the socket.

	explicit HTTPClientSession(const SocketAddress& address);
		/// Creates a HTTPClientSession using the given address.

	HTTPClientSession(const std::string& host, Poco::UInt16 port = HTTPSession::HTTP_PORT);
		/// Creates a HTTPClientSession using the given host and port.

	virtual ~HTTPClientSession();
		/// Destroys the HTTPClientSession and closes
		/// the underlying socket.

	void setHost(const std::string& host);
		/// Sets the host name of the target HTTP server.
		///
		/// The host must not be changed once there is an
		/// open connection to the server.
		
	const std::string& getHost() const;
		/// Returns the host name of the target HTTP server.
		
	void setPort(Poco::UInt16 port);
		/// Sets the port number of the target HTTP server.
		///
		/// The port number must not be changed once there is an
		/// open connection to the server.
	
	Poco::UInt16 getPort() const;
		/// Returns the port number of the target HTTP server.

	void setProxy(const std::string& host, Poco::UInt16 port = HTTPSession::HTTP_PORT);
		/// Sets the proxy host name and port number.
		
	void setProxyHost(const std::string& host);
		/// Sets the host name of the proxy server.
		
	void setProxyPort(Poco::UInt16 port);
		/// Sets the port number of the proxy server.
		
	const std::string& getProxyHost() const;
		/// Returns the proxy host name.
		
	Poco::UInt16 getProxyPort() const;
		/// Returns the proxy port number.
		
	virtual std::ostream& sendRequest(HTTPRequest& request);
		/// Sends the header for the given HTTP request to
		/// the server.
		///
		/// The HTTPClientSession will set the request's
		/// Host and Keep-Alive headers accordingly.
		///
		/// The returned output stream can be used to write
		/// the request body. The stream is valid until
		/// receiveResponse() is called or the session
		/// is destroyed.
		
	virtual std::istream& receiveResponse(HTTPResponse& response);
		/// Receives the header for the response to the previous 
		/// HTTP request.
		///
		/// The returned input stream can be used to read
		/// the response body. The stream is valid until
		/// sendRequest() is called or the session is
		/// destroyed.
	
protected:
	void reconnect();
		/// Connects the underlying socket to the HTTP server.

	int write(const char* buffer, std::streamsize length);
		/// Tries to re-connect if keep-alive is on.
	
	virtual std::string getHostInfo() const;
		/// Returns the target host and port number for proxy requests.

	void deleteResponseStream();
		/// Deletes the response stream and sets it to 0.

	void deleteRequestStream();
		/// Deletes the request stream and sets it to 0.

	void setResponseStream(std::istream* pRespStream);
		/// Sets the response stream iff _pResponseStream is 0.

	void setRequestStream(std::ostream* pRequestStream);
		/// Sets the request stream iff _pRequestStream is 0.

	std::istream* getResponseStream() const;
		/// Returns the currently set response stream. Can return 0.

	std::ostream* getRequestStream() const;
		/// Returns the currently set request stream. Can return 0.

	void setReconnect(bool recon);
		/// Sets _reconnect.
	
	void setExpectResponseBody(bool expect);
		/// Sets _expectResponseBody.

	bool getExpectResponseBody() const;
		/// Returns _expectResponseBody.

private:
	std::string   _host;
	Poco::UInt16  _port;
	std::string   _proxyHost;
	Poco::UInt16  _proxyPort;
	bool          _reconnect;
	bool          _expectResponseBody;
	std::ostream* _pRequestStream;
	std::istream* _pResponseStream;
};


//
// inlines
//
inline const std::string& HTTPClientSession::getHost() const
{
	return _host;
}


inline Poco::UInt16 HTTPClientSession::getPort() const
{
	return _port;
}


inline const std::string& HTTPClientSession::getProxyHost() const
{
	return _proxyHost;
}


inline Poco::UInt16 HTTPClientSession::getProxyPort() const
{
	return _proxyPort;
}


inline std::istream* HTTPClientSession::getResponseStream() const
{
	return _pResponseStream;
}


inline std::ostream* HTTPClientSession::getRequestStream() const
{
	return _pRequestStream;
}


inline void HTTPClientSession::setReconnect(bool recon)
{
	_reconnect = recon;
}


inline void HTTPClientSession::setExpectResponseBody(bool expect)
{
	_expectResponseBody = expect;
}


inline bool HTTPClientSession::getExpectResponseBody() const
{
	return _expectResponseBody;
}


} } // namespace Poco::Net


#endif // Net_HTTPClientSession_INCLUDED