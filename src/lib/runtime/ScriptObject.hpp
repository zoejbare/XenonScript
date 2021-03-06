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

#pragma once

//----------------------------------------------------------------------------------------------------------------------

#include "Value.hpp"

#include "../base/String.hpp"

#include "../common/Map.hpp"
#include "../common/StlAllocator.hpp"

//----------------------------------------------------------------------------------------------------------------------

struct XenonScriptObject
{
	struct MemberDefinition
	{
		uint32_t bindingIndex;
		uint8_t valueType;
	};

	typedef XENON_MAP_TYPE<
		XenonString*,
		MemberDefinition,
#if XENON_MAP_IS_UNORDERED
		XenonString::StlHash,
		XenonString::StlCompare,
#else
		XenonString::StlLess,
#endif
		XenonStlAllocator<XENON_MAP_NODE_TYPE(XenonString*, MemberDefinition)>
	> MemberDefinitionMap;

	typedef XENON_MAP_TYPE<
		XenonString*,
		XenonScriptObject*,
#if XENON_MAP_IS_UNORDERED
		XenonString::StlHash,
		XenonString::StlCompare,
#else
		XenonString::StlLess,
#endif
		XenonStlAllocator<XENON_MAP_NODE_TYPE(XenonString*, XenonScriptObject*)>
	> StringToPtrMap;

	static XenonScriptObject* CreateSchema(XenonString* pTypeName, const MemberDefinitionMap& definitions);
	static XenonScriptObject* CreateInstance(XenonScriptObject* const pSchema);
	static XenonScriptObject* CreateCopy(XenonScriptObject* const pObject);
	static void Dispose(XenonScriptObject* const pObject);

	static XenonValueHandle GetMemberValue(XenonScriptObject* const pObject, const uint32_t memberIndex, int* const pOutResult);
	static MemberDefinition GetMemberDefinition(XenonScriptObject* const pObject, XenonString* const pMemberName, int* const pOutResult);

	static int SetMemberValue(XenonScriptObject* const pObject, const uint32_t memberIndex, XenonValueHandle hValue);

	static XenonScriptObject* prv_createObject(XenonScriptObject*);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	XenonString* pTypeName;
	XenonScriptObject* pSchema;

	MemberDefinitionMap definitions;

	XenonValue::HandleArray members;
};

//----------------------------------------------------------------------------------------------------------------------
