#pragma once
#ifndef ES_APP_GUIS_GUI_USERSELECT_H
#define ES_APP_GUIS_GUI_USERSELECT_H

#include "components/MenuComponent.h"
#include "GuiComponent.h"

class GuiUserSelect : public GuiComponent
{
public:
	GuiUserSelect(Window* window);

    bool input(InputConfig* config, Input input) override;

private:
	void addEntry(const char* name, unsigned int color, bool add_arrow, const std::function<void()>& func);
	void selectUser(const std::string userName);

	MenuComponent mMenu;
};

#endif // ES_APP_GUIS_GUI_USERSELECT_H
