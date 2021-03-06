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

#include "../../BuiltInDecl.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpDivBool(XenonExecutionHandle hExec, XenonFunctionHandle, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	XenonValueHandle hLeft = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	XenonValueHandle hRight = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const bool left = XenonValueGetBool(hLeft);
	const bool right = XenonValueGetBool(hRight);

	// Release the input parameter values now that we have their data.
	XenonValueAbandon(hLeft);
	XenonValueAbandon(hRight);

	if(right)
	{
		// Create the output result and store it to an I/O register.
		XenonValueHandle hOutput = XenonValueCreateBool(hVm, left / right);

		XenonExecutionSetIoRegister(hExec, hOutput, 0);
		XenonValueAbandon(hOutput);
	}
	else
	{
		// Raise the divide-by-zero script exception.
		XenonExecutionRaiseStandardException(
			hExec,
			XENON_EXCEPTION_SEVERITY_NORMAL,
			XENON_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
			"Divide-by-zero error (bool)"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpDivInt8(XenonExecutionHandle hExec, XenonFunctionHandle, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	XenonValueHandle hLeft = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	XenonValueHandle hRight = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const int8_t left = XenonValueGetInt8(hLeft);
	const int8_t right = XenonValueGetInt8(hRight);

	// Release the input parameter values now that we have their data.
	XenonValueAbandon(hLeft);
	XenonValueAbandon(hRight);

	if(right != 0)
	{
		// Create the output result and store it to an I/O register.
		XenonValueHandle hOutput = XenonValueCreateInt8(hVm, left / right);

		XenonExecutionSetIoRegister(hExec, hOutput, 0);
		XenonValueAbandon(hOutput);
	}
	else
	{
		// Raise the divide-by-zero script exception.
		XenonExecutionRaiseStandardException(
			hExec,
			XENON_EXCEPTION_SEVERITY_NORMAL,
			XENON_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
			"Divide-by-zero error (int8)"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpDivInt16(XenonExecutionHandle hExec, XenonFunctionHandle, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	XenonValueHandle hLeft = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	XenonValueHandle hRight = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const int16_t left = XenonValueGetInt16(hLeft);
	const int16_t right = XenonValueGetInt16(hRight);

	// Release the input parameter values now that we have their data.
	XenonValueAbandon(hLeft);
	XenonValueAbandon(hRight);

	if(right != 0)
	{
		// Create the output result and store it to an I/O register.
		XenonValueHandle hOutput = XenonValueCreateInt16(hVm, left / right);

		XenonExecutionSetIoRegister(hExec, hOutput, 0);
		XenonValueAbandon(hOutput);
	}
	else
	{
		// Raise the divide-by-zero script exception.
		XenonExecutionRaiseStandardException(
			hExec,
			XENON_EXCEPTION_SEVERITY_NORMAL,
			XENON_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
			"Divide-by-zero error (int16)"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpDivInt32(XenonExecutionHandle hExec, XenonFunctionHandle, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	XenonValueHandle hLeft = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	XenonValueHandle hRight = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const int32_t left = XenonValueGetInt32(hLeft);
	const int32_t right = XenonValueGetInt32(hRight);

	// Release the input parameter values now that we have their data.
	XenonValueAbandon(hLeft);
	XenonValueAbandon(hRight);

	if(right != 0)
	{
		// Create the output result and store it to an I/O register.
		XenonValueHandle hOutput = XenonValueCreateInt32(hVm, left / right);

		XenonExecutionSetIoRegister(hExec, hOutput, 0);
		XenonValueAbandon(hOutput);
	}
	else
	{
		// Raise the divide-by-zero script exception.
		XenonExecutionRaiseStandardException(
			hExec,
			XENON_EXCEPTION_SEVERITY_NORMAL,
			XENON_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
			"Divide-by-zero error (int32)"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpDivInt64(XenonExecutionHandle hExec, XenonFunctionHandle, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	XenonValueHandle hLeft = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	XenonValueHandle hRight = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const int64_t left = XenonValueGetInt64(hLeft);
	const int64_t right = XenonValueGetInt64(hRight);

	// Release the input parameter values now that we have their data.
	XenonValueAbandon(hLeft);
	XenonValueAbandon(hRight);

	if(right != 0)
	{
		// Create the output result and store it to an I/O register.
		XenonValueHandle hOutput = XenonValueCreateInt64(hVm, left / right);

		XenonExecutionSetIoRegister(hExec, hOutput, 0);
		XenonValueAbandon(hOutput);
	}
	else
	{
		// Raise the divide-by-zero script exception.
		XenonExecutionRaiseStandardException(
			hExec,
			XENON_EXCEPTION_SEVERITY_NORMAL,
			XENON_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
			"Divide-by-zero error (int64)"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpDivUint8(XenonExecutionHandle hExec, XenonFunctionHandle, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	XenonValueHandle hLeft = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	XenonValueHandle hRight = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const uint8_t left = XenonValueGetUint8(hLeft);
	const uint8_t right = XenonValueGetUint8(hRight);

	// Release the input parameter values now that we have their data.
	XenonValueAbandon(hLeft);
	XenonValueAbandon(hRight);

	if(right != 0)
	{
		// Create the output result and store it to an I/O register.
		XenonValueHandle hOutput = XenonValueCreateUint8(hVm, left / right);

		XenonExecutionSetIoRegister(hExec, hOutput, 0);
		XenonValueAbandon(hOutput);
	}
	else
	{
		// Raise the divide-by-zero script exception.
		XenonExecutionRaiseStandardException(
			hExec,
			XENON_EXCEPTION_SEVERITY_NORMAL,
			XENON_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
			"Divide-by-zero error (uint8)"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpDivUint16(XenonExecutionHandle hExec, XenonFunctionHandle, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	XenonValueHandle hLeft = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	XenonValueHandle hRight = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const uint16_t left = XenonValueGetUint16(hLeft);
	const uint16_t right = XenonValueGetUint16(hRight);

	// Release the input parameter values now that we have their data.
	XenonValueAbandon(hLeft);
	XenonValueAbandon(hRight);

	if(right != 0)
	{
		// Create the output result and store it to an I/O register.
		XenonValueHandle hOutput = XenonValueCreateUint16(hVm, left / right);

		XenonExecutionSetIoRegister(hExec, hOutput, 0);
		XenonValueAbandon(hOutput);
	}
	else
	{
		// Raise the divide-by-zero script exception.
		XenonExecutionRaiseStandardException(
			hExec,
			XENON_EXCEPTION_SEVERITY_NORMAL,
			XENON_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
			"Divide-by-zero error (uint16)"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpDivUint32(XenonExecutionHandle hExec, XenonFunctionHandle, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	XenonValueHandle hLeft = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	XenonValueHandle hRight = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const uint32_t left = XenonValueGetUint32(hLeft);
	const uint32_t right = XenonValueGetUint32(hRight);

	// Release the input parameter values now that we have their data.
	XenonValueAbandon(hLeft);
	XenonValueAbandon(hRight);

	if(right != 0)
	{
		// Create the output result and store it to an I/O register.
		XenonValueHandle hOutput = XenonValueCreateUint32(hVm, left / right);
		XenonExecutionSetIoRegister(hExec, hOutput, 0);
		XenonValueAbandon(hOutput);
	}
	else
	{
		// Raise the divide-by-zero script exception.
		XenonExecutionRaiseStandardException(
			hExec,
			XENON_EXCEPTION_SEVERITY_NORMAL,
			XENON_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
			"Divide-by-zero error (uint32)"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpDivUint64(XenonExecutionHandle hExec, XenonFunctionHandle, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	XenonValueHandle hLeft = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	XenonValueHandle hRight = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const uint64_t left = XenonValueGetUint64(hLeft);
	const uint64_t right = XenonValueGetUint64(hRight);

	// Release the input parameter values now that we have their data.
	XenonValueAbandon(hLeft);
	XenonValueAbandon(hRight);

	if(right != 0)
	{
		// Create the output result and store it to an I/O register.
		XenonValueHandle hOutput = XenonValueCreateUint64(hVm, left / right);

		XenonExecutionSetIoRegister(hExec, hOutput, 0);
		XenonValueAbandon(hOutput);
	}
	else
	{
		// Raise the divide-by-zero script exception.
		XenonExecutionRaiseStandardException(
			hExec,
			XENON_EXCEPTION_SEVERITY_NORMAL,
			XENON_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
			"Divide-by-zero error (uint64)"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpDivFloat32(XenonExecutionHandle hExec, XenonFunctionHandle, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	XenonValueHandle hLeft = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	XenonValueHandle hRight = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const float left = XenonValueGetFloat32(hLeft);
	const float right = XenonValueGetFloat32(hRight);

	// Release the input parameter values now that we have their data.
	XenonValueAbandon(hLeft);
	XenonValueAbandon(hRight);

	if(right != 0.0f)
	{
		// Create the output result and store it to an I/O register.
		XenonValueHandle hOutput = XenonValueCreateFloat32(hVm, left / right);
		XenonExecutionSetIoRegister(hExec, hOutput, 0);
		XenonValueAbandon(hOutput);
	}
	else
	{
		// Raise the divide-by-zero script exception.
		XenonExecutionRaiseStandardException(
			hExec,
			XENON_EXCEPTION_SEVERITY_NORMAL,
			XENON_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
			"Divide-by-zero error (float32)"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpDivFloat64(XenonExecutionHandle hExec, XenonFunctionHandle, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	XenonValueHandle hLeft = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	XenonValueHandle hRight = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const double left = XenonValueGetFloat64(hLeft);
	const double right = XenonValueGetFloat64(hRight);

	// Release the input parameter values now that we have their data.
	XenonValueAbandon(hLeft);
	XenonValueAbandon(hRight);

	if(right != 0.0)
	{
		// Create the output result and store it to an I/O register.
		XenonValueHandle hOutput = XenonValueCreateFloat64(hVm, left / right);

		XenonExecutionSetIoRegister(hExec, hOutput, 0);
		XenonValueAbandon(hOutput);
	}
	else
	{
		// Raise the divide-by-zero script exception.
		XenonExecutionRaiseStandardException(
			hExec,
			XENON_EXCEPTION_SEVERITY_NORMAL,
			XENON_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
			"Divide-by-zero error (float64)"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------
