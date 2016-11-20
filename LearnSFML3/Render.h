#pragma once

#include <SFML/Graphics.hpp>

template <typename T> class TwoStateContainer;

template <typename T>
class RenderSFML3 {
public:

	sf::RenderWindow *mWindow;
	float tileWidth, tileHeight;

	//RenderSFML3(float tileWidth, float tileHeight);

	virtual void renderCell(T * cell, int x, int y) = 0;

	void renderFrame()
	{
		mWindow->display();
	}


	void clearFrame()
	{
		mWindow->clear();
	}
};


template <typename T, template<typename> class Container>
class RenderInterface {
public:
	void render(Container<T> * cell, int x, int y) {}
};

template <typename T>
class TwoRenderInterface : public RenderInterface<T, TwoStateContainer> {
public:
	RenderSFML3<T> * renderer;
	int activeState;  // 0 or 1

	TwoRenderInterface(RenderSFML3<T> * renderer) : renderer(renderer)
	{}

	void render(TwoStateContainer<T> * container, int x, int y)
	{
		T * currentState = &container->states[activeState];

		renderer->renderCell(currentState, x, y);
	}

	void clearFrame()
	{
		renderer->clearFrame();
	}

	void renderFrame()
	{
		renderer->renderFrame();
	}
};
