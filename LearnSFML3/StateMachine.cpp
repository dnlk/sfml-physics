#include "stdafx.h"

#include "Physics.h"
#include "Topology.h"

class TwoStateMachine {
public:
	Physics * physics;
	Topology * topology;
	int statePhase;

	TwoStateMachine(Physics * physics, Topology * topology) : topology(topology), physics(physics)
	{
		TwoStateInterface twoStateInterface(physics);
		topology->initialize(&twoStateInterface);
		statePhase = -1;
	}


	void update() {
		statePhase = (statePhase + 1) % 2;
		TwoStateInterface twoStateInterface(physics);
		topology->traverseUpdate(&twoStateInterface);
	}
};
