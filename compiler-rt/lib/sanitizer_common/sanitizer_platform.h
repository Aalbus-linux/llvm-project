//===-- sanitizer_platform.h ------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Common platform macros.
//===----------------------------------------------------------------------===//

#ifndef SANITIZER_PLATFORM_H
#define SANITIZER_PLATFORM_H

#if !defined(__linux__) && !defined(__FreeBSD__) && !defined(__NetBSD__) && \
  !defined(__APPLE__) && !defined(_WIN32) && \
  !defined(__Fuchsia__) && !defined(__rtems__) && \
  !(defined(__sun__) && defined(__svr4__))
# error "This operating system is not supported"
#endif

#if defined(__linux__)
# define SANITIZER_LINUX   1
#else
# define SANITIZER_LINUX   0
#endif

#if defined(__FreeBSD__)
# define SANITIZER_FREEBSD 1
#else
# define SANITIZER_FREEBSD 0
#endif

#if defined(__NetBSD__)
# define SANITIZER_NETBSD 1
#else
# define SANITIZER_NETBSD 0
#endif

#if defined(__sun__) && defined(__svr4__)
# define SANITIZER_SOLARIS 1
#else
# define SANITIZER_SOLARIS 0
#endif

#if defined(__APPLE__)
# define SANITIZER_MAC     1
# include <TargetConditionals.h>
# if TARGET_OS_IPHONE
#  define SANITIZER_IOS    1
# else
#  define SANITIZER_IOS    0
# endif
# if TARGET_OS_SIMULATOR
#  define SANITIZER_IOSSIM 1
# else
#  define SANITIZER_IOSSIM 0
# endif
#else
# define SANITIZER_MAC     0
# define SANITIZER_IOS     0
# define SANITIZER_IOSSIM  0
#endif

#if defined(__APPLE__) && TARGET_OS_IPHONE && TARGET_OS_WATCH
# define SANITIZER_WATCHOS 1
#else
# define SANITIZER_WATCHOS 0
#endif

#if defined(__APPLE__) && TARGET_OS_IPHONE && TARGET_OS_TV
# define SANITIZER_TVOS 1
#else
# define SANITIZER_TVOS 0
#endif

#if defined(_WIN32)
# define SANITIZER_WINDOWS 1
#else
# define SANITIZER_WINDOWS 0
#endif

#if defined(_WIN64)
# define SANITIZER_WINDOWS64 1
#else
# define SANITIZER_WINDOWS64 0
#endif

#if defined(__ANDROID__)
# define SANITIZER_ANDROID 1
#else
# define SANITIZER_ANDROID 0
#endif

#if defined(__Fuchsia__)
# define SANITIZER_FUCHSIA 1
#else
# define SANITIZER_FUCHSIA 0
#endif

#if defined(__rtems__)
# define SANITIZER_RTEMS 1
#else
# define SANITIZER_RTEMS 0
#endif

#define SANITIZER_POSIX \
  (SANITIZER_FREEBSD || SANITIZER_LINUX || SANITIZER_MAC || \
    SANITIZER_NETBSD || SANITIZER_SOLARIS)

#if __LP64__ || defined(_WIN64)
#  define SANITIZER_WORDSIZE 64
#else
#  define SANITIZER_WORDSIZE 32
#endif

#if SANITIZER_WORDSIZE == 64
# define FIRST_32_SECOND_64(a, b) (b)
#else
# define FIRST_32_SECOND_64(a, b) (a)
#endif

#if defined(__x86_64__) && !defined(_LP64)
# define SANITIZER_X32 1
#else
# define SANITIZER_X32 0
#endif

#if defined(__i386__) || defined(_M_IX86)
# define SANITIZER_I386 1
#else
# define SANITIZER_I386 0
#endif

#if defined(__mips__)
# define SANITIZER_MIPS 1
# if defined(__mips64)
#  define SANITIZER_MIPS32 0
#  define SANITIZER_MIPS64 1
# else
#  define SANITIZER_MIPS32 1
#  define SANITIZER_MIPS64 0
# endif
#else
# define SANITIZER_MIPS 0
# define SANITIZER_MIPS32 0
# define SANITIZER_MIPS64 0
#endif

#if defined(__s390__)
# define SANITIZER_S390 1
# if defined(__s390x__)
#  define SANITIZER_S390_31 0
#  define SANITIZER_S390_64 1
# else
#  define SANITIZER_S390_31 1
#  define SANITIZER_S390_64 0
# endif
#else
# define SANITIZER_S390 0
# define SANITIZER_S390_31 0
# define SANITIZER_S390_64 0
#endif

