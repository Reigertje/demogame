#ifndef TANUKIAGENT_H
#define TANUKIAGENT_H

#include "enemyagent.h"

#include "game/combat/weapon.h"

class TanukiAgent : public EnemyAgent {

	public:

		TanukiAgent();

		void onStep(float dt);

	private:

		static const int NORMAL = 0;
		static const int RUNNING = 1;
		static const int CORNERED = 2;
		static const int START_ATTACK = 3;
		static const int ATTACKING = 4;

		int state_;

		Attack attack_;

		float relax_time_;

};

#endif