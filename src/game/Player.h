//the player is a sprite and therefore inherits from that object

#ifndef GAME_PLAYER_H_
#define GAME_PLAYER_H_
#include "../Engine/Sprite.h"
#include "../Engine/Tilemap.h"
#include "../Engine/Line.h"
#include "../Engine/Text.h"
#include "../Engine/Sound.h"

class Player : public Sprite{
private:
	//we need behaviors that reflect the state the player is in
	static void control(Entity*); //this is the normal control state
	static void die(Entity*); //this is the death state when the player dies
	static void setup(Entity*); //this is the state that sets up the player and its variables

	//we need to have a few pointers to keep track of things
	Line* laser; //the laser projectile the player will launch
	Tilemap* walls; //the walls that the player must be aware of
	Sound* lasersound; //the sound the player makes when a laser is fired
	Sound* deathsound; //the sound the player makes when dying

	//animation values
	const int walkDelayMax = 5; //determines how long the frames must wait before the walk cycle
	int walkDelayTimer; //the delay timer
	int walkFrame; //the actual frame
	const int deathwait = 50; //the amount of time the player must wait once dead
	int waittime; //the amount of time that has elapsed since death

	//laser values
	bool laserLeft; //this is a value that determines how the laser moves
	bool laserRight; //this is a value that determines how the laser moves
	int laserX, laserY; //this is the position of the laser
	int laserStartX, laserStartY; //the starting position of the laser
	bool fired; //determines whether or not a laser has been fired
	bool savedDirection; //left is false, right is true
	int speed; //the speed of the player

	int score; //the score of the player

public:
	void moveDown(); //moves the player down
	void moveUp(); //moves the player up
	void moveLeft(); //moves the player left
	void moveRight(); //moves the player right
	void shootLaser(); //shoots a laser
	void kill(); //kills the player, used by enemies
	void run(); //makes the player run if the shift key is held
	int getLaserX() { return this->laserX; }; //returns the laser's x position
	int getLaserY() { return this->laserY; }; //returns the laser's y position

	void updateScore(int u) { this->score+=u; }; //updates the player's score
	int getScore() { return this->score; }; //returns the player's score

	void wallIs(Tilemap*); //gives the player the wall info

	void execute(); //execute the stored behavior

	Player();
	virtual ~Player();
};

#endif /* GAME_PLAYER_H_ */
