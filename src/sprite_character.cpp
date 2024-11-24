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
#include "sprite_character.h"
#include "cache.h"
#include "game_map.h"
#include "bitmap.h"
#include "output.h"
#include "player.h"

Sprite_Character::Sprite_Character(Game_Character* character, int x_offset, int y_offset) :
	character(character),
	tile_id(-1),
	character_index(0),
	chara_width(0),
	chara_height(0),
	x_offset(x_offset),
	y_offset(y_offset) {

	Update();
}

void Sprite_Character::Draw(Bitmap &dst) {
	if (UsesCharset()) {
		int row = character->GetFacing();
		auto frame = character->GetAnimFrame();
		if (frame >= lcf::rpg::EventPage::Frame_middle2) frame = lcf::rpg::EventPage::Frame_middle;
		SetSrcRect({frame * chara_width, row * chara_height, chara_width, chara_height});
	}

	SetFlashEffect(character->GetFlashColor());

	SetOpacity(character->GetOpacity());

	bool mode7 = Game_Map::GetIsMode7();
	if (!mode7) {
		SetX(character->GetScreenX() + x_offset);
		SetY(character->GetScreenY() + y_offset);
	}
	else {
		int originalX = character->GetScreenX();
		int originalY = character->GetScreenY(false);
		int originalYOff = character->GetScreenY(true);
		int originalOX = x_offset;
		int originalOY = y_offset + (originalYOff-originalY);
		// Get map properties.
		const int center_x = Player::screen_width / 2 - 8;
		const int center_y = Player::screen_height / 2 + 8;
		int yaw = Game_Map::GetMode7Yaw();
		int slant = Game_Map::GetMode7Slant();
		int horizon = Game_Map::GetMode7Horizon();
		int baseline = center_y + Game_Map::GetMode7Baseline();
		double scale = Game_Map::GetMode7Scale();
		// Rotate.
		double angle = (yaw * (2 * M_PI) / 360);
		int xx = originalX - center_x;
		int yy = originalY - center_y;
		double cosA = cos(-angle);
		double sinA = sin(-angle);
		int rotatedX = (cosA * xx) + (sinA * yy);
		int rotatedY = (cosA * yy) - (sinA * xx);
		// Transform
		double iConst = 1 + (slant / (baseline + horizon));
		double distanceBase = slant * scale / (baseline + horizon);
		double syBase = distanceBase * 2;
		double distance = (syBase - rotatedY) / 2;
		double zoom = (iConst - (distance / scale)) * 2.0;
		int sy = ((slant * scale) / distance) - horizon - 120 - 4;
		int sx = rotatedX * zoom;
		// Set
		SetX(center_x + sx + originalOX * zoom);
		SetY(center_y + sy + originalOY * zoom);
		SetZoomX(zoom);
		SetZoomY(zoom);
	}

	int bush_split = 4 - character->GetBushDepth();
	SetBushDepth(bush_split > 3 ? 0 : GetHeight() / bush_split);

	Sprite::Draw(dst);
}

void Sprite_Character::Update() {
	if (tile_id != character->GetTileId() ||
		character_name != character->GetSpriteName() ||
		character_index != character->GetSpriteIndex() ||
		refresh_bitmap
	) {
		tile_id = character->GetTileId();
		character_name = character->GetSpriteName();
		character_index = character->GetSpriteIndex();
		refresh_bitmap = false;

		if (UsesCharset()) {
			FileRequestAsync* char_request = AsyncHandler::RequestFile("CharSet", character_name);
			char_request->SetGraphicFile(true);
			request_id = char_request->Bind(&Sprite_Character::OnCharSpriteReady, this);
			char_request->Start();
		} else {
			const auto chipset_name = Game_Map::GetChipsetName();
			if (chipset_name.empty()) {
				OnTileSpriteReady(nullptr);
			} else {
				FileRequestAsync *tile_request = AsyncHandler::RequestFile("ChipSet", Game_Map::GetChipsetName());
				tile_request->SetGraphicFile(true);
				request_id = tile_request->Bind(&Sprite_Character::OnTileSpriteReady, this);
				tile_request->Start();
			}
		}
	}

	SetVisible(character->IsVisible());
	SetZ(character->GetScreenZ(x_offset, y_offset));
}

Game_Character* Sprite_Character::GetCharacter() {
	return character;
}

void Sprite_Character::SetCharacter(Game_Character* new_character) {
	character = new_character;
}

bool Sprite_Character::UsesCharset() const {
	return !character_name.empty();
}

void Sprite_Character::OnTileSpriteReady(FileRequestResult*) {
	const auto chipset = Game_Map::GetChipsetName();

	BitmapRef tile;
	if (!chipset.empty()) {
		tile = Cache::Tile(Game_Map::GetChipsetName(), tile_id);
	}
	else {
		tile = Bitmap::Create(16, 16, true);
	}

	SetBitmap(tile);

	SetSrcRect({ 0, 0, TILE_SIZE, TILE_SIZE });
	SetOx(8);
	SetOy(16);

	Update();
}

void Sprite_Character::ChipsetUpdated() {
	if (UsesCharset()) {
		return;
	}
	refresh_bitmap = true;
}

Rect Sprite_Character::GetCharacterRect(StringView name, int index, const Rect bitmap_rect) {
	Rect rect;
	rect.width = 24 * (TILE_SIZE / 16) * 3;
	rect.height = 32 * (TILE_SIZE / 16) * 4;

	// Allow large 4x2 spriteset of 3x4 sprites
	// when the character name starts with a $ sign.
	// This is not exactly the VX Ace way because
	// VX Ace uses a single 1x1 spriteset of 3x4 sprites.
	if (!name.empty() && name.front() == '$') {
		if (!Player::HasEasyRpgExtensions()) {
			Output::Debug("Ignoring large charset {}. EasyRPG Extension not enabled.", name);
		} else {
			rect.width = bitmap_rect.width * (TILE_SIZE / 16) / 4;
			rect.height = bitmap_rect.height * (TILE_SIZE / 16) / 2;
		}
	}
	rect.x = (index % 4) * rect.width;
	rect.y = (index / 4) * rect.height;
	return rect;
}

void Sprite_Character::OnCharSpriteReady(FileRequestResult*) {
	SetBitmap(Cache::Charset(character_name));
	auto rect = GetCharacterRect(character_name, character_index, GetBitmap()->GetRect());
	chara_width = rect.width / 3;
	chara_height = rect.height / 4;
	SetOx(chara_width / 2);
	SetOy(chara_height);
	SetSpriteRect(rect);

	Update();
}
