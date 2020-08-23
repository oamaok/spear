#pragma once

#include "client/gui/Gui.h"
#include "client/GuiCard.h"

namespace cl { namespace gui {

struct WidgetCard : WidgetBase<'c','a','r','d'>
{
	sf::Box<GuiCard> card;
	bool dragged = false;

	virtual void layout(GuiLayout &layout, const sf::Vec2 &min, const sf::Vec2 &max) override;
	virtual void paint(GuiPaint &paint) override;
	virtual bool onPointer(GuiPointer &pointer) override;
};

} }
