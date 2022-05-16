/*
    Copyright (c) 2005-2021 Intel Corporation

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef __TBB_assert_impl_H
#define __TBB_assert_impl_H

#include "oneapi/tbb/detail/_config.h"
#include "oneapi/tbb/detail/_utils.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#if _MSC_VER && _DEBUG
#include <crtdbg.h>
#endif

#include <mutex>

#if __TBBMALLOC_BUILD
namespace rml { namespace internal {

#else
namespace tbb {
namespace detail {
namespace r1 {
#endif

void __TBB_EXPORTED_FUNC assertion_failure(const char* location, int line, const char* expression, const char* comment);

//! Report a runtime warning.
void runtime_warning(const char* format, ...);

#if __TBBMALLOC_BUILD
}} // namespaces rml::internal
#else
} // namespace r1
} // namespace detail
} // namespace tbb
#endif

#endif // __TBB_assert_impl_H

