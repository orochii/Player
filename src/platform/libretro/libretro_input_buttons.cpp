/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

// Headers
#include "input_buttons.h"
#include "keys.h"
#include "libretro.h"

Input::ButtonMappingArray Input::GetDefaultButtonMappings() {
	// Keyboard not mapped because libretro expects that all input goes
	// through Retropad
	return {
#if defined(USE_JOYSTICK) && defined(SUPPORT_JOYSTICK)
		{UP, Keys::JOY_DPAD_UP},
		{DOWN, Keys::JOY_DPAD_DOWN},
		{LEFT, Keys::JOY_DPAD_LEFT},
		{RIGHT, Keys::JOY_DPAD_RIGHT},
		{DECISION, Keys::JOY_A},
		{CANCEL, Keys::JOY_B},
		{SHIFT, Keys::JOY_X},
		{FAST_FORWARD, Keys::JOY_Y},
		{N0, Keys::JOY_STICK_LEFT},
		{N5, Keys::JOY_STICK_RIGHT},
		{TOGGLE_FPS, Keys::JOY_SHOULDER_LEFT},
		{DEBUG_THROUGH, Keys::JOY_SHOULDER_RIGHT},
		{SETTINGS_MENU, Keys::JOY_START},
		{RESET, Keys::JOY_BACK},
#endif

#if defined(USE_JOYSTICK_AXIS) && defined(SUPPORT_JOYSTICK_AXIS)
		{LEFT, Keys::JOY_STICK_LEFT_X_LEFT},
		{RIGHT, Keys::JOY_STICK_LEFT_X_RIGHT},
		{DOWN, Keys::JOY_STICK_LEFT_Y_DOWN},
		{UP, Keys::JOY_STICK_LEFT_Y_UP},
		{N1, Keys::JOY_STICK_RIGHT_X_LEFT},
		{N3, Keys::JOY_STICK_RIGHT_Y_DOWN},
		{N7, Keys::JOY_STICK_RIGHT_Y_UP},
		{N9, Keys::JOY_STICK_RIGHT_X_RIGHT},
		{FAST_FORWARD_PLUS, Keys::JOY_TRIGGER_RIGHT},
		{DEBUG_MENU, Keys::JOY_TRIGGER_LEFT}
#endif
	};
}

Input::DirectionMappingArray Input::GetDefaultDirectionMappings() {
	return {
		{ Direction::DOWN, DOWN },
		{ Direction::LEFT, LEFT },
		{ Direction::RIGHT, RIGHT },
		{ Direction::UP, UP },
	};
}
