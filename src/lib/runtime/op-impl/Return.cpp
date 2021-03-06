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

#include "../OpDecl.hpp"

#include "../Decoder.hpp"
#include "../Execution.hpp"
#include "../Vm.hpp"

#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Pop the current frame from the frame stack, effectively returning from a function.
//
// 0x: RETURN
//
//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void OpCodeExec_Return(XenonExecutionHandle hExec)
{
	const int result = XenonExecution::PopFrame(hExec);

	if(result != XENON_SUCCESS)
	{
		char msg[64];
		snprintf(msg, sizeof(msg), "Failed to pop frame: error=%d", result);

		XenonExecution::RaiseFatalStandardException(hExec, XENON_STANDARD_EXCEPTION_RUNTIME_ERROR, msg);
	}
	else if(!hExec->hCurrentFrame)
	{
		// The entry point function was popped from the frame stack meaning execution is complete.
		hExec->finished = true;
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_Return(XenonDisassemble& disasm)
{
	disasm.onDisasmFn(disasm.pUserData, "RETURN", disasm.opcodeOffset);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
