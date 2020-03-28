/*
 * Capstone Engine developed by Alec Roberts and Steven Cole.
 * Started Jan 22 2020
 */

#include <iostream>
using namespace std;

//we only need to include the Engine.h file
#include "Engine/CapstoneEngine.h"

void boxBehavior(Entity*);
void lineBehavior(Entity*);
void cameraBehavior(Entity*);
void spriteBehavior(Entity*);

//Define commandline parameters for SDL2
int main(int, char**) {
	//create a simple instance of the engine
	Engine* mainEng = new Engine();
	mainEng->makeWindow(640, 480, "Hello, World!");	//create the window for the engine
	mainEng->setSize(1920/2, 1080/2);	//testing for setSize
	//we want to use a different resolution for the scene
	mainEng->setResolution(1920/4,1080/4);

	//set up an error log
	//ErrorLog log;
	//log.logError("Too much fun");
	//log.popupError();

	//SIMPLE TEST OF THE JOYSTICK
	Joystick testjoy;
	mainEng->addJoystick(&testjoy); //now add the joystick to the engine

	//create a scene instance
	Scene* scene1 = new Scene(mainEng);

	//create an entity
	Entity* newbox = new Box;

	//give the entity something to do
	newbox->giveJoystick(&testjoy);
	newbox->setBehavior(boxBehavior);

	//we want to modify the box's priority so it looks correct when moving it around the screen with the camera's focus
	newbox->setPriority(1);

	//give the box to the scene
	scene1->addEntity(newbox);

	//create a line
	Entity* newline = new Line;

	//also give the line the box
	//newline->coupleEntity(newbox); //commented out due to testing
	//newline->decoupleEntity(newbox);
	newline->attachEntity(newbox);
	//newline->detachEntity(newbox);

	//give the line something to do
	newline->setBehavior(lineBehavior);

	//give the line priority 2 so the camera looks correct when moving around
	newline->setPriority(2);

	//give the scene the line
	scene1->addEntity(newline);

	//create a camera
	Entity* newcamera = new Camera();

	//give the camera the box
	newcamera->attachEntity(newbox);

	//give the scene the camera and then make it the main camera
	scene1->addEntity(newcamera);
	scene1->setActiveCamera(static_cast<Camera*>(newcamera));

	//set the camera's size to the renderer
	static_cast<Camera*>(newcamera)->sizeToRenderer();
	static_cast<Camera*>(newcamera)->focusTo(-20,0);

	//give the camera its behavior
	newcamera->setBehavior(cameraBehavior);

	Sprite* newsprite = new Sprite;
	newsprite->loadImage("num.png");
	newsprite->setSize(16,16);
	newsprite->setFrameCount(2,2);
	newsprite->setFrame(0);
	newsprite->setBehavior(spriteBehavior);
	//newsprite->activateHorizontalFlip();
	newsprite->activateVerticalFlip();

	scene1->addEntity(newsprite);

	if (newsprite->collideAgainst(16,16, 16, 16)){
		cout << "Collission occured" << endl;
	}


	//Now fullscreen the window
	//mainEng->fullscreenWindow();

	//TODO This is a test of the audio so we don't need it until the next milestone
	//This is our code for testing audio
	/*
	Mix_Music* musicdata = nullptr; //We load in our music data to a pointer
	musicdata = Mix_LoadMUS("robomb.wav");	//load in the music (in this case a WAV created by me)
	if (!musicdata){	//if the music data was not found, we can display a message in the terminal for now
		cout << "could not find music" << endl;
	}
	Mix_VolumeMusic(50);	//lower the volume a bit, it's quite loud!
	*/




	//create the while loop for the 'game' logic
	while(mainEng->getRunning()){

		scene1->execute();
	}

	/*
	//we can delete the music now that we're done with it
	if (musicdata){
		Mix_FreeMusic(musicdata); //because of the nature of the file, we need a specific function to free it
	}
	*/

	//delete any scenes
	delete scene1;

	//clean up the engine (this will clean it up on its own)
	delete mainEng;

	return 0;
}

