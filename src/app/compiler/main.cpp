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

#include "XenonScript.h"

#if defined(XENON_PLATFORM_WINDOWS)
	#include <crtdbg.h>
#endif

#include <assert.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------

void OnMessageReported(void* const pUserData, const int messageType, const char* const message)
{
	(void) pUserData;

	if(!message)
	{
		return;
	}

	const char* tag = NULL;

	switch(messageType)
	{
		case XENON_MESSAGE_TYPE_VERBOSE:
			tag = "V";
			break;

		case XENON_MESSAGE_TYPE_INFO:
			tag = "I";
			break;

		case XENON_MESSAGE_TYPE_WARNING:
			tag = "W";
			break;

		case XENON_MESSAGE_TYPE_ERROR:
			tag = "E";
			break;

		case XENON_MESSAGE_TYPE_FATAL:
			tag = "!";
			break;

		default:
			assert(false);
			break;
	}

	fprintf((messageType >= XENON_MESSAGE_TYPE_ERROR) ? stderr : stdout, "[%s] %s\n", tag, message);
}

//----------------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;

#if defined(XENON_PLATFORM_WINDOWS)
	// This enables tracking of global heap allocations.  If any are leaked, they will show up in the
	// Visual Studio output window on application exit.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	XenonCompilerHandle hCompiler = XENON_COMPILER_HANDLE_NULL;
	XenonCompilerInit init;

	init.common.report.onMessageFn = OnMessageReported;
	init.common.report.pUserData = nullptr;
	init.common.report.reportLevel = XENON_MESSAGE_TYPE_VERBOSE;

	// Create the compiler context.
	int result = XenonCompilerCreate(&hCompiler, init);
	if(result != XENON_SUCCESS)
	{
		char msg[128];
		snprintf(msg, sizeof(msg) - 1, "Failed to create Xenon compiler context: error=\"%s\"", XenonGetErrorCodeString(result));
		OnMessageReported(NULL, XENON_MESSAGE_TYPE_FATAL, msg);
		return 1;
	}

	XenonProgramWriterHandle hProgramWriter = XENON_PROGRAM_WRITER_HANDLE_NULL;

	// Create the program writer.
	result = XenonProgramWriterCreate(&hProgramWriter, hCompiler);
	if(result != XENON_SUCCESS)
	{
		char msg[128];
		snprintf(msg, sizeof(msg) - 1, "Failed to create Xenon program writer: error=\"%s\"", XenonGetErrorCodeString(result));
		OnMessageReported(NULL, XENON_MESSAGE_TYPE_FATAL, msg);
		return 1;
	}

	auto writeOpLoadConstant = [](XenonSerializerHandle hSerializer, const uint32_t registerIndex, const uint32_t constantIndex)
	{
		XenonSerializerWriteUint8(hSerializer, XENON_OP_CODE_LOAD_CONSTANT);
		XenonSerializerWriteUint32(hSerializer, registerIndex);
		XenonSerializerWriteUint32(hSerializer, constantIndex);
	};

	auto writeOpLoadGlobal = [](XenonSerializerHandle hSerializer, const uint32_t registerIndex, const uint32_t constantIndex)
	{
		XenonSerializerWriteUint8(hSerializer, XENON_OP_CODE_LOAD_GLOBAL);
		XenonSerializerWriteUint32(hSerializer, registerIndex);
		XenonSerializerWriteUint32(hSerializer, constantIndex);
	};

	auto writeOpLoadLocal = [](XenonSerializerHandle hSerializer, const uint32_t registerIndex, const uint32_t constantIndex)
	{
		XenonSerializerWriteUint8(hSerializer, XENON_OP_CODE_LOAD_LOCAL);
		XenonSerializerWriteUint32(hSerializer, registerIndex);
		XenonSerializerWriteUint32(hSerializer, constantIndex);
	};

	auto writeOpStoreGlobal = [](XenonSerializerHandle hSerializer, const uint32_t constantIndex, const uint32_t registerIndex)
	{
		XenonSerializerWriteUint8(hSerializer, XENON_OP_CODE_STORE_GLOBAL);
		XenonSerializerWriteUint32(hSerializer, constantIndex);
		XenonSerializerWriteUint32(hSerializer, registerIndex);
	};

	auto writeOpStoreLocal = [](XenonSerializerHandle hSerializer, const uint32_t constantIndex, const uint32_t registerIndex)
	{
		XenonSerializerWriteUint8(hSerializer, XENON_OP_CODE_STORE_LOCAL);
		XenonSerializerWriteUint32(hSerializer, constantIndex);
		XenonSerializerWriteUint32(hSerializer, registerIndex);
	};

	auto writeOpLoadParam = [](XenonSerializerHandle hSerializer, const uint32_t gpRegIndex, const uint32_t ioRegIndex)
	{
		XenonSerializerWriteUint8(hSerializer, XENON_OP_CODE_LOAD_PARAM);
		XenonSerializerWriteUint32(hSerializer, gpRegIndex);
		XenonSerializerWriteUint32(hSerializer, ioRegIndex);
	};

	auto writeOpStoreParam = [](XenonSerializerHandle hSerializer, const uint32_t ioRegIndex, const uint32_t gpRegIndex)
	{
		XenonSerializerWriteUint8(hSerializer, XENON_OP_CODE_STORE_PARAM);
		XenonSerializerWriteUint32(hSerializer, ioRegIndex);
		XenonSerializerWriteUint32(hSerializer, gpRegIndex);
	};

	auto writeOpCall = [](XenonSerializerHandle hSerializer, const uint32_t constantIndex)
	{
		XenonSerializerWriteUint8(hSerializer, XENON_OP_CODE_CALL);
		XenonSerializerWriteUint32(hSerializer, constantIndex);
	};

	auto writeOpNop = [](XenonSerializerHandle hSerializer)
	{
		XenonSerializerWriteUint8(hSerializer, XENON_OP_CODE_NOP);
	};

	auto writeOpPop = [](XenonSerializerHandle hSerializer, const uint32_t registerIndex)
	{
		XenonSerializerWriteUint8(hSerializer, XENON_OP_CODE_POP);
		XenonSerializerWriteUint32(hSerializer, registerIndex);
	};

	auto writeOpPush = [](XenonSerializerHandle hSerializer, const uint32_t registerIndex)
	{
		XenonSerializerWriteUint8(hSerializer, XENON_OP_CODE_PUSH);
		XenonSerializerWriteUint32(hSerializer, registerIndex);
	};

	auto writeOpReturn = [](XenonSerializerHandle hSerializer)
	{
		XenonSerializerWriteUint8(hSerializer, XENON_OP_CODE_RETURN);
	};

	auto writeOpYield = [](XenonSerializerHandle hSerializer)
	{
		XenonSerializerWriteUint8(hSerializer, XENON_OP_CODE_YIELD);
	};

	XenonSerializerHandle hFileSerializer = XENON_SERIALIZER_HANDLE_NULL;
	XenonSerializerHandle hMainFuncSerializer = XENON_SERIALIZER_HANDLE_NULL;
	XenonSerializerHandle hSubFuncSerializer = XENON_SERIALIZER_HANDLE_NULL;

	XenonSerializerCreate(&hFileSerializer, XENON_SERIALIZER_MODE_WRITER);
	XenonSerializerCreate(&hMainFuncSerializer, XENON_SERIALIZER_MODE_WRITER);
	XenonSerializerCreate(&hSubFuncSerializer, XENON_SERIALIZER_MODE_WRITER);

	const char* const mainFuncSignature = "void Program.Main()";
	const char* const subFuncSignature = "int32 Program.DoWork(float64)";
	const char* const nativeFuncSignature = "void Program.PrintString(string)";
	const char* const builtInFuncSignature = XenonGetBuiltInFunctionSignature(XENON_BUILT_IN_OP_ADD_STRING);
	const char* const globalVariableName = "globalTestVar";
	const char* const localVariableName = "localTestVar";

	uint32_t constIndex0;
	XenonProgramWriterAddConstantNull(hProgramWriter, &constIndex0);

	uint32_t constIndex1;
	XenonProgramWriterAddConstantInt32(hProgramWriter, 123, &constIndex1);

	uint32_t constIndex2;
	XenonProgramWriterAddConstantFloat64(hProgramWriter, 1.2345, &constIndex2);

	uint32_t constIndex3;
	XenonProgramWriterAddConstantFloat64(hProgramWriter, 2.3456, &constIndex3);

	uint32_t constIndex4;
	XenonProgramWriterAddConstantString(hProgramWriter, "this is ", &constIndex4);

	uint32_t constIndex5;
	XenonProgramWriterAddConstantString(hProgramWriter, "a test string", &constIndex5);

	uint32_t constIndex6;
	XenonProgramWriterAddConstantString(hProgramWriter, globalVariableName, &constIndex6);

	uint32_t constIndex7;
	XenonProgramWriterAddConstantString(hProgramWriter, subFuncSignature, &constIndex7);

	uint32_t constIndex8;
	XenonProgramWriterAddConstantString(hProgramWriter, nativeFuncSignature, &constIndex8);

	uint32_t constIndex9;
	XenonProgramWriterAddConstantString(hProgramWriter, localVariableName, &constIndex9);

	uint32_t constIndex10;
	XenonProgramWriterAddConstantString(hProgramWriter, builtInFuncSignature, &constIndex10);

	// Add the program globals.
	XenonProgramWriterAddGlobal(hProgramWriter, globalVariableName, constIndex4);

	// void Program.Main()
	{
		writeOpLoadConstant(hMainFuncSerializer, 0, constIndex0);
		writeOpLoadConstant(hMainFuncSerializer, 1, constIndex1);
		writeOpLoadConstant(hMainFuncSerializer, 2, constIndex2);
		writeOpLoadConstant(hMainFuncSerializer, 3, constIndex3);
		writeOpLoadConstant(hMainFuncSerializer, 4, constIndex4);
		writeOpLoadConstant(hMainFuncSerializer, 5, constIndex5);
		writeOpLoadConstant(hMainFuncSerializer, 6, constIndex6);

		writeOpLoadGlobal(hMainFuncSerializer, 7, constIndex6);
		writeOpStoreGlobal(hMainFuncSerializer, constIndex6, 5);

		writeOpStoreParam(hMainFuncSerializer, 0, 3);
		writeOpCall(hMainFuncSerializer, constIndex7);
		writeOpLoadParam(hMainFuncSerializer, 8, 0);

		writeOpReturn(hMainFuncSerializer);

		const void* const pMainFuncData = XenonSerializerGetRawStreamPointer(hMainFuncSerializer);
		const size_t mainFuncLength = XenonSerializerGetStreamLength(hMainFuncSerializer);

		XenonProgramWriterAddFunction(hProgramWriter, mainFuncSignature, pMainFuncData, mainFuncLength, 0, 0);
	}

	// int32 Program.DoWork(float64)
	{
		writeOpLoadParam(hSubFuncSerializer, 0, 0);

		writeOpLoadLocal(hSubFuncSerializer, 1, constIndex9);
		writeOpStoreLocal(hSubFuncSerializer, constIndex9, 0);

		writeOpPush(hSubFuncSerializer, 0);
		writeOpYield(hSubFuncSerializer);
		writeOpPop(hSubFuncSerializer, 1);

		writeOpLoadConstant(hSubFuncSerializer, 0, constIndex4);
		writeOpStoreParam(hSubFuncSerializer, 0, 0);

		writeOpLoadConstant(hSubFuncSerializer, 1, constIndex5);
		writeOpStoreParam(hSubFuncSerializer, 1, 1);

		writeOpCall(hSubFuncSerializer, constIndex10);
		writeOpCall(hSubFuncSerializer, constIndex8);
		writeOpLoadConstant(hSubFuncSerializer, 0, constIndex0);
		writeOpStoreParam(hSubFuncSerializer, 0, 0);
		writeOpStoreParam(hSubFuncSerializer, 1, 0);

		writeOpLoadConstant(hSubFuncSerializer, 0, constIndex1);
		writeOpStoreParam(hSubFuncSerializer, 0, 1);

		writeOpNop(hSubFuncSerializer);
		writeOpReturn(hSubFuncSerializer);

		const void* const pSubFuncData = XenonSerializerGetRawStreamPointer(hSubFuncSerializer);
		const size_t subFuncLength = XenonSerializerGetStreamLength(hSubFuncSerializer);

		XenonProgramWriterAddFunction(hProgramWriter, subFuncSignature, pSubFuncData, subFuncLength, 1, 1);
		XenonProgramWriterAddLocalVariable(hProgramWriter, subFuncSignature, localVariableName, constIndex2);
	}

	// void Program.PrintString(string)
	{
		XenonProgramWriterAddNativeFunction(hProgramWriter, nativeFuncSignature, 1, 0);
	}

	result = XenonProgramWriterSerialize(hProgramWriter, hCompiler, hFileSerializer, XENON_ENDIAN_MODE_NATIVE);
	if(result == XENON_SUCCESS)
	{
		const void* const pFileData = XenonSerializerGetRawStreamPointer(hFileSerializer);
		const size_t fileLength = XenonSerializerGetStreamLength(hFileSerializer);

		const char* const outputFilename = "test.xc";

		char msg[128];
		snprintf(msg, sizeof(msg), "Writing file: %s", outputFilename);
		OnMessageReported(nullptr, XENON_MESSAGE_TYPE_INFO, msg);

		FILE* const pOutputFile = fopen(outputFilename, "wb");
		fwrite(pFileData, fileLength, 1, pOutputFile);
		fclose(pOutputFile);
	}

	XenonSerializerDispose(&hMainFuncSerializer);
	XenonSerializerDispose(&hSubFuncSerializer);
	XenonSerializerDispose(&hFileSerializer);

	// Dispose of the program writer.
	result = XenonProgramWriterDispose(&hProgramWriter);
	if(result != XENON_SUCCESS)
	{
		char msg[128];
		snprintf(msg, sizeof(msg) - 1, "Failed to dispose of Xenon program writer: error=\"%s\"", XenonGetErrorCodeString(result));
		OnMessageReported(NULL, XENON_MESSAGE_TYPE_WARNING, msg);
	}

	// Dispose of the compiler context.
	result = XenonCompilerDispose(&hCompiler);
	if(result != XENON_SUCCESS)
	{
		char msg[128];
		snprintf(msg, sizeof(msg) - 1, "Failed to dispose of Xenon compiler context: error=\"%s\"", XenonGetErrorCodeString(result));
		OnMessageReported(NULL, XENON_MESSAGE_TYPE_WARNING, msg);
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------
