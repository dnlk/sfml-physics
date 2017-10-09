#include "stdafx.h"

#include "Physics.h"
#include "Topology.h"
#include "Render.h"

template <typename T, template<typename, typename, typename> class TopologyT, typename Physics, typename Render>
class TwoStateMachine {
public:
	typedef TwoStateContainer<T> Cell;
	
	typedef TwoRenderInterface<T> RenderInterface;
	typedef TwoStateInterface<T> PhysicsInterface;
	typedef TopologyT<Cell, PhysicsInterface, RenderInterface> Topology;

	Physics * physics;
	Topology * topology;
	Render * renderer;
	RenderInterface * renderInterface;
	PhysicsInterface * stateInterface;

	int statePhase;
	float width;
	float height;
	int nRows;
	int nColumns;

	TwoStateMachine(float windowHeight, float windowWidth, int nRows, int nColumns): height(height), width(width), nRows(nRows), nColumns(nColumns)
	{
		

		physics = new Physics;
		stateInterface = new PhysicsInterface(physics);
		renderer = new Render(windowHeight, windowWidth, nColumns, nRows);
		renderInterface = new RenderInterface(renderer);
		topology = new Topology(stateInterface, renderInterface);
		topology->setupGrid(nColumns, nRows);
		
		topology->initialize();
		statePhase = -1;
	}

	void update() {
		statePhase = (statePhase + 1) % 2;
		stateInterface->activeState = statePhase;
		topology->traverseUpdate();
	}

	void render() {
		renderInterface->clearFrame();

		renderInterface->activeState = statePhase;
		topology->traverseApplyCoords();

		renderInterface->renderFrame();
	}
};
