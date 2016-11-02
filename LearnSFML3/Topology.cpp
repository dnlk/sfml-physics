#include "stdafx.h"

#pragma once

#include "Physics.h"
#include "Topology.h"


void Topology::traverseUpdate(StateInterface * stateInterface) {}
void Topology::initialize(StateInterface * stateInterface) {}

GridTopology::GridTopology(int nColumns, int nRows) : nColumns(nColumns), nRows(nRows)
{
	nCells = nColumns * nRows;
}


void GridTopology::initialize(StateInterface * stateInterface)
{
	gridCellArray = new TopoCell[nCells];

	for (int i = 0; i < nCells; i++) {
		gridCellArray[i].center = stateInterface->newState();
	}
}

void GridTopology::traverseUpdate(StateInterface * stateInterface)
{
	for (int i = 0; i < nCells; i++) {
		stateInterface->applyPhysics(
			gridCellArray[i].center,
			gridCellArray[i].neighbors
		);
	}
}

int GridTopology::coordsToIdx(int x, int y)
{
	return y * nColumns + x;
}

int GridTopology::getX(int idx)
{
	return idx % nCells;
}

int GridTopology::getY(int idx)
{
	return idx / nColumns;
}

void GridTopology::linkNeighbors()
{
	for (int i = 0; i < nCells; i++) {
		int x = getX(i);
		int y = getY(i);
		TopoCell & thisCell = gridCellArray[i];

		for (int j = 0; j < 4; j++)
		{
			int otherCellIdx = coordsToIdx(x, y);
			thisCell.neighbors[j] = gridCellArray[otherCellIdx].center;
		}
	}
}
