
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

    File Created At:        02/05/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------


                Implementation de la classe abstraite "BoundingBox"

    permet de definir et de dessiner un bounding box

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_OBJECT_BOUNDING_BOX_H_
#define NC_GRAPHICS_OBJECT_BOUNDING_BOX_H_

#include "../Material/DefaultVertexType.h"
#include "Object.h"

namespace Nc
{
    namespace Graphic
    {
        /// Basic 3d graphical object used to render an Axis Aligned Bounding Box (AABB)
        /**
            \todo put a reference counter here to destroy the static drawable of the box and avoid memory leek
        */
        class LGRAPHICS BoundingBox : public Object, public Box3f
        {
            public:
                BoundingBox();
                BoundingBox(const Box3f &box);
                BoundingBox(const Vector3f &min, const Vector3f &max);
                virtual ~BoundingBox();

                const char              *ClassName()                {return "BoundingBox";}
                virtual ISceneNode      *Clone() const              {return new BoundingBox(*this);}

                virtual void            TransformModelMatrixToRender(SceneGraph *scene)    {}

                /** Render the box */
                void                    Draw(SceneGraph *scene);

                /** \return the height of the box */
                inline float            Height() const              {return Length(2);}

            private:
                /** Create the geometry of the box */
                void    CreateGeometry();

                Color   _color;         ///< the color of the box

                static Drawable         *_drawable;     ///< the static drawable in static because it's the same for all boxs
                static IDefaultMaterial *_material;     ///< the static material instance used to render the drawables
        };
    }
}

#ifdef UNDEFINE_INC
    #undef NC_GRAPHICS_OBJECT_OBJECT_TPP_
    #undef UNDEFINE_INC
    #include "Object3d.tpp"
#endif

#endif