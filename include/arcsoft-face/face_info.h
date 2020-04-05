#pragma once

#include <cstdint>

#include "rect.h"

namespace tz::ai::arcsoft
{

class FaceInfo
{
public:
    using TrackID = uint32_t;
    using Direction = int;
public:
    FaceInfo(TrackID face_id, Rect const & rect, Direction dire);
public:
#pragma warning(push)
#pragma warning(disable: 4514)	// 未引用的内联函数已移除
    
    auto trackID() const noexcept -> TrackID { return track_id_; }
    auto trackID(TrackID face_id) noexcept -> void { track_id_ = face_id; }
    auto rect() const noexcept -> Rect const & { return rect_; }
    auto rect(Rect const & rect) noexcept -> void { rect_ = rect; }
    auto direction() const noexcept -> Direction { return dire_; }
    auto direction(Direction dire) noexcept -> void { dire_ = dire; }

#pragma warning(pop)
private:
    TrackID    track_id_;
    Rect       rect_   ;
    Direction  dire_   ; // 人脸在图片中大致方向
};

}   // namespace tz::ai::arcsoft