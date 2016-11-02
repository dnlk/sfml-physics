#pragma once

#include "Physics.h"

class Topology {
public:

	void initialize(StateInterface * stateInterface);
	void traverseUpdate(StateInterface * stateInterface);
};


class TopoCell {
public:
	PhysicsCell ** neighbors;
	PhysicsCell * center;
};


class GridTopology : public Topology {
public:
	int nCells, nColumns, nRows;
	TopoCell * gridCellArray;

	GridTopology(int nColumns, int nRows);

	void initialize(StateInterface * stateInterface);

	void traverseUpdate(StateInterface * stateInterface);

	int coordsToIdx(int x, int y);

	int getX(int idx);

	int getY(int idx);

	void linkNeighbors();
};
