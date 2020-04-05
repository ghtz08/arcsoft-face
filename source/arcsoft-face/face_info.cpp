#include "arcsoft-face/face_info.h"

#pragma warning(push, 0)

#include <arcsoft_face_sdk.h>

#pragma warning(pop)

namespace tz::ai::arcsoft
{

FaceInfo::FaceInfo(
    Rect const & rect,
    Direction    dire,
    TrackID      face_id
): rect_(rect), dire_(dire), track_id_(face_id)
{
    static_assert(ASF_OC_0 == 1);
}

}   // namespace tz::ai::arcsoft
