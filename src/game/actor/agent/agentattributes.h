#ifndef AGENTATTRIBUTES_H
#define AGENTATTRIBUTES_H

class AgentAttributes {

public:

	AgentAttributes() : regeneration_(0), vitality_(0), endurance_(0), recovery_(0), spirituality_(0), willpower_(0) {};

	int getRegeneration() const;

	void setRegeneration(int regeneration);

	int getVitality() const;

	void setVitality(int vitality);

	int getEndurance() const;

	void setEndurance(int endurance);

	int getRecovery() const;

	void setRecovery(int recovery);

	int getSpirituality() const;

	void setSpirituality(int spirituality);

	int getWillpower() const;

	void setWillpower(int willpower);

private:

	int regeneration_;

	int vitality_;

	int endurance_;

	int recovery_;

	int spirituality_;

	int willpower_;

};

#endif