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

#include "Frame.hpp"
#include "Program.hpp"
#include "Vm.hpp"

#include "../base/Mutex.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

XenonFrameHandle XenonFrame::Create(XenonExecutionHandle hExec, XenonFunctionHandle hFunction)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(hFunction != XENON_FUNCTION_HANDLE_NULL);

	XenonFrame* const pOutput = new XenonFrame();
	assert(pOutput != XENON_FRAME_HANDLE_NULL);

	// No need to lock the garbage collector here since only the execution context is allowed to create frames
	// and it will be handling the lock for us.
	XenonGcProxy::Initialize(pOutput->gcProxy, hExec->hVm->gc, prv_onGcDiscovery, prv_onGcDestruct, pOutput, false);

	pOutput->hExec = hExec;
	pOutput->hFunction = hFunction;

	if(hFunction->isNative)
	{
		// Initialize the value structures to avoid deleting garbage memory on clean up.
		// No other work needs to be done for native functions since this is intended to
		// be just a dummy frame.
		XenonValue::HandleStack::Initialize(pOutput->stack, 0);
		XenonValue::HandleArray::Initialize(pOutput->registers);
	}
	else
	{
		// Setup the register array.
		XenonValue::HandleStack::Initialize(pOutput->stack, XENON_VM_FRAME_STACK_SIZE);
		XenonValue::HandleArray::Initialize(pOutput->registers);
		XenonValue::HandleArray::Reserve(pOutput->registers, XENON_VM_GP_REGISTER_COUNT);

		pOutput->registers.count = XENON_VM_GP_REGISTER_COUNT;

		// Initialize each register value.
		for(size_t i = 0; i < pOutput->registers.count; ++i)
		{
			pOutput->registers.pData[i] = XenonValue::CreateNull();
		}

		// Build the local table for the new frame. This will intentionally copy each value from the function's local table
		// so any changes made the variables in the frame will not affect the prototypes in the function.
		for(auto& kv : hFunction->locals)
		{
			XenonString* const pKey = XENON_MAP_ITER_KEY(kv);
			XenonValueHandle hValue = XENON_MAP_ITER_VALUE(kv);

			XENON_MAP_FUNC_INSERT(pOutput->locals, pKey, XenonValueCopy(hExec->hVm, hValue));
		}

		XenonDecoder::Initialize(pOutput->decoder, hFunction->hProgram, hFunction->bytecodeOffsetStart);
	}

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFrame::PushValue(XenonFrameHandle hFrame, XenonValueHandle hValue)
{
	assert(hFrame != XENON_FRAME_HANDLE_NULL);
	assert(hValue != XENON_VALUE_HANDLE_NULL);

	return XenonValue::HandleStack::Push(hFrame->stack, hValue);
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFrame::PopValue(XenonFrameHandle hFrame, XenonValueHandle* const phOutValue)
{
	assert(hFrame != XENON_FRAME_HANDLE_NULL);
	assert(phOutValue != nullptr);
	assert(*phOutValue == XENON_VALUE_HANDLE_NULL);

	// Pop the stack, returning the value that was popped. The calling code will be responsible for releasing it.
	return XenonValue::HandleStack::Pop(hFrame->stack, phOutValue);
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFrame::PeekValue(
	XenonFrameHandle hFrame,
	XenonValueHandle* const phOutValue,
	const size_t index
)
{
	assert(hFrame != XENON_FRAME_HANDLE_NULL);
	assert(phOutValue != nullptr);
	assert(*phOutValue == XENON_VALUE_HANDLE_NULL);

	XenonValueHandle hValue = XENON_VALUE_HANDLE_NULL;

	int result = XenonValue::HandleStack::Peek(hFrame->stack, &hValue, index);
	if(result != XENON_SUCCESS)
	{
		return result;
	}

	(*phOutValue) = hValue;

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFrame::SetGpRegister(XenonFrameHandle hFrame, XenonValueHandle hValue, const uint32_t index)
{
	assert(hFrame != XENON_FRAME_HANDLE_NULL);
	assert(hValue != XENON_VALUE_HANDLE_NULL);
	assert(index < XENON_VM_GP_REGISTER_COUNT);

	hFrame->registers.pData[index] = hValue;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFrame::SetLocalVariable(XenonFrameHandle hFrame, XenonValueHandle hValue, XenonString* const pVariableName)
{
	assert(hFrame != XENON_FRAME_HANDLE_NULL);
	assert(hValue != XENON_VALUE_HANDLE_NULL);
	assert(pVariableName != nullptr);

	auto kv = hFrame->locals.find(pVariableName);
	if(kv == hFrame->locals.end())
	{
		return XENON_ERROR_KEY_DOES_NOT_EXIST;
	}

	XENON_MAP_ITER_PTR_VALUE(kv) = hValue;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonFrame::GetGpRegister(XenonFrameHandle hFrame, const uint32_t index, int* const pOutResult)
{
	assert(hFrame != XENON_FRAME_HANDLE_NULL);
	assert(pOutResult != nullptr);

	if(index >= XENON_VM_GP_REGISTER_COUNT)
	{
		(*pOutResult) = XENON_ERROR_INDEX_OUT_OF_RANGE;
		return XENON_VALUE_HANDLE_NULL;
	}

	(*pOutResult) = XENON_SUCCESS;
	return hFrame->registers.pData[index];
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonFrame::GetLocalVariable(XenonFrameHandle hFrame, XenonString* const pVariableName, int* const pOutResult)
{
	assert(hFrame != XENON_FRAME_HANDLE_NULL);
	assert(pVariableName != nullptr);
	assert(pOutResult != nullptr);

	if(!XENON_MAP_FUNC_CONTAINS(hFrame->locals, pVariableName))
	{
		(*pOutResult) = XENON_ERROR_KEY_DOES_NOT_EXIST;
		return XENON_VALUE_HANDLE_NULL;
	}

	(*pOutResult) = XENON_SUCCESS;
	return XENON_MAP_FUNC_GET(hFrame->locals, pVariableName);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonFrame::prv_onGcDiscovery(XenonGarbageCollector& gc, void* const pOpaque)
{
	XenonFrameHandle hFrame = reinterpret_cast<XenonFrameHandle>(pOpaque);
	assert(hFrame != XENON_FRAME_HANDLE_NULL);

	// Discover values in the stack.
	size_t stackSize = XenonValue::HandleStack::GetCurrentSize(hFrame->stack);
	for(size_t i = 0; i < stackSize; ++i)
	{
		XenonValueHandle hValue = hFrame->stack.memory.pData[i];

		if(XenonValue::CanBeMarked(hValue))
		{
			XenonGarbageCollector::MarkObject(gc, &hValue->gcProxy);
		}
	}

	// Discover values held in the general purpose registers.
	for(size_t i = 0; i < hFrame->registers.count; ++i)
	{
		XenonValueHandle hValue = hFrame->registers.pData[i];

		if(XenonValue::CanBeMarked(hValue))
		{
			XenonGarbageCollector::MarkObject(gc, &hValue->gcProxy);
		}
	}

	// Discover the local variable values.
	for(auto& kv : hFrame->locals)
	{
		XenonValueHandle hValue = XENON_MAP_ITER_VALUE(kv);

		if(XenonValue::CanBeMarked(hValue))
		{
			XenonGarbageCollector::MarkObject(gc, &hValue->gcProxy);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void XenonFrame::prv_onGcDestruct(void* const pOpaque)
{
	XenonFrameHandle hFrame = reinterpret_cast<XenonFrameHandle>(pOpaque);
	assert(hFrame != XENON_FRAME_HANDLE_NULL);

	XenonValue::HandleStack::Dispose(hFrame->stack);
	XenonValue::HandleArray::Dispose(hFrame->registers);

	delete hFrame;
}

//----------------------------------------------------------------------------------------------------------------------

void* XenonFrame::operator new(const size_t sizeInBytes)
{
	return XenonMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonFrame::operator delete(void* const pObject)
{
	XenonMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
