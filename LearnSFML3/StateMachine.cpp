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

	TwoStateMachine()
	{
		int nRows = 8;
		int nColumns = 4;

		physics = new Physics;
		stateInterface = new PhysicsInterface(physics);
		renderer = new Render(300.0, 400.0, nColumns, nRows);
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
