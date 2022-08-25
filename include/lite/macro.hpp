#pragma once
#ifndef NULLPTR
#  if __cplusplus >= 201103L
#    define NULLPTR nullptr
#  else
#    define NULLPTR NULL
#  endif
#endif

#ifndef NOEXCEPT
#  if __cplusplus >= 201103L
#    define NOEXCEPT noexcept
#  else
#    define NOEXCEPT
#  endif
#endif

#ifndef CONSTEXPR
#  if __cplusplus >= 201103L
#    define CONSTEXPR constexpr
#  else
#    define CONSTEXPR inline
#  endif
#endif

#ifndef NODISCARD
#  if __cplusplus >= 201703L
#    define NODISCARD [[nodiscard]]
#  else
#    define NODISCARD
#  endif
#endif
