#ifndef SCENE_H
#define SCENE_H

#include "shape.h"


class Scene
{
private:
	bool initialized;
public:
	_sys* sys;
	bool& break_main;

	Shape* shapes;
	unsigned int size;

	Scene(_sys*, bool&);
	~Scene();

	void InitScene();
	void ReleaseScene();


	void AddObject(unsigned int width, unsigned int height);

	void DrawScene();
};

#endif
