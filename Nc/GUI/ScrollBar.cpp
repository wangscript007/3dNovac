
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

    File Created At:        02/28/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include <Nc/Graphics/Object/Sprite.h>
#include "ScrollBar.h"
#include "WindowStyle.h"

#define NC_GUI_LOAD_SCROLLBAR_SPRITE(sprite, name)  \
    sprite = WindowStyle::Instance().GetNewSprite(name);  \
    if (sprite == NULL)  \
        throw Utils::Exception("ScrollBar", "Cannot get the sprite '" + name + "' from the WindowStyle");


using namespace Nc;
using namespace Nc::Graphic;
using namespace Nc::GUI;

ScrollBar::ScrollBar(Corner x, Corner y, const Vector2i &pos, int length, Orientation orientation, const std::string &lookName)
    : Widget(x, y, pos, Vector2i(0,0)), _orientation(orientation), _totalSize(0), _pageSize(0), _position(0), _buttonLeftPressed(false), _buttonRightPressed(false), _buttonSliderPressed(false)
{
    // load and configure every sprites
    NC_GUI_LOAD_SCROLLBAR_SPRITE(_spriteLeftButton, lookName + WindowStyle::SpriteName::ScrollBarLeftButton);
    NC_GUI_LOAD_SCROLLBAR_SPRITE(_spriteRightButton, lookName + WindowStyle::SpriteName::ScrollBarRightButton);
    NC_GUI_LOAD_SCROLLBAR_SPRITE(_spriteSliderBackground, lookName + WindowStyle::SpriteName::ScrollBarSliderBackground);
    NC_GUI_LOAD_SCROLLBAR_SPRITE(_spriteSliderLeft, lookName + WindowStyle::SpriteName::ScrollBarSliderLeft);
    NC_GUI_LOAD_SCROLLBAR_SPRITE(_spriteSliderMiddle, lookName + WindowStyle::SpriteName::ScrollBarSliderMiddle);
    NC_GUI_LOAD_SCROLLBAR_SPRITE(_spriteSliderRight, lookName + WindowStyle::SpriteName::ScrollBarSliderRight);
    NC_GUI_LOAD_SCROLLBAR_SPRITE(_spriteSliderExpand1, lookName + WindowStyle::SpriteName::ScrollBarSliderExpand);
    NC_GUI_LOAD_SCROLLBAR_SPRITE(_spriteSliderExpand2, lookName + WindowStyle::SpriteName::ScrollBarSliderExpand);

    NC_GUI_LOAD_SCROLLBAR_SPRITE(_spriteLeftButtonPressed, lookName + WindowStyle::SpriteName::ScrollBarLeftButton + "Pressed");
    NC_GUI_LOAD_SCROLLBAR_SPRITE(_spriteRightButtonPressed, lookName + WindowStyle::SpriteName::ScrollBarRightButton + "Pressed");
    NC_GUI_LOAD_SCROLLBAR_SPRITE(_spriteSliderLeftPressed, lookName + WindowStyle::SpriteName::ScrollBarSliderLeft + "Pressed");
    NC_GUI_LOAD_SCROLLBAR_SPRITE(_spriteSliderMiddlePressed, lookName + WindowStyle::SpriteName::ScrollBarSliderMiddle + "Pressed");
    NC_GUI_LOAD_SCROLLBAR_SPRITE(_spriteSliderRightPressed, lookName + WindowStyle::SpriteName::ScrollBarSliderRight + "Pressed");
    NC_GUI_LOAD_SCROLLBAR_SPRITE(_spriteSliderExpand1Pressed, lookName + WindowStyle::SpriteName::ScrollBarSliderExpand + "Pressed");
    NC_GUI_LOAD_SCROLLBAR_SPRITE(_spriteSliderExpand2Pressed, lookName + WindowStyle::SpriteName::ScrollBarSliderExpand + "Pressed");

    // set the real size of the scroll bar
    _size = (orientation == Vertical)   ? Vector2i(_spriteLeftButton->TextureBox().Length()[1], length)
                                        : Vector2i(length, _spriteLeftButton->TextureBox().Length()[1]);
}

