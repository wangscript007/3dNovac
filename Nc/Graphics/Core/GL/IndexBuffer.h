
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

    File Created At:        27/02/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_CORE_GL_INDEXBUFFER_H_
#define NC_GRAPHICS_CORE_GL_INDEXBUFFER_H_

#include "DataBuffer.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// Define an index buffer used to describe a GeometryBuffer
            class LGRAPHICS IndexBuffer : public DataBuffer<unsigned int>
            {
                public:
                    IndexBuffer();
                    IndexBuffer(const Array<unsigned int> &tabIndices, unsigned int stride);
                    template<unsigned int D>
                    IndexBuffer(const Array<unsigned int, D> &tabIndices, unsigned int stride)
                        : DataBuffer<unsigned int>(GL_ELEMENT_ARRAY_BUFFER, tabIndices.Size() / stride, stride, GL_STATIC_DRAW, tabIndices.Data), _nbElements(tabIndices.Size())
                    {
                    }
                    virtual ~IndexBuffer();

                    /** Update the data */
                    template<unsigned int D>
                    void    UpdateData(const Array<unsigned int, D> &tabIndices, unsigned int stride, bool keepContent = false)
                    {
                        DataBuffer<unsigned int>::Init(GL_ELEMENT_ARRAY_BUFFER, tabIndices.Size() / stride, stride, GL_STATIC_DRAW, tabIndices.Data, keepContent);
                        _nbElements = tabIndices.Size();
                    }

                    /** Draw the elements index */
                    inline void             Draw(GLenum primitiveType)                  {glDrawElements(primitiveType, _nbElements, GL_UNSIGNED_INT, NULL);}

                    /** Return the number of elements */
                    inline unsigned int     NbElements()                                {return _nbElements;}

                private:
                    unsigned int            _nbElements;            ///< the number of elements in the buffer
            };
        }
    }
}

#endif
