
/*-----------------------------------------------------------------------------

	3dNovac Graphics
	Copyright (C) 2010-2011, 3dNovac Team

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

    File Created At:        04/04/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "DefaultLightingMaterial.h"
#include "../Material/Drawable.h"
#include "LightingEffect.h"

using namespace Nc::Graphic;
using namespace Nc::Graphic::DefaultVertexType;

enum Uniforms
{
    UniformM = 0,
    UniformPV,
    UniformNormalMatrix,
    UniformLight,
    UniformLightPass,
    UniformLightColor,
    UniformTextured,
    UniformDiffuse,
    UniformLightMap,
    UniformNormalMap,
    UniformBumpMapping,
    NbUniform
};

DefaultLightingMaterial::DefaultLightingMaterial() : ILightingMaterial("DefaultLightingMaterial")
{
    _program.Attach("Nc:Shader:lightShader.vs", GL::Enum::VertexShader);
    _program.Attach("Nc:Shader:lightShader.fs", GL::Enum::FragmentShader);
    _program.Link();
    _program.Enable();
    _program.InitNumberOfUniform(NbUniform);
    _program.UseUniformLocation("MMatrix");
    _program.UseUniformLocation("VPMatrix");
    _program.UseUniformLocation("NormalMatrix");
    _program.UseUniformLocation("Light");
    _program.UseUniformLocation("LightPass");
    _program.UseUniformLocation("LightColor");
    _program.UseUniformLocation("Textured");
    _program.UseUniformLocation("Diffuse");
    _program.UseUniformLocation("LightMap");
    _program.UseUniformLocation("NormalMap");
    _program.UseUniformLocation("BumpMapping");
    _program.InitNumberOfAttrib(4);
    _program.UseAttribLocation("InCoord");
    _program.UseAttribLocation("InTexCoord");
    _program.UseAttribLocation("InColor");
    _program.UseAttribLocation("InNormal");

    _lightMap.GenereSphereMap(64);      // creation d'une light map 3d de 64 float de diametre
    _patternMask.Enable(BumpMapping);   // active par defaut le bump maping

    _rasterMode.SetDepthMask(false);
    _blend.SetFactors(GL::Enum::One, GL::Enum::One);
}

DefaultLightingMaterial::~DefaultLightingMaterial()
{
}

bool    DefaultLightingMaterial::Configure(Drawable &drawable)
{
    GL::VertexDescriptor  &desc = drawable.Geometry->Descriptor();
    _program.Enable();
    for (unsigned int i = 0; i < desc.Size(); ++i)
    {
        if (desc[i].Name == ComponentsName::Coord)
            desc[i].IndexAttrib = _program.Attrib(0);
        else if (desc[i].Name == ComponentsName::TexCoord)
            desc[i].IndexAttrib = _program.Attrib(1);
        else if (desc[i].Name == ComponentsName::Color)
            desc[i].IndexAttrib = _program.Attrib(2);
        else if (desc[i].Name == ComponentsName::Normal)
            desc[i].IndexAttrib = _program.Attrib(3);
    }
    return true;
}

#ifdef _DEBUG
// Draw the normals of the given geometry with a geometry shader
void DrawNormal(SceneGraph *scene, const TMatrix &mvp, Drawable &drawable)
{
    static GL::Program normalProgram;

    // create the program only in the first pass
    if (!normalProgram.IsValid())
    {
        normalProgram.Attach("Nc:Shader:normalShader.vs", GL::Enum::VertexShader);
        normalProgram.Attach("Nc:Shader:normalShader.gs", GL::Enum::GeometryShader);
        normalProgram.Attach("Nc:Shader:normalShader.fs", GL::Enum::FragmentShader);
        normalProgram.Link();
        normalProgram.Enable();
        normalProgram.UseUniformLocation("MVPMatrix");
        normalProgram.UseAttribLocation("InCoord");
        normalProgram.UseAttribLocation("InColor");
        normalProgram.UseAttribLocation("InNormal");
    }

    // disable the attrib witch we don't need
    GL::VertexDescriptor    &desc = drawable.Geometry->Descriptor();
    Array<unsigned int>     lastAttribs(desc.Size());
    for (unsigned int i = 0; i < desc.Size(); ++i)
    {
        lastAttribs.Data[i] = desc[i].IndexAttrib;
        if (desc[i].Name == ComponentsName::Coord)
            desc[i].IndexAttrib = normalProgram.Attrib(0);
        else if (desc[i].Name == ComponentsName::Color)
            desc[i].IndexAttrib = normalProgram.Attrib(1);
        else if (desc[i].Name == ComponentsName::Normal)
            desc[i].IndexAttrib = normalProgram.Attrib(2);
        else
            desc[i].IndexAttrib = -1;
    }

    normalProgram.Enable();

    // set the model view projection matrix
    normalProgram.SetUniform(0, mvp);
    // render the normals
    drawable.Render();

    // restore the index attrib
    for (unsigned int i = 0; i < desc.Size(); ++i)
        desc[i].IndexAttrib = lastAttribs.Data[i];
}
#endif

void    DefaultLightingMaterial::Render(SceneGraph *scene, const TMatrix &modelMatrix, Drawable &drawable)
{
    if (_currentLightingEffect == NULL)
        return;

    // calcul the view/projection, normal matrix
    TMatrix pv = scene->ProjectionMatrix() * scene->ViewMatrix();
    TMatrix normalMatrix(modelMatrix);
    normalMatrix.Inverse();

    // only in debug, draw the normals
    #ifdef _DEBUG
    if (_patternMask.Enabled(DisplayNormal))
        DrawNormal(scene, pv * modelMatrix, drawable);
    #endif

    const ListPLight        &listLight = _currentLightingEffect->Lights();
    const Color             &ambiantColor = _currentLightingEffect->ColorAmbiant();

    drawable.Enable();
    _program.Enable(); // enable the shader

    // set the matrix
    _program.SetUniform(UniformM, modelMatrix);
    _program.SetUniform(UniformPV, pv);
    _program.SetUniform(UniformNormalMatrix, normalMatrix, false);

    // set the ambiant color light
    _program.SetUniform(UniformLightColor, ambiantColor.R, ambiantColor.G, ambiantColor.B, ambiantColor.A);

    // Texture unit 0 --> the diffuse
    if (drawable.Config->Textures.Size() > 0 &&
        drawable.Config->Textures[0].IsValid() &&
        drawable.Config->Textures[0].Is2d())
    {
        _program.SetUniform(UniformTextured, 1);
        _program.SetUniform(UniformDiffuse, 0);
        GL::Texture::ActiveTexture(0);
        drawable.Config->Textures[0].Enable();
    }
    else
        _program.SetUniform(UniformTextured, 0);

    // First pass --> render the scene with the ambiant color
    _program.SetUniform(UniformLightPass, 0);
//    drawable.GetGeometry().GetVBO().MaskDescriptor = 0x07;
/// \todo in the first pass of the lightingMaterial, no need to bind the normal attrib
    drawable.Render();
//    drawable.GetGeometry().GetVBO().MaskDescriptor = lastDesc;

    // if the lighting is disabled, stop here
    if (_patternMask.Enabled(Disabled))
        return;

    // next pass --> render the lights
    _program.SetUniform(UniformLightPass, 1);

    _blend.Enable();
    _rasterMode.Enable();    // disable the depth mask write, no need

    // active the light map
    _program.SetUniform(UniformLightMap, 1);
    GL::Texture::ActiveTexture(1);
    _lightMap.Enable();

    // active the normal mapping
    if (_patternMask.Enabled(BumpMapping) && drawable.Config->Textures.Size() > 1 &&
        drawable.Config->Textures[1].IsValid() &&
        drawable.Config->Textures[1].Is2d())
    {
        _program.SetUniform(UniformBumpMapping, 1);
        _program.SetUniform(UniformNormalMap, 2);
        GL::Texture::ActiveTexture(2);
        drawable.Config->Textures[1].Enable();
    }
    else
        _program.SetUniform(UniformBumpMapping, 0);

    TMatrix matrixLight;
    for (ListPLight::const_iterator it = listLight.begin(); it != listLight.end(); it++)
    {
        Vector3f pos;
        (*it)->GetRecursiveMatrix(matrixLight); /// \todo To obtain the light position, we need to caculate recursively it's matrix, this could decrease the perf
        matrixLight.Transform(pos);
        _program.SetUniform(UniformLight, pos.Data[0], pos.Data[1], pos.Data[2], 1.0f / ((*it)->radius));
        _program.SetUniform(UniformLightColor, (*it)->color.R, (*it)->color.G, (*it)->color.B, (*it)->color.A);

        // rendering pass for the current light
        drawable.Render();
    }

    _rasterMode.Disable();
    _blend.Disable();
    drawable.Disable();
}