#if defined(__powerpc__)
# define SANITIZER_PPC 1
# if defined(__powerpc64__)
#  define SANITIZER_PPC32 0
#  define SANITIZER_PPC64 1
// 64-bit PPC has two ABIs (v1 and v2).  The old powerpc64 target is
// big-endian, and uses v1 ABI (known for its function descriptors),
// while the new powerpc64le target is little-endian and uses v2.
// In theory, you could convince gcc to compile for their evil twins
// (eg. big-endian v2), but you won't find such combinations in the wild
// (it'd require bootstrapping a whole system, which would be quite painful
// - there's no target triple for that).  LLVM doesn't support them either.
#  if _CALL_ELF == 2
#   define SANITIZER_PPC64V1 0
#   define SANITIZER_PPC64V2 1
#  else
#   define SANITIZER_PPC64V1 1
#   define SANITIZER_PPC64V2 0
#  endif
# else
#  define SANITIZER_PPC32 1
#  define SANITIZER_PPC64 0
#  define SANITIZER_PPC64V1 0
#  define SANITIZER_PPC64V2 0
# endif
#else
# define SANITIZER_PPC 0
# define SANITIZER_PPC32 0
# define SANITIZER_PPC64 0
# define SANITIZER_PPC64V1 0
# define SANITIZER_PPC64V2 0
#endif

#if defined(__arm__)
# define SANITIZER_ARM 1
#else
# define SANITIZER_ARM 0
#endif

#if SANITIZER_SOLARIS && SANITIZER_WORDSIZE == 32
# define SANITIZER_SOLARIS32 1
#else
# define SANITIZER_SOLARIS32 0
#endif

#if defined(__myriad2__)
# define SANITIZER_MYRIAD2 1
#else
# define SANITIZER_MYRIAD2 0
#endif

#if defined(__riscv) && (__riscv_xlen == 64)
#define SANITIZER_RISCV64 1
#else
#define SANITIZER_RISCV64 0
#endif


#if defined(__linux__) && !defined(__GLIBC__)
# define SANITIZER_NONGNU 1
#else
# define SANITIZER_NONGNU 0
#endif

// By default we allow to use SizeClassAllocator64 on 64-bit platform.
// But in some cases (e.g. AArch64's 39-bit address space) SizeClassAllocator64
// does not work well and we need to fallback to SizeClassAllocator32.
// For such platforms build this code with -DSANITIZER_CAN_USE_ALLOCATOR64=0 or
// change the definition of SANITIZER_CAN_USE_ALLOCATOR64 here.
#ifndef SANITIZER_CAN_USE_ALLOCATOR64
# if (SANITIZER_ANDROID && defined(__aarch64__)) || SANITIZER_FUCHSIA
#  define SANITIZER_CAN_USE_ALLOCATOR64 1
# elif defined(__mips64) || defined(__aarch64__)
#  define SANITIZER_CAN_USE_ALLOCATOR64 0
# else
#  define SANITIZER_CAN_USE_ALLOCATOR64 (SANITIZER_WORDSIZE == 64)
# endif
#endif

// The range of addresses which can be returned my mmap.
// FIXME: this value should be different on different platforms.  Larger values
// will still work but will consume more memory for TwoLevelByteMap.
#if defined(__mips__)
#if SANITIZER_GO && defined(__mips64)
#define SANITIZER_MMAP_RANGE_SIZE FIRST_32_SECOND_64(1ULL << 32, 1ULL << 47)
#else
# define SANITIZER_MMAP_RANGE_SIZE FIRST_32_SECOND_64(1ULL << 32, 1ULL << 40)
#endif
#elif SANITIZER_RISCV64
#define SANITIZER_MMAP_RANGE_SIZE FIRST_32_SECOND_64(1ULL << 32, 1ULL << 38)
#elif defined(__aarch64__)
# if SANITIZER_MAC
// Darwin iOS/ARM64 has a 36-bit VMA, 64GiB VM
#  define SANITIZER_MMAP_RANGE_SIZE FIRST_32_SECOND_64(1ULL << 32, 1ULL << 36)
# else
#  define SANITIZER_MMAP_RANGE_SIZE FIRST_32_SECOND_64(1ULL << 32, 1ULL << 48)
# endif
#elif defined(__sparc__)
#define SANITIZER_MMAP_RANGE_SIZE FIRST_32_SECOND_64(1ULL << 32, 1ULL << 52)
#else
# define SANITIZER_MMAP_RANGE_SIZE FIRST_32_SECOND_64(1ULL << 32, 1ULL << 47)
#endif

// Whether the addresses are sign-extended from the VMA range to the word.
// The SPARC64 Linux port implements this to split the VMA space into two
// non-contiguous halves with a huge hole in the middle.
#if defined(__sparc__) && SANITIZER_WORDSIZE == 64
#define SANITIZER_SIGN_EXTENDED_ADDRESSES 1
#else
#define SANITIZER_SIGN_EXTENDED_ADDRESSES 0
#endif

