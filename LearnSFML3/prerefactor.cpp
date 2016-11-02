// LearnSFML3.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <SFML/Graphics.hpp>

#include <iostream>
#include <windows.h>


class Cell {
public:
	double oxygen;
	Cell() {}

	void set_oxygen(double o2) {
		oxygen = o2;
	}
};


class CellGrid {

public:

	int mColumns, mRows, mWidth, mHeight, numTiles;
	float tileWidth, tileHeight;
	float diffusionRate = .01;
	sf::RenderWindow *mWindow;

	CellGrid(int columns, int rows, int width, int height)
		: mColumns(columns), mRows(rows), mWidth(width), mHeight(height)
	{
		tileWidth = (float)width / (float)columns;
		tileHeight = (float)height / (float)rows;
		numTiles = columns * rows;

		std::cout << "columns: " << mColumns << ", rows: " << mRows << ", tilewidth: " << tileWidth << ", tileheight" << tileHeight << std::endl;

	}

	Cell * cellGrid;
	Cell * cellGridBuffer;

	void init() {
		int nElements = mColumns * mRows;
		cellGrid = new Cell[nElements];
		cellGridBuffer = new Cell[nElements];
		for (int i = 0; i < nElements; i++) {
			float oxygenLevel = (255.0 / (float)nElements) * i;
			cellGrid[i].set_oxygen(oxygenLevel);
		}
	}

	void swapCellGrids() {
		Cell * tempCellGrid = cellGrid;
		cellGrid = cellGridBuffer;
		cellGridBuffer = tempCellGrid;
	}

	Cell * byCoords(int x, int y) {
		int idx = x + y * mColumns;
		return &cellGrid[idx];
	}

	Cell * bufferByCoords(int x, int y) {
		int idx = x + y * mColumns;
		return &cellGridBuffer[idx];
	}

	void attachWindow(sf::RenderWindow * window) {
		mWindow = window;
	}

	void handleEvents() {
		sf::Event event;
		while (mWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				mWindow->close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				clickHandler(event);
			}
		}
	}

	void clickHandler(sf::Event event) {
		int x = event.mouseButton.x;
		int y = event.mouseButton.y;

		int c = (int)((float)x / tileWidth);
		int r = (int)((float)y / tileHeight);

		int idx = r * mColumns + c;

		//cellGrid.byCoords(x, y)->set_oxygen(10.0);

		std::cout << "row: " << r << "column: " << c << std::endl;

		//std::cout << byCoords(c, r)->oxygen << std::endl;

		//byCoords(c, r)->oxygen += 100.0;
		cellGrid[idx].oxygen += 100.0;

		//std::cout << byCoords(c, r)->oxygen << std::endl << std::endl;
	}

	void render() {

		mWindow->clear();

		for (int i = 0; i < numTiles; i++) {
			int c = i % mColumns;
			int r = i / mColumns;

			float posX = float(c) * tileWidth;
			float posY = float(r) * tileHeight;

			sf::RectangleShape tileShape(sf::Vector2f(tileWidth - 2, tileHeight - 2));

			tileShape.setPosition(sf::Vector2f(posX, posY));

			sf::Uint8 redColor(66);
			sf::Uint8 greenColor(188);
			sf::Uint8 blueColor(66);
			sf::Uint8 alpha(byCoords(c, r)->oxygen);

			sf::Color fillColor(redColor, greenColor, blueColor, alpha);

			tileShape.setFillColor(fillColor);

			mWindow->draw(tileShape);
		}

		mWindow->display();
	}

	float doDiffusionCalc(int c, int r, int dc, int dr) {
		Cell * thisCell = byCoords(c, r);
		Cell * thisCellBuffer = bufferByCoords(c, r);
		Cell * neighborCell = byCoords(c + dc, r + dr);
		Cell * neighborCellBuffer = byCoords(c + dc, r + dr);

		float thisOxygen = thisCell->oxygen;
		float neighborOxygen = neighborCell->oxygen;

		float oxygenDifferance = thisOxygen - neighborOxygen;
		float oxygenDelta = oxygenDifferance * diffusionRate;

		return oxygenDelta;
	}

	void updateCellGrid() {
		for (int i = 0; i < numTiles; i++)
		{
			int c = i % mColumns;
			int r = i / mColumns;

			Cell * thisCell = byCoords(c, r);
			float oxygen = thisCell->oxygen;

			Cell * thisBuffer = bufferByCoords(c, r);

			if (c > 0)
			{
				oxygen -= doDiffusionCalc(c, r, -1, 0);
			}

			if (r > 0)
			{
				oxygen -= doDiffusionCalc(c, r, 0, -1);
			}

			if (c < mColumns - 1)
			{
				oxygen -= doDiffusionCalc(c, r, 1, 0);
			}

			if (r < mRows - 1)
			{
				oxygen -= doDiffusionCalc(c, r, 0, 1);
			}

			thisBuffer->oxygen = oxygen;

		}

		swapCellGrids();
	}

};

CellGrid * makeCellGrid(int columns, int rows, int width, int height) {

	CellGrid * cellGrid = new CellGrid(columns, rows, width, height);
	cellGrid->init();
	return cellGrid;
}


int _main()
{
	int width = 800;
	int height = 600;
	int columns = 10;
	int rows = 8;

	CellGrid * cellGrid = makeCellGrid(columns, rows, width, height);
	sf::RenderWindow window(sf::VideoMode(width, height), "boo yeah");
	cellGrid->attachWindow(&window);

	while (window.isOpen())
	{
		Sleep(50);
		cellGrid->handleEvents();
		cellGrid->updateCellGrid();
		cellGrid->render();
	}

	return 0;
}