void boxBehavior(Entity* b){
	Box* temp = static_cast<Box*>(b);

	//let's make the box red
	temp->setColor(255,0,0);

	//let's also make the box semitransparent
	temp->setTransparency(155);

	//draw the box
	temp->draw();

	//save the position of the square into variables
	int x = temp->getX(), y = temp->getY();

	//create static variables for bouncing the cube around the screen
	static int bouncex = 1, bouncey = 1;

	//make conditions for if the cube hits the boundary of the resolution
	if (x > (signed)b->getEngine()->getResW()){
		bouncex = -1;
	}
	if (x < 0){
		bouncex = 1;
	}
	if (y > (signed)b->getEngine()->getResH()){
		bouncey = -1;
	}
	if (y < 0){
		bouncey = 1;
	}

	//always increment the ball by the amount of the bonuce variables
	x+=bouncex;
	y+=bouncey;


	/*code no longer needed, but can be used to control the cube in place of the above code
	//alternate between keyboard and joystick based on what's available
	if (temp->getJoystick() && temp->getJoystick()->getPluggedIn()){
		//allow us to modify those variables with the keyboard
		if (temp->getJoystick()->getLeftStickY() > 100)
			y++;
		if (temp->getJoystick()->getLeftStickY() < -100)
			y--;
		if (temp->getJoystick()->getLeftStickX() < -100)
			x--;
		if (temp->getJoystick()->getLeftStickX() > 100)
			x++;
	}
	else{
		//allow us to modify those variables with the keyboard
		if (Engine::getKey(SDL_SCANCODE_DOWN))
			y++;
		if (Engine::getKey(SDL_SCANCODE_UP))
			y--;
		if (Engine::getKey(SDL_SCANCODE_LEFT))
			x--;
		if (Engine::getKey(SDL_SCANCODE_RIGHT))
			x++;
	}
	*/



	//put those modified variables into the box
	temp->setPosition(x, y);
	temp->setSize(10, 10); //make the box bigger



	//A possibly better way of handling this code would be to create an inherited object out of the box
	//This will remove the need for any static variables
}


void lineBehavior(Entity* e){
	//I want to create four lines that extend to each corner of the renderer and attach to the box at each corner
	Line* temp = static_cast<Line*>(e); //store the line in a temporary variable
	Box* attachedBox = static_cast<Box*>(e->getAttachedEntity((unsigned int)0)); //store the box we attached to the line to a variable

	//let's make the line yellow
	temp->setColor(255,255,0);

	//let's also make the line semitransparent
	temp->setTransparency(155);

	//we only want the lines to be drawn if the box is attached to our line
	if (attachedBox){
		//now set the line's position
		temp->setLinePosition(0,0,attachedBox->getX(),attachedBox->getY());
		//and draw
		temp->draw();

		//repeat the last two calls for each corner to demonstrate flexibility
		//now set the line's position
		temp->setLinePosition(temp->getEngine()->getResW(),0,attachedBox->getX() + attachedBox->getW(),attachedBox->getY());
		//and draw
		temp->draw();

		//now set the line's position
		temp->setLinePosition(temp->getEngine()->getResW(),temp->getEngine()->getResH(),attachedBox->getX() + attachedBox->getW(),attachedBox->getY() + attachedBox->getH());
		//and draw
		temp->draw();

		//now set the line's position
		temp->setLinePosition(0,temp->getEngine()->getResH(),attachedBox->getX(),attachedBox->getY() + attachedBox->getH());
		//and draw
		temp->draw();
	}
}

void cameraBehavior(Entity* c){
	//create a temporary pointer in order to store the camera for easy access
	Camera* temp = static_cast<Camera*>(c);
	//create a temporary box pointer for the same purpose
	Box* tempbox = static_cast<Box*>(temp->getAttachedEntity(0));

	//now just focus the camera to the center of the box
	temp->focusTo((tempbox->getX())+((signed)tempbox->getW()/2)+(temp->getW()/2),(tempbox->getY()+((signed)tempbox->getH() / 2)+(temp->getH()/2)));
}

void spriteBehavior(Entity* s){
	//we just want to demonstrate things like rotation
	Sprite* temp = static_cast<Sprite*>(s);
	static int t = 0;
	temp->setRotation(t++);
	temp->draw();


}
