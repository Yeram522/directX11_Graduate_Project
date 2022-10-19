////////////////////////////////////////////////////////////////////////////////
// Filename: scene.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SCENE_H_
#define _SCENE_H_

#include <string>

using namespace std;

class Scene
{
public:
	Scene(string);

	string name;

	bool isLoaded(); //Returns true if the Scene is loaded.

	bool Initialize();

    void Update();
};

#endif

