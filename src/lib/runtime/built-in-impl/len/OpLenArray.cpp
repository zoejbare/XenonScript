//
// Copyright (c) 2022, Zoe J. Bare
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

#include "../../BuiltInDecl.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpLenArray(XenonExecutionHandle hExec, XenonFunctionHandle, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Get the parameter operand.
	XenonValueHandle hParam;
	XenonExecutionGetIoRegister(hExec, &hParam, 0);

	// Verify the value pulled from the I/O register is an array.
	if(XenonValueIsArray(hParam))
	{
		size_t length = 0;
		XenonValueGetArrayLength(hParam, &length);

		const int64_t outputLength = int64_t(length);

		// Get the VM associated with the execution context.
		XenonVmHandle hVm;
		XenonExecutionGetVm(hExec, &hVm);

		// Create the output result and store it to an I/O register.
		XenonValueHandle hOutput = XenonValueCreateInt64(hVm, (outputLength > 0) ? outputLength : 0);
		XenonExecutionSetIoRegister(hExec, hOutput, 0);
		XenonValueAbandon(hOutput);
	}
	else
	{
		// Raise the type-mismatch script exception.
		XenonExecutionRaiseStandardException(
			hExec,
			XENON_EXCEPTION_SEVERITY_NORMAL,
			XENON_STANDARD_EXCEPTION_TYPE_ERROR,
			"Type mismatch; expected array"
		);
	}

	// Release the input parameter value.
	XenonValueAbandon(hParam);
}

//----------------------------------------------------------------------------------------------------------------------
