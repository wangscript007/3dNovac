
/*-----------------------------------------------------------------------------

	3dNovac GUIDemo
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

    File Created At:        27/09/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include <Nc/GUI/ScrollArea.h>
#include <Nc/GUI/ScrollBar.h>
#include <Nc/GUI/ComboBox.h>
#include <Nc/GUI/Looks.h>
#include <Nc/GUI/SubWindow.h>

#include "MainMenu.h"
#include "GameEngine.h"

using namespace Nc;
using namespace Nc::Utils;
using namespace Nc::System;
using namespace Nc::GUI;
using namespace Nc::Graphic;
using namespace SampleViewer;

MainMenu::MainMenu(Nc::GUI::SceneGraph *gui)
    : _GUI(gui)
{
    _currentSampleWindow = NULL;

    // create the main layout
    Layout *mainLayout = new Layout(Layout::Horizontal, Center);
    mainLayout->UseLook(new BoxLook("Background"));
    mainLayout->Percent(Vector2f(100, 100));
    _GUI->AddChild(mainLayout);

    // create the description window
    Widget *descriptionWindow = CreateDescriptionSampleWindow(mainLayout);
    descriptionWindow->Percent(Vector2f(0, 90));

    // create the sample window used to render the samples
    _widgetSampleWindow = new Widget(Center);
    _widgetSampleWindow->UseLook(new BoxLook());
    _widgetSampleWindow->PaddingH(5);
    _widgetSampleWindow->PaddingV(5);
    _widgetSampleWindow->Percent(Vector2f(100, 90));
    mainLayout->AddChild(_widgetSampleWindow);

    // set the auto resizing
    mainLayout->SetExpandRatio(_widgetSampleWindow, 100);

    // create the fps widget on top of the main layout
    _GUI->AddChild(new FPSWidget(Right | Top));

    //_console = new GUI::Console();
    //_scene->AddChild(_console);
}

MainMenu::~MainMenu()
{
}

void    MainMenu::AddSample(const std::string &name)
{
    _sampleComboBox->AddItem(new Item(name));
}

Widget  *MainMenu::CreateDescriptionSampleWindow(Layout *parent)
{
    Layout *descriptionLayout = new Layout(Layout::Vertical, Center, Vector2i(300, 0));
    descriptionLayout->PaddingH(5);
    descriptionLayout->PaddingV(5);
    parent->AddChild(descriptionLayout);

    // create the select window with the combobox and the button
    Widget *windowSelectSample = CreateSelectSampleWindow(descriptionLayout);
    windowSelectSample->Percent(Vector2f(100, 0));

    // create the window description area
    WindowBox *winDescArea = new WindowBox("Description", CenterH | Top);
    winDescArea->MarginTop(5);
    winDescArea->Percent(Vector2f(100, 100));
    descriptionLayout->AddChild(winDescArea);
    descriptionLayout->SetExpandRatio(winDescArea, 100);

    // create the text area to describe the selected sample
    _descriptionTextArea = new TextArea(CenterH | Bottom);
    _descriptionTextArea->Percent(Vector2f(100, 100));
    winDescArea->AddChild(_descriptionTextArea);

    return descriptionLayout;
}

Widget  *MainMenu::CreateSelectSampleWindow(Layout *parent)
{
    Widget *windowSelectSample = new Widget(CenterH | Top, Vector2i(0,60));
    windowSelectSample->UseLook(new BoxLook("Widget"));
    parent->AddChild(windowSelectSample);

    Layout *selectSampleLayout = new Layout(Layout::Horizontal, Center);
    selectSampleLayout->Percent(Vector2f(100, 100));
    windowSelectSample->AddChild(selectSampleLayout);

    _sampleComboBox = new ComboBox(_GUI, Left | CenterV, Vector2i(250,0));
    _sampleComboBox->MarginRight(5);
    _sampleComboBox->Percent(Vector2f(100, 0));
    selectSampleLayout->AddChild(_sampleComboBox);
    selectSampleLayout->SetExpandRatio(_sampleComboBox, 100);

    Button *button = new Button("Start", Center, Vector2i(70, 28), "arial");
    selectSampleLayout->AddChild(button);
    button->HandlerEngineName(GameEngine::ClassName());
    button->HandlerId(GameEngine::StartSample);
    return windowSelectSample;
}

GUI::SubWindow      *MainMenu::CreateSampleWindow(Window *windowParent)
{
    if (_currentSampleWindow != NULL)
        _widgetSampleWindow->RemoveWidget(_currentSampleWindow);

    _currentSampleWindow = new GUI::SubWindow(windowParent, Center);
    _currentSampleWindow->Percent(Vector2f(100, 100));
    _widgetSampleWindow->AddChild(_currentSampleWindow);
    return _currentSampleWindow;
}

void    MainMenu::CloseSampleWindow()
{
    if (_currentSampleWindow != NULL)
        _currentSampleWindow->GetSubWindow()->Close();
}

