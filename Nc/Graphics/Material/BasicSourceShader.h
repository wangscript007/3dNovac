
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

    File Created At:        05/03/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHIC_BASICSOURCESHADER_H_
#define NC_GRAPHIC_BASICSOURCESHADER_H_

#include "../Define.h"

namespace Nc
{
    namespace Graphic
    {
        /// Reference the sources of the shaders used with the DefaultMaterial and BasicVertexType
        namespace BasicSource
        {
            extern const char* LGRAPHICS ColoredVertexShader;               ///< Default Vertex shader to render a Colored VertexType
            extern const char* LGRAPHICS ColoredFragmentShader;             ///< Default Fragment shader to render a Colored VertexType

            extern const char* LGRAPHICS Colored2dVertexShader;             ///< Default Vertex shader to render a Colored2d VertexType
            extern const char* LGRAPHICS Colored2dFragmentShader;           ///< Default Fragment shader to render a Colored2d VertexType

            extern const char* LGRAPHICS TexturedVertexShader;              ///< Default Vertex shader to render a Textured VertexType
            extern const char* LGRAPHICS TexturedFragmentShader;            ///< Default Fragment shader to render a Textured VertexType

            extern const char* LGRAPHICS Textured2dVertexShader;            ///< Default Vertex shader to render a Textured2d VertexType
            extern const char* LGRAPHICS Textured2dFragmentShader;          ///< Default Fragment shader to render a Textured2d VertexType

            extern const char* LGRAPHICS Textured3dVertexShader;            ///< Default Vertex shader to render a Textured3d VertexType
            extern const char* LGRAPHICS Textured3dFragmentShader;          ///< Default Fragment shader to render a Textured3d VertexType
        }
    }
}

#endif // NC_GRAPHIC_SOURCEBASICSHADER_H_
