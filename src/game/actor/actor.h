#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include "engine/geom/vec2f.h"
#include "game/combat/combat.h"

class Scene;
class Shape;
class PhysicsShape;
class DrawObject;
class Interaction;
class Agent;

class Actor {

friend class Scene;

public:

	enum Type {
		UNDEFINED,
		AGENT
	};

	Actor(Type type = UNDEFINED);

	virtual ~Actor();

	long getId() const;

	Type getType() const;

	bool isAgent() const;

	Agent* asAgent();

	vec2f getPosition() const;

	Scene* getScene();

	void setPosition(const vec2f& position);

	void hit(const AttackHit& hit);

	void step(float dt);

	bool hitsInteractionShape(const Shape& shape) const;

	bool hitsHitShape(const Shape& shape) const;

	virtual bool canInteract(const Interaction& interaction) { return false; };

	virtual bool interact(const Interaction& interaction) { return false; };

	virtual void onStep(float dt) = 0;

protected:

	void setDestroyOnRemove(bool destroy_on_remove);

	void setPhysicsShape(PhysicsShape* physics_shape);

	PhysicsShape* getPhysicsShape();

	void addDrawObject(DrawObject* draw_object, const vec2f& offset = vec2f());

	DrawObject* getDrawObject(int index);

	void setDrawObjectOffset(int index, const vec2f& offset);

	vec2f getDrawObjectOffset(int index) const;



	void addHitShape(Shape* hit_shape, const vec2f& offset = vec2f());

	Shape* getHitShape(int index);

	void setHitShapeOffset(int index, const vec2f& offset);

	vec2f getHitShapeOffset(int index) const;



	void addInteractionShape(Shape* interaction_shape, const vec2f& offset = vec2f());

	Shape* getInteractionShape(int index);

	void setInteractionShapeOffset(int index, const vec2f& offset);

	vec2f getInteractionShapeOffset(int index) const;




	void addInteraction(Interaction* interaction);

	void removeInteraction(Interaction* interaction);

	const std::vector<AttackHit>& getAttackHits() const;

	const std::vector<Interaction*>& getInteractions();



	virtual void onAddToScene() {};

	virtual void onRemoveFromScene() {};

private:

	struct compareById {
		bool operator() (long a, long b) const {return a < b;} 
		bool operator() (long a, Actor * b) const {return a < b->getId();}
		bool operator() (Actor* a, Actor* b) const {return a->getId() < b->getId();}
		bool operator() (Actor* a, long b) const {return a->getId() < b;}
	};

	static long next_id;

	static long getNextId();

	void addToScene(Scene* scene);

	void removeFromScene(Scene* scene);

	long id_;

	Type type_;

	Scene* scene_;

	std::vector<DrawObject*> draw_objects_;

	std::vector<vec2f> draw_object_offsets_;

	std::vector<Shape*> hit_shapes_;

	std::vector<vec2f> hit_shape_offsets_;

	std::vector<Shape*> interaction_shapes_;

	std::vector<vec2f> interaction_shape_offsets_;

	PhysicsShape* physics_shape_;

	vec2f position_;

	std::vector<AttackHit> attack_hits_;

	std::vector<Interaction*> interactions_;

};

#endif