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

#include "Decoder.hpp"
#include "Execution.hpp"
#include "Function.hpp"
#include "GarbageCollector.hpp"
#include "OpDecl.hpp"
#include "Program.hpp"
#include "ScriptObject.hpp"
#include "Value.hpp"

#include "../base/RwLock.hpp"
#include "../base/Thread.hpp"

#include "../common/Dependency.hpp"
#include "../common/Report.hpp"

//----------------------------------------------------------------------------------------------------------------------

struct XenonVm
{
	typedef XENON_MAP_TYPE<
		int,
		XenonScriptObject*,
#if XENON_MAP_IS_UNORDERED
		std::hash<int>,
		std::equal_to<int>,
#else
		std::less<int>,
#endif
		XenonStlAllocator<XENON_MAP_NODE_TYPE(int, XenonScriptObject*)>
	> EmbeddedExceptionMap;

	struct OpCode
	{
		typedef void (*ExecuteCallback)(XenonExecutionHandle);
		typedef void (*DisassembleCallback)(XenonDisassemble&);

		ExecuteCallback execFn;
		DisassembleCallback disasmFn;
	};

	typedef XenonArray<OpCode> OpCodeArray;

	static XenonVmHandle Create(const XenonVmInit& init);
	static void Dispose(XenonVmHandle hVm);

	static int SetGlobalVariable(XenonVmHandle hVm, XenonValueHandle hValue, XenonString* const pVariableName);

	static XenonProgramHandle GetProgram(XenonVmHandle hVm, XenonString* const pProgramName, int* const pOutResult);
	static XenonFunctionHandle GetFunction(XenonVmHandle hVm, XenonString* const pFunctionSignature, int* const pOutResult);
	static XenonValueHandle GetGlobalVariable(XenonVmHandle hVm, XenonString* const pVariableName, int* const pOutResult);
	static XenonScriptObject* GetObjectSchema(XenonVmHandle hVm, XenonString* const pTypeName, int* const pOutResult);

	static XenonValueHandle CreateStandardException(XenonVmHandle hVm, const int exceptionType, const char* const message);

	static void ExecuteOpCode(XenonVmHandle hVm, XenonExecutionHandle hExec, const int opCode);
	static void DisassembleOpCode(XenonVmHandle hVm, XenonDisassemble& disasm, const int opCode);

	static void prv_setupOpCodes(XenonVmHandle);
	static void prv_setupBuiltIns(XenonVmHandle);
	static void prv_setupEmbeddedExceptions(XenonVmHandle);

	static int32_t prv_gcThreadMain(void*);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	OpCodeArray opCodes;
	EmbeddedExceptionMap embeddedExceptions;

	XenonProgram::StringToHandleMap programs;
	XenonFunction::StringToHandleMap functions;
	XenonValue::StringToHandleMap globals;
	XenonScriptObject::StringToPtrMap objectSchemas;
	XenonExecution::HandleToBoolMap executionContexts;

	XenonReport report;
	XenonGarbageCollector gc;
	XenonThread gcThread;
	XenonRwLock gcRwLock;

	bool isShuttingDown;
};

//----------------------------------------------------------------------------------------------------------------------
