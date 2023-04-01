#pragma once

#include "jet_speed.h"
#include "visuals.h"
#include "autopilot.h"
#include "misc.h"

namespace features {

	inline void initialize() {
		features::autopilot::initialize();
	}

	inline void on_pre_frame(float deltaTime) {
		features::jet_speed::preframe();
		features::autopilot::preframe(deltaTime);
		features::misc::preframe();
	}

	inline void on_render() {
		features::visuals::on_render();
		features::autopilot::on_render();
	}
}

