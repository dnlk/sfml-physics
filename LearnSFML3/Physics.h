#pragma once

template <typename T>
class Physics {
public:
	virtual T newCell() =0;

	virtual void update(T * thisPhysicsCell, T ** neighborPhysicsCells, T * cellToMutate) =0;
};

template <typename T>
class TwoStateContainer {
public:
	T * states;

	TwoStateContainer(T cell1, T cell2) {
		states = new T[2];
		states[0] = cell1;
		states[1] = cell2;
	}
};

template <typename T, template<typename> class Container>
class StateInterface {
public:
	Physics<T> * physics;

	virtual void applyPhysics(Container<T> * cell, Container<T> ** neighbors) = 0;

	virtual Container<T> * newState() =0;

	//virtual void initialiseCell(PhysicsCell * cell) =0;

};

template <typename T>
class TwoStateInterface : public StateInterface<T, TwoStateContainer> {
public:
	Physics<T> * physics;
	int activeState;  // 0 or 1

	TwoStateInterface<T>::TwoStateInterface(Physics<T> * physics) : physics(physics) {}

	void applyPhysics(TwoStateContainer<T> * container, TwoStateContainer<T> ** neighborContainers)
	{
		T * currentState = &container->states[activeState];
		T * stateToMutate = &container->states[1 - activeState];

		T * wrappedStateArray[4];

		for (int i = 0; i < 4; i++) {
			TwoStateContainer<T> * c = neighborContainers[i];
			if (c == nullptr) {
				wrappedStateArray[i] = nullptr;
			}
			else {
				wrappedStateArray[i] = &c->states[activeState];
			}
		}

		physics->update(currentState, wrappedStateArray, stateToMutate);
	}

	TwoStateContainer<T> * newState()
	{
		TwoStateContainer<T> * state = new TwoStateContainer<T>(physics->newCell(), physics->newCell());
		return state;
	}

};

