//
// Copyright (c) 2021, Zoe J. Bare
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//

#include "String.hpp"

#include <assert.h>
#include <string.h>

#define XXH_INLINE_ALL 1
#define XXH_ACCEPT_NULL_INPUT_POINTER 1
#include <xxhash.h>

//----------------------------------------------------------------------------------------------------------------------

bool XenonString::StlRawCompare::operator()(const char* const left, const char* const right)
{
	return RawCompare(left, right);
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonString::StlRawCompare::operator()(const char* const left, const char* const right) const
{
	return RawCompare(left, right);
}

//----------------------------------------------------------------------------------------------------------------------

size_t XenonString::StlRawHash::operator()(const char* const string)
{
	return RawHash(string);
}

//----------------------------------------------------------------------------------------------------------------------

size_t XenonString::StlRawHash::operator()(const char* const string) const
{
	return RawHash(string);
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonString::StlCompare::operator()(
	XenonString* const pLeft,
	XenonString* const pRight
)
{
	return Compare(pLeft, pRight);
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonString::StlCompare::operator()(
	const XenonString* const pLeft,
	const XenonString* const pRight
) const
{
	return XenonString::Compare(pLeft, pRight) ;
}

//----------------------------------------------------------------------------------------------------------------------

size_t XenonString::StlHash::operator()(XenonString* const pObject)
{
	return pObject->hash;
}

//----------------------------------------------------------------------------------------------------------------------

size_t XenonString::StlHash::operator()(const XenonString* const pObject) const
{
	return pObject->hash;
}

//----------------------------------------------------------------------------------------------------------------------

XenonString* XenonString::Create(const char* const stringData)
{
	// TODO: Implement string pooling.

	XenonString* const pOutput = new XenonString();
	if(!pOutput)
	{
		return nullptr;
	}

	const size_t length = (stringData) ? strlen(stringData) : 0;

	pOutput->length = length;
	pOutput->hash = RawHash(stringData ? stringData : "");
	pOutput->data = (length > 0) ? reinterpret_cast<char*>(XenonMemAlloc(length + 1)) : nullptr;

	XenonReference::Initialize(pOutput->ref, prv_onDestruct, pOutput);

	if(stringData && pOutput->data)
	{
		memcpy(pOutput->data, stringData, length);
		pOutput->data[length] = '\0';
	}

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

int32_t XenonString::AddRef(XenonString* const pString)
{
	return (pString)
		? XenonReference::AddRef(pString->ref)
		: -1;
}

//----------------------------------------------------------------------------------------------------------------------

int32_t XenonString::Release(XenonString* const pString)
{
	return (pString)
		? XenonReference::Release(pString->ref)
		: -1;
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonString::Compare(const XenonString* const pLeft, const XenonString* const pRight)
{
	assert(pLeft != nullptr);
	assert(pRight != nullptr);

	if(pLeft->data == pRight->data)
	{
		// Same string in memory.
		return true;
	}

	if(pLeft->length != pRight->length)
	{
		// Different string lengths.
		return false;
	}

	if(pLeft->hash != pRight->hash)
	{
		// Different hashes which can only be generated by different
		// strings since the hashing is deterministic.
		return false;
	}

	return strcmp(pLeft->data, pRight->data) == 0;
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonString::RawCompare(const char* left, const char* right)
{
	assert(left != nullptr);
	assert(right != nullptr);

	if(left == right)
	{
		// Same string in memory.
		return true;
	}

	return strcmp(left, right) == 0;
}

//----------------------------------------------------------------------------------------------------------------------

size_t XenonString::RawHash(const char* const string)
{
	auto calculateFnv1aHash = [](const char* const string, const size_t length) -> size_t
	{
#ifdef XENON_CPU_WIDTH_64_BIT
		const uint64_t fnvOffsetBasis = 0xCBF29CE484222325ull;
		const uint64_t fnvPrime = 0x00000100000001B3ull;

		uint64_t output = fnvOffsetBasis;

#else
		const uint32_t fnvOffsetBasis = 0x811C9DC5ul;
		const uint32_t fnvPrime = 0x01000193ul;

		uint32_t output = fnvOffsetBasis;

#endif

		// Calculate the FNV1a hash from the input data.
		for(size_t i = 0; i < length; ++i)
		{
			output ^= string[i];
			output *= fnvPrime;
		}

		return size_t(output);
	};

	assert(string != nullptr);

	const size_t length = strlen(string);
	const size_t seed = calculateFnv1aHash(string, length);

	return size_t(
#ifdef XENON_CPU_WIDTH_64_BIT
		XXH64
#else
		XXH32
#endif
		(string, length, seed)
	);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonString::prv_onDestruct(void* const pOpaque)
{
	XenonString* const pString = reinterpret_cast<XenonString*>(pOpaque);

	if(pString->data)
	{
		XenonMemFree(pString->data);
	}

	delete pString;
}

//----------------------------------------------------------------------------------------------------------------------

void* XenonString::operator new(const size_t sizeInBytes)
{
	return XenonMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonString::operator delete(void* const pObject)
{
	XenonMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
