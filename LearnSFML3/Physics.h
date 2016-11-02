#pragma once

class PhysicsCell {
public:
	PhysicsCell();
};


class Physics {
public:
	PhysicsCell newCell();

	void update(PhysicsCell * thisPhysicsCell, PhysicsCell ** neighborPhysicsCells, PhysicsCell * cellToMutate);
};


class StateInterface {
public:
	Physics * physics;

	void applyPhysics(PhysicsCell * cell, PhysicsCell ** neighbors);

	PhysicsCell * newState();

	void initialiseCell(PhysicsCell * cell);

};


class TwoStateContainer : public PhysicsCell {
public:
	PhysicsCell * states;

	TwoStateContainer(PhysicsCell cell1, PhysicsCell cell2);
};


class TwoStateInterface : public StateInterface {
public:
	Physics * physics;

	TwoStateInterface(Physics * physics);

	void applyPhysics(TwoStateContainer * container, TwoStateContainer ** neighborContainers);

	TwoStateContainer * newState();
};
