
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

    File Created At:        24/12/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

    class ObjectContainer, herite of Object and contain a list of Object

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_OBJECT_CONTAINER_H_
#define NC_GRAPHICS_OBJECT_CONTAINER_H_

#include "Object.h"

namespace Nc
{
    namespace Graphic
    {
        /// Define an Object witch has a list of Object
        class LGRAPHICS ObjectContainer : public Object
        {
            public:
                ObjectContainer();

                virtual ~ObjectContainer();

                /** Add an object in the container */
                inline void AddObject(Object *o)        {_listObject.push_back(o);}

                /** Remove the given object in the container */
                inline void RemoveObject(Object *o)
                {
                    ListPObject::iterator it = find(_listObject.begin(), _listObject.end(), o);
                    if (it != _listObject.end())
                        _listObject.erase(it);
                }

            protected:
                ListPObject     _listObject;        ///< list of objects
        };
    }
}

#endif
