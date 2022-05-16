#include "assert_impl.h"

namespace rml {
    namespace internal {

        // TODO: consider extension for formatted error description string
        static void assertion_failure_impl(const char* location, int line, const char* expression, const char* comment) {

            std::fprintf(stderr, "Assertion %s failed (located in the %s function, line in file: %d)\n",
                expression, location, line);

            if (comment) {
                std::fprintf(stderr, "Detailed description: %s\n", comment);
            }
#if _MSC_VER && _DEBUG
            if (1 == _CrtDbgReport(_CRT_ASSERT, location, line, "tbb_debug.dll", "%s\r\n%s", expression, comment ? comment : "")) {
                _CrtDbgBreak();
            }
            else
#endif
            {
                std::fflush(stderr);
                std::abort();
            }
        }

        // Do not move the definition into the assertion_failure function because it will require "magic statics".
        // It will bring a dependency on C++ runtime on some platforms while assert_impl.h is reused in tbbmalloc 
        // that should not depend on C++ runtime
        static std::atomic<tbb::detail::do_once_state> assertion_state;

        void __TBB_EXPORTED_FUNC assertion_failure(const char* location, int line, const char* expression, const char* comment) {
#if __TBB_MSVC_UNREACHABLE_CODE_IGNORED
            // Workaround for erroneous "unreachable code" during assertion throwing using call_once
#pragma warning (push)
#pragma warning (disable: 4702)
#endif
    // We cannot use std::call_once because it brings a dependency on C++ runtime on some platforms 
    // while assert_impl.h is reused in tbbmalloc that should not depend on C++ runtime
            atomic_do_once([&]() { assertion_failure_impl(location, line, expression, comment); }, assertion_state);
#if __TBB_MSVC_UNREACHABLE_CODE_IGNORED
#pragma warning (pop)
#endif
        }

        //! Report a runtime warning.
        void runtime_warning(const char* format, ...) {
            char str[1024]; std::memset(str, 0, 1024);
            va_list args; va_start(args, format);
            vsnprintf(str, 1024 - 1, format, args);
            va_end(args);
            fprintf(stderr, "TBB Warning: %s\n", str);
        }

    }
} // namespaces rml::internal

namespace tbb {
    namespace detail {
        namespace r1 {
            // TODO: consider extension for formatted error description string
            static void assertion_failure_impl(const char* location, int line, const char* expression, const char* comment) {

                std::fprintf(stderr, "Assertion %s failed (located in the %s function, line in file: %d)\n",
                    expression, location, line);

                if (comment) {
                    std::fprintf(stderr, "Detailed description: %s\n", comment);
                }
#if _MSC_VER && _DEBUG
                if (1 == _CrtDbgReport(_CRT_ASSERT, location, line, "tbb_debug.dll", "%s\r\n%s", expression, comment ? comment : "")) {
                    _CrtDbgBreak();
                }
                else
#endif
                {
                    std::fflush(stderr);
                    std::abort();
                }
            }

            // Do not move the definition into the assertion_failure function because it will require "magic statics".
            // It will bring a dependency on C++ runtime on some platforms while assert_impl.h is reused in tbbmalloc 
            // that should not depend on C++ runtime
            static std::atomic<tbb::detail::do_once_state> assertion_state;

            void __TBB_EXPORTED_FUNC assertion_failure(const char* location, int line, const char* expression, const char* comment) {
#if __TBB_MSVC_UNREACHABLE_CODE_IGNORED
                // Workaround for erroneous "unreachable code" during assertion throwing using call_once
#pragma warning (push)
#pragma warning (disable: 4702)
#endif
    // We cannot use std::call_once because it brings a dependency on C++ runtime on some platforms 
    // while assert_impl.h is reused in tbbmalloc that should not depend on C++ runtime
                atomic_do_once([&]() { assertion_failure_impl(location, line, expression, comment); }, assertion_state);
#if __TBB_MSVC_UNREACHABLE_CODE_IGNORED
#pragma warning (pop)
#endif
            }

            //! Report a runtime warning.
            void runtime_warning(const char* format, ...) {
                char str[1024]; std::memset(str, 0, 1024);
                va_list args; va_start(args, format);
                vsnprintf(str, 1024 - 1, format, args);
                va_end(args);
                fprintf(stderr, "TBB Warning: %s\n", str);
            }
        }
    }
}