ScrollBar::~ScrollBar()
{
    delete _spriteLeftButton;
    delete _spriteRightButton;
    delete _spriteSliderBackground;
    delete _spriteSliderLeft;
    delete _spriteSliderMiddle;
    delete _spriteSliderRight;
    delete _spriteSliderExpand1;
    delete _spriteSliderExpand2;

    delete _spriteLeftButtonPressed;
    delete _spriteRightButtonPressed;
    delete _spriteSliderLeftPressed;
    delete _spriteSliderMiddlePressed;
    delete _spriteSliderRightPressed;
    delete _spriteSliderExpand1Pressed;
    delete _spriteSliderExpand2Pressed;
}

ScrollBar::ScrollBar(const ScrollBar &sc)
{
    Copy(sc);
}

ScrollBar &ScrollBar::operator = (const ScrollBar &sc)
{
    Widget::operator = (sc);
    Copy(sc);
    return *this;
}

void ScrollBar::Copy(const ScrollBar &sc)
{
    _orientation = sc._orientation;
    _totalSize = sc._totalSize;
    _pageSize = sc._pageSize;
    _position = sc._position;
    _buttonLeftPressed = sc._buttonLeftPressed;
    _buttonRightPressed = sc._buttonRightPressed;
    _buttonSliderPressed = sc._buttonSliderPressed;
    _spriteLeftButton = (Graphic::Sprite*)sc._spriteLeftButton->Clone();
    _spriteRightButton = (Graphic::Sprite*)sc._spriteRightButton->Clone();
    _spriteSliderBackground = (Graphic::Sprite*)sc._spriteSliderBackground->Clone();
    _spriteSliderLeft = (Graphic::Sprite*)sc._spriteSliderLeft->Clone();
    _spriteSliderMiddle = (Graphic::Sprite*)sc._spriteSliderMiddle->Clone();
    _spriteSliderRight = (Graphic::Sprite*)sc._spriteSliderRight->Clone();
    _spriteSliderExpand1 = (Graphic::Sprite*)sc._spriteSliderExpand1->Clone();
    _spriteSliderExpand2 = (Graphic::Sprite*)sc._spriteSliderExpand2->Clone();
    _spriteLeftButtonPressed = (Graphic::Sprite*)sc._spriteLeftButtonPressed->Clone();
    _spriteRightButtonPressed = (Graphic::Sprite*)sc._spriteRightButtonPressed->Clone();
    _spriteSliderLeftPressed = (Graphic::Sprite*)sc._spriteSliderLeftPressed->Clone();
    _spriteSliderMiddlePressed = (Graphic::Sprite*)sc._spriteSliderMiddlePressed->Clone();
    _spriteSliderRightPressed = (Graphic::Sprite*)sc._spriteSliderRightPressed->Clone();
    _spriteSliderExpand1Pressed = (Graphic::Sprite*)sc._spriteSliderExpand1Pressed->Clone();
    _spriteSliderExpand2Pressed = (Graphic::Sprite*)sc._spriteSliderExpand2Pressed->Clone();
}

void ScrollBar::MouseMotionEvent(const Nc::System::Event &event)
{
    if (_buttonSliderPressed)
    {
        float diff = (_orientation == Vertical) ? -(_lastPosMouse[1] - event.mouseMove.Y) : (event.mouseMove.X - _lastPosMouse[0]);

        _position = GetPagePosition(GetSliderTranslation(_position) + diff);

        _lastPosMouse[0] = event.mouseMove.X;
        _lastPosMouse[1] = event.mouseMove.Y;
        _stateChanged = true;
    }
}

void ScrollBar::MouseButtonEvent(const Nc::System::Event &event)
{
    if (event.mouseButton.Button == System::Mouse::Left)
    {
        MouseButtonLeft(event);
        MouseButtonRight(event);
        MouseButtonSlider(event);
    }
}

