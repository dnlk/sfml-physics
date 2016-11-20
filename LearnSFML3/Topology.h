#pragma once

#include "Physics.h"
#include "Render.h"

#include <iostream>

template <typename T>
class Topology {
public:

	virtual void initialize() =0;
	virtual void traverseUpdate() =0;
	virtual void traverseApplyCoords() = 0;
};

template <typename T>
class Cell {
public:
	T ** neighbors;
	T * center;
};

template <typename T, typename Physics, typename Render>
class GridTopology : public Topology<T> {
public:
	int nCells, nColumns, nRows;
	Cell<T> * gridCellArray;
	Physics * stateInterface;
	Render * renderInterface;

	GridTopology(Physics * stateInterface, Render * renderInterface)
		: stateInterface(stateInterface), renderInterface(renderInterface)
	{}


	void setupGrid(int columns, int rows)
	{
		nColumns = columns;
		nRows = rows;
		nCells = nColumns * nRows;
	}

	void initialize()
	{
		gridCellArray = new Cell<T>[nCells];

		for (int i = 0; i < nCells; i++) {
			gridCellArray[i].center = stateInterface->newState();
		}

		linkNeighbors();
	}

	void traverseUpdate()
	{
		for (int i = 0; i < nCells; i++) {
			Cell<T> gridCell = gridCellArray[i];
			stateInterface->applyPhysics(
				gridCell.center,
				gridCell.neighbors
			);
		}
	}


	void traverseApplyCoords()
	{
		for (int i = 0; i < nCells; i++) {
			int x = getX(i);
			int y = getY(i);
			Cell<T> & thisCell = gridCellArray[i];

			renderInterface->render(thisCell.center, x, y);
		}
	}

	int coordsToIdx(int x, int y)
	{
		return y * nColumns + x;
	}

	int getX(int idx)
	{
		return idx % nColumns;
	}

	int getY(int idx)
	{
		return idx / nColumns;
	}

	T * getByCoords(int x, int y)
	{
		if (x < 0 || y < 0 || x >= nColumns || y >= nRows) {
			return nullptr;
		}
		else {
			int idx = coordsToIdx(x, y);
			return gridCellArray[idx].center;
		}
	}

	void linkNeighbors()
	{
		for (int i = 0; i < nCells; i++) {
			int x = getX(i);
			int y = getY(i);
			Cell<T> & thisCell = gridCellArray[i];

			thisCell.neighbors = new T*[4];

			int offsetsX[4] = { -1, 0, 0, 1 };
			int offsetsY[4] = { 0, -1, 1, 0 };

			for (int j = 0; j < 4; j++)
			{
				int dx = offsetsX[j];
				int otherX = dx + x;
				int dy = offsetsY[j];
				int otherY = dy + y;
				T * otherCell = getByCoords(otherX, otherY);

				bool isNull;

				if (otherCell == nullptr) {
					thisCell.neighbors[j] = nullptr;
					isNull = true;
				}
				else {
					thisCell.neighbors[j] = otherCell;
					isNull = false;
				}

				std::cout << "x: " << x << ", dx: " << dx << ", y: " << y << ": dy" << dy << ", isNull: " << isNull << std::endl;
			}

			std::cout << "Neighbors of " << x << ", " << y << std::endl;
			for (int j = 0; j < 4; j++) {
				T * t = thisCell.neighbors[j];
				if (!(t == nullptr)) {
					std::cout << "  " << offsetsX[j] << ", " << offsetsY[j] << std::endl;
				}
			}
		}
	}
};
