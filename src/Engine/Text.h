#ifndef ENGINE_TEXT_H_
#define ENGINE_TEXT_H_
#include "Tile.h"

class Text : public Image, public Entity{

protected:
	std::string string_to_draw; //the string that will be drawn to the string
	int x, y; //the position of the text
	int w, h; //the width and height of the text
	std::vector<Tile*> tiles; //the tiles stored in the text string
	bool hasfont; //determines whether or not a font is loaded
	static void defaultBehavior(Entity*);


public:
	void setString(std::string); //sets the string of the text
	void clearString(); //clears the string of the text
	std::string getString(); //retrieves the string stored in the object
	void drawText(); //draws the text loaded into the map
	void setSize(int, int); //sets the size of the font
	void setPosition(int, int); //sets the position of the text

	//text will be 16x6

	void execute(); //code that is executed in the scene

	Text();
	virtual ~Text();
};

#endif /* ENGINE_TEXT_H_ */