void    ScrollBar::MouseButtonLeft(const Nc::System::Event &event)
{
    bool inRect = false;
    Vector2i pos;
    Vector2i size;
    Vector2i mousePos = WindowInput::MousePositionInGLCoord(static_cast<Graphic::WindowInput*>(event.emitter)->AttachedWindow()->Height());
    GetReelPosRecursif(pos);

    if (_orientation == Vertical)
    {
        pos[1] += _size[1];
        size[0] = _spriteRightButton->TextureBox().Length()[0];
        size[1] = -_spriteRightButton->TextureBox().Length()[1];
    }
    else
        size = _spriteLeftButton->TextureBox().Length();

    if (Math::InRect(pos, size, mousePos))
        inRect = true;

    if (inRect && !_buttonLeftPressed && event.type == System::Event::MouseButtonPressed)
        _buttonLeftPressed = true;
    else if (_buttonLeftPressed && event.type == System::Event::MouseButtonReleased)
    {
        if (inRect && _position > 0)
        {
            _position--;
            _stateChanged = true;
        }
        _buttonLeftPressed = false;
    }
}

void    ScrollBar::MouseButtonRight(const Nc::System::Event &event)
{
    bool inRect = false;
    Vector2i pos;
    Vector2i size;
    Vector2i mousePos = WindowInput::MousePositionInGLCoord(static_cast<Graphic::WindowInput*>(event.emitter)->AttachedWindow()->Height());
    GetReelPosRecursif(pos);

    if (_orientation == Vertical)
    {
        pos[1] += _spriteRightButton->Size()[0];
        size[0] = _spriteRightButton->TextureBox().Length()[0];
        size[1] = -_spriteRightButton->TextureBox().Length()[1];
    }
    else
    {
        pos[0] += (_size[0] - _spriteRightButton->TextureBox().Length()[0]);
        size = _spriteRightButton->TextureBox().Length();
    }

    if (Math::InRect(pos, size, mousePos))
        inRect = true;

    if (inRect && !_buttonRightPressed && event.type == System::Event::MouseButtonPressed)
        _buttonRightPressed = true;
    else if (_buttonRightPressed && event.type == System::Event::MouseButtonReleased)
    {
        if (inRect && _position < _totalSize - _pageSize)
        {
            _position++;
            _stateChanged = true;
        }
        _buttonRightPressed = false;
    }
}

void    ScrollBar::MouseButtonSlider(const Nc::System::Event &event)
{
    bool inRect = false;
    Vector2i pos;
    Vector2i size;
    Vector2i mousePos = WindowInput::MousePositionInGLCoord(static_cast<Graphic::WindowInput*>(event.emitter)->AttachedWindow()->Height());
    GetReelPosRecursif(pos);

    if (_orientation == Vertical)
    {
        pos[1] += _size[1] - _spriteLeftButton->TextureBox().Length()[0] - GetSliderTranslation(_position);
        size[1] = -GetSliderSize();
        size[0] = _spriteSliderMiddle->TextureBox().Length()[1];
    }
    else
    {
        pos[0] += _spriteLeftButton->TextureBox().Length()[0] + GetSliderTranslation(_position);
        size[0] = GetSliderSize();
        size[1] = _spriteSliderMiddle->TextureBox().Length()[1];
    }

    if (Math::InRect(pos, size, mousePos))
        inRect = true;

    if (inRect && !_buttonSliderPressed && event.type == System::Event::MouseButtonPressed)
    {
        _lastPosMouse = WindowInput::MousePosition();
        _buttonSliderPressed = true;
    }
    else if (_buttonSliderPressed && event.type == System::Event::MouseButtonReleased)
    {
        _buttonSliderPressed = false;
    }
}

float   ScrollBar::GetSliderSize()
{
    float s;
    if (_orientation == Vertical)
        s = _size[1] - _spriteLeftButton->Size()[0]  - _spriteRightButton->Size()[0];
    else
        s = _size[0] - _spriteLeftButton->Size()[0]  - _spriteRightButton->Size()[0];

    if (_totalSize != 0 && _pageSize != 0)
        s = _pageSize * s / _totalSize;
    return s;
}

float   ScrollBar::GetSliderTranslation(float position)
{
    float s;
    if (_orientation == Vertical)
        s = _size[1] - _spriteLeftButton->Size()[0]  - _spriteRightButton->Size()[0];
    else
        s = _size[0] - _spriteLeftButton->Size()[0]  - _spriteRightButton->Size()[0];
    float p = position * s / _totalSize;
    s -= GetSliderSize();
    if (p < 0)
        return 0;
    else if (p > s)
        return s;
    return p;
}

