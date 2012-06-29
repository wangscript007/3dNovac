
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

#ifndef SAMPLEVIEWER_MAINMENU_H_
#define SAMPLEVIEWER_MAINMENU_H_

#include <Nc/GUI/GUI.h>
#include <Nc/GUI/WidgetSubWindow.h>
#include <Nc/GUI/TextArea.h>

namespace SampleViewer
{
    class MainMenu : Nc::Utils::NonCopyable
    {
        public:
            MainMenu(Nc::GUI::SceneGraph *gui);
            ~MainMenu();

            void                        AddSample(const std::string &name);

            const std::string           *Sample() const         {return (_sampleComboBox->CurrentItem()) ? &_sampleComboBox->CurrentItem()->Data() : NULL;}

            Nc::GUI::WidgetSubWindow    *CreateSampleWindow(Nc::Graphic::Window *windowParent);

            void                        CloseSampleWindow();

        private:
            Nc::GUI::Widget             *CreateDescriptionSampleWindow(Nc::GUI::Layout *parent);
            Nc::GUI::Widget             *CreateSelectSampleWindow(Nc::GUI::Layout *parent);

        private:
            Nc::GUI::SceneGraph         *_GUI;
            Nc::GUI::ComboBox           *_sampleComboBox;
            Nc::GUI::TextArea           *_descriptionTextArea;

            Nc::GUI::Widget             *_widgetSampleWindow;
            Nc::GUI::WidgetSubWindow    *_currentSampleWindow;
    };
}

#endif
