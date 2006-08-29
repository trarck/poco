//
// URIStreamOpener.h
//
// $Id: //poco/1.2/Foundation/include/Poco/URIStreamOpener.h#1 $
//
// Library: Foundation
// Package: URI
// Module:  URIStreamOpener
//
// Definition of the URIStreamOpener class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
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


#ifndef Foundation_URIStreamOpener_INCLUDED
#define Foundation_URIStreamOpener_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Mutex.h"
#include <istream>
#include <map>


namespace Poco {


class URI;
class URIStreamFactory;
class Path;


class Foundation_API URIStreamOpener
	/// The URIStreamOpener class is used to create and open input streams
	/// for resourced identified by Uniform Resource Identifiers.
	///
	/// For every URI scheme used, a URIStreamFactory must be registered.
	/// A FileStreamFactory is automatically registered for file URIs.
{
public:
	URIStreamOpener();
		/// Creates the URIStreamOpener and registers a FileStreamFactory
		/// for file URIs.

	~URIStreamOpener();
		/// Destroys the URIStreamOpener and deletes all registered
		/// URI stream factories.

	std::istream* open(const URI& uri) const;
		/// Tries to create and open an input stream for the resource specified
		/// by the given uniform resource identifier.
		///
		/// If no URIStreamFactory has been registered for the URI's
		/// scheme, a UnknownURIScheme exception is thrown.
		/// If the stream cannot be opened for any reason, an
		/// IOException is thrown.
		///
		/// The given URI must be a valid one. This excludes file system paths.
		///
		/// Whoever calls the method is responsible for deleting
		/// the returned stream.

	std::istream* open(const std::string& pathOrURI) const;
		/// Tries to create and open an input stream for the resource specified
		/// by the given path or uniform resource identifier.
		///
		/// If the stream cannot be opened for any reason, an
		/// Exception is thrown.
		///
		/// The method first tries to interpret the given pathOrURI as an URI.
		/// If this fails, the pathOrURI is treated as local filesystem path.
		/// If this also fails, an exception is thrown.
		///
		/// Whoever calls the method is responsible for deleting
		/// the returned stream.

	std::istream* open(const std::string& basePathOrURI, const std::string& pathOrURI) const;
		/// Tries to create and open an input stream for the resource specified
		/// by the given path or uniform resource identifier.
		///
		/// pathOrURI is resolved against basePathOrURI (see URI::resolve() and
		/// Path::resolve() for more information).
		///
		/// If the stream cannot be opened for any reason, an
		/// Exception is thrown.
		///
		/// Whoever calls the method is responsible for deleting
		/// the returned stream.
		
	void registerStreamFactory(const std::string& scheme, URIStreamFactory* pFactory);
		/// Registers a URIStreamFactory for the given scheme. If another factory
		/// has already been registered for the scheme, an ExistsException is thrown.
		///
		/// The URIStreamOpener takes ownership of the factory and deletes it when it is
		/// no longer needed (in other words, when the URIStreamOpener is deleted).

	void unregisterStreamFactory(const std::string& scheme);
		/// Unregisters and deletes the URIStreamFactory for the given scheme.
		///
		/// Throws a NotFoundException if no URIStreamFactory has been registered
		/// for the given scheme.
		
	bool supportsScheme(const std::string& scheme);
		/// Returns true iff a URIStreamFactory for the given scheme
		/// has been registered.

	static URIStreamOpener& defaultOpener();
		/// Returns a reference to the default URIStreamOpener.

protected:
	std::istream* openFile(const Path& path) const;

private:
	URIStreamOpener(const URIStreamOpener&);
	URIStreamOpener& operator = (const URIStreamOpener&);

	typedef std::map<std::string, URIStreamFactory*> FactoryMap;
	
	FactoryMap        _map;
	mutable FastMutex _mutex;
};


} // namespace Poco


#endif // Foundation_URIStreamOpener_INCLUDED