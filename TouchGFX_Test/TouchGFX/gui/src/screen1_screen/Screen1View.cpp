#include <gui/screen1_screen/Screen1View.hpp>
#include <touchgfx/Color.hpp>
#include <stdlib.h>

Screen1View::Screen1View()
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::setRandomColor()
{
	if(toggleButton1.getState())
	{
	 box1.setColor(touchgfx::Color::getColorFrom24BitRGB(rand()&0xff, rand()&0xff, rand()&0xff));
	 box1.invalidate();
	}
}

void Screen1View::onSliderValueChange(int value)
{
	Unicode::snprintf(textArea1Buffer,TEXTAREA1_SIZE,"%03d",value);
	textArea1.resizeToCurrentText();
	textArea1.invalidate();
	imageProgress1.setValue(value);
	imageProgress1.invalidate();

}

