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

#include "GcProxy.hpp"
#include "GarbageCollector.hpp"

#include "../common/Atomic.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

void XenonGcProxy::Initialize(
	XenonGcProxy& output,
	XenonGarbageCollector& gc,
	XenonGcDiscoveryCallback onGcDiscoveryFn,
	XenonDisposeCallback onGcDisposeFn,
	void* const pObject,
	const bool autoMark
)
{
	assert(onGcDiscoveryFn != nullptr);
	assert(onGcDisposeFn != nullptr);
	assert(pObject != nullptr);

	output.onGcDiscoveryFn = onGcDiscoveryFn;
	output.onGcDisposeFn = onGcDisposeFn;
	output.pGc = &gc;
	output.pPrev = nullptr;
	output.pNext = nullptr;
	output.pObject = pObject;
	output.pending = false;
	output.marked = false;
	output.autoMark = autoMark;

	XenonGarbageCollector::LinkObject(gc, &output);
}

//----------------------------------------------------------------------------------------------------------------------
