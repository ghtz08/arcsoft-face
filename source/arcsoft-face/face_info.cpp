#include "arcsoft-face/face_info.h"

namespace tz::ai::arcsoft
{

FaceInfo::FaceInfo(
    TrackID face_id,
    Rect const & rect,
    Direction dire
): track_id_(face_id), rect_(rect), dire_(dire)
{
}

}   // namespace tz::ai::arcsoft
