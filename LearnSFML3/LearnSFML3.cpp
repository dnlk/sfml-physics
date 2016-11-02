// LearnSFML3.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <SFML/Graphics.hpp>

#include <iostream>
#include <windows.h>

#include "StateMachine.cpp"
#include "Physics.h"
#include "Topology.h"


float DIFFUSION_CONSTANT = .1;


class OxyCell : public PhysicsCell {
public:
	float oxygen;
};


class GamePhysics : public Physics {
	OxyCell newCell() {
		OxyCell oxyCell;
		oxyCell.oxygen = 10;
		return oxyCell;
	}
	
	void update(OxyCell * thisCell, OxyCell ** neighbors, OxyCell * cellToMutate)
	{
		float deltaO = 0.0;
		
		for (int i = 0; i < 4; i++) {
			if (neighbors[i] == nullptr) {
				continue;
			}
			else {
				float diff = neighbors[i]->oxygen - thisCell->oxygen;
				deltaO += diff;
			}
		}

		cellToMutate->oxygen = thisCell->oxygen + deltaO;
	}
};

int main() {
	GamePhysics physics;
	GridTopology topology(8, 10);
	TwoStateMachine stateMachine(&physics, &topology);
}
