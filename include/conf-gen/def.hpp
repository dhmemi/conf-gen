
#pragma once

#ifdef __cpp_attributes
#define CFG_NO_DISCARD [[nodiscard]]
#else
#define NO_DISCARD
#endif

#ifdef __NO_INLINE__
#define CFG_INLINE
#else
#define CFG_INLINE inline
#endif
