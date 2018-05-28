#ifndef AGENT_H
#define AGENT_H

#include "../actor.h"
#include "agentattributes.h"
#include "agentstatus.h"

class BodyDrawObject;
class BodyPartDrawObject;

class Agent : public Actor {

public:

	static const bool LEFT = true;
	static const bool RIGHT = false;

	enum Group {
		UNDEFINED,
		HUMAN,
		TANUKI
	};

	Agent(Group group = UNDEFINED);

	Agent(const AgentAttributes& base_attributes, Group group = UNDEFINED);

	virtual ~Agent();

	virtual void onStep(float dt);

	Group getGroup() const;

	float getSpeed() const;

	float getJump() const;

	bool isMoving() const;

	bool getOrientation() const;

	int getState() const;

	virtual DefenceProperties getDefenceProperties() const;

	virtual AgentAttributes getAttributes() const;

	const AgentAttributes& getBaseAttributes() const;

	const AgentStatus& getStatus() const;

protected:

	void setGroup(Group group);

	void setBaseAttributes(const AgentAttributes& base_attributes);

	void setStatus(const AgentStatus& status);

	void setState(int state);

	void setSpeed(float speed);

	void setJump(float jump);

	void setOrientation(bool orientation);

	void setBodyDrawObject(const std::string& key);

	void setAnimation(const std::string& animation);

	void transitionAnimation(const std::string& animation);

	void addBodyPart(BodyPartDrawObject* bodypart);

	void removeBodyPart(BodyPartDrawObject* bodypart);

	void addBodyPart(const std::string& key);

	bool jump();

	void haltJump();

	void moveLeft();

	void moveRight();

	void standStill();

	BodyDrawObject* getBodyDrawObject();

	virtual bool canJump();

	virtual void onAddToScene();

	virtual void onRemoveFromScene();

private:

	float speed_;

	float jump_;

	bool moving_;

	bool orientation_;

	bool blocking_;

	int state_;

	Group group_;

	AgentStatus status_;

	AgentAttributes base_attributes_;

	BodyDrawObject* body_draw_object_;

	void handleAttackHit(const AttackHit& attack_hit);

};

#endif