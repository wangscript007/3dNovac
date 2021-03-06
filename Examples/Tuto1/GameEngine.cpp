
#include <Nc/Graphics/Camera/StandardCamera3d.h>
#include <Nc/Graphics/Object/BasicMeshCreator.h>
#include <Nc/Graphics/Scene/SceneGraphManager.h>

#include "GameEngine.h"

using namespace Nc;
using namespace Nc::Graphic;

GameEngine::GameEngine(Graphic::Window *window, Nc::Engine::Manager *manager)
  : Nc::Engine::MainEngine(manager), _window(window)
{
    // add the window Input to the Inputs of the engine
    AddInput(_window->Input());
}

GameEngine::~GameEngine()
{
}

void	GameEngine::ReleaseContent()
{
    // delete the scene and it's childs node
	_context->Active();
	delete _scene3d;
    delete _context;
}

void	GameEngine::LoadContent()
{
	// create the camera, scene and set it to the SceneGraphManager of the Graphic engine
	_scene3d = new SceneGraph();
	_camera = new StandardCamera3d(_window);
	_scene3d->AddChild(_camera);
	_window->SceneManager()->AddScene(_scene3d);

	// create a new repere to display at the center of the scene
	_scene3d->AddChild(BasicMeshCreator::Axis(1));

    // no need to active/disable the context at each loop
    _pattern.Disable(Nc::Engine::HasAContext);
}

void	GameEngine::Update(float runningTime)
{
  // update the scene (will update it's effects and camera)
  _scene3d->UpdateNode(runningTime);
}

void	GameEngine::ManageWindowEvent(System::Event &event)
{
  // resize the camera if needed
  if (event.type == System::Event::Resized)
    _camera->Resized(event);
  MainEngine::ManageWindowEvent(event);
}

void	GameEngine::KeyboardEvent(System::Event &event)
{
  // send the events to the camera
  _camera->KeyboardEvent(event);
}

void	GameEngine::MouseButtonEvent(System::Event &event)
{
  // send the events to the camera
  _camera->MouseButtonEvent(event);
}

void	GameEngine::MouseMotionEvent(System::Event &event)
{
  // send the events to the camera
  _camera->MouseMotionEvent(event);
}
