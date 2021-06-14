#include "guis/GuiUserSelect.h"
#include "UserConfig.h"

GuiUserSelect::GuiUserSelect(Window* window) : GuiComponent(window), mMenu(window, "USER SELECT")
{
    std::map<std::string, User> users = *UserConfig::get()->getUsers();

    typedef std::map<std::string, User>::const_iterator it_type;
    for(it_type iterator = users.cbegin(); iterator != users.cend(); iterator++)
    {
        addEntry(iterator->second.fullName.c_str(), 0x777777FF, true, [this, iterator] { selectUser(iterator->first.c_str()); });
    }

	addChild(&mMenu);
	setSize(mMenu.getSize());
	setPosition((Renderer::getScreenWidth() - mSize.x()) / 2, Renderer::getScreenHeight() * 0.15f);
}

void GuiUserSelect::selectUser(const std::string userName)
{
    UserConfig::get()->setActiveUser(userName);
    UserConfig::get()->writeToXML();
    delete this;
}

void GuiUserSelect::addEntry(const char* name, unsigned int color, bool add_arrow, const std::function<void()>& func)
{
	std::shared_ptr<Font> font = Font::get(FONT_SIZE_MEDIUM);

	// populate the list
	ComponentListRow row;
	row.addElement(std::make_shared<TextComponent>(mWindow, name, font, color), true);

	if(add_arrow)
	{
		std::shared_ptr<ImageComponent> bracket = makeArrow(mWindow);
		row.addElement(bracket, false);
	}

	row.makeAcceptInputHandler(func);

	mMenu.addRow(row);
}

bool GuiUserSelect::input(InputConfig* config, Input input)
{
    if(GuiComponent::input(config, input))
        return true;

    if((config->isMappedTo("b", input) || config->isMappedTo("hotkeyenable", input)) && input.value != 0)
    {
        delete this;
        return true;
    }

    return false;
}