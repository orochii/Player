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

#ifndef EP_SPRITE_ENEMY_H
#define EP_SPRITE_ENEMY_H

// Headers
#include "sprite_battler.h"
#include "async_handler.h"

class BattleAnimation;
class Game_Battler;
class Game_Enemy;

/**
 * Sprite_Battler class, used for battle sprites
 */
class Sprite_Enemy : public Sprite_Battler {
public:
<<<<<<< Updated upstream
	const int ANIMATED_FRAMES = 5;
	const int CYCLE_LENGTH = 10;
=======
<<<<<<< Updated upstream
=======
	const int ANIMATED_FRAMES = 5;
	const int CYCLE_LENGTH = 40;
>>>>>>> Stashed changes
	enum AnimFrame {
		AnimFrame_Idle1,
		AnimFrame_Idle2,
		AnimFrame_Action,
		AnimFrame_Damage,
		AnimFrame_Hurt,
	};
<<<<<<< Updated upstream
=======
>>>>>>> Stashed changes
>>>>>>> Stashed changes
	/**
	 * Constructor.
	 *
	 * @param battler game battler to display
	 */
	Sprite_Enemy(Game_Enemy* battler);

	~Sprite_Enemy() override;

	/**
	 * Updates sprite state.
	 */
	void Update();

	void Draw(Bitmap& dst) override;

	Game_Enemy* GetBattler() const;

	void Refresh();

	void ResetZ() final;

	void SetAction(int type, int duration = 20);

protected:
	void CreateSprite();
	void OnMonsterSpriteReady(FileRequestResult* result);
	void SetFrame(int idx);

	bool animated = false;
	int cycle = 0;
	int frameIdx = 0;
	int actionType = -1;
	int actionTime = 0;
	std::string sprite_name;
	BitmapRef graphic;
	int hue = 0;
	float zoom = 1.0;

	FileRequestBinding request_id;
};


#endif