float ScrollBar::GetPagePosition(float translation)
{
    float s;
    if (_orientation == Vertical)
        s = _size[1] - _spriteLeftButton->Size()[0]  - _spriteRightButton->Size()[0];
    else
        s = _size[0] - _spriteLeftButton->Size()[0]  - _spriteRightButton->Size()[0];
    float t = translation * _totalSize / s;
    if (t < 0)
        return 0;
    else if (t > _totalSize - _pageSize)
        return _totalSize - _pageSize;
    return t;
}

void ScrollBar::Update()
{
    float s = GetSliderSize();
    float t = GetSliderTranslation(_position);

    float s2 = s - _spriteSliderLeft->Size()[0] - _spriteSliderMiddle->Size()[0] - _spriteSliderRight->Size()[0];
    s2 /= 2;

    if (_orientation == Vertical)
    {
        _spriteSliderBackground->Size(Vector2f(_size[1] - _spriteRightButton->Size()[0] - _spriteLeftButton->Size()[0], _spriteSliderBackground->Size()[1]));
        _spriteSliderBackground->Matrix.Translation(0, _size[1] - _spriteLeftButton->Size()[0], 0);

        _spriteSliderLeft->Matrix.Translation(0, _size[1] - _spriteLeftButton->Size()[0] - t, 0);
        _spriteLeftButton->Matrix.Translation(0, _size[1], 0);

        if (s2 > _spriteSliderExpand1->TextureBox().Length()[0])
            _spriteSliderExpand1->Size(Vector2f(s2, _spriteSliderExpand1->Size()[1]));
        else
            _spriteSliderExpand1->Size(Vector2f(_spriteSliderExpand1->TextureBox().Length()[0], _spriteSliderExpand1->Size()[1]));
        _spriteSliderExpand1->Matrix.Translation(0, _size[1] - _spriteLeftButton->Size()[0] - t - _spriteSliderLeft->Size()[0], 0);


        _spriteSliderMiddle->Matrix.Translation(0, _size[1] - _spriteLeftButton->Size()[0] - t - _spriteSliderLeft->Size()[0] - _spriteSliderExpand1->Size()[0], 0);

        if (s2 > _spriteSliderExpand2->TextureBox().Length()[0])
            _spriteSliderExpand2->Size(Vector2f(s2, _spriteSliderExpand2->Size()[1]));
        else
            _spriteSliderExpand2->Size(Vector2f(_spriteSliderExpand2->TextureBox().Length()[0], _spriteSliderExpand2->Size()[1]));
        _spriteSliderExpand2->Matrix.Translation(0, _size[1] - _spriteLeftButton->Size()[0] - t - _spriteSliderLeft->Size()[0] - _spriteSliderExpand1->Size()[0] - _spriteSliderMiddle->Size()[0], 0);

        _spriteSliderRight->Matrix.Translation(0, _size[1] - _spriteLeftButton->Size()[0] - t - _spriteSliderLeft->Size()[0] - _spriteSliderExpand1->Size()[0] - _spriteSliderMiddle->Size()[0] - _spriteSliderExpand2->Size()[0] /*_spriteRightButton->Size()[0] + t*/, 0);

        _spriteRightButton->Matrix.Translation(0, _spriteRightButton->Size()[0], 0);
    }
    else
    {
        _spriteSliderBackground->Size(Vector2f(_size[0] - _spriteLeftButton->Size()[0]  - _spriteRightButton->Size()[0], _spriteSliderBackground->Size()[1]));
        _spriteSliderBackground->Matrix.Translation(_spriteLeftButton->Size()[0], 0, 0);

        _spriteSliderLeft->Matrix.Translation(_spriteLeftButton->Size()[0] + t, 0, 0);

        if (s2 > _spriteSliderExpand1->TextureBox().Length()[0])
            _spriteSliderExpand1->Size(Vector2f(s2, _spriteSliderExpand1->Size()[1]));
        else
            _spriteSliderExpand1->Size(Vector2f(_spriteSliderExpand1->TextureBox().Length()[0], _spriteSliderExpand1->Size()[1]));
        _spriteSliderExpand1->Matrix.Translation(_spriteLeftButton->Size()[0] + t + _spriteSliderLeft->Size()[0], 0, 0);


        _spriteSliderMiddle->Matrix.Translation(_spriteLeftButton->Size()[0] + t + _spriteSliderLeft->Size()[0] + _spriteSliderExpand1->Size()[0], 0, 0);

        if (s2 > _spriteSliderExpand2->TextureBox().Length()[0])
            _spriteSliderExpand2->Size(Vector2f(s2, _spriteSliderExpand2->Size()[1]));
        else
            _spriteSliderExpand2->Size(Vector2f(_spriteSliderExpand2->TextureBox().Length()[0], _spriteSliderExpand2->Size()[1]));
        _spriteSliderExpand2->Matrix.Translation(_spriteLeftButton->Size()[0] + t + _spriteSliderLeft->Size()[0] + _spriteSliderExpand1->Size()[0] + _spriteSliderMiddle->Size()[0], 0, 0);

        _spriteSliderRight->Matrix.Translation(_spriteLeftButton->Size()[0] + t + _spriteSliderLeft->Size()[0] + _spriteSliderExpand1->Size()[0] + _spriteSliderMiddle->Size()[0] + _spriteSliderExpand2->Size()[0], 0, 0);
        _spriteRightButton->Matrix.Translation(_size[0] - _spriteRightButton->Size()[0], 0, 0);
    }


    _spriteLeftButtonPressed->Size(_spriteLeftButton->Size());
    _spriteLeftButtonPressed->Matrix = _spriteLeftButton->Matrix;
    _spriteSliderLeftPressed->Size(_spriteSliderLeft->Size());
    _spriteSliderLeftPressed->Matrix = _spriteSliderLeft->Matrix;
    _spriteSliderExpand1Pressed->Size(_spriteSliderExpand1->Size());
    _spriteSliderExpand1Pressed->Matrix = _spriteSliderExpand1->Matrix;
    _spriteSliderMiddlePressed->Size(_spriteSliderMiddle->Size());
    _spriteSliderMiddlePressed->Matrix = _spriteSliderMiddle->Matrix;
    _spriteSliderExpand2Pressed->Size(_spriteSliderExpand2->Size());
    _spriteSliderExpand2Pressed->Matrix = _spriteSliderExpand2->Matrix;
    _spriteSliderRightPressed->Size(_spriteSliderRight->Size());
    _spriteSliderRightPressed->Matrix = _spriteSliderRight->Matrix;
    _spriteRightButtonPressed->Size(_spriteRightButton->Size());
    _spriteRightButtonPressed->Matrix = _spriteRightButton->Matrix;
}

