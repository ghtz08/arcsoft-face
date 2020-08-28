#include "arcsoft-face/image_ref.h"

#pragma warning(push, 0)

#include <type_traits>

#include <asvloffscreen.h>

#pragma warning(pop)

namespace
{

using Format = tz::ai::arcsoft::ImageRef::Format;

#define FORMAT_ASSERT(a, b) \
    static_assert(static_cast<std::underlying_type_t<Format>>(Format::a) == b)

FORMAT_ASSERT(B8G8R8, ASVL_PAF_RGB24_B8G8R8);
FORMAT_ASSERT(Y4U1V1, ASVL_PAF_I420);

#undef FORMAT_ASSERT

}   // namespace

namespace tz::ai::arcsoft
{

}   // namespace tz::ai::arcsoft
