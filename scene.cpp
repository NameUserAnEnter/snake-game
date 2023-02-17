#include "scene.h"



 void Scene::DrawScene()
{
	for (int i = 0; i < size; i++)
	{
		shapes[i].Update();
	}
}

 void Scene::AddObject(unsigned int width, unsigned int height)
{
	if (shapes != nullptr)
	{
		shapes = (Shape*)realloc(shapes, sizeof(Shape) * (size + 1));
		if (shapes != nullptr)
		{
			size++;

			shapes[size - 1] = Shape(sys);
			if(width != 0 && height != 0) shapes[size - 1].InitShape(width, height);
			//return &shapes[size - 1];
		}
		else
		{
			break_main = true;
		}
	}
	else
	{
		break_main = true;
	}
}


 Scene::Scene(_sys* _sys_, bool& _break_main_): break_main(_break_main_)
{
	sys = _sys_;

	size = 0;

	shapes = nullptr;
	initialized = false;
}

 void Scene::InitScene()
{
	size = 0;
	initialized = true;

	shapes = nullptr;
	shapes = (Shape*)calloc(0, sizeof(Shape));
	if (shapes == nullptr)
	{
		break_main = true;
	}
}

 void Scene::ReleaseScene()
{
	if (initialized)
	{
		for (int i = 0; i < size; i++) shapes[i].ReleaseShape();
		free(shapes);
	}
}

 Scene::~Scene()
{
}
