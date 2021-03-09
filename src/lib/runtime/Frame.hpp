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

#include "Function.hpp"

#include "../shared/Array.hpp"
#include "../shared/Stack.hpp"
#include "../shared/Value.hpp"

//----------------------------------------------------------------------------------------------------------------------

struct XenonFrame
{
	typedef XenonArray<XenonFrameHandle> HandleArray;
	typedef XenonStack<XenonFrameHandle> HandleStack;

	static XenonFrameHandle Create(XenonFunctionHandle hFunction);
	static void Dispose(XenonFrameHandle hFrame);

	static int PushValue(XenonFrameHandle hFrame, XenonValueHandle hValue);
	static int PopValue(XenonFrameHandle hFrame, XenonValueHandle* const phOutValue);
	static int PeekValue(XenonFrameHandle hFrame, XenonValueHandle* const phOutValue, const size_t index);

	static void SetGpRegister(XenonFrameHandle hFrame, XenonValueHandle hValue, const uint32_t index);
	static XenonValueHandle GetGpRegister(XenonFrameHandle hFrame, const uint32_t index);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	XenonValue::HandleStack stack;
	XenonValue::HandleArray registers;

	XenonFunctionHandle hFunction;

	uint8_t* ip;
};

//----------------------------------------------------------------------------------------------------------------------
