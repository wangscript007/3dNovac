
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

#ifndef NC_GRAPHIC_CAMERA_CAMERA3D_H_
#define NC_GRAPHIC_CAMERA_CAMERA3D_H_

#include <Nc/Core/Math/Plane.h>
#include "Camera.h"

namespace Nc
{
    namespace Graphic
    {
        /// Abstract class to define 3d Camera
        /**
            A 3d Camera needs to have the routines to manage the events and to update the mouvement of the camera.
            The camera is composed by an Eye, a Center and a vector Up

            \todo Recode gluUnProject witch is deprecated and reimplement the Get3dCoordinateFromProjection method.
        */
        class LGRAPHICS Camera3d : public Camera
        {
            public:
                NC_UTILS_DEFINE_PARENT_CLASS(Camera);
                NC_UTILS_DEFINE_VISITABLE(System::Object);

            public:
				// /!\ near et far sont des define windows provenant de windef.h, ne pas changer le nom nearf en near !
                Camera3d(const char *className, float ratioAspect, float nearf, float farf, float fieldOfView);
                virtual ~Camera3d() {};

                /** Set the projection parameters */
                void            SetProjection(float ratioAspect, float nearf, float farf, float fieldOfView);
                /** Update the projectionMatrix to a perspective projection */
                virtual void    UpdateProjection(SceneGraph *scene);
                /**
                    Fix the camera. Set the View Matrix of the scene by using the property _viewMatrix.
                    To avoid any useless compute, this matrix is compute at the call of the method, "UpdateViewMatrix"
                */
                virtual void    Fix(SceneGraph *scene);

                /** Update the ratio aspect of the projection */
                virtual void    Resized(const System::Event &event);

                //Vector3f        Get3dCoordinateFromProjection(int x, int y);

                /** Update the eye of the camera */
                virtual void    MajEye() = 0;

                // accesseurs
                /** \return the eye of the camera */
                inline const Vector3f   &Eye() const                    {return _eye;}
                /** Set the eye of the camera */
                inline void             Eye(const Vector3f &eye)        {_eye = eye;}
                /** \return the center of the camera */
                inline const Vector3f   &Center() const                 {return _center;}
                /** Set the center of the camera, maybe you will need to call "MajEye" after */
                inline void             Center(const Vector3f &pos)     {_center = pos;}
                /** \return the up vector of the camera */
                inline const Vector3f   &Up() const                     {return _up;}
                /** Set the up vector of the camera */
                inline void             Up(const Vector3f &up)          {_up = up;}

                /** Compute the view matrix used to render a scene */
                virtual void            UpdateViewMatrix();
                /** Compute the projection frustum */
                virtual void            UpdateProjectionFrustum();
                /** Compute the view frustum */
                virtual void            UpdateViewFrustum();

                /** \return true if the given \p point is the frustum */
                virtual bool            PointInFrustum(const Vector3f &point);
                /** \return true if the given \p box is the frustum */
                virtual bool            BoxInFrustum(const Vector3f &center, float size);

            protected:
                /** To update the mouvement of the camera with the keystates of WindowInput */
                virtual void    Update(float RunningTime) = 0; // Running Time in second

            protected:
                // eye properties
                Vector3f        _eye;               ///< Vector to define the eye of the camera
                Vector3f        _center;            ///< Vector to define the center of the camera
                Vector3f        _up;                ///< Vector to define the up of the camera

                // projection properties
                float           _ratioAspect;       ///< define the ratio of the perspective projection
                float           _near;              ///< define the near value of the perspective projection
                float           _far;               ///< define the far value of the perspective projection
                float           _fieldOfView;       ///< define the fieldOfView of the perspective projection

                // frustum information
                float           _hNear;
                float           _hFar;
                float           _wNear;
                float           _wFar;
                float           _frustumTanFOV;
                Vector3f        _frustumX;
                Vector3f        _frustumY;
                Vector3f        _frustumZ;
                Vector3f        _frustumNTL;
                Vector3f        _frustumNTR;
                Vector3f        _frustumNBR;
                Vector3f        _frustumNBL;
                Vector3f        _frustumFTL;
                Vector3f        _frustumFTR;
                Vector3f        _frustumFBR;
                Vector3f        _frustumFBL;
                Planef          _planes[6];         ///< the six planes of the frustum

            private:
                bool            _viewMatrixUpdated; ///< statement to avoid to update the scene View Matrix each time
                TMatrix         _viewMatrix;        ///< the view matrix witch correspond to the vectors _eye / _center / _up. This matrix is used to set the View Matrix of the scene at the call of the method "Fix".
        };
    }
}


#endif
