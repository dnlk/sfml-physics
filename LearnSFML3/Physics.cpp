#include "stdafx.h"

#include "Physics.h"


PhysicsCell::PhysicsCell() {}

PhysicsCell Physics::newCell() 
{
	PhysicsCell cell;
	return cell;
}

void Physics::update(PhysicsCell * thisPhysicsCell, PhysicsCell ** neighborPhysicsCells, PhysicsCell * cellToMutate) {}

TwoStateContainer::TwoStateContainer(PhysicsCell cell1, PhysicsCell cell2) {
	states = new decltype(cell1)[2];
	states[0] = cell1;
	states[1] = cell2;
}


void StateInterface::applyPhysics(PhysicsCell * cell, PhysicsCell ** neighbors) {}

PhysicsCell * StateInterface::newState() 
{
	return &physics->newCell();
}

void StateInterface::initialiseCell(PhysicsCell * cell) {}

TwoStateInterface::TwoStateInterface(Physics * physics) : physics(physics) {}

void TwoStateInterface::applyPhysics(TwoStateContainer * container, TwoStateContainer ** neighborContainers)
{
	auto currentState = &container->states[0];
	auto stateToMutate = &container->states[1];

	decltype(currentState) wrappedStateArray[4];

	for (int i = 0; i < 4; i++) {
		TwoStateContainer * c = neighborContainers[i];
		wrappedStateArray[i] = &c->states[0];
	}

	physics->update(currentState, wrappedStateArray, stateToMutate);
}

TwoStateContainer * TwoStateInterface::newState()
{
	TwoStateContainer * state = new TwoStateContainer(physics->newCell(), physics->newCell());
	return state;
}
