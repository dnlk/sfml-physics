// LearnSFML3.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <SFML/Graphics.hpp>

#include <iostream>
#include <windows.h>

#include "StateMachine.cpp"
#include "Physics.h"
#include "Topology.h"
#include "Render.h"


float DIFFUSION_CONSTANT = .1;


class OxyCell {
public:
	float oxygen;
};


class RenderOxyCell : public RenderSFML3<OxyCell> {
public:
	float windowWidth, windowHeight, nColumns, nRows, tileWidth, tileHeight;

	RenderOxyCell(float windowWidth, float windowHeight, int nColumns, int nRows)
		: windowWidth(windowWidth), windowHeight(windowHeight), nColumns(nColumns), nRows(nRows)
	{
		tileWidth = windowWidth / (float)nColumns;
		tileHeight = windowHeight / (float)nRows;
		mWindow = new sf::RenderWindow (sf::VideoMode(windowWidth, windowHeight), "boo yeah");
	}

	void renderCell(OxyCell * cell, int x, int y)
	{
		float posX = float(x) * tileWidth;
		float posY = float(y) * tileHeight;

		sf::RectangleShape tileShape(sf::Vector2f(tileWidth - 2, tileHeight - 2));

		tileShape.setPosition(sf::Vector2f(posX, posY));

		sf::Uint8 redColor(66);
		sf::Uint8 greenColor(188);
		sf::Uint8 blueColor(66);
		sf::Uint8 alpha(cell->oxygen);

		sf::Color fillColor(redColor, greenColor, blueColor, alpha);

		tileShape.setFillColor(fillColor);

		mWindow->draw(tileShape);
	}
};


class GamePhysics : public Physics<OxyCell> {

	int oxyInit = 0;
	float diffuseRatio = 0.1;

	OxyCell newCell() {
		OxyCell oxyCell;
		oxyCell.oxygen = oxyInit;
		oxyInit += 4;
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
				deltaO += diff * diffuseRatio;
			}
		}

		cellToMutate->oxygen = thisCell->oxygen + deltaO;
	}
};

int main() {
	// int windowHeight = 300;
	// int windowWidth = 450;
	// int nColumns = 8;
	// int nRows = 10;

	
	TwoStateMachine<OxyCell, GridTopology, GamePhysics, RenderOxyCell> stateMachine;
	
	while (stateMachine.renderer->mWindow->isOpen())
	{
		Sleep(50);

		stateMachine.update();
		stateMachine.render();
	}
	
}