// The AArch64 and RISC-V linux ports use the canonical syscall set as
// mandated by the upstream linux community for all new ports. Other ports
// may still use legacy syscalls.
#ifndef SANITIZER_USES_CANONICAL_LINUX_SYSCALLS
# if (defined(__aarch64__) || defined(__riscv)) && SANITIZER_LINUX
# define SANITIZER_USES_CANONICAL_LINUX_SYSCALLS 1
# else
# define SANITIZER_USES_CANONICAL_LINUX_SYSCALLS 0
# endif
#endif

// udi16 syscalls can only be used when the following conditions are
// met:
// * target is one of arm32, x86-32, sparc32, sh or m68k
// * libc version is libc5, glibc-2.0, glibc-2.1 or glibc-2.2 to 2.15
//   built against > linux-2.2 kernel headers
// Since we don't want to include libc headers here, we check the
// target only.
#if defined(__arm__) || SANITIZER_X32 || defined(__sparc__)
#define SANITIZER_USES_UID16_SYSCALLS 1
#else
#define SANITIZER_USES_UID16_SYSCALLS 0
#endif

#if defined(__mips__)
# define SANITIZER_POINTER_FORMAT_LENGTH FIRST_32_SECOND_64(8, 10)
#else
# define SANITIZER_POINTER_FORMAT_LENGTH FIRST_32_SECOND_64(8, 12)
#endif

/// \macro MSC_PREREQ
/// \brief Is the compiler MSVC of at least the specified version?
/// The common \param version values to check for are:
///  * 1800: Microsoft Visual Studio 2013 / 12.0
///  * 1900: Microsoft Visual Studio 2015 / 14.0
#ifdef _MSC_VER
# define MSC_PREREQ(version) (_MSC_VER >= (version))
#else
# define MSC_PREREQ(version) 0
#endif

#if SANITIZER_MAC && !(defined(__arm64__) && SANITIZER_IOS)
# define SANITIZER_NON_UNIQUE_TYPEINFO 0
#else
# define SANITIZER_NON_UNIQUE_TYPEINFO 1
#endif

// On linux, some architectures had an ABI transition from 64-bit long double
// (ie. same as double) to 128-bit long double.  On those, glibc symbols
// involving long doubles come in two versions, and we need to pass the
// correct one to dlvsym when intercepting them.
#if SANITIZER_LINUX && (SANITIZER_S390 || SANITIZER_PPC32 || SANITIZER_PPC64V1)
#define SANITIZER_NLDBL_VERSION "GLIBC_2.4"
#endif

#if SANITIZER_GO == 0
# define SANITIZER_GO 0
#endif

// On PowerPC and ARM Thumb, calling pthread_exit() causes LSan to detect leaks.
// pthread_exit() performs unwinding that leads to dlopen'ing libgcc_s.so.
// dlopen mallocs "libgcc_s.so" string which confuses LSan, it fails to realize
// that this allocation happens in dynamic linker and should be ignored.
#if SANITIZER_PPC || defined(__thumb__)
# define SANITIZER_SUPPRESS_LEAK_ON_PTHREAD_EXIT 1
#else
# define SANITIZER_SUPPRESS_LEAK_ON_PTHREAD_EXIT 0
#endif

#if SANITIZER_FREEBSD || SANITIZER_MAC || SANITIZER_NETBSD || \
  SANITIZER_SOLARIS
# define SANITIZER_MADVISE_DONTNEED MADV_FREE
#else
# define SANITIZER_MADVISE_DONTNEED MADV_DONTNEED
#endif

// Older gcc have issues aligning to a constexpr, and require an integer.
// See https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56859 among others.
#if defined(__powerpc__) || defined(__powerpc64__)
# define SANITIZER_CACHE_LINE_SIZE 128
#else
# define SANITIZER_CACHE_LINE_SIZE 64
#endif

// Enable offline markup symbolizer for Fuchsia and RTEMS.
#if SANITIZER_FUCHSIA || SANITIZER_RTEMS
#define SANITIZER_SYMBOLIZER_MARKUP 1
#else
#define SANITIZER_SYMBOLIZER_MARKUP 0
#endif

// Enable ability to support sanitizer initialization that is
// compatible with the sanitizer library being loaded via
// `dlopen()`.
#if SANITIZER_MAC
#define SANITIZER_SUPPORTS_INIT_FOR_DLOPEN 1
#else
#define SANITIZER_SUPPORTS_INIT_FOR_DLOPEN 0
#endif

#endif // SANITIZER_PLATFORM_H
