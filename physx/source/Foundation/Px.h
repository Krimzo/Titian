//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Copyright (c) 2008-2021 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.

#ifndef PXFOUNDATION_PX_H
#define PXFOUNDATION_PX_H

/** \addtogroup foundation
@{
*/

#include "foundation/PxSimpleTypes.h"

/** files to always include */
#include <string.h>
#include <stdlib.h>

#if !PX_DOXYGEN
namespace physx {
#endif

    typedef uint32_t PxU32;

    class PxAllocatorCallback;
    class PxErrorCallback;
    struct PxErrorCode;
    class PxAssertHandler;

    class PxInputStream;
    class PxInputData;
    class PxOutputStream;

    class PxVec2;
    class PxVec3;
    class PxVec4;
    class PxMat33;
    class PxMat44;
    class PxPlane;
    class PxQuat;
    class PxTransform;
    class PxBounds3;

    /** enum for empty constructor tag*/
    enum PxEMPTY
    {
        PxEmpty
    };

    /** enum for zero constructor tag for vectors and matrices */
    enum PxZERO
    {
        PxZero
    };

    /** enum for identity constructor flag for quaternions, transforms, and matrices */
    enum PxIDENTITY
    {
        PxIdentity
    };

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif // #ifndef PXFOUNDATION_PX_H
