#ifndef AGENTSTATUS
#define AGENTSTATUS

#include "agentattributes.h"

class AgentStatus {

public:

	AgentStatus() : hp_(0), ap_(0), mp_(0) {};

	static int getStatusMaxForAttribute(int attribute);

	static float getStatusRestorionForAttribute(int attribute);

	float getHp() const;

	void setHp(float hp);

	float getAp() const;

	void setAp(float ap);

	float getMp() const;

	void setMp(float mp);

	void initialize(const AgentAttributes& attributes);

	void step(const AgentAttributes& attributes, float dt);

private:

	float hp_;

	float ap_;
	
	float mp_;


};

#endif