void ScrollBar::Draw(Graphic::SceneGraph *scene)
{
    bool push = false;
    if (_orientation == Vertical)
    {
        push = true;

        scene->PushModelMatrix();
        TMatrix r;
        r.Rotation(Vector3f(0, 0, 1), -90, false);
        scene->ModelMatrix() *= r;
    }

    if (_buttonLeftPressed)
        _spriteLeftButtonPressed->RenderNode(scene);
    else
        _spriteLeftButton->RenderNode(scene);

    _spriteSliderBackground->RenderNode(scene);

    if (_buttonSliderPressed)
    {
        _spriteSliderLeftPressed->RenderNode(scene);
        _spriteSliderExpand1Pressed->RenderNode(scene);
        _spriteSliderMiddlePressed->RenderNode(scene);
        _spriteSliderExpand2Pressed->RenderNode(scene);
        _spriteSliderRightPressed->RenderNode(scene);
    }
    else
    {
        _spriteSliderLeft->RenderNode(scene);
        _spriteSliderExpand1->RenderNode(scene);
        _spriteSliderMiddle->RenderNode(scene);
        _spriteSliderExpand2->RenderNode(scene);
        _spriteSliderRight->RenderNode(scene);
    }

    if (_buttonRightPressed)
        _spriteRightButtonPressed->RenderNode(scene);
    else
        _spriteRightButton->RenderNode(scene);

    if (push)
        scene->PopModelMatrix();
}

