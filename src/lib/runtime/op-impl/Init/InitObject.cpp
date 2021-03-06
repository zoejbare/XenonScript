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

#include "../../OpDecl.hpp"

#include "../../Decoder.hpp"
#include "../../Execution.hpp"
#include "../../Frame.hpp"
#include "../../Program.hpp"
#include "../../Vm.hpp"

#include <stdio.h>
#include <inttypes.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Initialize a new object by type name, storing it in a general-purpose register.
//
// 0x: INIT_OBJECT r#, c#
//
//   r# = General-purpose register where the new object will be stored.
//   c# = Constant index to the name of the object type to create.
//
//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void OpCodeExec_InitObject(XenonExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t constantIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	XenonValueHandle hObjectTypeName= XenonProgram::GetConstant(hExec->hCurrentFrame->hFunction->hProgram, constantIndex, &result);
	if(result == XENON_SUCCESS)
	{
		// Verify the loaded value is a string.
		if(XenonValueIsString(hObjectTypeName))
		{
			// Get the object schema matching the type name.
			XenonScriptObject* const pObjectSchema = XenonVm::GetObjectSchema(hExec->hVm, hObjectTypeName->as.pString, &result);
			if(result == XENON_SUCCESS)
			{
				// Create a new object from the schema.
				XenonValueHandle hObject = XenonValue::CreateObject(hExec->hVm, pObjectSchema);

				// Verify the created value is an object.
				if(XenonValueIsObject(hObject))
				{
					result = XenonFrame::SetGpRegister(hExec->hCurrentFrame, hObject, registerIndex);
					if(result != XENON_SUCCESS)
					{
						// Raise a fatal script exception.
						XenonExecutionRaiseStandardException(
							hExec,
							XENON_EXCEPTION_SEVERITY_FATAL,
							XENON_STANDARD_EXCEPTION_RUNTIME_ERROR,
							"Failed to set general-purpose register: r(%" PRIu32 ")",
							registerIndex
						);
					}

					XenonValueAbandon(hObject);
				}
				else
				{
					// Raise a fatal script exception.
					XenonExecutionRaiseStandardException(
						hExec,
						XENON_EXCEPTION_SEVERITY_FATAL,
						XENON_STANDARD_EXCEPTION_RUNTIME_ERROR,
						"Failed to create object value"
					);
				}
			}
			else
			{
				// Raise a fatal script exception.
				XenonExecutionRaiseStandardException(
					hExec,
					XENON_EXCEPTION_SEVERITY_FATAL,
					XENON_STANDARD_EXCEPTION_RUNTIME_ERROR,
					"Failed to find object schema: type=%s",
					hObjectTypeName->as.pString->data
				);
			}
		}
		else
		{
			// Raise a fatal script exception.
			XenonExecutionRaiseStandardException(
				hExec,
				XENON_EXCEPTION_SEVERITY_FATAL,
				XENON_STANDARD_EXCEPTION_TYPE_ERROR,
				"Type mismatch; expected string: c(%" PRIu32 ")",
				constantIndex
			);
		}
	}
	else
	{
		// Raise a fatal script exception.
		XenonExecutionRaiseStandardException(
			hExec,
			XENON_EXCEPTION_SEVERITY_FATAL,
			XENON_STANDARD_EXCEPTION_RUNTIME_ERROR,
			"Failed to retrieve constant value: c(%" PRIu32 ")",
			constantIndex
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_InitObject(XenonDisassemble& disasm)
{
	int result;

	const uint32_t registerIndex = XenonDecoder::LoadUint32(disasm.decoder);
	const uint32_t constantIndex = XenonDecoder::LoadUint32(disasm.decoder);

	XenonValueHandle hValue = XenonProgram::GetConstant(disasm.hProgram, constantIndex, &result);
	XenonString* const pValueData = XenonValue::GetDebugString(hValue);

	char instr[512];
	snprintf(instr, sizeof(instr), "INIT_OBJECT r%" PRIu32 ", c%" PRIu32 " %s", registerIndex, constantIndex, pValueData->data);
	disasm.onDisasmFn(disasm.pUserData, instr, disasm.opcodeOffset);

	XenonString::Release(pValueData);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
