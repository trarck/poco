//
// DynamicAny.h
//
// $Id: //poco/Main/Foundation/include/Poco/DynamicAny.h#8 $
//
// Library: Foundation
// Package: Core
// Module:  DynamicAny
//
// Definition of the DynamicAny class.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
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


#ifndef Foundation_DynamicAny_INCLUDED
#define Foundation_DynamicAny_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/DynamicAnyHolder.h"
#include <typeinfo>


namespace Poco {


class Foundation_API DynamicAny
	/// DynamicAny allows to store data of different types and to convert between these types transparently.
	/// DynamicAny puts forth the best effort to provide intuitive and reasonable conversion semantics and prevent 
	/// unexpected data loss, particularly when performing narrowing or signedness conversions of numeric data types.
	///
	/// Loss of signedness is not allowed for numeric values. This means that if an attempt is made to convert 
	/// the internal value which is a negative signed integer to an unsigned integer type storage, a RangeException is thrown. 
	/// Overflow is not allowed, so if the internal value is a larger number than the target numeric type size can accomodate, 
	/// a RangeException is thrown.
	///
	/// Precision loss, such as in conversion from floating-point types to integers or from double to float on platforms
	/// where they differ in size (provided internal actual value fits in float min/max range), is allowed.
	/// 
	/// String truncation is allowed -- it is possible to convert between string and character when string length is 
	/// greater than 1. An empty string gets converted to the char '\0', a non-empty string is truncated to the first character. 
	///
	/// Bolean conversion are performed as follows:
	///
	/// A string value "false" (not case sensitive) or "0" can be converted to a boolean value false, any other string 
	/// not being false by the above criteria evaluates to true (e.g: "hi" -> true).
	/// Integer 0 values are false, everything else is true.
	/// Floating point values equal to the minimal FP representation on a given platform are false, everything else is true.
	///
	/// A DynamicAny can be created from and converted to a value of any type for which a specialization of 
	/// DynamicAnyHolderImpl is available. For supported types, see DynamicAnyHolder documentation.
{
public:
	DynamicAny();
		/// Creates a DynamicAny holding an int with value 0.

	template <typename T> 
	DynamicAny(const T &val):
		_pHolder(new DynamicAnyHolderImpl<T>(val))
		/// Creates the DynamicAny from the given value.
	{
	}

	DynamicAny(const char* pVal);
		// Convenience constructor for const char* which gets mapped to a std::string internally, i.e. pVal is deep-copied.

	DynamicAny(const DynamicAny& other);
		/// Copy constructor.

	~DynamicAny();
		/// Destroys the DynamicAny.

	void swap(DynamicAny& other);
		/// Swaps the content of the this DynamicAny with the other DynamicAny.

	template <typename T> 
	void convert(T& val) const
		/// Invoke this method to perform a safe conversion.
		///
		/// Example usage:
		///     DynamicAny any("42");
		///     int i;
		///     any.convert(i);
		///
		/// Throws a RangeException if the value does not fit
		/// into the result variable.
		/// Throws a NotImplementedException if conversion is
		/// not available for the given type.
	{
		_pHolder->convert(val);
	}
	
	template <typename T> 
	T convert() const
		/// Invoke this method to perform a safe conversion.
		///
		/// Example usage:
		///     DynamicAny any("42");
		///     int i = any.convert<int>();
		///
		/// Throws a RangeException if the value does not fit
		/// into the result variable.
		/// Throws a NotImplementedException if conversion is
		/// not available for the given type.
	{
		T result;
		_pHolder->convert(result);
		return result;
	}
	
	template <typename T>
	operator T() const
		/// Safe conversion operator for implicit type
		/// conversions.
		///
		/// Throws a RangeException if the value does not fit
		/// into the result variable.
		/// Throws a NotImplementedException if conversion is
		/// not available for the given type.
	{
		T result;
		_pHolder->convert(result);
		return result;
	}
	
	template <typename T>
	const T& extract() const
		/// Returns a const reference to the actual value.
		///
		/// Must be instantiated with the exact type of
		/// the stored value, otherwise a BadCastException
		/// is thrown.
	{
		DynamicAnyHolderImpl<T>* pHolder = dynamic_cast<DynamicAnyHolderImpl<T>*>(_pHolder);
		if (pHolder)
			return pHolder->value();
		else
			throw BadCastException();
	}

	template <typename T> 
	DynamicAny& operator = (const T& other)
		/// Assignment operator
	{
		DynamicAny tmp(other);
		swap(tmp);
		return *this;
	}
	
	template <typename T> 
	bool operator == (const T& other)
		/// Equality operator
	{
		T value;
		_pHolder->convert(value);
		return value == other;
	}

	bool operator == (const char* other)
		/// Equality operator
	{
		std::string value;
		_pHolder->convert(value);
		return value == other;
	}

	template <typename T> 
	bool operator != (const T& other)
		/// Inequality operator
	{
		T value;
		_pHolder->convert(value);
		return value != other;
	}

	bool operator != (const char* other)
		/// Inequality operator
	{
		std::string value;
		_pHolder->convert(value);
		return value != other;
	}

    const std::type_info& type() const;
		/// Returns the type information of the stored content.
	
private:
	DynamicAnyHolder* _pHolder;
};


} // namespace Poco


#endif // Foundation_DynamicAny_INCLUDED