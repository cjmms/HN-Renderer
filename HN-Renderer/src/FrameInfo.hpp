#pragma once
#include "pch.h"
#include "Camera.hpp"

// record all specific frame information into this struct
// including:
// frame index
// frame time
// command buffer
// camera status

namespace HN
{
	struct FrameInfo
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer cmdBuffer;
		Camera& camera;
	};



}