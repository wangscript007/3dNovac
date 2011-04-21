
/*-----------------------------------------------------------------------------

	3dNovac Graphics
	Copyright (C) 2010-2011, The 3dNovac Team

    This file is part of 3dNovac.

    3dNovac is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    3dNovac is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with 3dNovac.  If not, see <http://www.gnu.org/licenses/>.

    File Created At:        13/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPIHCS_GRAPHIC_ENGINE_H_
#define NC_GRAPIHCS_GRAPHIC_ENGINE_H_

#include <map>

#include <Nc/Core/Engine/MainEngine.h>

#include "Define.h"
#include "Scene/SceneGraphManager.h"
#include "Window/Context/GLContext.h"

namespace Nc
{
    /// Base namespace of the 3dNovac Graphical Engine
    namespace Graphic
    {
        /// The threaded GraphicEngine of 3dNovac
        /**
            Helps you to have a rendering thread.
            The engine has a Window instance and to create it you will need to set the pointer to a member function of the "Main" engine    <br\>
            The rendering is done with the SceneGraphManager.
        */
        class LGRAPHICS   Engine : public Nc::Engine::IEngine
        {
            public:
                typedef void (Nc::Engine::MainEngine::*CreateWindowFunc)(Window *win);      ///< the typedef of the member function pointer used to create the window

            public:
                Engine(Nc::Engine::Manager *manager, CreateWindowFunc func);
                virtual ~Engine();

                /** Render the scene with the SceneGraphManager, Check the WindowInput Event and update the elapsed time to compute the FPS */
                virtual void Execute(float runningTime);

                /** Create and return a GLContext shared with the rendering context (Call the function CreateNewSharedContext of GLContext) */
                inline GLContext            *CreateSharedContext()              {return _context->CreateNewSharedContext();}

            // accessor
                inline SceneGraphManager    &GetSceneManager()      {return _sceneGraphManager;}                ///< Return the SceneGraphManager used to render the scene
                inline Window               *GetWindow()            {return _win;}                              ///< Return the window instance used to setup the Window/WindowInput/GLContext
                static unsigned int         GetFPS()                {return (unsigned int)(1./_elapsedTime);}   ///< Return the current number of frame per second (static, so you could call it anytime)

            protected:
                /** To Create the GLContext, this function is called by the IEngine */
                virtual inline void         CreateContext();
                /** To Active the GLContext, this function is called by the IEngine */
                virtual inline void         ActiveContext()                     {if (_context != NULL) _context->Active();}
                /** To Disable the GLContext, this function is called by the IEngine */
                virtual inline void         DisableContext()                    {if (_context != NULL) _context->Disable();}

                virtual void CheckGLVersion();

                SceneGraphManager       _sceneGraphManager;     ///< The Scene graph manager that is used to render the scene

                CreateWindowFunc        _createWinFunction;     ///< The member function pointer of the "Main" engine used to create the window
                Window                  *_win;                  ///< The instance of the window
                GLContext               *_context;              ///< The GL context used to render the scene with the scene graph manager
                static double           _elapsedTime;           ///< Elapsed Time between 2 frame (in second)
        };
    }
}

#endif

