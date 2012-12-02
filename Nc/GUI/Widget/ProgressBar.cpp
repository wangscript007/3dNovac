
/*-----------------------------------------------------------------------------

	3dNovac GUI
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

    File Created At:        06/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "ProgressBar.h"

using namespace Nc;
using namespace Nc::GUI;
using namespace Nc::Graphic;

ProgressBar::ProgressBar(const AlignmentMask &alignment, const Vector2i &size, const Utils::FileName &file)
    : Widget(alignment, size),
    _progressBox(0, 0, size.data[0], size.data[1]),
    _percent(0)
{
    Init(file);
}

ProgressBar::ProgressBar(const AlignmentMask &alignment, const Vector2i &size, const Graphic::GL::Texture &texture)
    : Widget(alignment, size),
    _progressBox(0, 0, size.data[0], size.data[1]),
    _percent(0)
{
    Init(texture);
}

ProgressBar::~ProgressBar()
{
}

void ProgressBar::Init(const Graphic::GL::Texture &texture)
{
    _colorLeft.Init(0, 1, 0);
    _colorRight.Init(0, 1, 0);
    _nbEvolution = 1;

    // creation des drawable
    _indexDrawable = _drawables.size();
    GL::GeometryBuffer<GL::DefaultVertexType::Colored2d, false> *geometry1 = new GL::GeometryBuffer<GL::DefaultVertexType::Colored2d, false>(GL::Enum::TriangleStrip);
    geometry1->VBO().Init(4, GL::Enum::DataBuffer::StreamDraw);
    _drawables.push_back(new GL::Drawable(geometry1));

    GL::GeometryBuffer<GL::DefaultVertexType::Textured2d, false> *geometry2 = new GL::GeometryBuffer<GL::DefaultVertexType::Textured2d, false>(GL::Enum::TriangleStrip);
    geometry2->VBO().Init(4, GL::Enum::DataBuffer::StreamDraw);
    GL::Drawable *dr = new GL::Drawable(geometry2);
    dr->Config->Textures.InitSize(1);
    dr->Config->Textures[0] = texture;
    dr->Config->GetBlend().SetPattern(GL::Blend::Alpha);
    _drawables.push_back(dr);
    ChooseDefaultMaterial();
}

void ProgressBar::UpdateState()
{
    Widget::UpdateState();
    Array<GL::DefaultVertexType::Colored2d, 4>   verticesProgress;
    verticesProgress[0].Fill(_progressBox.Min(0), _progressBox.Min(1), _colorLeft);
    verticesProgress[1].Fill(_progressBox.Min(0) + (_percent * _progressBox.Length(0) / 100.f), _progressBox.Min(1), _colorRight);
    verticesProgress[2].Fill(_progressBox.Min(0), _progressBox.Max(1), _colorLeft);
    verticesProgress[3].Fill(_progressBox.Min(0) + (_percent * _progressBox.Length(0) / 100.f), _progressBox.Max(1), _colorRight);
    static_cast<GL::GeometryBuffer<GL::DefaultVertexType::Colored2d, false>*>(_drawables[_indexDrawable]->Geometry)->VBO().UpdateData(verticesProgress.data);

    Color   c(1,1,1);
    Array<GL::DefaultVertexType::Textured2d, 4>   verticesTexture;
    verticesTexture[0].Fill(0, 0, 0, 0, c);
    verticesTexture[1].Fill(_size.data[0], 0, 1, 0, c);
    verticesTexture[2].Fill(0, _size.data[1], 0, 1, c);
    verticesTexture[3].Fill(_size.data[0], _size.data[1], 1, 1, c);
    static_cast<GL::GeometryBuffer<GL::DefaultVertexType::Textured2d, false>*>(_drawables[_indexDrawable+1]->Geometry)->VBO().UpdateData(verticesTexture.data);
}

void ProgressBar::Draw(SceneGraph *scene)
{
    Widget::Draw(scene);
    GetMaterial()->Render(scene, *_drawables[_indexDrawable]);
    GetMaterial()->Render(scene, *_drawables[_indexDrawable+1]);
}
