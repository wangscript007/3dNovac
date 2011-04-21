
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

    File Created At:        27/12/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_CORE_GL_BLEND_H_
#define NC_GRAPHICS_CORE_GL_BLEND_H_

#include "../../Define.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// help to manipulate the blending opengl method
            /**
                \todo manage the transparency color and rate
            */
            class LCORE  Blend
            {
                public:
                    /// The pattern of the using blend function
                    enum    Pattern
                    {
                        Disabled = 0,
                        Alpha,                  ///< Color Pixel = Src * a + Dest * (1 - a)
                        Add,                    ///< Color Pixel = Src + Dest
                        Mutiply                 ///< Color Pixel = Src * Dest.
                    };

                public:
                    Blend() : _pattern(Disabled)     {}
                    Blend(Pattern p) : _pattern(p)  {}
                    ~Blend()    {}

                    /** Set the blending pattern */
                    void SetPattern(Pattern p)      {_pattern = p;}

                    /** Enable the blend func */
                    inline void Enable() const
                    {
                        if (_pattern != Disabled)
                        {
                            glEnable(GL_BLEND);
                            if (_pattern == Alpha)
                                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                            else if (_pattern == Add)
                                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                            else if (_pattern == Mutiply)
                                glBlendFunc(GL_DST_COLOR, GL_ZERO);
                        }
                    }

                    /** Disable the blending */
                    inline void Disable() const     {if (_pattern != Disabled) glDisable(GL_BLEND);}

                protected:
                    Pattern     _pattern;       ///< the blending pattern
            };
        }
    }
}

#endif
