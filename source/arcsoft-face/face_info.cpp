#include "arcsoft-face/face_info.h"

namespace tz::ai::arcsoft
{

FaceInfo::FaceInfo(
    FaceID face_id,
    Rect const & rect,
    Direction dire
): face_id_(face_id), rect_(rect), dire_(dire)
{
}

}   // namespace tz::ai::arcsoft
