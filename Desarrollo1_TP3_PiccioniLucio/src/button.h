#pragma once

#include "raylib.h"

#include "menus.h"

struct Button
{
	Rectangle rec = { 0,0,0,0 };
	Menus option = Menus::None;
	Color color = { 255, 182, 193, 255 };
};