// Brick knockout game
// main.cpp
// Version 1.0
// Written by Joshua Kooistra

// Notes
/*
Add more sounds
- Explosion
- Background music
- Magnetic paddle grinding a ball against a wall
- Paddle pushing against a wall
- Fire

Change help screens

Add more bricks

Add more levels
*/

/* Changes for Release 0.11
Changed the code for sound, allowing more than 1 sound to play at a time

Added more sounds
- Laser fire
- Coin collection sounds
- Powerup message announcements
- Added 10 new bricks
*/

// Objective
//
// To create a game where the player completes levels by knocking out bricks with a metal ball.
// The ball is kept in the play field by the player by rebounding the ball off of a paddle at
//   the bottom of the screen. Players gain points for each block that is removed.
// The game finishes when the player runs out of lives. Lives are lost when the there are no
//   balls left on the play field.
// If the player completes all levels then the game cycles through the levels again at increased
//   speed and for increased points.
// Scores are recorded in a high score chart

// Exclude unneeded header files
#define WIN32_LEAN_AND_MEAN

// Include the windows header
#include <Windows.h>

// Include time functions
#include <time.h>

// Includes for sound
//#include <mmsystem.h>
//#pragma comment (lib, "winmm.lib")

// SFML Includes
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

// Include standard library
#include <stdlib.h>

// Include string functions
#include <string.h>
#include <sstream>

// Include generic international functions
#include <tchar.h>

// Include file input/output functions
#include <stdio.h>
#include <iostream>
#include <fstream>

// Include project header files
#include "bitmapobject.h"

// Give the window a name
#define WINDOWCLASS "Brick Knockout Game"

// Give the window a caption
#define WINDOWTITLE "Brick Knockout Game"

// Declare and define constants
const int TILESIZE = 8; // Build the game on 8x8 tiles
const int GAMEHEIGHT = 60; // Game height in tiles
const int GAMEWIDTH = 80; // Game width in tiles
const int BRICKSIZE = 16; // Build the bricks on 16x16 tiles
const int BGAMEHEIGHT = 30; // Game height in bricks
const int BGAMEWIDTH = 40; // Game width in bricks
const int LABEL_EXTRALIVES = 0; // Label number in the Labels.bmp bitmap for extra lives
const int LABEL_SCORE = 1; // Label number in the Labels.bmp bitmap for score
const int LABEL_DIGIT = 2; // Label number in the Labels.bmp bitmap for digits
const int BRICKCOLOURS = 9; // The number of brick colours
const int HELPSCREENS = 5; // The number of help screens
const int KNOCKOUTBALLSIZE = 7; // Size a ball needs to be to knockout grey bricks

// Initial values
const int INITPADDLESIZE = 8; // Default paddle size
const int INITPADDLESPEED = 5; // Default paddle speed

// Powerup Constants
const int INCPADDLESIZE = 1; // Increase paddle size
const int DECPADDLESIZE = 2; // Decrease paddle size
const int INCPADDLESPEED = 3; // Increase paddle speed
const int DECPADDLESPEED = 4; // Decrease paddle speed
const int INCBALLSIZE = 5; // Increase ball sizes
const int DECBALLSIZE = 6; // Decrease ball sizes
const int INCBALLSPEED = 7; // Increase ball speeds  // Not used anymore
const int DECBALLSPEED = 8; // Decrease ball speeds  // Not used anymore
const int EXTRABALL = 9; // Gain an extra ball
const int MAGNETIC = 10; // Gain the magnetic paddle for a duration
const int EXTRALIFE = 11; // Gain an extra life
const int FIREBALL = 12; // Gain the fire ball for a duration
const int GUNS = 13; // Gain guns for a duration
const int EXPLOSIVE = 14; // Gain the explosive ball for a duration
const int NUM_COMMONPOWERUPS = 10; // Number of common powerups
const int NUM_RAREPOWERUPS = 4; // Number of rare powerups
const int POWERUPCHANCE = 3; // Inverse chance of a brick containing a powerup
const int COINSPEED = 5; // Inverse speed at which the coin turns
const int POWERUPTIME = 600; // The time added to powerups that have time limits (divide by 20 for seconds)
const int FIREANIMATION = 6; // Speed of the fireball animation
const int MESSAGETIME = 100; // Time in frames for a message to be displayed (20 frames for second)

// Explosion constants
const int FRAMES = 15; // Number of frames before the explosion reduces in size

// Laser constants
const int LASERSPEED = 6; // Number of frames the laser travels per game update

// Confirmation constants
const int CONFIRMATIONBOXES = 6; // The number of confirmation boxes available
const int CONFIRMQUIT = 1; // Confirm quitting the game
const int CONFIRMNEW = 2; // Confirm starting a new game
const int CONFIRMEDITORSTART = 3; // Confirm starting the level editor
const int CONFIRMEDITORQUIT = 4; // Confirm quitting the level editor
const int CONFIRMEDITORLEVELCHANGE = 5; // Confirm changing from a modified level in the editor
const int CONFIRMRESTORE = 6; // Confirm restoring the original game levels

// Declare functions

// Draw Functions
void DrawGame(); // Draw the game board
void DrawBackground(); // Draw the 640x480 Background
void DrawPaddle(); // Draw an 8x8 Paddle Tile
void DrawBorders(int type); // Draw the borders
void DrawBorder(int x, int y, int tileX, int tileY); // Draw an 8x8 Border Tile
void DrawBalls(); // Draw the 8x8 Ball tiles
void DrawBricks(); // Draw the bricks
void DrawBrick(int x, int y, int tileX, int tileY); // Draw an 16x16 Brick Tile
void DrawBrickCorner(int x, int y, int tileX, int tileY); // Overlay a 4x4 corner on a brick for proper edging
void DrawExtraLives(); // Draws the extra lives
void DrawScore(); // Draws the score in the top right corner
void DrawHelp(); // Draw help screen
void DrawCoins(); // Draw the powerup coins
void DrawExplosions(); // Draw the explosions
void DrawBullets(); // Draw the bullets
void DrawMessages(); // Draw the messages
void DrawConfirmation(); // Draws a confirmation dialog box
void DrawGameMenu(); // Draws the game menu

// Get Functions

int GetPaddleSize(); // Return the paddle size
int GetPaddlePosition(); // Return the paddle position on the board
int GetPaddleSpeed(); // Return the speed of the paddle
int GetPaddleDirection(); // Get the direction the paddle is moving
int GetLevel(); // Get the level number
int GetMaxLevel(); // Returns the max level in the game
int GetLife(); // Returns the number of lives
int GetNumBricks(); // Returns the number of bricks
int GetHelpColour(); // Returns the numeric value for the brick colour used on the help screen
int GetHelpStyle(); // Returns the numeriv value for the style brick used on the help screen
int GetPausedGame(); // Return the value of gamePaused
int GetScore(); // Returns the score
int GetScoreMultiplier(); // Returns the score mulitplier

// Set functions

void AdjustPaddleSize(int size); // Set the paddle size 4 to 12
void AdjustPaddleSpeed(int speedChange); // Change the speed of the paddle
void SetPaddleDirection(int num); // Set the direction the paddle is moving
void MovePaddlePosition(); // Move the paddle position moveX pixels
void ReleaseBall(int num); // Release the ball from the paddle
void MoveBalls(); // Move the corresponding ball
void ChangeLevel(int num); // Advance or retreat num of levels
void SetMaxLevel(); // Retrieves the max level from the levels file
void SetBrickStyles(); // Retrieves the number of brick styles from the levels file
void AdjustBallSize(int num, int sizeChange); // Adjusts ball num's size and map
void LoseBall(int num); // Lose the num ball
void AddBall(); // Gain 1 or 2 extra balls
void UseLife(); // Place a new ball
void AddLife(); // Adds one to the lives if it's not at max
void ResetNumBricks(); // Resets the number of bricks to 0
bool ChangeNumBricks(int num); // Changes the number of bricks and initiates a new level
void ChangeHelpColour(int num); // Changes the value for the brick colour used on the help screen by value num
void ChangeHelpStyle(int num); // Changes the value of the brick style used on the help screen
void AddScore(int x, int y, int brickMultiplier); // Adds to the score based on the block type (x and y), level and multipliers
void ChangeScoreMultiplier(int num); // Changes the score multiplier
void ClearCoins(); // Clear the coin array
void DropCoins(); // Drops the coins 1 step
void AddCoin(int powerup, int x, int y); // Adds a coin to the board up to a maximum of 20
void LoseCoin(int num); // Removes a coin from the array
void GainPowerup(int num); // Gains a powerup
void AddFire(); // Adds fire powerup to the balls, removes explosive
void AddExplosive(); // Adds explosive powerup to the balls, removes fire.
void ExplodeBrick(int num, int x, int y); // Explode the bricks around the recently removed x,y brick based on ball size
void ResetExplosions(); // Reset the explosions
void AddExplosion(int x, int y, int size); // Add an explosion to the game
void RemoveExplosion(int num); // Remove an explosion
void ResetBullets(); // Reset the bullets
void AddBullets(); // Add bullets to the game
void RemoveBullet(int num); // Remove a bullet
void MoveBullets(); // Move the bullets and check for collisions
void AddMessage(int num); // Add a message to the queue
void RemoveMessage(); // Remove the first message
void ClearMessages(); // Clear the message queue
void SetConfirmation(int num); // Set the confirmation box
void ChangeConfirmationAction(int num); // Change the confirmation action
void SelectConfirmationAction(); // Select the current confirmation action
void RestoreLevels(); // Restore the original levels
void QuitGame(); // Releases memory and closes the program

// Load Functions

void LoadBackground(int num); // Load the backgroud for level num
bool LoadLevel(int num); // Load level num
void LoadCoinMap(); // Loads the coin map

// Game functions

bool GameInit(); // Initialise the game
void GameLoop(); // The main game loop
void FinishGame(); // Clean up when the game is done

void StartGame(); // Start a new game
void GameOver(); // All lives are lost
void PauseGame(); // Increment the gamePaused counter
void UnpauseGame(); // Set gamePaused to 0

bool CollisionCheck(int num, int moveX, int moveY); // Checks if the ball collides
	/*
	False = No Collision / Move
	True = Collision / Don't Move
	*/
int MyPower(int base, int power); // Returns base to the power (positive integars only)

// Delcare Global Variables
HINSTANCE mainInstance = NULL; // Handle for the main app
HWND mainWindow = NULL; // Handle for the main window

// Structure for a ball
struct Ball{
	int size; // Between 1 and 7; -1 = no ball
	int x; // Horizontal position of the ball
	int y; // Vertical position of the ball
	int speedX; // Horizontal speed of the ball
	int speedY; // Vertical speed of the ball
	int speedMod; // Not used
	int map[16][16]; // 16x16 pixel map of the ball
	bool stuck; // Is the ball stuck
	int fire; // Fire powerup
	int explosive; // Explosive powerup
	int noRebound; // Can't be hit with the paddle (for s hort time after being hit)
	int bricks; // Number of bricks hit before hitting the paddle again
	int greyBricks; // Number of grey bricks hit in a row
};

// Structure for a coin
struct Coin{
	int x; // Horizontal position of the coin
	int y; // Vertical position of the coin
	int rotationPos; // Rotation state
	int powerup; // Powerup the coin has
};

// Structure for an explosion
struct Explosion{
	int x; // Horizontal positional of the brick that triggered the explosion
	int y; // Vertical positional of the brick that triggered the explosion
	int size; // defines the siz and duration of the explosion
};

// Structure for a laser bullet
struct Bullet{
	int x; // Horizontal position of the laser
	int y; // Vertical position of the laser
	bool remove; // Mark the bullet for removal
};

// Graphics
BitMapObject bmoBoard;// Play area
BitMapObject bmoBackground; // Loads the background bitmaps
BitMapObject bmoBall; // Loads the ball bitmap
BitMapObject bmoBorder; // Loads the border bitmap
BitMapObject bmoBricks; // Loads the brick bitmap
BitMapObject bmoPaddle; // Loads the paddle bitmap
BitMapObject bmoLaser; // Loads the laser bitmap
BitMapObject bmoLabels; // Loads the Label bitmap
BitMapObject bmoHelp; // Loads the title screen and help bitmap
BitMapObject bmoCoin; // Loads the powerup bitmap
BitMapObject bmoExplosion; // Loads the explosion bitmap
BitMapObject bmoFireball; // Loads the fireball bitmap
BitMapObject bmoExplosiveBall; // Loads the explosive ball bitmap
BitMapObject bmoMessages; // Loads the messages bitmap
BitMapObject bmoEditorCursor; // Loads the editor cursor bitmap
BitMapObject bmoEditorFrames; // Load the editor frames bitmap
BitMapObject bmoConfirmation; // Load the confirmation bitmap
BitMapObject bmoGameMenu; // Load the game menu bitmap

// Game variables
int paddleSize = 8; // Number of 8pi blocks in the centre of the paddle
int paddlePos = 279; // The first pixel position of the paddle on the x axis
int paddleSpeed = 5; // The speed of the paddle
int paddleDirection = 0; // The direction the paddle is moving
__int64 timer1 = 0; // Timer used for determining how much time has passed
__int64 timer2 = 0; // Timer used for determining how much time has passed
Ball balls[5]; // Array of 5 16x16 balls
Coin coins[20]; // Array of 20 coins
Explosion explosions[25]; // Array of 20 explosions
Bullet bullets[20]; // Array of 100 bullets
int magnetic; // Number game cycles till magnetic wears off
int laser; // Number of game cycles till laser wears off
int livesRemaining = 4; // Number of extra lives left
int level = 1; // Current level (map) in the game
int maxLevel = 1; // The humber of levels (maps)
int levelMap[BGAMEWIDTH][BGAMEHEIGHT+1][3]; // The array of bricks for the level
int scoreMultiplier = 1; // A multiplier for the score
int brickStyles = 0; // The number of brick styles
int numBricks = 0; // The number of bricks left on the current level
int gamePaused = 1; // Game is paused if not 0. Also defines the help page currently showing
int score = 0; // The players score
int helpColour = 1; // The brick colour used on the help screen (1 = grey)
int helpStyle = 1; // The brick style used for the help screen (1 = swirls)
int coinMap[8][16][16]; // The different pixel layouts for powerup coins
bool gameLost = true; // Game is currently lost
int messages[3]; // Game messages
int messageTimer; // Number of game cycles to a message clears
int confirmationBox = 0; // Confirmationation boxes
int confirmationAction = 0; // Confirmation box action selected
int numConfirmationActions = 0; // The number of actions for the current confimation request
int levelChangeRequest = 0; // The direction of current level change request
bool levelModified = false; // Level has been modifed


// Level Editor Functions
void StartEditor(); // Enter the level editor
void QuitEditor(); // Quit the level editor
void DrawEditorBricks(); // Draw the level editor bricks
void DrawEditorCursors(); // Draw the editor cursors
void DrawBrickSelection(); // Draw the frame with the available brick options
void DrawEditorMenu(); // Draw the editor menu
void DrawEditorLevel(); // Draw the editor level
void LevelEditorAddBrick(int x, int y, int style, int colour); // Add a brick to the editor map
void MoveCursorHorizontally(int num); // Move the cursor horizontally
void MoveCursorVertically(int num); // Move the cursor vertically
void ChangeSelectedColour(int num); // Change the colour selected
void ChangeSelectedStyle(int num); // Change the style selected
void ChangeSelectedLevel(int num); // Change the level selected
bool LoadEditorLevel(int num); // Load the selected editor level
void SaveLevel(); // Save the current level

// Level Editor Constants
const int EDITORWIDTH = 40; // Number of bricks in the editor horizontally
const int EDITORHEIGHT = 20; // Number of bricks in the editor vertically
const int CURSORSIZE = 24; // Pixel width and height of the editor cursor
const int CURSORTIMING = 80; // Frames till a cursor colour change

// Level Editor variables
bool levelEditor = false; // Level editor mode is active
int levelEditorX; // Horizontal position of the cursor on the 40x20 map
int levelEditorY; // Vertical position of the cursor on the 40x20 map
int levelEditorBrickColour; // The currently selected block colour
int levelEditorBrickStyle; // The currently selected block colour
int levelEditorMap[BGAMEWIDTH][BGAMEHEIGHT][2]; // The array of bricks for the currently edited map
int levelEditorLevel; // The level being edited
int cursorTimer = 0; // Timer for cursor animation
int maxCursorTime = CURSORTIMING * 5; // Maximum number of ticks on the cursor timer before wrapping to 0
bool ctrlHeld = false; // Control held down
bool shiftHeld = false; // Shift held down
bool spaceHeld = false; // Space bar held down
bool delHeld = false; // Del key held down

// Sound file constants
std::string SOUND_BRICKKO = "BrickKO.WAV"; // Sound file when the ball knocks out a brick
std::string SOUND_BRICKREBOUND = "BrickRebound.WAV"; // Sound file when the ball rebounds off an indestructable brick
std::string SOUND_BORDERREBOUND = "BorderRebound.WAV"; // Sound file when the ball rebounds off a border
std::string SOUND_PADDLEREBOUND = "PaddleRebound.WAV"; // Sound file when the ball rebounds off the paddle
std::string SOUND_LOSELIFE = "LoseLife.WAV"; // Sound file when a life is lost
std::string SOUND_GAMEOVER = "GameOver.WAV"; // Sound file when the game is lost
std::string SOUND_COIN = "Coin"; // Base sound file name when a coin is collected
std::string SOUND_PADDLESIZEINC = "PaddleSizeInc.WAV"; // Sound file when paddle size increase is gained
std::string SOUND_PADDLESIZEDEC = "PaddleSizeDec.WAV"; // Sound file when paddle size decrease is gained
std::string SOUND_PADDLESPEEDINC = "PaddleSpeedInc.WAV"; // Sound file when paddle speed increase is gained
std::string SOUND_PADDLESPEEDDEC = "PaddleSpeedDec.WAV"; // Sound file when paddle speed decrease is gained
std::string SOUND_BALLSIZEINC = "BallSizeInc.WAV"; // Sound file when ball size increase is gained
std::string SOUND_BALLSIZEDEC = "BallSizeDec.WAV"; // Sound file when ball size decrease is gained
std::string SOUND_EXTRABALL = "ExtraBall.WAV"; // Sound file when an extra ball is gained
std::string SOUND_MAGNETISM = "Magnetism.WAV"; // Sound file when magnetism is gained
std::string SOUND_EXTRALIFE = "ExtraLife.WAV"; // Sound file when an extra life is gained
std::string SOUND_FIREBALL = "Fireball.WAV"; // Sound file when fireball is gained
std::string SOUND_GUNS = "Laser.WAV"; // Sound file when laser guns are gained
std::string SOUND_EXPLOSIVE = "Explosive.WAV"; // Sound file when explosive ball is gained
std::string SOUND_LASERFIRE = "LaserFire.WAV"; // Sound file when laser gun is fired
const int SOUNDQUEUESIZE = 50; // Size of the sond queue

// Sound functions
void initSound(); // Initialise the sound buffers and the sound queue
void AddSound(std::string sound); // Create a sound for the given sound buffer and add it to the queue
void StopSound(); // Turns the sound effects off
void StartSound(); // Turns the sound effects on
void StopMusic(); // Turns the music off
void StartMusic(); // Turns the music on

// Sound variables
bool soundLoaded = false; // Marker to indicate if the sound failed to load
bool soundOn = true; // Marker to indicate if the sound is turned on
bool musicOn = true; // Marker to indicate if the music is turned on

sf::SoundBuffer bufferBrickKO; // The buffer for the brick knock out sound
sf::SoundBuffer bufferBrickRebound; // The buffer for the ball rebound sound on a brick
sf::SoundBuffer bufferBorderRebound; // The buffer for the ball rebound sound on a border
sf::SoundBuffer bufferPaddleRebound; // The buffer for the ball rebound sound on the paddle
sf::SoundBuffer bufferLoseLife; // The buffer for the lose life sound
sf::SoundBuffer bufferGameOver; // The buffer for the game over sound
sf::SoundBuffer bufferCoin1; // The buffer for the first coin collection sound
sf::SoundBuffer bufferCoin2; // The buffer for the second coin collection sound
sf::SoundBuffer bufferCoin3; // The buffer for the third coin collection sound
sf::SoundBuffer bufferPaddleSizeInc; // The buffer for the paddle size increase powerup sound
sf::SoundBuffer bufferPaddleSizeDec; // The buffer for the paddle size decrease powerup sound
sf::SoundBuffer bufferPaddleSpeedInc; // The buffer for the paddle speed increase powerup sound
sf::SoundBuffer bufferPaddleSpeedDec; // The buffer for the paddle speed decrease powerup sound
sf::SoundBuffer bufferBallSizeInc; // The buffer for the ball size increase powerup sound
sf::SoundBuffer bufferBallSizeDec; // The buffer for the ball size decrease powerup sound
sf::SoundBuffer bufferExtraBall; // The buffer for the extra ball powerup sound
sf::SoundBuffer bufferMagnetism; // The buffer for the magnetism powerup sound
sf::SoundBuffer bufferExtraLife; // The buffer for the extra life powerup sound
sf::SoundBuffer bufferFireball; // The buffer for the fireball powerup sound
sf::SoundBuffer bufferGuns; // The buffer for the laser guns powerup sound
sf::SoundBuffer bufferExplosive; // The buffer for the explosive powerup sound
sf::SoundBuffer bufferLaserFire; // The buffer for the laser fire sound

sf::Sound soundQueue[SOUNDQUEUESIZE]; // The queue for playing sounds

LRESULT CALLBACK TheWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) // Respond to events
{
	int n; // Counter
	bool noneStuck = false; // Marker to decide whether to shoot or release a ball

	//Check what event was triggered
	switch(uMsg)
	{
	// case WM_CHAR: // A character key was pressed
	//	{
	//		return(0);
	//	}break;
	case WM_KEYDOWN: // A key was pressed
		{
			if(wParam == VK_ESCAPE) // Check for ESC key pressed
			{
				if(confirmationBox) // If a confirmation box is present
				{
					// Do nothing
					return(0); // Handled message
				}
				SetConfirmation(CONFIRMQUIT); // Ask for confirmation to quit
				return(0); // Handled message
			}
			if(wParam == VK_SHIFT) // Check for Control key pressed
			{
				shiftHeld = true;
				return(0); // Handled message
			}
			if(wParam == VK_CONTROL) // Check for Shift key pressed
			{
				ctrlHeld = true;
				return(0); // Handled message
			}
			if(wParam == VK_LEFT) // Check for left arrow pressed
			{
				if(confirmationBox) // If a confirmation box is present
				{
					ChangeConfirmationAction(-1); // Reduce the confirmation action by one
					return(0); // Handled message
				}
				if(levelEditor) // If the game is in editor mode...
				{
					if(ctrlHeld) //  If the control key is held down...
					{
						if(levelModified) // If the current level has been modified...
						{
							levelChangeRequest = -1; // New level decrease request
							SetConfirmation(CONFIRMEDITORLEVELCHANGE); // Ask for confirmation to change the level
							return(0); // Handled message
						}
						ChangeSelectedLevel(-1); // Change back one level
						return(0);
					}
					if(shiftHeld) // If the shift key is held down...
					{
						ChangeSelectedColour(-1); // Change the colour back one
						return(0); // Message handled
					}
					MoveCursorHorizontally(-1); // Move the editor selection cursor
				}
				else // If the game isn't in editor mode...
				{
					if(gamePaused) // When the game is paused...
					{
						ChangeHelpStyle(1); // Change the background brick style
					}
					else // When the game is unpaused...
					{
						SetPaddleDirection(-1); // Start moving the paddle to the left
					}
				}
				return(0); // Handled message
			}
			if(wParam == VK_RIGHT) // Check for right arrow pressed
			{
				if(confirmationBox) // If a confirmation box is present
				{					
					ChangeConfirmationAction(1); // Increase the confirmation action by one
					return(0); // Handled message
				}
				if(levelEditor) // If the game is in editor mode...
				{
					if(ctrlHeld) //  If the control key is held down...
					{
						if(levelModified) // If the current level has been modified
						{
							levelChangeRequest = 1; // New level decrease request
							SetConfirmation(CONFIRMEDITORLEVELCHANGE); // Ask for confirmation to change level
							return(0); // Handled message
						}
						ChangeSelectedLevel(1); // Change back one level
						return(0);
					}
					if(shiftHeld) // If the shift key is held down...
					{
						ChangeSelectedColour(1); // Change the colour forward one
						return(0); // message handled
					}
					MoveCursorHorizontally(1); // Move the editor selection cursor
				}
				else // If the game isn't in editor mode...
				{
					if(gamePaused) // When the game is paused...
					{
						ChangeHelpStyle(-1); // Change the background brick style
					}
					else // When the game is unpaused...
					{
						SetPaddleDirection(1); // Start moving the paddle to the right
					}
				}
				return(0); // Handled message
			}
			if(wParam == VK_DOWN) // Check for down arrow pressed
			{
				if(confirmationBox) // If a confirmation box is present
				{
					// Do nothing
					return(0); // Handled message
				}
				if(levelEditor) // If the game is in editor mode...
				{
					// Check if Ctrl is pressed

					if(shiftHeld) // If the shift key is held down...
					{
						ChangeSelectedStyle(1); // Change the style forward one
						return(0); // message handled
					}
					MoveCursorVertically(1); // Move the editor selection cursor
				}
				else // If the game isn't in editor mode...
				{
					if(gamePaused) // When the game is paused...
					{
						ChangeHelpColour(-1); // Change the background brick colour
					}
				}
				return(0); // Handled message
			}
			if(wParam == VK_UP) // Check for up arrow pressed
			{
				if(confirmationBox) // If a confirmation box is present
				{
					// Do nothing
					return(0); // Handled message
				}
				if(levelEditor) // If the game is in editor mode...
				{
					// Check if Ctrl is pressed
					
					if(shiftHeld) // If the shift key is held down...
					{
						ChangeSelectedStyle(-1); // Change the style back one
						return(0); // message handled
					}
					MoveCursorVertically(-1); // Move the editor selection cursor
				}
				else // If the game isn't in editor mode...
				{
					if(gamePaused) // When the game is paused...
					{
						ChangeHelpColour(1); // Change the background brick colour
					}
				}
				return(0); // Handled message
			}
			if(wParam == VK_DELETE) // Check for the del key pressed
			{				
				if(confirmationBox) // If a confirmation box is present
				{
					// Do nothing
					return(0); // Handled message
				}
				if(levelEditor) // If the game is in editor mode...
				{
					delHeld = true; // Del key is held down
				}
				return(0); // Handled message
			}
			if(wParam == VK_RETURN) // Check for return/enter pressed
			{
				if(confirmationBox) // If a confirmation box is present...
				{
					SelectConfirmationAction(); // Select the current confirmation action
					return(0); // Handled message
				}
				return(0); // Handled message
			}
			if(wParam == VK_SPACE) // Check for the space bar pressed
			{
				if(confirmationBox) // If a confirmation box is present...
				{
					SelectConfirmationAction(); // Select the current confirmation action
					return(0); // Handled message
				}
				if(levelEditor) // If the game is in editor mode...
				{
					spaceHeld = true; // Spacebar is held down
				}
				else // If the game isn't in editor mode...
				{
					if(gamePaused) // When the game is paused...
					{
						PauseGame(); // Increment the paused counter (change the help screen)
					}
					else // When the game is unpaused...
					{
						noneStuck = true; // No stuck balls found so far
						// Release any balls stuck to the paddle
						n = 0;
						while(n < 5)
						{
							if(balls[n].size != -1) // Check ball n exists...
							{
								if(balls[n].stuck) // And if it's stuck...
								{
									ReleaseBall(n); // Release it
									noneStuck = false; // Found a stuck ball
								}
							}
							n++;
						}

						if(noneStuck) // If no stuck balls were found...
						{
							if(laser > 0) // If lasers are active...
							{
								AddBullets(); // Fire the bullets
							}
						}
					}
				}
				return(0); // Handled message
			}
			if(wParam == 0x04C || wParam == 0x06C) // If L or l is pressed...
			{
				if(confirmationBox) // If a confirmation box is present
				{
					// Do nothing
					return(0); // Handled message
				}
				if(levelEditor) // If the game is in editor mode...
				{
					// Nothing
				}
				else // If the game isn't in editor mode...
				{
					if(gamePaused) // If the game is paused...
					{
						if(!gameLost) // If there is a game in progress...
						{
							SetConfirmation(CONFIRMEDITORSTART); // Ask for confirmation to start the level editor
							return(0); // Message handled
						}
						StartEditor(); // Start the level editor
					}
				}
				return 0; // Message handled
			}
			if(wParam == 0x04E || wParam == 0x06E) // If N or n is pressed...
			{
				if(confirmationBox) // If a confirmation box is present
				{
					// Do nothing
					return(0); // Handled message
				}
				if(levelEditor) // If the game is in editor mode...
				{
					// Nothing
				}
				else // If the game isn't in editor mode...
				{
					if(gamePaused) // When the game is paused...
					{
						if(!gameLost) // If there is a game in progress...
						{
							SetConfirmation(CONFIRMNEW); // Ask for confirmation to start a new game
							return(0); // Message handled
						}
						StartGame(); // Start a new game
					}
				}
				return(0); // Message handled
			}
			if(wParam == 0x050 || wParam == 0x070) // If P or p is pressed...
			{
				if(confirmationBox) // If a confirmation box is present
				{
					// Do nothing
					return(0); // Handled message
				}
				if(levelEditor) // If the game is in editor mode...
				{
					// Nothing
				}
				else // If the game isn't in editor mode...
				{
					if(gamePaused) // When the game is paused...
					{
						if(gameLost)
						{
						}
						else
						{
							UnpauseGame(); // Unpause the game
						}
					}
					else // When the game is unpaused...
					{
						PauseGame(); // Pause the game
					}
				}
				return(0); // Handled Message
			}

			// Cheat used to troubleshoot. Comment out before compiling the release version
			if(wParam == 0x051 || wParam == 0x071) // If Q or q is pressed...
			{
				if(confirmationBox) // If a confirmation box is present
				{
					// Do nothing
					return(0); // Handled message
				}
				if(levelEditor) // If the game is in editor mode...
				{
					if(levelModified) // If a level was changed...
					{
						SetConfirmation(CONFIRMEDITORQUIT); // Ask for confirmation to quit the editor
						return(0); // Handled message
					}
					QuitEditor(); // Quit the editor
				}
				else // If the game isn't in editor mode...
				{
					if(gamePaused) // When the game is paused...
					{
						SetConfirmation(CONFIRMQUIT); // Ask for confirmation to quit
						return(0); // Handled message
					}
					else // When the game is unpaused...
					{
						// GainPowerup(INCPADDLESIZE);
						// GainPowerup(DECPADDLESIZE);
						// GainPowerup(INCPADDLESPEED);
						// GainPowerup(DECPADDLESPEED);
						// GainPowerup(INCBALLSIZE);
						// GainPowerup(DECBALLSIZE);
						// GainPowerup(EXTRABALL);
						// GainPowerup(MAGNETIC);
						// GainPowerup(EXTRALIFE);
						// GainPowerup(FIREBALL);
						// GainPowerup(GUNS);
						// GainPowerup(EXPLOSIVE);
						// GainPowerup(FIREBALL);
						// ChangeLevel(1);
					}
				}
				return(0); // Handled Message
			}
			if(wParam == 0x052 || wParam == 0x072) // If R or r is pressed...
			{
				if(confirmationBox) // If a confirmation box is action
				{
					// Do Nothing
					return(0); // Message Handled
				}
				if(levelEditor)
				{
					SetConfirmation(CONFIRMRESTORE); // Ask for confirmation to restore original levels
					return(0); // Message Handled
				}
				return(0); // Message Handled
			}
			if(wParam == 0x053 || wParam == 0x073) // If S or s is pressed...
			{
				if(confirmationBox) // If a confirmation box is action
				{
					// Do Nothing
					return(0); // Message Handled
				}
				if(levelEditor)
				{
					SaveLevel(); // Save the current level
					return(0); // Message Handled
				}
				if(soundOn) // If sound effects are on...
				{
					StopSound(); // Turn sound effects off
					return(0); // Message Handled
				}
				else // If sound effects are off...
				{
					StartSound(); // Turn sound effects on
					return(0); // Message Handled
				}
			}
		}break;
	case WM_KEYUP:
		{			
			if(wParam == VK_SHIFT) // Check for Control key released
			{
				shiftHeld = false;
				return(0); // Handled message
			}
			if(wParam == VK_CONTROL) // Check for Shift key released
			{
				ctrlHeld = false;
				return(0); // Handled message
			}
			if(wParam == VK_DELETE) // Check for the del key pressed
			{		
				if(levelEditor) // If the game is in editor mode...
				{
					delHeld = false; // Del key is no longer held down
				}				
				else // If the game isn't in editor mode...
				{
				}
				return(0); // Handled message
			}
			if(wParam == VK_SPACE) // Check for the space bar pressed
			{
				if(levelEditor) // If the game is in editor mode...
				{
					spaceHeld = false; // Spacebar is no longer held down
				}
				else // If the game isn't in editor mode...
				{
				}
				return(0); // Handled message
			}
			if(wParam == VK_LEFT) // Check for left arrow released
			{
				if(confirmationBox) // If a confirmation box is present
				{
					// Do nothing
					return(0); // Handled message
				}
				if(levelEditor) // If the game is in editor mode...
				{
					// Nothing
				}
				else // If the game isn't in editor mode...
				{
					if(gamePaused) // When the game is paused...
					{
						// Do nothing
					}
					else // When the game is unpaused...
					{
						if (GetPaddleDirection() == -1) // If the paddle is moving left...
						{
							SetPaddleDirection(0); // Stop the paddle moving
						}
					}
				}
				return(0); // Handled message
			}
			if(wParam == VK_RIGHT) // Check for right arrow released
			{
				if(confirmationBox) // If a confirmation box is present
				{
					// Do nothing
					return(0); // Handled message
				}
				if(levelEditor) // If the game is in editor mode...
				{
					// Nothing
				}
				else // If the game isn't in editor mode...
				{
					if(gamePaused) // When the game is paused...
					{
						// Do nothing
					}
					else // When the game is unpaused...
					{
						if (GetPaddleDirection() == 1) // If the paddle is moving right...
						{
							SetPaddleDirection(0); // Stop the paddle moving
						}
					}
				}
				return(0); // Handled message
			}
		}break;
	case WM_DESTROY: // Window is being destroyed
		{
			PostQuitMessage(0); // Tell the application we're quitting
			return(0); // Handled the message
		}break;
	case WM_PAINT: // Window needs to be redrawn
		{
			PAINTSTRUCT ps; // A variable needed for painting information
			HDC hdc = BeginPaint(hwnd, &ps); // Start painting
			
			// Redraw the map
			BitBlt(hdc, 0, 0, GAMEWIDTH*TILESIZE, GAMEHEIGHT*TILESIZE, bmoBoard, 0 , 0, SRCCOPY);
			// End painting
			EndPaint(hwnd, &ps);

			return(0);
		}break;
	}
	// Pass along any undefined messages to the default handler
	return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{  // Create the window and dispatch events
	mainInstance = hInstance; // Assigning the instance to the global variable

	WNDCLASSEX wcx; // Delcare a window class variable
	wcx.cbSize = sizeof(WNDCLASSEX); // Setting the size of the structure
	wcx.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS; // Setting the class style
	wcx.lpfnWndProc = TheWindowProc; // Windows procedure
	wcx.cbClsExtra = 0; // Class Extra
	wcx.cbWndExtra = 0; // Window Extra
	wcx.hInstance = mainInstance; // Application Handle
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION); // Icon
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW); // Cursor
	wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // Background colour
	wcx.lpszMenuName = NULL; // Menu
	wcx.lpszClassName = WINDOWCLASS; // Class name
	wcx.hIconSm = NULL; // Small icon

	if(!RegisterClassEx(&wcx)) // register the window class and return 0 on failure
	{
		return(0);
	}

	//Create the main window
	mainWindow = CreateWindowEx(0, WINDOWCLASS, WINDOWTITLE, WS_BORDER | WS_SYSMENU | WS_CAPTION | WS_VISIBLE, 0, 0, 320, 240, NULL, NULL, mainInstance, NULL);

	if(!mainWindow) // Error checking
	{
		return(0);
	}

	if(!GameInit()) // Attempt to initialise the game
	{
		return(0);
	}

	MSG msg; // Message variable
	for( ; ; )
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // Look for a message
		{
			if(msg.message == WM_QUIT) // If the message is quit then break the loop
			{
				break;
			}

			TranslateMessage(&msg); // translate the message
			DispatchMessage(&msg); // then dispatch the message
		}

		GameLoop(); // Run the game loop
	}

	FinishGame(); // Clean up the game when we're done

	return(msg.wParam); // Return the wParam for the QUIT message
}

bool GameInit() // Initiate the game
{
	srand(time(NULL)); // Initiate the random number generate witha  unique start number

	// Set the client area size
	RECT tempRect;
	SetRect(&tempRect, 0, 0, GAMEWIDTH*TILESIZE, GAMEHEIGHT*TILESIZE); // Set the client to 640x480
	AdjustWindowRect(&tempRect, WS_BORDER | WS_SYSMENU | WS_CAPTION | WS_VISIBLE, FALSE); // Adjust the window accordingly
	SetWindowPos(mainWindow, NULL, 0, 0, tempRect.right - tempRect.left, tempRect.bottom - tempRect.top, SWP_NOMOVE); // Set the window width and height

	// Create the play area graphics
	HDC hdc = GetDC(mainWindow);
	bmoBoard.Create(hdc, GAMEWIDTH*TILESIZE, GAMEHEIGHT*TILESIZE);
	FillRect(bmoBoard, &tempRect, (HBRUSH)GetStockObject(BLACK_BRUSH));
	ReleaseDC(mainWindow, hdc);
	
	LoadBackground(level); // Load the appropriate background graphic
	bmoBall.Load(NULL, "Ball.bmp"); // Load the graphics for the balls
	bmoBorder.Load(NULL, "Border.bmp"); // Load the graphics for the border
	bmoBricks.Load(NULL, "Bricks.bmp"); // Load the graphics for the bricks
	bmoPaddle.Load(NULL, "Paddle.bmp"); // Load the graphics for the paddle
	bmoLaser.Load(NULL, "Laser.bmp"); // Loas the graphics for the laser
	bmoLabels.Load(NULL, "Labels.bmp"); // Load the graphics for the score and lives
	bmoHelp.Load(NULL, "Help.bmp"); // Load the graphics for the help menus
	bmoCoin.Load(NULL, "Powerups.bmp"); // Load the graphics for the powerup coins
	bmoExplosion.Load(NULL, "Explosions.bmp"); // Load the graphics for the explosions
	bmoFireball.Load(NULL, "Fireball.bmp"); // Loads the graphics for fireballs
	bmoExplosiveBall.Load(NULL, "Explosiveball.bmp"); // Loads the graphics for fireballs
	bmoMessages.Load(NULL, "Messages.bmp"); // Loads the messages bitmap
	bmoEditorCursor.Load(NULL, "Cursor.bmp"); // Loads the editor cursor bitmap
	bmoEditorFrames.Load(NULL, "EditorFrames.bmp"); // Loads the editors frames bitmap
	bmoConfirmation.Load(NULL, "Confirmation.bmp"); // Loads the confirmation bitmap
	bmoGameMenu.Load(NULL, "GameMenu.bmp"); // Load the game menu bitmap

	initSound();

	gamePaused = 1; // Make sure the game starts paused
	score = 0; // Set the start score to 0
	LoadCoinMap(); // Load in the pixel maps for the powerup coins
	SetMaxLevel(); // Read the maximum number of levels from the levels.txt file
	SetBrickStyles(); // Retrieves the number of brick styles from levels.txt file
	LoadLevel(level); // Load the starting level
	ResetExplosions(); // Initiate the explosions
	ResetBullets(); // Initiate the bullets
	UseLife(); // Use a life to setup the board elements

	DrawGame();

	return(true);
}

void GameLoop() // Keep the game moving at the correct pace
{
	int n = 0; // Counter

	// When the game is unpaused...

	// Set timer2 to the current time
	QueryPerformanceCounter((LARGE_INTEGER *)&timer2);

	// Update the game 20 times a second
	if(timer2 - timer1 > 50000)
	{
		if(confirmationBox) // When there is a confirmation box...
		{
			// Do not update game elements
			DrawGame();

			QueryPerformanceCounter((LARGE_INTEGER *)&timer1); // Reset timer1 to the current time
			return;
		}

		if(levelEditor) // If the level editor is active
		{
			cursorTimer++; // Increment the cursor timer

			if(cursorTimer >= maxCursorTime) // If the cursor timer has reached it's maximum time
			{
				cursorTimer -= maxCursorTime; // Wrap back to 0
			}

			if(delHeld) // If the del key is held down
			{
				// Add the currently selected block
				LevelEditorAddBrick(levelEditorX, levelEditorY, 0, 0);
			}

			if(spaceHeld) // If the space bar is held down...
			{
				// Add the currently selected block
				LevelEditorAddBrick(levelEditorX, levelEditorY, levelEditorBrickStyle, levelEditorBrickColour);
			}

			DrawGame();

			QueryPerformanceCounter((LARGE_INTEGER *)&timer1); // Reset timer1 to the current time
			return;
		}

		if(gamePaused) // When the game is paused...
		{
			// Do not update game elements
			DrawGame();

			QueryPerformanceCounter((LARGE_INTEGER *)&timer1); // Reset timer1 to the current time
			return;
		}

		// Countdown the magnetic timer if it's active
		if(magnetic > 0)
		{
			magnetic--;
		}
		// Countdown the laser timer if it's active
		if(laser > 0)
		{
			laser--;
		}
		// Countdown the message timer and remove a message if needed
		if(messageTimer > 0)
		{
			messageTimer--;
			if(messageTimer == 0) // If the timer reaches 0
			{
				RemoveMessage(); // If there was a message to remove
			}
		}
		// Countdown the fire and explosive and noRebound timers for each ball
		n = 0;
		while(n < 5)
		{
			if(balls[n].size != -1)
			{
				if(balls[n].fire > 0)
				{
					balls[n].fire--;
				}
				if(balls[n].explosive > 0)
				{
					balls[n].explosive--;
				}
				if(balls[n].noRebound > 0)
				{
					balls[n].noRebound--;
				}
			}
			else
			{
				break;
			}
			n++;
		}
		// Countdown the explosions
		n = 0;
		while(n < 25)
		{
			if(explosions[n].size > 0)
			{
				explosions[n].size--;

				if(explosions[n].size == 0) // If the explosion no longer exists...
				{
					RemoveExplosion(n); // Remove the explosion
				}
				else // If the explosion still exists...
				{
					n++; // Move onto the next explosion
				}
			}
			else
			{
				break;
			}
		}

		// Move the paddle one frame if it is currently moving
		if(GetPaddleDirection())
		{
			MovePaddlePosition();
		}

		MoveBalls(); // Move the balls one frame
		DropCoins(); // Move the Coins one frame
		MoveBullets(); // Move the bullets one frame
		DrawGame(); // Redraw the game

		QueryPerformanceCounter((LARGE_INTEGER *)&timer1); // Reset timer1 to the current time
	}
}

void UseLife() // Lose a life and reset the necessary elements
{
	int n; // Counter

	// Start by removing all the balls
	n = 0;
	while(n < 5)
	{
		balls[n].size = -1;
		AdjustBallSize(n,0);
		balls[n].x = 0;
		balls[n].y = 0;
		balls[n].speedX = 0;
		balls[n].speedY = 0;
		balls[n].speedMod = 0;
		balls[n].stuck = false;
		balls[n].fire = 0;
		balls[n].explosive = 0;
		balls[n].noRebound = 0;
		balls[n].bricks = 1;
		balls[n].greyBricks = 0;
		n++;
	}

	// If there are lives left then set up the initial ball
	if(livesRemaining > 0)
	{
		// Set the starting ball to the middle of the paddle
		balls[0].size = 4;
		AdjustBallSize(0,0);
		balls[0].x = GetPaddlePosition() + (GetPaddleSize()+2)*4 - TILESIZE;
		balls[0].y = 479 - 14 - (9+(balls[0].size));
		balls[0].speedX = 0;
		balls[0].speedY = 0;
		balls[0].speedMod = 0;
		balls[0].stuck = true;
		balls[0].fire = 0;
		balls[0].explosive = 0;
		balls[0].noRebound = 0;
		balls[0].bricks = 1;
		balls[0].greyBricks = 0;

		// Reset the game timer to the current time
		QueryPerformanceCounter((LARGE_INTEGER *)&timer1);

		// Reset powerups
		paddleSize = INITPADDLESIZE;
		paddleSpeed = INITPADDLESPEED;
		magnetic = 0;
		laser = 0;

		// Remove a life
		livesRemaining--;
		if(!gamePaused) // If the game isn't paused...
		{
			// Play the lose life sound
			AddSound(SOUND_LOSELIFE); // Play the sound for losing a life
		}
	} else { // If there were no lives left
		GameOver(); // Game is lost
	}
}

void GameOver() // Game is over
{
	AddSound(SOUND_GAMEOVER); // Play the lose game sound
	gameLost = true;
	gamePaused = 1;	// Pause the game
}

void DrawGame() // Draw the game board
{
	if(levelEditor) // If the level editor is active
	{
		// Draw the background
		DrawBackground();

		// Draw the level editor bricks
		DrawEditorBricks();

		// Draw the manu frame
		DrawEditorMenu();

		// Draw the brick selection frame
		DrawBrickSelection();

		// Draw the level selection frame
		DrawEditorLevel();
		
		// Draw the cursors
		DrawEditorCursors();

		// Draw the border
		DrawBorders(2);

		if(confirmationBox) // If there is a confirmation request...
		{
			DrawConfirmation(); // Draw the confirmation box
		}

		// Invalidate the window rect
		InvalidateRect(mainWindow, NULL, FALSE);
		return;
	}

	if(gamePaused) // If the game is paused...
	{
		DrawHelp(); // Draw the help screen

		DrawGameMenu(); // Draw the game menu

		if(confirmationBox) // If there is a confirmation request...
		{
			DrawConfirmation(); // Draw the confirmation box
		}

		InvalidateRect(mainWindow, NULL, FALSE);
		return;
	}

	// When the game is unpaused...

	// Draw the background
	DrawBackground();

	// Draw the bullets
	DrawBullets();

	// Draw the paddle
	DrawPaddle();

	// Draw the Bricks
	DrawBricks();

	// Draw the Powerup Coins
	DrawCoins();

	// Draw the balls
	DrawBalls();

	// Draw the explosions
	DrawExplosions();

	// Draw the messages
	DrawMessages();

	// Draw the border
	DrawBorders(1);

	// Draw extra lives
	DrawExtraLives();

	// Draw the score
	DrawScore();

	if(confirmationBox) // If there is a confirmation request...
	{
		DrawConfirmation(); // Draw the confirmation box
	}

	// Invalidate the window rect
	InvalidateRect(mainWindow, NULL, FALSE);
}

void DrawBackground() // Draw the level background
{
	// Mask first
	BitBlt(bmoBoard, 0, 0, 640, 480, bmoBackground, 0, 0, SRCAND);
	// Then image
	BitBlt(bmoBoard, 0, 0, 640, 480, bmoBackground, 0, 0, SRCPAINT);
}

void DrawBorders(int type) // Draw the boarders along top and sides. (Include the bottom if type 0 border is requested)
{
	int x, y; // Counters

	// Draw the top and bottom borders
	for(x = 0; x < GAMEWIDTH; x++)
	{
		DrawBorder(x, 0, 2, 0); // Draw the top border
		
		if(type == 0) // Add a bottom if type 0 is requested
		{
			DrawBorder(x, GAMEHEIGHT-1, 2, 1); // Draw the bottom border
		}
		if(type == 2) // Add a bottom border 2/3 down for type 2
		{
			DrawBorder(x, EDITORHEIGHT*2, 2, 1); // Draw the mid border
		}
	}
	// Draw the side borders
	for(y = 0; y < GAMEHEIGHT; y++)
	{
		if(type == 2 && y > EDITORHEIGHT*2) // Stop 2/3 of the way for type 2
		{
			// Do nothing
		}
		else
		{
			DrawBorder(0, y, 0, 0); // Draw the left border
			DrawBorder(GAMEWIDTH-1, y, 4, 0); // Draw the right border
		}
	}
	// Draw the corners
	DrawBorder(0, 0, 1, 0); // Draw the top-left corner
	DrawBorder(GAMEWIDTH-1, 0, 3, 0); // Draw the top-right corner

	if(type == 0) // Draw corners in the bottom to match the bottom bar when type 0 is requested
	{
		DrawBorder(0, GAMEHEIGHT-1, 1, 1); // Draw the bottom-left corner
		DrawBorder(GAMEWIDTH-1, GAMEHEIGHT-1, 3, 1); // Draw the bottom-right corner
	}
	if(type == 2) // Draw the bottom corners 2/3 of the way down for type 2
	{
		DrawBorder(0, EDITORHEIGHT*2, 1, 1); // Draw the bottom-left corner
		DrawBorder(GAMEWIDTH-1, EDITORHEIGHT*2, 3, 1); // Draw the bottom-right corner
	}
}

void DrawBorder(int x, int y, int tileX, int tileY) // Draw a single border tile
{
	// Mask first
	BitBlt(bmoBoard, x*TILESIZE, y*TILESIZE, TILESIZE, TILESIZE, bmoBorder, tileX*TILESIZE, tileY*TILESIZE, SRCAND);
	// Then image
	BitBlt(bmoBoard, x*TILESIZE, y*TILESIZE, TILESIZE, TILESIZE, bmoBorder, tileX*TILESIZE, tileY*TILESIZE, SRCPAINT);
}

void DrawPaddle() // Draw the game paddle
{
	int x; // Counter
	int paddleColour = 0; // Colour
	
	// Paddle colour cycles when magnetic is active
	paddleColour = magnetic / 5; // Colour changes every 5 frames (0.25s)
	paddleColour = paddleColour % 10; // 10 colours to cycle through

	// Left side of paddle
	// Mask first
	BitBlt(bmoBoard, GetPaddlePosition(), 464, 8, 16, bmoPaddle, paddleColour*24, 16, SRCAND);
	// Then image
	BitBlt(bmoBoard, GetPaddlePosition(), 464, 8, 16, bmoPaddle, paddleColour*24, 0, SRCPAINT);
	if(laser > 0) // If the laser powerup is active...
	{ // Overlay the laser
		// Mask first
		BitBlt(bmoBoard, GetPaddlePosition(), 464, 8, 16, bmoLaser, 0, 16, SRCAND);
		// Then image
		BitBlt(bmoBoard, GetPaddlePosition(), 464, 8, 16, bmoLaser, 0, 0, SRCPAINT);
	}

	// Middle of the paddle
	x = 0;
	while(x < GetPaddleSize())
	{
		// Mask first
		BitBlt(bmoBoard, GetPaddlePosition() + (x+1)*8, 464, 8, 16, bmoPaddle, 8 + paddleColour*24, 16, SRCAND);
		// Then image
		BitBlt(bmoBoard, GetPaddlePosition() + (x+1)*8, 464, 8, 16, bmoPaddle, 8 + paddleColour*24, 0, SRCPAINT);
		
		if(laser > 0) // If the laser powerup is active...
		{ // Overlay the laser
			// Mask first
			BitBlt(bmoBoard, GetPaddlePosition() + (x+1)*8, 464, 8, 16, bmoLaser, 8, 16, SRCAND);
			// Then image
			BitBlt(bmoBoard, GetPaddlePosition() + (x+1)*8, 464, 8, 16, bmoLaser, 8, 0, SRCPAINT);
		}
		x++;
	}

	//Right side of the paddle
	// Mask first
	BitBlt(bmoBoard, GetPaddlePosition() + (x+1)*8, 464, 8, 16, bmoPaddle, 16 + paddleColour*24, 16, SRCAND);
	// Then image
	BitBlt(bmoBoard, GetPaddlePosition() + (x+1)*8, 464, 8, 16, bmoPaddle, 16 + paddleColour*24, 0, SRCPAINT);
	if(laser > 0) // If the laser powerup is active...
	{ // Overlay the laser
		// Mask first
		BitBlt(bmoBoard, GetPaddlePosition() + (x+1)*8, 464, 8, 16, bmoLaser, 16, 16, SRCAND);
		// Then image
		BitBlt(bmoBoard, GetPaddlePosition() + (x+1)*8, 464, 8, 16, bmoLaser, 16, 0, SRCPAINT);
	}
}

void DrawBalls() // Draw the game balls
{
	int n; // Counter
	int m; // Random variable
	// int colourAdjust; // Used to adjust the colour of the ball when a powerup is applied
	int offsetX; // Offset the fireball overlay horizontally
	int offsetY; // Offset the fireball overlay vertially
	int startX = 32; // Starting horizontal pixel position to draw from in the fireball graphics
	int startY; // Start vertical position to draw from in the fireball graphics
	int graphicSize; // Size of the graphics to be drawn
	
	// 0 = Grey
	// 1 = Red
	// 2 = Orange

	// Draw each of the 5 balls
	n = 0;
	while(n < 5)
	{
		if(balls[n].size != -1) // If the ball exists...
		{
			// Mask first
			BitBlt(bmoBoard, balls[n].x, balls[n].y, 16, 16, bmoBall, 16*(balls[n].size-1), 16, SRCAND);
			// Then image
			BitBlt(bmoBoard, balls[n].x, balls[n].y, 16, 16, bmoBall, 16*(balls[n].size-1), 0, SRCPAINT);

			if(balls[n].fire) // If the fireball powerup is active...
			{
				// Calculate the graphic offset
				if(balls[n].speedX < 0) // If the ball is travelling left...
				{
					offsetX = 0 + (7 - balls[n].size); // Offset the graphic horitzontally
				}
				else // If the ball is travelling right...
				{
					offsetX = -16 + 3*(7 - balls[n].size); // Offset the graphic horizontally
				}

				if(balls[n].speedY < 0) // If the ball is travelling up...
				{
					offsetY = 0 + (7 - balls[n].size); // Offset the graphic vertically
				}
				else // If the ball is travelling down...
				{
					offsetY = -16 + 3*(7 - balls[n].size); // Offset the graphic vertically
				}

				// Draw the corresponding overlay
				m = balls[n].fire/FIREANIMATION % 4; // Set the animation frame to be used
				graphicSize = 32 - 4*(7 - balls[n].size); // Set the size of the graphic to be used
				
				switch(balls[n].size) // Set the start Y position based on the balls size
				{
				case 1:
					startY = 1056; // Set the vertical start position to draw from in the fireball graphic
					break;
				case 2:
					startY = 960; // Set the vertical start position to draw from in the fireball graphic
					break;
				case 3:
					startY = 832; // Set the vertical start position to draw from in the fireball graphic
					break;
				case 4:
					startY = 672; // Set the vertical start position to draw from in the fireball graphic
					break;
				case 5:
					startY = 480; // Set the vertical start position to draw from in the fireball graphic
					break;
				case 6:
					startY = 256; // Set the vertical start position to draw from in the fireball graphic
					break;
				case 7:
					startY = 0; // Set the vertical start position to draw from in the fireball graphic
					break;
				}

				// Draw the flames
				if(balls[n].speedY < 0) // If the ball is travelling up...
				{
					// Mask first
					BitBlt(bmoBoard, balls[n].x + offsetX, balls[n].y + offsetY, graphicSize, graphicSize, bmoFireball,
						startX + graphicSize*(balls[n].speedX+4), startY + graphicSize*(2*m+1) , SRCAND);
					// Then image
					BitBlt(bmoBoard, balls[n].x + offsetX, balls[n].y + offsetY, graphicSize, graphicSize, bmoFireball,
						startX + graphicSize*(balls[n].speedX+4), startY + graphicSize*(2*m), SRCPAINT);						
				}
				else if(balls[n].speedY > 0) // If the ball is travelling down...
				{
					// Mask first
					BitBlt(bmoBoard, balls[n].x + offsetX, balls[n].y + offsetY, graphicSize, graphicSize, bmoFireball,
						startX + graphicSize*(balls[n].speedX+4) + graphicSize*9, startY + graphicSize*(2*m+1), SRCAND);
					// Then image
					BitBlt(bmoBoard, balls[n].x + offsetX, balls[n].y + offsetY, graphicSize, graphicSize, bmoFireball,
						startX + graphicSize*(balls[n].speedX+4) + graphicSize*9, startY + graphicSize*(2*m), SRCPAINT);
					}
				else // If the ball isn't travelling...
				{
					// Draw a standard fireball
					// Mask first
					BitBlt(bmoBoard, balls[n].x, balls[n].y, 16, 16, bmoFireball, 16, 16*(7-balls[n].size), SRCAND);
					// Then image
					BitBlt(bmoBoard, balls[n].x, balls[n].y, 16, 16, bmoFireball, 0, 16*(7-balls[n].size), SRCPAINT);
				}				
			}

			if(balls[n].explosive) // If the explosive powerup is active...
			{
				// Calculate the graphic offset
				if(balls[n].speedX < 0) // If the ball is travelling left...
				{
					offsetX = 0 + (7 - balls[n].size); // Offset the graphic horitzontally
				}
				else // If the ball is travelling right...
				{
					offsetX = -16 + 3*(7 - balls[n].size); // Offset the graphic horizontally
				}

				if(balls[n].speedY < 0) // If the ball is travelling up...
				{
					offsetY = 0 + (7 - balls[n].size); // Offset the graphic vertically
				}
				else // If the ball is travelling down...
				{
					offsetY = -16 + 3*(7 - balls[n].size); // Offset the graphic vertically
				}

				// Draw the corresponding overlay
				m = balls[n].explosive/FIREANIMATION % 4; // Set the animation frame to be used
				graphicSize = 32 - 4*(7 - balls[n].size); // Set the size of the graphic to be used
				
				switch(balls[n].size) // Set the start Y position based on the balls size
				{
				case 1:
					startY = 1056; // Set the vertical start position to draw from in the fireball graphic
					break;
				case 2:
					startY = 960; // Set the vertical start position to draw from in the fireball graphic
					break;
				case 3:
					startY = 832; // Set the vertical start position to draw from in the fireball graphic
					break;
				case 4:
					startY = 672; // Set the vertical start position to draw from in the fireball graphic
					break;
				case 5:
					startY = 480; // Set the vertical start position to draw from in the fireball graphic
					break;
				case 6:
					startY = 256; // Set the vertical start position to draw from in the fireball graphic
					break;
				case 7:
					startY = 0; // Set the vertical start position to draw from in the fireball graphic
					break;
				}

				// Draw the flames
				if(balls[n].speedY < 0) // If the ball is travelling up...
				{
					// Mask first
					BitBlt(bmoBoard, balls[n].x + offsetX, balls[n].y + offsetY, graphicSize, graphicSize, bmoExplosiveBall,
						startX + graphicSize*(balls[n].speedX+4), startY + graphicSize*(2*m+1) , SRCAND);
					// Then image
					BitBlt(bmoBoard, balls[n].x + offsetX, balls[n].y + offsetY, graphicSize, graphicSize, bmoExplosiveBall,
						startX + graphicSize*(balls[n].speedX+4), startY + graphicSize*(2*m), SRCPAINT);						
				}
				else if(balls[n].speedY > 0) // If the ball is travelling down...
				{
					// Mask first
					BitBlt(bmoBoard, balls[n].x + offsetX, balls[n].y + offsetY, graphicSize, graphicSize, bmoExplosiveBall,
						startX + graphicSize*(balls[n].speedX+4) + graphicSize*9, startY + graphicSize*(2*m+1), SRCAND);
					// Then image
					BitBlt(bmoBoard, balls[n].x + offsetX, balls[n].y + offsetY, graphicSize, graphicSize, bmoExplosiveBall,
						startX + graphicSize*(balls[n].speedX+4) + graphicSize*9, startY + graphicSize*(2*m), SRCPAINT);
					}
				else // If the ball isn't travelling...
				{
					// Draw a standard fireball
					// Mask first
					BitBlt(bmoBoard, balls[n].x, balls[n].y, 16, 16, bmoExplosiveBall, 16, 16*(7-balls[n].size), SRCAND);
					// Then image
					BitBlt(bmoBoard, balls[n].x, balls[n].y, 16, 16, bmoExplosiveBall, 0, 16*(7-balls[n].size), SRCPAINT);
				}				
			}
		}
		n++;
	}
}

void DrawBricks() // Draw the bricks
{
	int x, y; // Counters

	// Varibles use to determine which block to draw
	int topSide, bottomSide, leftSide, rightSide;
	int tlCorner, trCorner, brCorner, blCorner;

	// The centre block (unshaded) in the bitmap for the block type and colour
	int centreX, centreY;
	
	// Step through the level map
	for(x = 0; x < BGAMEWIDTH; x++)
	{
		for(y = 0; y < BGAMEHEIGHT; y++)
		{
			if(levelMap[x][y][0] || levelMap[x][y][1]) // If a block exists then draw it
			{
				// Varibles used to determine which block to draw
				topSide = 0;
				bottomSide = 0;
				leftSide = 0;
				rightSide = 0;
				tlCorner = 0;
				trCorner = 0;
				brCorner = 0;
				blCorner = 0;
				
				// The centre block (unshaded) in the bitmap for the block type and colour
				centreX = (levelMap[x][y][0]*10)-8;
				centreY = (levelMap[x][y][1]*3)-2;

				// Check if there's a block of the same type and colour above this block
				if(y != 0)
				{
					if(levelMap[x][y][0] == levelMap[x][y-1][0] && levelMap[x][y][1] == levelMap[x][y-1][1])
					{
						topSide = 1;
					}
				}
				// Check if there's a block of the same type and colour below this block
				if(y != BGAMEHEIGHT-1)
				{
					if(levelMap[x][y][0] == levelMap[x][y+1][0] && levelMap[x][y][1] == levelMap[x][y+1][1])
					{
						bottomSide = 1;
					}
				}
				// Check if there's a block of the same type and colour left of this block
				if(x != 0)
				{
					if(levelMap[x][y][0] == levelMap[x-1][y][0] && levelMap[x][y][1] == levelMap[x-1][y][1])
					{
						leftSide = 1;
					}
				}
				// Check if there's a block of the same type and colour right of this block
				if(x != BGAMEWIDTH-1)
				{
					if(levelMap[x][y][0] == levelMap[x+1][y][0] && levelMap[x][y][1] == levelMap[x+1][y][1])
					{
						rightSide = 1;
					}
				}

				// Check for every combination of matches Top, Bottom, Left, Right and draw the appropriately shaded block.
				// Additional checks for diagonally adjacent mismatches when the corresponding adjacent matches are present
				// x and y checks also present to ensure there are not out of bounds errors on the levelMap array
				if(topSide)
				{
					if(bottomSide)
					{
						if(leftSide)
						{
							if(x != 0)
							{
								if(y != 0)
								{
									// Top-Left corner check (checking for not present)
									if(levelMap[x][y][0] != levelMap[x-1][y-1][0] || levelMap[x][y][1] != levelMap[x-1][y-1][1])
									{
										// Same bricks exist above and to the left, but not diagonally up and left
										tlCorner = 1; // Need to draw a shader in the top-left corner
									}
								}
								if(y != BGAMEHEIGHT-1)
								{				
									// Bottom-Left corner check (checking for not present)					
									if(levelMap[x][y][0] != levelMap[x-1][y+1][0] || levelMap[x][y][1] != levelMap[x-1][y+1][1])
									{
										// Same bricks exist below and to the left, but not diagonally down and left
										blCorner = 1; // Need to draw a shader in the bottom-left corner
									} 
								}
							}
							if(rightSide)
							{
								if(x != BGAMEWIDTH-1)
								{
									if(y != 0)
									{
										// Top-Right corner check (checking for not present)
										if(levelMap[x][y][0] != levelMap[x+1][y-1][0] || levelMap[x][y][1] != levelMap[x+1][y-1][1])
										{	
											// Same bricks exist above and to the right, but not diagonally up and right
											trCorner = 1; // Need to draw a shader in the top-right corner
										}
									}
									if(y != BGAMEHEIGHT-1)
									{
										// Bottom-Right corner check (checking for not present)
										if(levelMap[x][y][0] != levelMap[x+1][y+1][0] || levelMap[x][y][1] != levelMap[x+1][y+1][1])
										{	
											// Same bricks exist below and to the right, but not diagonally down and right
											brCorner = 1; // Need to draw a shader in the bottom-right corner
										}
									}
								}
								// All sides match
								DrawBrick(x, y, centreX, centreY); // Draw an unshaded block
							}
							else
							{
								// Top, Left and Bottom sides match
								DrawBrick(x, y, centreX+1, centreY); // Draw a block with only right side shaded.
							}
						}
						else
						{							
							if(rightSide)
							{								
								if(x != BGAMEWIDTH-1)
								{
									if(y != 0)
									{
										// Top-Right corner check (checking for not present)
										if(levelMap[x][y][0] != levelMap[x+1][y-1][0] || levelMap[x][y][1] != levelMap[x+1][y-1][1])
										{
											// Same bricks exist above and to the right, but not diagonally up and right
											trCorner = 1; // Need to draw a shader in the top-right corner
										}
									}
									if(y != BGAMEHEIGHT-1)
									{
										// Bottom-Right corner check (checking for not present)
										if(levelMap[x][y][0] != levelMap[x+1][y+1][0] || levelMap[x][y][1] != levelMap[x+1][y+1][1])
										{	
											// Same bricks exist below and to the right, but not diagonally down and right
											brCorner = 1; // Need to draw a shader in the bottom-right corner
										}
									}
								}
								// Top, Right and Bottom sides match
								DrawBrick(x, y, centreX-1, centreY); // Draw a block with only left side shaded
							}
							else
							{
								// Top and Bottom sides match
								DrawBrick(x, y, centreX+2, centreY-1); // Draw a brick with both left and right sides shaded
							}
						}
					}
					else
					{
						if(leftSide)
						{
							if(x != 0)
							{
								if(y != 0)
								{
									// Top-Left corner check (checking for not present)
									if(levelMap[x][y][0] != levelMap[x-1][y-1][0] || levelMap[x][y][1] != levelMap[x-1][y-1][1])
									{
										// Same bricks exist above and to the left, but not diagonally up and left
										tlCorner = 1; // Need to draw a shader in the top-left corner
									}
								}
							}							
							if(rightSide)
							{
								if(x != BGAMEWIDTH-1)
								{
									if(y != 0)
									{
										// Top-Right corner check (checking for not present)
										if(levelMap[x][y][0] != levelMap[x+1][y-1][0] || levelMap[x][y][1] != levelMap[x+1][y-1][1])
										{	
											// Same bricks exist above and to the right, but not diagonally up and right
											trCorner = 1; // Need to draw a shader in the top-right corner
										}
									}
								}
								// Top, left and Right sides match
								DrawBrick(x, y, centreX, centreY+1); // Draw a brick with only bottom shaded
							}
							else
							{
								// Top and Left sides match
								DrawBrick(x, y, centreX+1, centreY+1); // Draw a brick with right and bottom sides shaded
							}
						}
						else
						{							
							if(rightSide)
							{
								if(x != BGAMEWIDTH-1)
								{
									if(y != 0)
									{
										// Top-Right corner check (checking for not present)
										if(levelMap[x][y][0] != levelMap[x+1][y-1][0] || levelMap[x][y][1] != levelMap[x+1][y-1][1])
										{	
											// Same bricks exist above and to the right, but not diagonally up and right
											trCorner = 1; // Need to draw a shader in the top-right corner
										}
									}
								}
								// Top and Right sides match
								DrawBrick(x, y, centreX-1, centreY+1); // Draw a brick with left and bottom sides shaded
							}
							else
							{
								// Only Top side matches
								DrawBrick(x, y, centreX+3, centreY+1); // Draw a brick with Left, Right and Bottom sides shaded
							}
						}
					}
				}
				else
				{
					if(bottomSide)
					{
						if(leftSide)
						{
							if(x != 0)
							{
								if(y != BGAMEHEIGHT-1)
								{	
									// Bottom-Left corner check (checking for not present)								
									if(levelMap[x][y][0] != levelMap[x-1][y+1][0] || levelMap[x][y][1] != levelMap[x-1][y+1][1])
									{
										// Same bricks exist below and to the left, but not diagonally down and left
										blCorner = 1; // Need to draw a shader in the bottom-left corner
									}
								}
							}
							if(rightSide)
							{
								if(x != BGAMEWIDTH-1)
								{
									if(y != BGAMEHEIGHT-1)
									{
										// Bottom-Right corner check (checking for not present)
										if(levelMap[x][y][0] != levelMap[x+1][y+1][0] || levelMap[x][y][1] != levelMap[x+1][y+1][1])
										{	
											// Same bricks exist below and to the right, but not diagonally down and right
											brCorner = 1; // Need to draw a shader in the bottom-right corner
										}
									}
								}
								// Left, Right and Bottom sides match
								DrawBrick(x, y, centreX, centreY-1); // Draw a brick with only top side shaded
							}
							else
							{
								// Left and Bottom sides match
								DrawBrick(x, y, centreX+1, centreY-1); // Draw a brick with top and right sides shaded
							}
						}
						else
						{
							if(rightSide)
							{
								if(x != BGAMEWIDTH-1)
								{
									if(y != BGAMEHEIGHT-1)
									{
										// Bottom-Right corner check (checking for not present)
										if(levelMap[x][y][0] != levelMap[x+1][y+1][0] || levelMap[x][y][1] != levelMap[x+1][y+1][1])
										{	
											// Same bricks exist below and to the right, but not diagonally down and right
											brCorner = 1; // Need to draw a shader in the bottom-right corner
										}
									}
								}
								// Right and Bottom sides match
								DrawBrick(x, y, centreX-1, centreY-1); // Draw a brick with top and left sides shaded
							}
							else
							{
								// Only Bottom side matches
								DrawBrick(x, y, centreX+3, centreY-1); // Draw a brick with top, left and right sides shaded
							}
						}
					}
					else
					{
						if(leftSide)
						{
							if(rightSide)
							{
								// Left and Right sides match
								DrawBrick(x, y, centreX+2, centreY+1); // Draw a brick with top and bottom sides shaded
							}
							else
							{
								// Only Left side matches
								DrawBrick(x, y, centreX+4, centreY+1); // Draw a brick with top, right and bottom sides shaded
							}
						}
						else
						{
							if(rightSide)
							{
								// Onlt Right side matches
								DrawBrick(x, y, centreX+2, centreY); // Draw a brick with top, left and bottom sides shaded
							}
							else
							{
								// No sides match
								DrawBrick(x, y, centreX+3, centreY); // Draw a brick with all sides shaded
							}
						}
					}
				}

				// Overlay shades on the corners of the block based on the earlier diagonal checks
				if(tlCorner)
				{
					DrawBrickCorner(x, y, centreX+4, centreY-1); // Draw the inverse shade in the top-left corner
				}
				if(trCorner)
				{
					DrawBrickCorner(x, y, centreX+5, centreY-1); // Draw the inverse shade in the top-right corner
				}
				if(brCorner)
				{
					DrawBrickCorner(x, y, centreX+6, centreY-1); // Draw the inverse shade in the bottom-left corner
				}
				if(blCorner)
				{
					DrawBrickCorner(x, y, centreX+7, centreY-1); // Draw the inverse shade in the bottom-right corner
				}
			}
		}
	}
}

void DrawBrick(int x, int y, int tileX, int tileY) // Draw the requested brick
{
	// Mask first
	BitBlt(bmoBoard, x*BRICKSIZE, y*BRICKSIZE, BRICKSIZE, BRICKSIZE, bmoBricks, 0, 0, SRCAND);
	// Then image
	BitBlt(bmoBoard, x*BRICKSIZE, y*BRICKSIZE, BRICKSIZE, BRICKSIZE, bmoBricks, tileX*BRICKSIZE, tileY*BRICKSIZE, SRCPAINT);
}

void DrawBrickCorner(int x, int y, int tileX, int tileY) // Overlay and inversed shade in a bricks corner
{
	// Mask first
	BitBlt(bmoBoard, x*BRICKSIZE, y*BRICKSIZE, BRICKSIZE, BRICKSIZE, bmoBricks, tileX*BRICKSIZE, (tileY+1)*BRICKSIZE, SRCAND);
	// Then image
	BitBlt(bmoBoard, x*BRICKSIZE, y*BRICKSIZE, BRICKSIZE, BRICKSIZE, bmoBricks, tileX*BRICKSIZE, tileY*BRICKSIZE, SRCPAINT);
}

void DrawExtraLives() // Draw the extra lives box in the top left corner
{
	int n; // Counter
	
	// Draw the label first
	// Mask first
	BitBlt(bmoBoard, 0, 0, 48, 16, bmoLabels, 0, LABEL_EXTRALIVES*2*(TILESIZE*2) + TILESIZE*2, SRCAND);
	// Then image
	BitBlt(bmoBoard, 0, 0, 48, 16, bmoLabels, 0, LABEL_EXTRALIVES*2*(TILESIZE*2), SRCPAINT);
	
	// Expand the box for each extra life and draw it
	n = 0;
	while(n < GetLife())
	{
		
		// Draw the box background
		// Mask first
		BitBlt(bmoBoard, (n+1)*TILESIZE + 32, 0, 16, 16, bmoLabels, 48, LABEL_EXTRALIVES*2*(TILESIZE*2) + TILESIZE*2, SRCAND);
		// Then image
		BitBlt(bmoBoard, (n+1)*TILESIZE + 32, 0, 16, 16, bmoLabels, 48, LABEL_EXTRALIVES*2*(TILESIZE*2), SRCPAINT);
		
		// Overlay a small ball to represent each extra life
		// Mask first
		BitBlt(bmoBoard, (n+1)*TILESIZE + 32, 4, 16, 16, bmoBall, TILESIZE*2*(1), TILESIZE*2, SRCAND);
		// Then image
		BitBlt(bmoBoard, (n+1)*TILESIZE + 32, 4, 16, 16, bmoBall, TILESIZE*2*(1), 0, SRCPAINT);

		n++;
	}
}

void DrawScore() // Draw the score box in the top right corner
{
	// Variables for holding different numerals and place holders in the score
	int temp1, temp2, posScore;

	temp1 = score;
	temp2 = score;
	posScore = 0;

	// Count the digits in the score by dividing by 10 till theres no more result
	while(temp1/10)
	{
		posScore++;
		temp1 = temp1/10;
	}

	// Draw the box background with enough room to draw the score
	// Mask first
	BitBlt(bmoBoard, GAMEWIDTH*TILESIZE-(48+(posScore+1)*8), 0, 48, 16, bmoLabels, 0, LABEL_SCORE*2*(TILESIZE*2) + TILESIZE*2, SRCAND);
	// Then image
	BitBlt(bmoBoard, GAMEWIDTH*TILESIZE-(48+(posScore+1)*8), 0, 48, 16, bmoLabels, 0, LABEL_SCORE*2*(TILESIZE*2), SRCPAINT);
	
	// Draw each digit in the score
	while(posScore >= 0)
	{
		// Extend the background for the digit
		// Mask first
		BitBlt(bmoBoard, GAMEWIDTH*TILESIZE-(8+(posScore+1)*8), 0, 16, 16, bmoLabels, 48, LABEL_SCORE*2*(TILESIZE*2) + TILESIZE*2, SRCAND);
		// Then image
		BitBlt(bmoBoard, GAMEWIDTH*TILESIZE-(8+(posScore+1)*8), 0, 16, 16, bmoLabels, 48, LABEL_SCORE*2*(TILESIZE*2), SRCPAINT);

		// Calculate and draw each digit
		temp1 = temp2/MyPower(10,posScore); // Digit = temporary score / 10^posScore
		temp2 -= temp1*MyPower(10,posScore); // Remove the printed digit from the temporary score
		// Mask first
		BitBlt(bmoBoard, GAMEWIDTH*TILESIZE-(8+(posScore+1)*8), 0, 8, 16, bmoLabels, temp1*TILESIZE, LABEL_DIGIT*2*(TILESIZE*2) + TILESIZE*2, SRCAND);
		// Then image
		BitBlt(bmoBoard, GAMEWIDTH*TILESIZE-(8+(posScore+1)*8), 0, 8, 16, bmoLabels, temp1*TILESIZE, LABEL_DIGIT*2*(TILESIZE*2), SRCPAINT);

		posScore--;
	}
}

void DrawHelp() // Draw the help screen
{
	int x, y; // Counters
	int centreX, centreY; // The x and y co-ordinates for the 'plain' block of the chosen colour and style

	centreX = (GetHelpStyle()*10)-8;
	centreY = (GetHelpColour()*3)-2;

	//Fill the help background with current brick colour and style
	for(x = 0; x < BGAMEWIDTH; x++)
	{
		for(y = 0; y < BGAMEHEIGHT; y++)
		{
			DrawBrick(x, y, centreX, centreY); // Draw a plain brick
		}
	}

	// Draw a border around the 320x240 help window
	for(x = 9; x < BGAMEWIDTH-9; x++)
	{
		for(y = 6; y < BGAMEHEIGHT-6; y++)
		{
			if(x == 9)
			{
				if(y == 6)
				{
					DrawBrickCorner(x, y, centreX+6, centreY-1); // Draw the top-left border of the help window
				} else {
					if(y == BGAMEHEIGHT-7)
					{
						DrawBrickCorner(x, y, centreX+5, centreY-1); // Draw the bottom-left border of the help window
					} else {
						DrawBrick(x, y, centreX+1, centreY); // Draw the left side border of the help window
					}
				}
			} else {
				if(x == BGAMEWIDTH-10)
				{
					if(y == 6)
					{
						DrawBrickCorner(x, y, centreX+7, centreY-1); // Draw the top-right border of the help window
					} else {
						if(y == BGAMEHEIGHT-7)
						{
							DrawBrickCorner(x, y, centreX+4, centreY-1); // Draw the bottom-right border of the help window
						} else {
							DrawBrick(x, y, centreX-1, centreY); // Draw the right side border of the help window
						}
					}
				} else {
					if(y == 6)
					{
						DrawBrick(x, y, centreX, centreY+1); // Draw the top border of the help window
					} else {
						if(y == BGAMEHEIGHT-7)
						{
							DrawBrick(x, y, centreX, centreY-1); // Draw the bottom border of the help window
						}
					}
				}
			}
		}
	}

	// Draw the game boarder with type 0 (include bottom line)
	DrawBorders(0);

	// Draw the extra lives
	DrawExtraLives();

	// Draw the current score
	DrawScore();

	// Draw the help panel
	// Mask first
	BitBlt(bmoBoard, (GAMEWIDTH*TILESIZE/2)-160, (GAMEHEIGHT*TILESIZE/2)-128, 320, 256, bmoHelp, 0, (GetPausedGame()-1)*256, SRCAND);
	// Then image
	BitBlt(bmoBoard, (GAMEWIDTH*TILESIZE/2)-160, (GAMEHEIGHT*TILESIZE/2)-128, 320, 256, bmoHelp, 0, (GetPausedGame()-1)*256, SRCPAINT);


	// Draw the 'Press spacebar to see more help' message
	for(x = 0; x < 2; x++)
	{
		// Mask first
		BitBlt(bmoBoard, (GAMEWIDTH*TILESIZE/2)-127 + (x*127), (GAMEHEIGHT*TILESIZE/2)+164, 127, 24, bmoHelp, 320, x*24, SRCAND);
		// Then image
		BitBlt(bmoBoard, (GAMEWIDTH*TILESIZE/2)-127 + (x*127), (GAMEHEIGHT*TILESIZE/2)+164, 127, 24, bmoHelp, 320, x*24, SRCPAINT);
	}
}

void DrawCoins() // Draw the powerup coins
{
	int n; // Counter
	
	// Draw all coins that exist
	n = 0;
	while(n < 20)
	{
		if(coins[n].rotationPos >= 0)
		{
			// Mask first
			BitBlt(bmoBoard, coins[n].x, coins[n].y, 16, 16, bmoCoin, 16*(coins[n].powerup-1), 16*(2*( (coins[n].rotationPos)/COINSPEED )+1), SRCAND);
			// Then image
			BitBlt(bmoBoard, coins[n].x, coins[n].y, 16, 16, bmoCoin, 16*(coins[n].powerup-1), 16*(2*( (coins[n].rotationPos)/COINSPEED )), SRCPAINT);
		}
		n++;
	}
}

void DrawExplosions() // Draw the explosions
{
	int n; // Counter
	int type; // Random explosion graphic

	// Draw all the explosions that exist
	n = 0;
	while(n < 25)
	{
		if(explosions[n].size > 0)
		{
			type = rand() % 4; // Pick a random graphic type out of 4 for each size
			// Mask first
			BitBlt(bmoBoard, explosions[n].x, explosions[n].y, 80, 80, bmoExplosion, 80*((explosions[n].size - 1)/FRAMES), 80*(2*type+1), SRCAND);
			// Then image
			BitBlt(bmoBoard, explosions[n].x, explosions[n].y, 80, 80, bmoExplosion, 80*((explosions[n].size - 1)/FRAMES), 80*(2*type), SRCPAINT);
		}
		n++;
	}
}

void DrawBullets() // Draw all the bullets in the game
{
	int n; // Counter

	n = 0;
	while(n < 20) // Cycle through the bullets
	{
		if(bullets[n].x != 0) // If the bullet exists...
		{
			// Mask first
			BitBlt(bmoBoard, bullets[n].x, bullets[n].y, 2, 6, bmoLaser, 24, 0, SRCAND);
			// Then image
			BitBlt(bmoBoard, bullets[n].x, bullets[n].y, 2, 6, bmoLaser, 24, 0, SRCPAINT);
		}
		else // If the bullet doesn't exist
		{
			break; // Stop drawing bullets
		}
		n++;
	}
}

void DrawMessages() // Draw the messages
{
	int n; // Counter

	n = 0;
	while(n < 3) // Cycle through the messages
	{
		// Mask first
		BitBlt(bmoBoard, GAMEWIDTH*TILESIZE/2 - 80, TILESIZE*35 + n*36, 160, 32, bmoMessages, (messages[n]-1)*160, 32, SRCAND);
		// Then image
		BitBlt(bmoBoard, GAMEWIDTH*TILESIZE/2 - 80, TILESIZE*35 + n*36, 160, 32, bmoMessages, (messages[n]-1)*160, 0, SRCPAINT);
		
		n++;
	}
}

void DrawConfirmation() // Draws a confirmation dialog box
{
	int x, y; // Counters
	int frameSizeX = 320; // Horizontal frame size
	int frameSizeY = 160; // Vertical frame size
	int posX, posY; // Placement position for the confirmation box
	
	posX = GAMEWIDTH*TILESIZE/2 - frameSizeX/2; // Horizontal position of the confirmation box
	posY = GAMEHEIGHT*TILESIZE/2 - frameSizeY/2; // Vertical position of the confirmation box

	for(x = 0; x < BGAMEWIDTH; x++) 
	{
		for(y = 0; y < BGAMEHEIGHT; y++)
		{
			// Fade the screen before placing the confirmation window
			// Mark first
			BitBlt(bmoBoard, x*BRICKSIZE, y*BRICKSIZE, 16, 16, bmoConfirmation, 16, 0, SRCAND);
			// Then image
			BitBlt(bmoBoard, x*BRICKSIZE, y*BRICKSIZE, 16, 16, bmoConfirmation, 0, 0, SRCPAINT);
		}
	}

	// Draw the confirmation box
	// Mark first
	BitBlt(bmoBoard, posX, posY, frameSizeX, frameSizeY, bmoConfirmation,
		frameSizeX*confirmationAction, frameSizeY*(confirmationBox-1)+16, SRCAND);
	// Then image
	BitBlt(bmoBoard, posX, posY, frameSizeX, frameSizeY, bmoConfirmation,
		frameSizeX*confirmationAction, frameSizeY*(confirmationBox-1)+16, SRCPAINT);
}

void DrawGameMenu() // Draws the game menu
{
	int frameSizeX = 148; // Horizontal frame size
	int frameSizeY = 87; // Vertical frame size
	int posX, posY; // Placement position for the confirmation box

	// Calculate the menu placement
	posX = GAMEWIDTH*TILESIZE/2 - frameSizeX/2;
	posY = 15;
	
	// Draw the menu
	// Mark first
	BitBlt(bmoBoard, posX, posY, frameSizeX, frameSizeY, bmoGameMenu, 0, 0, SRCAND);
	// Then image
	BitBlt(bmoBoard, posX, posY, frameSizeX, frameSizeY, bmoGameMenu, 0, 0, SRCPAINT);
}

void StartGame() // Start a new game
{
	// Reset the paddle
	paddleSize = 8;
	paddlePos = 279;
	paddleSpeed = 4;
	paddleDirection = 0;
	
	livesRemaining = 4; // Reset the lives to 4 (one will be used)
	level = 1; // Reset the level to 1
	scoreMultiplier = 1; // Reset the score multiplier to 1
	score = 0; // Reset the score to 0

	LoadLevel(level); // Load the starting level
	LoadBackground(level); // Load the level background
	UseLife(); // Use a life
	
	gameLost = false; // New game isn't lost
	UnpauseGame(); // Unpause the game
}

void FinishGame()
{
	// Clean up anything here before the game quits
}

void AdjustPaddleSize(int size) // Change the paddle size
{
	if(size > 0) // If the size change is positive...
	{
		paddleSize++; // Increase the paddle size by one
	}
	if(size < 0) // If the size change is negative...
	{
		paddleSize--; // Decrease the paddle size by one
	}
	if(paddleSize < 2) // Paddle size can not go below two
	{
		paddleSize = 2;
	}
	if(paddleSize > 14) // Paddle size can not go above fourteen
	{
		paddleSize = 14;
	}
	// Make sure the paddle doesn't go beyond the borders
	if(GetPaddlePosition() + 8*(GetPaddleSize()+2) >= GAMEWIDTH*TILESIZE - 9)
	{
		paddlePos = GAMEWIDTH*TILESIZE - 9 - 8*(GetPaddleSize()+2); // Move the paddle if necessary
	}
	return;
}

int GetPaddleSize() // Gives the size of the paddle
{
	return paddleSize;
}

void MovePaddlePosition() // Move the paddle
{ 
	int n, m; // Counters
	int x, y; // More counters
	int tempPos; // Temporary paddle position for collision calculations
	int ballAdj; // Number of pixels to move all remaining stuck balls

	bool ballBounced; // Used when checking if the paddle hit a ball
	bool paddleOverlap; // Used when checking if the paddle overlaps the ball when pushed against a border
	// bool coinCollected; // Used when checking if the paddle hit a powerup coin

	// Move and check one pixel for each point of paddle speed
	m = 0;
	while(m < GetPaddleSpeed())
	{
		if( (GetPaddlePosition() + GetPaddleDirection()) < TILESIZE)
		{  // If the paddle hits the left border...
			SetPaddleDirection(0); // Stop the paddle form moving
			return;
		}
		if (GetPaddlePosition() + GetPaddleDirection() + (TILESIZE*2 + GetPaddleSize()*TILESIZE) >= (TILESIZE*GAMEWIDTH-TILESIZE+1))
		{ // If the paddle hits the right border...
			SetPaddleDirection(0); // Stop the paddle form moving
			return;
		}

		tempPos = paddlePos + GetPaddleDirection(); // Calculate whe the paddle is moving to

		// Move all balls that are stuck and check the rest for collisions
		n = 0;
		while(n < 5)
		{
			if(balls[n].size == -1) // Stop when no more balls exist
			{
				break;
			}

			ballBounced = false; // Ball not hit

			if(!balls[n].stuck) // Check for collision with balls that aren't stuck to it
			{
				// Check each pixel row of the ball
				for(y = 0; y < 16; y++)
				{
					// Find the right most pixel of the ball in each row
					x = 15;
					while(x > 0)
					{
						if(balls[n].map[x][y])
						{
							break;
						}
						x--;
					}					
					if(x) // If a ball pixel exists in that row...
					{
						// Then check that pixel and it's mirror opposite against each its corresponding row on the paddle
						if(balls[n].y + y >= (GAMEHEIGHT*TILESIZE) - (TILESIZE*2-2) &&
							balls[n].y + y < GAMEHEIGHT*TILESIZE-2)
						{
							if(balls[n].y + y >= (GAMEHEIGHT*TILESIZE) - (TILESIZE*2-2) +1 &&
								balls[n].y + y < GAMEHEIGHT*TILESIZE-3)
							{
								if(balls[n].y + y >= (GAMEHEIGHT*TILESIZE) - (TILESIZE*2-2) +2 &&
									balls[n].y + y < GAMEHEIGHT*TILESIZE-4)
								{
									if(balls[n].x + x >= tempPos && balls[n].x + x <= tempPos + 8*(GetPaddleSize()+2) ||
										balls[n].x + (15-x) >= tempPos && balls[n].x + (15-x) <= tempPos + 8*(GetPaddleSize()+2))
									{
										ballBounced = true; // Paddle hit the ball
									}
								}
								if(balls[n].x + x >= tempPos + 1 && balls[n].x + x <= tempPos + 8*(GetPaddleSize()+2) - 1 ||
									balls[n].x + (15-x) >= tempPos + 1 && balls[n].x + (15-x) <= tempPos + 8*(GetPaddleSize()+2) - 1)
								{
									ballBounced = true; // Paddle hit the ball
								}
							}
							if(balls[n].x + x >= tempPos + 3 && balls[n].x + x <= tempPos + 8*(GetPaddleSize()+2) - 3 ||
								balls[n].x + (15-x) >= tempPos + 3 && balls[n].x + (15-x) <= tempPos + 8*(GetPaddleSize()+2) - 3)
							{
								ballBounced = true; // Paddle hit the ball
							}
						}
					}
				}
			}

			if(ballBounced) // If the paddle hit the ball...
			{
				if(balls[n].noRebound <= 0) // If the ball is allowed to rebound off the paddle...
				{
					AddSound(SOUND_PADDLEREBOUND); // Play the ball hitting the paddle sound
					if(magnetic) // If the magnetic ability is on...
					{
						balls[n].stuck = true; // The ball becomes stuck to the paddle
						balls[n].speedX = 0;
						balls[n].speedY = 0;
					}
					else // If the paddle is not magnetic...
					{
						ReleaseBall(n); // Then the ball bounces off in a direction depending on where it hit
					}
					m = GetPaddleSpeed(); // Stop moving the paddle when it hits a ball
				}
			}

			ballAdj = 0; // Currently there is no extra ball adjustment

			if(balls[n].stuck) // Move stuck and newly stuck balls
			{
				balls[n].x += GetPaddleDirection(); // Move the stuck ball as well
				balls[n].x += ballAdj; // Move the ball any extra adjuments created by previous stuck balls
				
				if(GetPaddleDirection() > 0) // If the paddle moved right...
				{
					if( balls[n].x > (GAMEWIDTH*TILESIZE) - TILESIZE - (TILESIZE*2 - (7 - balls[n].size)) )
					// If the ball moves past the border...
					{
						paddleOverlap = false; // Paddle doesn't overlap the ball yet

						// Move the ball back into the game
						balls[n].x = (GAMEWIDTH*TILESIZE) - TILESIZE - (TILESIZE*2 - (7 - balls[n].size));

						if( balls[n].y > (GAMEHEIGHT*TILESIZE) - (2*TILESIZE-2) - (2*TILESIZE - (7 - balls[n].size)) )
						// If the ball isn't sitting on top of the paddle...
						{
							// Move the ball up one pixel on the paddle
							balls[n].y--;
						}
						
						// Check each pixel row of the ball
						for(y = 0; y < 16; y++)
						{
							// Find the left most pixel of the ball in each row
							x = 0;
							while(x < 16)
							{
								if(balls[n].map[x][y])
								{
									break;
								}
								x++;
							}					
							if(x < 16) // If a ball pixel exists in that row...
							{
								// Then check that pixel against each its corresponding row on the paddle
								if(balls[n].y + y >= (GAMEHEIGHT*TILESIZE) - (TILESIZE*2-2) &&
									balls[n].y + y < GAMEHEIGHT*TILESIZE-2)
								{
									if(balls[n].y + y >= (GAMEHEIGHT*TILESIZE) - (TILESIZE*2-2) +1 &&
										balls[n].y + y < GAMEHEIGHT*TILESIZE-3)
									{
										if(balls[n].y + y >= (GAMEHEIGHT*TILESIZE) - (TILESIZE*2-2) +2 &&
											balls[n].y + y < GAMEHEIGHT*TILESIZE-4)
										{
											if(balls[n].x + x >= tempPos && balls[n].x + x <= tempPos + 8*(GetPaddleSize()+2))
											{
												paddleOverlap = true; // Paddle overlaps the ball
											}
										}
										if(balls[n].x + x >= tempPos + 1 && balls[n].x + x <= tempPos + 8*(GetPaddleSize()+2) - 1)
										{
											paddleOverlap = true; // Paddle overlaps the ball
										}
									}
									if(balls[n].x + x >= tempPos + 3 && balls[n].x + x <= tempPos + 8*(GetPaddleSize()+2) - 3)
									{
										paddleOverlap = true; // Paddle overlaps the ball
									}
								}
							}
						}

						if(paddleOverlap) // If the paddle overlaps the ball...
						{
							paddlePos--; // Move the paddle left one
							ballAdj--; // All other stuck balls move left one too
						}
						
						m = GetPaddleSpeed(); // Stop moving the paddle when the ball is pushed against the border
					}
				}
				else // If the paddle moved left...
				{							
					if( balls[n].x < TILESIZE - (7 - balls[n].size) )
					// If the ball moves past the border...
					{
						paddleOverlap = false; // Paddle doesn't overlap the ball yet

						// Move the ball back into the game
						balls[n].x = TILESIZE - (7 - balls[n].size);

						if( balls[n].y > (GAMEHEIGHT*TILESIZE) - (2*TILESIZE-2) - (2*TILESIZE - (7 - balls[n].size)) )
						// If the ball isn't sitting on top of the paddle...
						{
							// Move the ball up one pixel on the paddle
							balls[n].y--;
						}
						
						// Check each pixel row of the ball
						for(y = 0; y < 16; y++)
						{
							// Find the right most pixel of the ball in each row
							x = 15;
							while(x >= 0)
							{
								if(balls[n].map[x][y])
								{
									break;
								}
								x--;
							}					
							if(x >= 0) // If a ball pixel exists in that row...
							{
								// Then check that pixel against each its corresponding row on the paddle
								if(balls[n].y + y >= (GAMEHEIGHT*TILESIZE) - (TILESIZE*2-2) &&
									balls[n].y + y < GAMEHEIGHT*TILESIZE-2)
								{
									if(balls[n].y + y >= (GAMEHEIGHT*TILESIZE) - (TILESIZE*2-2) +1 &&
										balls[n].y + y < GAMEHEIGHT*TILESIZE-3)
									{
										if(balls[n].y + y >= (GAMEHEIGHT*TILESIZE) - (TILESIZE*2-2) +2 &&
											balls[n].y + y < GAMEHEIGHT*TILESIZE-4)
										{
											if(balls[n].x + x >= tempPos && balls[n].x + x <= tempPos + 8*(GetPaddleSize()+2))
											{
												paddleOverlap = true; // Paddle overlaps the ball
											}
										}
										if(balls[n].x + x >= tempPos + 1 && balls[n].x + x <= tempPos + 8*(GetPaddleSize()+2) - 1)
										{
											paddleOverlap = true; // Paddle overlaps the ball
										}
									}
									if(balls[n].x + x >= tempPos + 3 && balls[n].x + x <= tempPos + 8*(GetPaddleSize()+2) - 3)
									{
										paddleOverlap = true; // Paddle overlaps the ball
									}
								}
							}
						}

						if(paddleOverlap) // If the paddle overlaps the ball...
						{
							paddlePos++; // Move the paddle right one
							ballAdj++; // All other stuck balls move right one too
						}
						
						m = GetPaddleSpeed(); // Stop moving the paddle when the ball is pushed against the border
					}
				}
			}

			n++;
		}
		
		/*
		n = 0;
		while(n < 20)
		{
			if(!coins[n].rotationPos) // Stop when no more coins exist
			{
				break;
			}
			
			coinCollected = false; // Coin not hit

			// Check for collision with a coin			
			for(y = 0; y < 16; y++) // Check each pixel row of the coin
			{
				// Find the right most pixel of the coin in each row
				x = 16;
				while(x > 0)
				{
					if(coinMap[coins[n].rotationPos/COINSPEED][x][y])
					{
						break;
					}
					x--;
				}
				if(x) // If a coin pixel exists in that row...
				{
					// Then check that pixel and it's mirror opposite against each its corresponding row on the paddle
					if(coins[n].y + y >= (GAMEHEIGHT*TILESIZE-1) - (TILESIZE*2-2) &&
						coins[n].y + y < GAMEHEIGHT*TILESIZE-2)
					{
						if(coins[n].y + y >= (GAMEHEIGHT*TILESIZE-1) - (TILESIZE*2-2) +1 &&
							coins[n].y + y < GAMEHEIGHT*TILESIZE-3)
						{
							if(coins[n].y + y >= (GAMEHEIGHT*TILESIZE-1) - (TILESIZE*2-2) +2 &&
								coins[n].y + y < GAMEHEIGHT*TILESIZE-4)
							{
								if(coins[n].x + x >= tempPos && coins[n].x + x < tempPos + 8*(GetPaddleSize()+2) ||
									coins[n].x + (15-x) >= tempPos && coins[n].x + (15-x) < tempPos + 8*(GetPaddleSize()+2) )
								{
									coinCollected = true; // Paddle hit the coin
								}
							}
							if(coins[n].x + x >= tempPos + 1 && coins[n].x + x < tempPos + 8*(GetPaddleSize()+2) - 1 ||
								coins[n].x + (15-x) >= tempPos + 1 && coins[n].x + (15-x) < tempPos + 8*(GetPaddleSize()+2) - 1)
							{
								coinCollected = true; // Paddle hit the coin
							}
						}
						if(coins[n].x + x >= tempPos + 3 && coins[n].x + x < tempPos + 8*(GetPaddleSize()+2) - 3 ||
							coins[n].x + (15-x) >= tempPos + 3 && coins[n].x + (15-x) < tempPos + 8*(GetPaddleSize()+2) - 3)
						{
							coinCollected = true; // Paddle hit the coin
						}
					}
				}
			}

			if(coinCollected) // If the paddle hit the coin...
			{
				AddSound(SOUND_COIN); // Play the coin collection sound
				GainPowerup(coins[n].powerup); // Gain the coin's powerup
				LoseCoin(n); // Remove the coin from the game
			}

			n++;
		}
		*/

		paddlePos += GetPaddleDirection(); // Move the paddle

		m++;
	}
}

int GetPaddlePosition() // Gives the x position of the left side of the paddle
{
	return paddlePos;
}

void AdjustPaddleSpeed(int speedChange) // Change the speed with which the paddle moves
{
	if(speedChange > 0) // If the speed change is positive...
	{
		paddleSpeed++; // Increase the speed by one
	}
	if(speedChange < 0) // If the speed change is negative...
	{
		paddleSpeed--; // Decrease the speed by one
	}
	if(paddleSpeed < 2) // The speed can not be less than two
	{
		paddleSpeed = 2;
	}
	if(paddleSpeed > 10) // The speed can not be greater than ten
	{
		paddleSpeed = 10;
	}
}

int GetPaddleSpeed() // Gives the paddle's speed
{
	return paddleSpeed;
}

void ReleaseBall(int num) // Releases the ball from the paddle in a direction depending on its placement on the paddle
{
	int ballPadPos; // Position where the ball touchs the paddle
	int padPixels; // Number of pixels in the paddle

	balls[num].stuck = false; // Ball is no longer stuck
	balls[num].noRebound = 4; // Ball can not be hit again for 4 frames
	balls[num].greyBricks = 0; // Reset the grey brick count

	//Ball Position + TILESIZE to refer to the centre of the ball - Paddle Position
	ballPadPos = balls[num].x + TILESIZE - GetPaddlePosition();

	// (Paddle size + 2) * TILESIZE
	padPixels = (GetPaddleSize() + 2) * TILESIZE; // 48, 56, 64, 72, 80, 88, 96, 104, 112
	
	// How the direction of release should be broken up over the pixels of the paddle depending on paddle size
	// 0-5, 6-11, 12-17, 18-23, 24-29, 30-35, 36-41, 42-47
	// 0-6, 7-13, 14-20, 21-27, 28-34, 35-41, 42-48, 49-55
	// 0-7, 8-15, 16-23, 24-31, 32-39, 40-47, 48-55, 56-63	
	// 0-8, 9-17, 18-26, 27-35, 36-44, 45-53, 54-62, 63-71
	// 0-9, 10-19, 20-29, 30-39, 40-49, 50-59, 60-69, 70-79
	// 0-10, 11-21, 22-32, 33-43, 44-54, 55-65, 66-76, 77-87
	// 0-11, 12-23, 24-35, 36-47, 48-59, 60-71, 72-83, 84-95
	// 0-12, 13-25, 26-38, 39-51, 52-64, 65-77, 78-90, 91-103
	// 0-13, 14-27, 28-41, 42-55, 56-69, 70-83, 84-97, 98-111

	balls[num].speedX = (ballPadPos/(GetPaddleSize() +2)) - 4; // SpeedX of the ball between -4 and 3
	if(balls[num].speedX >= 0) // SpeedX of the between -4 and 4, excluding 0
		balls[num].speedX++;
	if(balls[num].speedX > 4) // Ensure speedX of the ball isn't greater than 4
		balls[num].speedX = 4;
	if(balls[num].speedX < -4) // Ensure speedX of the ball isn't lower than -4
		balls[num].speedX = -4;

	// Set the Y speed of the ball based on the X speed
	balls[num].speedY = abs(balls[num].speedX) - 5; // between -1 and -5
	if(balls[num].speedY >= 0) // Ensure there are no speeds above -1
	{
		balls[num].speedY = -1;
	}
}

void MoveBalls() // Move all the balls currently on the game board
{
	//int tileX, tileY
	int num; // ball number
	int dirX, dirY; // X and Y directions

	// Move each of the five balls
	num = 0;
	while(num < 5)
	{
		// Ignore balls that are stuck
		if(balls[num].stuck == false)
		{
			// Ignore balls that don't exist yet
			if(balls[num].size != -1)
			{
				// Movement
				if(balls[num].speedX > 0) // Set the X direction variable
				{
					dirX = 1;
				}
				else {
					dirX = -1;
				}
				if(balls[num].speedY > 0) // Set the Y direction variable
				{
					dirY = 1;
				}
				else
				{
					dirY = -1;
				}
				
				// Check each pixel movement for collisions before moving
				switch(abs(balls[num].speedX))
				{
				case 0: break;
				case 1:
					if(!CollisionCheck(num, 0, dirY)) // collision check y, move
					{
						balls[num].y += dirY;
					} else {break;}
					if(!CollisionCheck(num, 0, dirY)) // collision check y, move
					{
						balls[num].y += dirY;
					} else {break;}
					if(!CollisionCheck(num, dirX, 0)) // collision check x, move
					{
						balls[num].x += dirX;
					} else {break;}
					if(!CollisionCheck(num, 0, dirY)) // collision check y, move
					{
						balls[num].y += dirY;
					} else {break;}
					if(!CollisionCheck(num, 0, dirY)) // collision check y, move
					{
						balls[num].y += dirY;
					} else {break;}
					break;
				case 2:
					if(!CollisionCheck(num, 0, dirY)) // collision check y, move
					{
						balls[num].y += dirY;
					} else {break;}
					if(!CollisionCheck(num, dirX, 0)) // collision check x, move
					{
						balls[num].x += dirX;
					} else {break;}
					if(!CollisionCheck(num, 0, dirY)) // collision check y, move
					{
						balls[num].y += dirY;
					} else {break;}
					if(!CollisionCheck(num, dirX, 0)) // collision check x, move
						{
						balls[num].x += dirX;
					} else {break;}
					if(!CollisionCheck(num, 0, dirY)) // collision check y, move
					{
						balls[num].y += dirY;
					} else {break;}
					break;
				case 3:
					if(!CollisionCheck(num, dirX, 0)) // collision check x, move
					{
						balls[num].x += dirX;
					} else {break;}
					if(!CollisionCheck(num, 0, dirY)) // collision check y, move
					{
						balls[num].y += dirY;
					} else {break;}
					if(!CollisionCheck(num, dirX, 0)) // collision check x, move
					{
						balls[num].x += dirX;
					} else {break;}
					if(!CollisionCheck(num, 0, dirY)) // collision check y, move
					{
						balls[num].y += dirY;
					} else {break;}
					if(!CollisionCheck(num, dirX, 0)) // collision check x, move
					{
						balls[num].x += dirX;
					} else {break;}
					break;
				case 4:
					if(!CollisionCheck(num, dirX, 0)) // collision check x, move
					{
						balls[num].x += dirX;
					} else {break;}
					if(!CollisionCheck(num, dirX, 0)) // collision check x, move
					{
						balls[num].x += dirX;
					} else {break;}
					if(!CollisionCheck(num, 0, dirY)) // collision check y, move
					{
						balls[num].y += dirY;
					} else {break;}
					if(!CollisionCheck(num, dirX, 0)) // collision check x, move
					{
						balls[num].x += dirX;
					} else {break;}
					if(!CollisionCheck(num, dirX, 0)) // collision check x, move
					{
						balls[num].x += dirX;
					} else {break;}
				}
			}
		}
		num++;
	}
}

void LoadBackground(int num) // Load the level background from the corresponding bitmap
{
	FILE *backgroundFile; // Varible used to test if the file exists

	// Variables for storing a dynamic filename
	std::ostringstream levelString;
	std::string filename;

	// Set the filename to Background#.bmp where # is the level number
	levelString << "Background" << num << ".bmp";
	filename = levelString.str(); // Convert the filename to a usable string

	backgroundFile = fopen(filename.c_str(), "r"); // Attempt to load the background
	if(backgroundFile != NULL) // If the background file exists...
	{
		bmoBackground.Load(NULL, filename.c_str()); // Load the background bitmap
		fclose(backgroundFile); // Close the file
	}
	else // If the background file does not exist...
	{
		bmoBackground.Load(NULL, "Background1.bmp"); // Load the first background
	}

	return;
}

bool LoadLevel(int num) // Load the level map from file.
{
	int x, y; // Counters
	int levelID = 0; // The level number retrieved from the levels file
	char ch; // Place holder for each character as it's read from the file.
	bool levelFound = false; // Check bit to see if the level desired was found or not
	FILE *levelFile; // File to read the level data from
	int powerup; // Determines the powerup to be assigned to a block
	
	// Variables for handling the characters retrieved from the file
	std::ostringstream fileNum;
	std::string strNum;

	levelFile = fopen("Levels.txt", "r"); // Open Levels.txt for reading
	
	if(levelFile != NULL) // Check that the file opened
	{
		do {
			ch = fgetc(levelFile); // Grab the first character in the file
			if(ch == 'L') // Found a level marker now check if it's the level we want
			{
				fileNum.str(""); // Reset the data stream
				ch = fgetc(levelFile); // Retrieve the first digit
				while(ch != '\n' && ch != EOF) // Retrieve all the characters left on the line
				{
					fileNum << ch; // Concatenate each character retrieved into one variable
					ch = fgetc(levelFile); // Retrieve the next character
				}
				strNum = fileNum.str(); // Convert the level digits into a string
				levelID = atoi(strNum.c_str()); // Convert the string into an integar

				// Moving from searching to loading
				if(levelID == num) // If we found the right level
				{
					// Fill the values of the levelMap array with the level data
					y = 0;
					ResetNumBricks(); // Reset the number of bricks in the level to zero;
					while(y < BGAMEHEIGHT)
					{
						x = 0;
						while(x < BGAMEWIDTH)
						{
							ch = fgetc(levelFile);
							while (ch != '(' && ch != EOF) // Skip through the file till the first open bracket
							{
								ch = fgetc(levelFile);
							}
							
							fileNum.str(""); // Reset the data stream
							ch = fgetc(levelFile); // Retrieve the first digit
							
							// Retrieve the first block value
							while(ch != ',' && ch != EOF) // Block values are seperated by commas
							{
								fileNum << ch; // Concatinate each digit of the value into one variable
								ch = fgetc(levelFile); // Reading the value one character at a time
							}
							strNum = fileNum.str(); // Convert the value into a string
							// Store the first value as an integar in the current position of the level map
							levelMap[x][y][0] = atoi(strNum.c_str());
							
							fileNum.str(""); // Reset the data stream						
							ch = fgetc(levelFile); // Retrieve the first digit

							// Retrieve the second block value
							while(ch != ')' && ch != EOF) // Block definition ends with a closed bracket
							{
								fileNum << ch; // Concatinate each digit of the value into one variable
								ch = fgetc(levelFile); // Reading the value one character at a time
							}
							strNum = fileNum.str(); // Convert the value into a string
							// Store the first value as an integar in the current position of the level map
							levelMap[x][y][1] = atoi(strNum.c_str());
							
							// If there is a brick and it's not grey. Grey bricks don't count towards finishing a level
							if(levelMap[x][y][0] != 0 && levelMap[x][y][1] > 1)
							{
								ChangeNumBricks(1); // Count another brick to be knocked out to complete the level
							}

							// Deciding whether a block gets a powerup coin
							levelMap[x][y][2] = 0; // Default for all brick locations is zero
							
							// Check there is a brick and it's not grey. Grey bricks don't get powerup coins
							if(levelMap[x][y][0] != 0 && levelMap[x][y][1] > 1)
							{
								if( !(rand() % POWERUPCHANCE) ) // 1 in POWERUPCHANCE chance for a block to have a powerup
								{
									/* Various powerup chances
									Extra Life	1
									Guns		4
									Fireball	10
									Explosive	10
									Magnetic	25
									Extra Ball	25
									PSize+		25
									PSpeed+		25
									BSize+		25
									PSize-		25
									PSpeed-		25
									BSize-		50

									Total 250
									*/
									powerup = rand() % 250; // Random number that will determine the powerup

									if(powerup < 50) // 1 in 5 chance for a decrease ball size token
									{
										levelMap[x][y][2] = DECBALLSIZE;
									}
									else if (powerup < 75) // 1 in 10 chance for a decrease paddle size token
									{
										levelMap[x][y][2] = DECPADDLESPEED;
									}
									else if (powerup < 100) // 1 in 10 chance for a decrease paddle speed token
									{
										levelMap[x][y][2] = DECPADDLESIZE;
									}
									else if (powerup < 125) // 1 in 10 chance for a increase ball size token
									{
										levelMap[x][y][2] = INCBALLSIZE;
									}
									else if (powerup < 150) // 1 in 10 chance for a increase paddle size token
									{
										levelMap[x][y][2] = INCPADDLESPEED;
									}
									else if (powerup < 175) // 1 in 10 chance for a increase paddle speed token
									{
										levelMap[x][y][2] = INCPADDLESIZE;
									}
									else if (powerup < 200) // 1 in 10 chance for an extra ball token
									{
										levelMap[x][y][2] = EXTRABALL;
									}
									else if (powerup < 225) // 1 in 10 chance for a magnetic coin
									{
										levelMap[x][y][2] = MAGNETIC;
									}
									else if (powerup < 235) // 1 in 25 chance for an explosive ball token
									{
										levelMap[x][y][2] = EXPLOSIVE;
									}
									else if (powerup < 245) // 1 in 25 chance for a fireball ball token
									{
										levelMap[x][y][2] = FIREBALL;
									}
									else if (powerup < 249) // 2 in 125 chance for a gun token
									{
										levelMap[x][y][2] = GUNS;
									}
									else if (powerup < 250) // 1 in 250 chance for an extra life token
									{
										levelMap[x][y][2] = EXTRALIFE;
									}
									else
									{
										levelMap[x][y][2] = 0;
									}
								}
							}
							x++;
						}
						y++;
					}
					levelFound = true; // Level was found and loaded
					fclose(levelFile); // Stop reading the level file

					ClearCoins(); // Clear any coins that are falling
					ClearMessages(); // Clear the message queue
					return levelFound; // Level was loaded
				}
			}
		} while (ch != EOF);
		fclose(levelFile); // Stop reading rhe level file
	}
	return levelFound; // Level wasn't loaded
}

void SetPaddleDirection(int num) // Set the direction the paddle is moving
{
	if(num > 0) // If the movement change is positive...
	{
		paddleDirection = 1; // Set the paddle to move right
	}
	else if(num < 0) // If the movement change is negative...
	{
		paddleDirection = -1; // Set the paddle to move left
	} else
	{
		paddleDirection = 0; // Otherwise stop the paddle
	}
	return;
}

int GetPaddleDirection() // Gives the direction the paddle is moving
{
	return paddleDirection;
}

void ChangeLevel(int num) // Change the level
{
	int tempLevel; // Used to hold the new level number till it's confirmed to be found
	// int n; // Counter
	int x, y; // More Counters

	// Reset the level map
	for(x = 0; x < BGAMEWIDTH; x++)
	{
		for(y = 0; y < BGAMEHEIGHT; y++)
		{
			levelMap[x][y][0] = 0; 
			levelMap[x][y][1] = 0;
			levelMap[x][y][2] = 0;
		}
	}
	
	// The level we want to change to
	tempLevel = level + num;

	while(tempLevel < 1) // If the new level is less than level 1...
	{		
		if(GetScoreMultiplier() == 1) // If the score multiplier is at it's lowest...
		{
			tempLevel = 1; // Set the level to 1
		}
		else // If the multiplier has been increased...
		{
			tempLevel += GetMaxLevel(); // Cycle back to the last level
			ChangeScoreMultiplier(-1); // And decrease the score multiplier
		}
	}
	
	while(tempLevel > GetMaxLevel()) // If the new level is greater than the number of levels
	{
		tempLevel -= GetMaxLevel(); // Cycle back to the first level
		ChangeScoreMultiplier(1); // And increase the score multiplier
	}

	if(LoadLevel(tempLevel)) // Load the new level and if it loaded...
	{
		level = tempLevel; // Make the level change permanent
	}
	else // If it failed to load...
	{
		LoadLevel(1); // Load level 1
	}
	
	paddleSize = 8; // Reset the paddle size
	paddleSpeed = 4; // Reset the paddle speed

	magnetic = 0; // Remove magnetic

	// Reset the balls using the UseLife() function
	if(GetLife() == 5) // If we have the maximum number of lives already...
	{
		UseLife(); // We use a life first
		AddLife(); // Then add the life back
	}
	else // If we don't have the maximum number of lives...
	{
		AddLife(); // We add the life first to ensure we don't accidentally end the game
		UseLife(); // Then we use the life
	}

	LoadBackground(level); // Load the new background
	
	// Reset the the timing variable to the current time
	QueryPerformanceCounter((LARGE_INTEGER *)&timer1);

	ResetExplosions(); // Remove all explosions
	ResetBullets(); // Reset all bullets

	DrawGame(); // Redraw the game
}

int GetLevel() // Gives the current level
{
	return level;
}

void SetMaxLevel() // Retrieve the number of levels
{
	char ch; // Place holder for each character as it's read form the file
	FILE *levelFile; // File to read the level data from

	// Variables for handling the characters retrieved from the file
	std::ostringstream fileNum;
	std::string strNum;

	levelFile = fopen("Levels.txt", "r");  // Open Levels.txt for reading

	if(levelFile != NULL) // Check that the file opened
	{
		do {
			ch = fgetc(levelFile); // Grab the first character in the file
			if(ch == 'M') // Found the max level marker
			{
				fileNum.str(""); // Reset the data stream
				ch = fgetc(levelFile); // Retrieve the first digit

				while(ch != '\n' && ch != EOF) // Retrieve all the characters left on the line
				{
					fileNum << ch; // Concatinate each character into a variable
					ch = fgetc(levelFile); // Retrieve the next character
				}

				strNum = fileNum.str(); // Convert the max level value into string
				maxLevel = atoi(strNum.c_str()); // Store the max level as an integar

				if (maxLevel < 1) // If the max level is less than one...
				{
					maxLevel = 1; // Make the max level equal to one
				}
			}
		} while (ch != EOF);

		fclose (levelFile); // Close the level file
	}
}

void SetBrickStyles() // Retrieves the brick stles from the levels file
{
	char ch; // Place holder for each character as it's read form the file
	FILE *levelFile; // File to read the level data from

	// Variables for handling the characters retrieved from the file
	std::ostringstream fileNum;
	std::string strNum;

	levelFile = fopen("Levels.txt", "r");  // Open Levels.txt for reading

	if(levelFile != NULL) // Check that the file opened
	{
		do {
			ch = fgetc(levelFile); // Grab the first character in the file
			if(ch == 'B') // Found the max level marker
			{
				fileNum.str(""); // Reset the data stream
				ch = fgetc(levelFile); // Retrieve the first digit

				while(ch != '\n' && ch != EOF) // Retrieve all the characters left on the line
				{
					fileNum << ch; // Concatinate each character into a variable
					ch = fgetc(levelFile); // Retrieve the next character
				}

				strNum = fileNum.str(); // Convert the max level value into string
				brickStyles = atoi(strNum.c_str()); // Store the max level as an integar

				if (brickStyles < 1) // If the max level is less than one...
				{
					brickStyles = 1; // Make the max level equal to one
				}
			}
		} while (ch != EOF);

		fclose (levelFile); // Close the level file
	}
}

int GetMaxLevel() // Gives the maximum level number
{
	return maxLevel;
}

bool CollisionCheck(int num, int moveX, int moveY) // Checks whether a ball will hit an object if it moves
{
	// Check order: Borders, Paddle, Bricks

	int x, y; // Counters
	
	// If the ball is moving left then check left side for collisions and react
	if(moveX == -1)
	{
		for(y = 0; y < 16; y++)
		{
			for(x = 0; x < 16; x++)
			{
				if(balls[num].map[x][y] == 1) // Check each pixel of the ball
				{
					if(balls[num].x + x + moveX < TILESIZE)	// If the left side of the ball hits a border...
					{
						balls[num].speedX -= balls[num].speedX * 2; // Reverse the ball's horizontal speed
						AddSound(SOUND_BORDERREBOUND); // Play the sound for hitting a border
						return true; // Collision occured and was resolved
					}
					// If the left side of the ball hits the paddle...
					if(balls[num].noRebound <= 0) // If the ball is allowed to rebound off the paddle...
					{
						// Paddle varies in length as it is rounded
						if( (balls[num].y + y > (TILESIZE*GAMEHEIGHT-1) - (TILESIZE*2 - 2)) &&
							(balls[num].y + y < (TILESIZE*GAMEHEIGHT-1) - 1) )
						{
							if( (balls[num].y + y > (TILESIZE*GAMEHEIGHT-1) - (TILESIZE*2 - 3)) &&
								(balls[num].y + y < (TILESIZE*GAMEHEIGHT-1) - 2) )
							{
								if( (balls[num].y + y > (TILESIZE*GAMEHEIGHT-1) - (TILESIZE*2 - 4)) &&
									(balls[num].y + y < (TILESIZE*GAMEHEIGHT-1) - 3) )
									// If the ball is in line with the longest part of the paddle...
								{
									if(balls[num].x + x + moveX >= GetPaddlePosition() &&
										balls[num].x + x + moveX <= (GetPaddlePosition() + TILESIZE*(GetPaddleSize()+2) - 1) )
										// If the left side of the ball has hit this part of the paddle...
									{
										balls[num].bricks = 1; // Reset the brick mulitplier for this ball
										AddSound(SOUND_PADDLEREBOUND); // Play the sound for hitting the paddle
										if(magnetic) // If the paddle is currently magnetic...
										{
											balls[num].stuck = true; // The ball sticks to the paddle
											balls[num].speedX = 0; // The ball stops moving horizontally
											balls[num].speedY = 0; // The ball stops moving vertically
											return true; // Collision occured and was resolved
										}
										else // If the paddle is not currently magnetic...
										{
											ReleaseBall(num); // Rebound the ball in a direction depending on where it hit the paddle
											return true; // Collision occured and was resolved
										}
									}
								}

								// If the ball is in line with the second longest sections of the paddle...

								if(balls[num].x + x + moveX >= (GetPaddlePosition() + 1) &&
									balls[num].x + x + moveX <= (GetPaddlePosition() + TILESIZE*(GetPaddleSize()+2) - 2) )
									// If the left side of the ball hit this slightly narrower part of the paddle...
								{
									balls[num].bricks = 1; // Reset the brick mulitplier for this ball
									AddSound(SOUND_PADDLEREBOUND); // Play the sound for hitting the paddle
									if(magnetic) // If the paddle is currently magnetic...
									{
										balls[num].stuck = true; // The ball sticks to the paddle
										balls[num].speedX = 0; // The ball stops moving horizontally
										balls[num].speedY = 0; // The ball stops moving vertically
										return true; // Collision occured and was resolved
									}
									else // If the paddle is not currently magnetic...
									{
										ReleaseBall(num); // Rebound the ball in a direction depending on where it hit the paddle
										return true; // Collision occured and was resolved
									}
								}
							}

							// If the ball is in line with the narrowest sections of the paddle...

							if(balls[num].x + x + moveX >= (GetPaddlePosition() + 3) &&
								balls[num].x + x + moveX <= (GetPaddlePosition() + TILESIZE*(GetPaddleSize()+2) - 4) )
								// If the left side of the ball hit the narrowest part of the paddle...
							{
								balls[num].bricks = 1; // Reset the brick mulitplier for this ball
								AddSound(SOUND_PADDLEREBOUND); // Play the sound for hitting the paddle
								if(magnetic) // If the paddle is currently magnetic...
								{
									balls[num].stuck = true; // The ball sticks to the paddle
									balls[num].speedX = 0; // The ball stops moving horizontally
									balls[num].speedY = 0; // The ball stops moving vertically
									return true; // Collision occured and was resolved
								}
								else // If the paddle is not currently magnetic...
								{
									ReleaseBall(num); // Rebound the ball in a direction depending on where it hit the paddle
									return true; // Collision occured and was resolved
								}
							}
						}
					}
					
					// If the left side of the ball hits a brick..
					if( levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][0] ||
						levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][1] )
					{
						// If it is a grey brick...
						if(levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][1] == 1)
						{
							// If the the ball isn't big enough to knockout a grey brick..
							if(balls[num].size < KNOCKOUTBALLSIZE && balls[num].greyBricks < 20)
							{ // Grey brick isn't knocked out
								balls[num].speedX -= balls[num].speedX * 2; // Reverse the balls horizontal speed
								
								AddSound(SOUND_BRICKREBOUND); // Play the sound for rebounding off a brick
								balls[num].greyBricks++; // Increase the grey ball counter
								return true; // Collision detected and was resolved
							}
							else // If the ball is big enough to knockout a grey brick...
							{ // Grey brick is knocked out
								// No score for grey bricks

								AddSound(SOUND_BRICKKO); // Add the KO sound to the queue
								// Remove the brick from the level map
								levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][0] = 0;
								levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][1] = 0;
								
								// If there was a powerup coin attached to the brick...
								// (Shouldn't be for grey bricks, but code is added anyway incase it's changed)
								if(levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][2])
								{
									// Create the powerup coin to the game in place of the brick
									AddCoin(levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][2],
										((balls[num].x + x + moveX)/BRICKSIZE)*BRICKSIZE,
										((balls[num].y + y)/BRICKSIZE)*BRICKSIZE);

									// Remove the powerup coin data from the level map
									levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][2] = 0;
								}

								if(!balls[num].fire) // If it not a fireball... 
								{
									balls[num].speedX -= balls[num].speedX * 2; // Reverse the ball's horizontal direction
								}
								if(balls[num].explosive) // If the ball is explosive...
								{
									// Knock out extra bricks based on the size of the ball
									ExplodeBrick(num, (balls[num].x + x + moveX)/BRICKSIZE, (balls[num].y + y)/BRICKSIZE);
								}
								balls[num].greyBricks = 0; // Reset the grey brick counter
								return true; // Collision detected and was resolved
							}
						}
						else // If it is not a grey brick...
						{
							// Add points based on the type of brick that was knocked out and active multipliers
							AddScore(levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][0],
									 levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][1],
									 balls[num].bricks);

							AddSound(SOUND_BRICKKO); // Play the sound for knocking out a brick
							ChangeNumBricks(-1); // Reduce the number of bricks required to clear the level

							// Remove the brick from the level map
							levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][0] = 0;
							levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][1] = 0;

							// If there was a powerup coin attached to the brick...
							if(levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][2])
							{
								// Create the powerup coin to the game in place of the brick
								AddCoin(levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][2],
									((balls[num].x + x + moveX)/BRICKSIZE)*BRICKSIZE,
									((balls[num].y + y)/BRICKSIZE)*BRICKSIZE);
							
								// Remove the powerup coin data from the level map
								levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][2] = 0;
							}

							if(!balls[num].fire) // If it not a fireball... 
							{
								balls[num].speedX -= balls[num].speedX * 2; // Reverse the ball's horizontal direction
							}
							if(balls[num].explosive) // If the ball is explosive...
							{
								// Knock out extra bricks based on the size of the ball
								ExplodeBrick(num, (balls[num].x + x + moveX)/BRICKSIZE, (balls[num].y + y)/BRICKSIZE);
							}
							balls[num].bricks++; // Increase the brick score multiplier for the ball
							balls[num].greyBricks = 0; // Reset the grey brick counter
						}
						return true; // Collision detected and was resolved
					}
				}
			}
		}
	}

	// If the ball is moving right then check right side for collisions and react
	if(moveX == 1)
	{
		for(y = 0; y < 16; y++)
		{
			for(x = 15; x >= 0; x--)
			{
				if(balls[num].map[x][y] == 1) // Check each pixel of the ball
				{
					// If the right side of the ball hits a border...
					if(balls[num].x + x + moveX > (TILESIZE*GAMEWIDTH-1) - TILESIZE) 
					{
						balls[num].speedX -= balls[num].speedX * 2; // Reverse the ball's horizontal speed
						AddSound(SOUND_BORDERREBOUND); // Play the sound for hitting a border
						return true; // Collision detected and was resolved
					}

					// If the right side of the ball hits the paddle...
					if(balls[num].noRebound <= 0) // If the ball is allowed to rebound off the paddle...
					{
						// Paddle varies in length as it is rounded
						if( (balls[num].y + y > (TILESIZE*GAMEHEIGHT-1) - (TILESIZE*2 - 2)) &&
							(balls[num].y + y < (TILESIZE*GAMEHEIGHT-1) - 1) )
						{
							if( (balls[num].y + y > (TILESIZE*GAMEHEIGHT-1) - (TILESIZE*2 - 3)) &&
								(balls[num].y + y < (TILESIZE*GAMEHEIGHT-1) - 2) )
							{
								if( (balls[num].y + y > (TILESIZE*GAMEHEIGHT-1) - (TILESIZE*2 - 4)) &&
									(balls[num].y + y < (TILESIZE*GAMEHEIGHT-1) - 3) )
									// If the ball is in line with the longest part of the paddle...
								{  
									if(balls[num].x + x + moveX >= GetPaddlePosition() &&
										balls[num].x + x + moveX <= (GetPaddlePosition() + TILESIZE*(GetPaddleSize()+2) - 1) )
										// If the right side of the ball has hit this part of the paddle...
									{
										balls[num].bricks = 1; // Reset the brick mulitplier for this ball
										AddSound(SOUND_PADDLEREBOUND); // Play the sound for hitting the paddle
										if(magnetic) // If the paddle is currently magnetic...
										{
											balls[num].stuck = true; // The ball sticks to the paddle
											balls[num].speedX = 0; // The ball stops moving horizontally
											balls[num].speedY = 0; // The ball stops moving vertically
											return true; // Collision detected and was resolved
										}
										else // If the paddle is not currently magnetic...
										{
											ReleaseBall(num); // Rebound the ball in a direction depending on where it hit the paddle
											return true; // Collision detected and was resolved
										}
									}
								}

								// If the ball is in line with the second longest sections of the paddle...

								if(balls[num].x + x + moveX >= (GetPaddlePosition() + 1) &&
									balls[num].x + x + moveX <= (GetPaddlePosition() + TILESIZE*(GetPaddleSize()+2) - 2) )
									// If the right side of the ball hit this slightly narrower part of the paddle...
								{
									balls[num].bricks = 1; // Reset the brick mulitplier for this ball
									AddSound(SOUND_PADDLEREBOUND); // Play the sound for hitting the paddle
									if(magnetic) // If the paddle is currently magnetic...
									{
										balls[num].stuck = true; // The ball sticks to the paddle
										balls[num].speedX = 0; // The ball stops moving horizontally
										balls[num].speedY = 0; // The ball stops moving vertically
										return true; // Collision detected and was resolved
									}
									else // If the paddle is not currently magnetic...
									{
										ReleaseBall(num); // Rebound the ball in a direction depending on where it hit the paddle
										return true; // Collision detected and was resolved
									}
								}
							}

							// If the ball is in line with the narrowest sections of the paddle...

							if(balls[num].x + x + moveX >= (GetPaddlePosition() + 3) &&
								balls[num].x + x + moveX <= (GetPaddlePosition() + TILESIZE*(GetPaddleSize()+2) - 4) )
								// If the right side of the ball hit the narrowest part of the paddle...
							{
								balls[num].bricks = 1; // Reset the brick mulitplier for this ball
								AddSound(SOUND_PADDLEREBOUND); // Play the sound for hitting the paddle
								if(magnetic) // If the paddle is currently magnetic...
								{
									balls[num].stuck = true; // The ball sticks to the paddle
									balls[num].speedX = 0; // The ball stops moving horizontally
									balls[num].speedY = 0; // The ball stops moving vertically
									return true; // Collision detected and was resolved
								}
								else // If the paddle is not currently magnetic...
								{
									ReleaseBall(num); // Rebound the ball in a direction depending on where it hit the paddle
									return true; // Collision detected and was resolved
								}
							}
						}
					}
					
					// If the right side of the ball hits a brick...
					if( levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][0] ||
						levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][1] )
					{
						// If it is a grey brick...
						if(levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][1] == 1)
						{
							// If the the ball isn't big enough to knockout a grey brick...
							if(balls[num].size < KNOCKOUTBALLSIZE && balls[num].greyBricks < 20)
							{ // Grey brick isn't knocked out
								balls[num].speedX -= balls[num].speedX * 2; // Reverse the balls horizontal speed
								
								AddSound(SOUND_BRICKREBOUND); // Play the sound for rebounding off a brick
								balls[num].greyBricks++; // Increment the grey brick counter
								return true; // Collision detected and was resolved
							}
							else // If the ball is big enough to knockout a grey brick...
							{ // Grey brick is knocked out
								// No score for grey bricks

								AddSound(SOUND_BRICKKO); // Play the sound for knocking out a brick
								// Remove the brick from the level map
								levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][0] = 0;
								levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][1] = 0;
								
								// If there was a powerup coin attached to the brick...
								// (Shouldn't be for grey bricks, but code is added anyway incase it's changed)
								if(levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][2])
								{
									// Create the powerup coin to the game in place of the brick
									AddCoin(levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][2],
										((balls[num].x + x + moveX)/BRICKSIZE)*BRICKSIZE,
										((balls[num].y + y)/BRICKSIZE)*BRICKSIZE);

									// Remove the powerup coin data from the level map
									levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][2] = 0;
								}

								if(!balls[num].fire) // If it not a fireball... 
								{
									balls[num].speedX -= balls[num].speedX * 2; // Reverse the balls horizontal direction
								}
								if(balls[num].explosive) // If the ball is explosive...
								{
									// Knock out extra bricks based on the size of the ball
									ExplodeBrick(num, (balls[num].x + x + moveX)/BRICKSIZE, (balls[num].y + y)/BRICKSIZE);
								}
								balls[num].greyBricks = 0; // Reset the grey brick counter
								return true; // Collision detected and was resolved
							}
						}
						else // If it is not a grey brick...
						{
							// Add points based on the type of brick that was knocked out and active multipliers
							AddScore(levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][0],
									 levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][1],
									 balls[num].bricks);

							AddSound(SOUND_BRICKKO); // Play the sound for knocking out a brick
							ChangeNumBricks(-1); // Reduce the number of bricks required to clear the level

							// Remove the brick from the level map
							levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][0] = 0;
							levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][1] = 0;

							// If there was a powerup coin attached to the brick...
							if(levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][2])
							{
								// Create the powerup coin to the game in place of the brick
								AddCoin(levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][2],
									((balls[num].x + x + moveX)/BRICKSIZE)*BRICKSIZE,
									((balls[num].y + y)/BRICKSIZE)*BRICKSIZE);
							
								// Remove the powerup coin data from the level map
								levelMap[(balls[num].x + x + moveX)/BRICKSIZE][(balls[num].y + y)/BRICKSIZE][2] = 0;
							}

							if(!balls[num].fire) // If it not a fireball... 
							{
								balls[num].speedX -= balls[num].speedX * 2; // Reverse the balls horizontal direction
							}
							if(balls[num].explosive) // If the ball is explosive...
							{
								// Knock out extra bricks based on the size of the ball
								ExplodeBrick(num, (balls[num].x + x + moveX)/BRICKSIZE, (balls[num].y + y)/BRICKSIZE);
							}
							balls[num].bricks++; // Increase the brick score multiplier for the ball
							balls[num].greyBricks = 0; // Reset the grey brick counter
						}
						return true; // Collision detected and was resolved
					}
				}
			}
		}
	}

	// If the ball is moving up then check top side for collisions and react
	if(moveY == -1)
	{
		for(y = 0; y < 16; y++)
		{
			for(x = 0; x < 16; x++)
			{
				if(balls[num].map[x][y] == 1) // Check each pixel of the ball
				{
					if(balls[num].y + y + moveY < TILESIZE) // If the top side of the ball hits a border...
					{
						balls[num].speedY -= balls[num].speedY * 2; // Reverse the balls vertical speed
						AddSound(SOUND_BORDERREBOUND); // Play the sound for hitting the border
						return true; // Collision detected and was resolved
					}

					// If the top side of the ball hits a brick...
					if( levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][0] ||
						levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][1] )
					{
						// If it is a grey brick...
						if(levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][1] == 1)
						{
							// If the the ball isn't big enough to knockout a grey brick...
							if(balls[num].size < KNOCKOUTBALLSIZE && balls[num].greyBricks < 20)
							{  // Grey brick isn't knocked out
								balls[num].speedY -= balls[num].speedY * 2; // Reverse the balls vertical speed
								AddSound(SOUND_BRICKREBOUND); // Play the sound for rebounding off a brick
								balls[num].greyBricks++; // Increment the grey brick counter
								return true; // Collision detected and was resolved
							}
							else // If the ball is big enough to knockout a grey brick...
							{ // Grey brick is knocked out
								// No score for grey bricks

								AddSound(SOUND_BRICKKO); // Play the sound for knocking out a brick
								// Remove the brick from the level map
								levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][0] = 0;
								levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][1] = 0;

								// If there was a powerup coin attached to the brick...
								// (Shouldn't be for grey bricks, but code is added anyway incase it's changed)
								if(levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][2])
								{
									// Create the powerup coin to the game in place of the brick
									AddCoin(levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][2],
										((balls[num].x + x)/BRICKSIZE)*BRICKSIZE,
										((balls[num].y + y + moveY)/BRICKSIZE)*BRICKSIZE);

									// Remove the powerup coin data from the level map
									levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][2] = 0;
								}

								if(!balls[num].fire) // If it not a fireball... 
								{
									balls[num].speedY -= balls[num].speedY * 2; // Reverse the balls vertical direction
								}
								if(balls[num].explosive) // If the ball is explosive...
								{
									// Knock out extra bricks based on the size of the ball
									ExplodeBrick(num, (balls[num].x + x)/BRICKSIZE, (balls[num].y + y + moveY)/BRICKSIZE);
								}
								balls[num].greyBricks = 0; // Reset the grey brick counter
								return true; // Collision detected and was resolved
							}
						}
						else // If it is not a grey brick...
						{
							// Add points based on the type of brick that was knocked out and active multipliers
							AddScore(levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][0],
									 levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][1],
									 balls[num].bricks);
							
							AddSound(SOUND_BRICKKO); // Add the KO sound to the queue
							ChangeNumBricks(-1); // Reduce the number of bricks required to clear the level

							// Remove the brick from the level map
							levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][0] = 0;
							levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][1] = 0;

							// If there was a powerup coin attached to the brick...
							if(levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][2])
							{
								// Create the powerup coin to the game in place of the brick
								AddCoin(levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][2],
									((balls[num].x + x)/BRICKSIZE)*BRICKSIZE,
									((balls[num].y + y + moveY)/BRICKSIZE)*BRICKSIZE);

								// Remove the powerup coin data from the level map
								levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][2] = 0;
							}

							if(!balls[num].fire) // If it not a fireball... 
							{
								balls[num].speedY -= balls[num].speedY * 2; // Reverse the balls horizontal direction
							}
							if(balls[num].explosive) // If the ball is explosive...
							{
								// Knock out extra bricks based on the size of the ball
								ExplodeBrick(num, (balls[num].x + x)/BRICKSIZE, (balls[num].y + y + moveY)/BRICKSIZE);
							}
							balls[num].bricks++; // Increase the brick score multiplier for the ball
							balls[num].greyBricks = 0; // Reset the grey brick counter
						}
						return true; // Collision detected and was resolved
					}
				}
			}
		}
	}

	// If the ball is moving down then check bottom side for collisions and react
	if(moveY == 1)
	{		
		for(x = 0; x < 16; x++)
		{
			for(y = 15; y >= 0; y--)
			{
				if(balls[num].map[x][y] == 1) // Check each pixel of the ball
				{
					// If the bottom side of the ball hits the paddle...
					if(balls[num].noRebound <= 0) // If the ball is allowed to rebound off the paddle...
					{
						// Paddle varies in length as it is rounded
						if( (balls[num].y + y + moveY > (TILESIZE*GAMEHEIGHT-1) - (TILESIZE*2 - 2)) &&
							(balls[num].y + y + moveY <= (TILESIZE*GAMEHEIGHT-1) - 3) )
						{
							if(balls[num].y + y + moveY > ((TILESIZE*GAMEHEIGHT-1) - (TILESIZE*2 - 2) + 1)&&
								(balls[num].y + y + moveY <= (TILESIZE*GAMEHEIGHT-1) - 4) )
							{
								if(balls[num].y + y + moveY > ((TILESIZE*GAMEHEIGHT-1) - (TILESIZE*2 - 2) + 2)&&
									(balls[num].y + y + moveY <= (TILESIZE*GAMEHEIGHT-1) - 5) )
									// If the ball is in line with the longest part of the paddle...
								{
									if(balls[num].x + x >= GetPaddlePosition() &&
										balls[num].x + x <= (GetPaddlePosition() + TILESIZE*(GetPaddleSize()+2) - 1) )
										// If the bottom side of the ball has hit this part of the paddle...
									{
										balls[num].bricks = 1; // Reset the brick mulitplier for this ball
										AddSound(SOUND_PADDLEREBOUND); // Play the sound for hitting the paddle
										if(magnetic) // If the paddle is currently magnetic...
										{
											balls[num].stuck = true; // The ball sticks to the paddle
											balls[num].speedX = 0; // The ball stops moving horizontally
											balls[num].speedY = 0; // The ball stops moving vertically
											return true; // Collision detected and was resolved
										}
										else // If the paddle is not currently magnetic...
										{
											ReleaseBall(num); // Rebound the ball in a direction depending on where it hit the paddle
											return true; // Collision detected and was resolved
										}
									}
								}

								// If the ball is in line with the second longest sections of the paddle...

								if(balls[num].x + x >= GetPaddlePosition() + 1 &&
									balls[num].x + x <= (GetPaddlePosition() + TILESIZE*(GetPaddleSize()+2) - 2) )
									// If the right side of the ball hit this slightly narrower part of the paddle...
								{
									balls[num].bricks = 1; // Reset the brick mulitplier for this ball
									AddSound(SOUND_PADDLEREBOUND); // Play the sound for hitting the paddle
									if(magnetic) // If the paddle is currently magnetic...
									{
										balls[num].stuck = true; // The ball sticks to the paddle
										balls[num].speedX = 0; // The ball stops moving horizontally
										balls[num].speedY = 0; // The ball stops moving vertically
										return true; // Collision detected and was resolved
									}
									else // If the paddle is not currently magnetic...
									{
										ReleaseBall(num); // Rebound the ball in a direction depending on where it hit the paddle
										return true; // Collision detected and was resolved
									}
								}
							}

							// If the ball is in line with the narrowest sections of the paddle...

							if(balls[num].x + x >= GetPaddlePosition() + 3 &&
								balls[num].x + x <= (GetPaddlePosition()+(GetPaddleSize()+2)*TILESIZE-1) - 3)
								// If the right side of the ball hit the narrowest part of the paddle...
							{
								balls[num].bricks = 1; // Reset the brick mulitplier for this ball
								AddSound(SOUND_PADDLEREBOUND); // Play the sound for hitting the paddle
								if(magnetic) // If the paddle is currently magnetic...
								{
									balls[num].stuck = true; // The ball sticks to the paddle
									balls[num].speedX = 0; // The ball stops moving horizontally
									balls[num].speedY = 0; // The ball stops moving vertically
									return true; // Collision detected and was resolved
								}
								else // If the paddle is not currently magnetic...
								{
									ReleaseBall(num); // Rebound the ball in a direction depending on where it hit the paddle
									return true; // Collision detected and was resolved
								}
							}
						}
					}

					// If the bottom side of the ball hits a brick...
					if( levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][0] ||
						levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][1] )
					{
						// If it is a grey brick...
						if(levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][1] == 1)
						{
							// If the the ball isn't big enough to knockout a grey brick...
							if(balls[num].size < KNOCKOUTBALLSIZE && balls[num].greyBricks < 20)
							{  // Grey brick isn't knocked out
								balls[num].speedY -= balls[num].speedY * 2; // Reverse the balls vertical speed
								
								AddSound(SOUND_BRICKREBOUND); // Play the sound for rebounding off a brick
								balls[num].greyBricks++; // Increment the grey brick counter
								return true; // Collision detected and was resolved
							}
							else // If the ball is big enough to knockout a grey brick...
							{ // Grey brick is knocked out
								// No score for grey bricks

								AddSound(SOUND_BRICKKO); // Play the sound for knocking out a brick
								// Remove the brick from the level map
								levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][0] = 0;
								levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][1] = 0;

								// If there was a powerup coin attached to the brick...
								// (Shouldn't be for grey bricks, but code is added anyway incase it's changed)
								if(levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][2])
								{
									// Create the powerup coin to the game in place of the brick
									AddCoin(levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][2],
										((balls[num].x + x)/BRICKSIZE)*BRICKSIZE,
										((balls[num].y + y + moveY)/BRICKSIZE)*BRICKSIZE);
									
									// Remove the powerup coin data from the level map
									levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][2] = 0;
								}
								if(!balls[num].fire) // If it not a fireball... 
								{
									balls[num].speedY -= balls[num].speedY * 2; // Reverse the balls vertical direction
								}
								if(balls[num].explosive) // If the ball is explosive...
								{
									// Knock out extra bricks based on the size of the ball
									ExplodeBrick(num, (balls[num].x + x)/BRICKSIZE, (balls[num].y + y + moveY)/BRICKSIZE);
								}
								balls[num].greyBricks = 0; // Reset the grey brick counter
								return true; // Collision detected and was resolved
							}
						}
						else // If it is not a grey brick...
						{
							// Add points based on the type of brick that was knocked out and active multipliers
							AddScore(levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][0],
									 levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][1],
									 balls[num].bricks);
						
							AddSound(SOUND_BRICKKO); // Play the sound for knocking out a brick
							ChangeNumBricks(-1); // Reduce the number of bricks required to clear the level

							// Remove the brick from the level map
							levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][0] = 0;
							levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][1] = 0;

							// Add a Coin if there was one attached to the brick
							if(levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][2])
							{
								// Create the powerup coin to the game in place of the brick
								AddCoin(levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][2],
									((balls[num].x + x)/BRICKSIZE)*BRICKSIZE,
									((balls[num].y + y + moveY)/BRICKSIZE)*BRICKSIZE);

								// Remove the powerup coin data from the level map
								levelMap[(balls[num].x + x)/BRICKSIZE][(balls[num].y + y + moveY)/BRICKSIZE][2] = 0;
							}

							if(!balls[num].fire) // If it not a fireball... 
							{
								balls[num].speedY -= balls[num].speedY * 2; // Reverse the balls vertical direction
							}
							if(balls[num].explosive) // If the ball is explosive...
							{
								// Knock out extra bricks based on the size of the ball
								ExplodeBrick(num, (balls[num].x + x)/BRICKSIZE, (balls[num].y + y + moveY)/BRICKSIZE);
							}
							balls[num].bricks++; // Increase the brick score multiplier for the ball
							balls[num].greyBricks = 0; // Reset the grey brick counter
						}
						return true; // Collision detected and was resolved
					}
				}
			}
		}
		// If the ball is lost...
		if(balls[num].y > (TILESIZE*GAMEHEIGHT-1))
		{
			LoseBall(num); // Remove the ball
			return true; // Collision detected and was resolved
		}
	}
	return false; // No collision detected
}

void AdjustBallSize(int num, int sizeChange) // Adjust the size of the given ball and change it's pixel map
{
	int x, y; // Counters

	if(sizeChange > 0) // If the change in positive...
	{
		balls[num].size++; // Increase the ball's size by one
	}
	if(sizeChange < 0) // If the change in negative...
	{
		balls[num].size--; // Decrease the ball's size by one
	}
	if(sizeChange != 0) // If the change is not neutral...
	{
		if(balls[num].size > 7) // If the new ball size is above seven...
		{
			balls[num].size = 7; // The ball size is seven
		}
		if(balls[num].size < 1) // If the new ball size is below one
		{
			balls[num].size = 1; // The new ball size is one
		}
	}

	switch(balls[num].size) // Change the ball's pixel map based on it's size
	// 0 = pixel absent
	// 1 = pixel present
	{
	case -1: // No ball, blank 16x16 grid
		{
			for(x = 0;x < 16; x++)
			{
				for(y = 0; y < 16; y++)
				{
					balls[num].map[x][y] = 0;
				}
			}
		}break;
	case 1: // 4x4 block centred on a 16x16 grid
		{
			for(x = 0;x < 16; x++)
			{
				for(y = 0; y < 16; y++)
				{
					if(x > 5 && x < 10 && y > 5 && y < 10)
					{
						balls[num].map[x][y] = 1;
					}
					else
					{
						balls[num].map[x][y] = 0;
					}
				}
			}

		}break;
	case 2: // 6x6 block centred on a  16x16 grid
		{
			for(x = 0; x < 16; x++)
			{
				for(y = 0; y < 16; y++)
				{
					if(x > 4 && x < 11 && y > 4 && y < 11)
					{
						 // With a 1x1 block missing from the corners
						if((x == 5 || x == 10) && (y == 5 || y == 10))
						{
							balls[num].map[x][y] = 0;
						}
						else
						{
							balls[num].map[x][y] = 1;
						}
					}
					else
					{
						balls[num].map[x][y] = 0;
					}
				}
			}
		}break;
	case 3: // 8x8 block on a 16x16 grid
		{
			for(x = 0;x < 16; x++)
			{
				for(y = 0; y < 16; y++)
				{
					if(x > 3 && x < 12 && y > 3 && y < 12)
					{
						// With a 2x2 block missing from the corners
						if((x == 4 || x == 5 || x == 10 || x == 11) &&
							(y == 4 || y == 5 || y == 10 || y == 11) &&
							// With the exception of the inner 1x1 of the 2x2 block
							!((x == 5 || x == 10) && (y == 5 || y == 10)))
						{
							balls[num].map[x][y] = 0;
						}
						else
						{
							balls[num].map[x][y] = 1;
						}
					}
					else
					{
						balls[num].map[x][y] = 0;
					}
				}
			}
		}break;
	case 4:// 10x10 block on a 16x16 grid
		{
			for(x = 0;x < 16; x++)
			{
				for(y = 0; y < 16; y++)
				{
					if(x > 2 && x < 13 && y > 2 && y < 13)
					{
						// With a 3x3 block missing from the corner
						if(( (x >= 3 && x <= 5) || (x >= 10 && x <= 12) ) &&
							( (y >= 3 && y <= 5) || (y >= 10 && y <= 12) ) &&
							// With exception of the inner 2x2 block of the 3x3 block
							!((x == 4 || x == 5 || x == 10 || x == 11) &&
							(y == 4 || y == 5 || y == 10 || y == 11) &&
							// With a double exception of the outer 1x1 block of the 2x2 block
							!((x == 4 || x == 11) && (y == 4 || y == 11))) )
						{
							balls[num].map[x][y] = 0;
						}
						else
						{
							balls[num].map[x][y] = 1;
						}
					}
					else
					{
						balls[num].map[x][y] = 0;
					}
				}
			}
		}break;
	case 5:// 12x12 block on a 16x16 grid
		{
			for(x = 0;x < 16; x++)
			{
				for(y = 0; y < 16; y++)
				{
					if(x > 1 && x < 14 && y > 1 && y < 14)
					{
						// With a 4x4 block missing from the corner
						if(( (x >= 2 && x <= 5) || (x >= 10 && x <= 13) ) &&
							( (y >= 2 && y <= 5) || (y >= 10 && y <= 13) ) &&
							// With exception of the inner 3x3 block of the 4x4 block
							!(( (x >= 3 && x == 5) || (x >= 10 && x <= 12) ) &&
							( (y >= 3 && y == 5) || (y >= 10 && y <= 12) ) &&
							// With a double exception of the outer 1x1 block of the 3x3 block
							!((x == 3 || x == 12) && (y == 3 || y == 12))) )
						{
							balls[num].map[x][y] = 0;
						}
						else
						{
							balls[num].map[x][y] = 1;
						}
					}
					else
					{
						balls[num].map[x][y] = 0;
					}
				}
			}
		}break;
	case 6:// 14x14 block on a 16x16 grid
		{
			for(x = 0;x < 16; x++)
			{
				for(y = 0; y < 16; y++)
				{
					if(x > 0 && x < 15 && y > 0 && y < 15)
					{
						// With a 5x5 block missing from the corner
						if(( (x >= 1 && x <= 5) || (x >= 10 && x <= 14) ) &&
							( (y >= 1 && y <= 5) || (y >= 10 && y <= 14) ) &&
							// With exception of the inner 4x4 block of the 5x5 block
							!(( (x >= 2 && x <= 5) || (x >= 10 && x <= 13) ) &&
							( (y >= 2 && y <= 5) || (y >= 10 && y <= 13) ) &&
							// With a double exception of the outer 2x2 block of the 4x4 block
							!((x == 2 || x == 3 || x == 12 || x == 13) &&
							(y == 2 || y == 3 || y == 12 || y == 13) &&
							// With a triple exception of the inner 1x1 block of the 2x2 block
							!((x == 3 || x == 12) && (y == 3 || y == 12))) ) )
						{
							balls[num].map[x][y] = 0;
						}
						else
						{
							balls[num].map[x][y] = 1;
						}
					}
					else
					{
						balls[num].map[x][y] = 0;
					}
				}
			}
		}break;
	case 7:// Fill a 16x16 grid
		{
			for(x = 0;x < 16; x++)
			{
				for(y = 0; y < 16; y++)
				{
					if(x >= 0 && x <= 15 && y >= 0 && y <= 15)
					{
						// With a 6x6 block missing from the corners
						if(( (x >= 0 && x <= 5) || (x >= 10 || x <= 15) ) &&
							( (y >= 0 && y <= 5) || (y >= 10 || y <= 15) ) &&
							// With an exception of the inner 5x5 block of the 6x6 blocks
							!(( (x >= 1 && x <= 5) || (x >= 10 && x <= 14) ) &&
							( (y >= 1 && y <= 5) || (y >= 10 && y <= 14) ) &&
							// With a double exception of the outer 3x3 block of the 5x5 blocks
							!(( (x >= 1 && x <= 3) || (x >= 12 && x <= 14) ) &&
							( (y >= 1 && y <= 3) || (y >= 12 && y <= 14) ) &&
							// With a triple exception of the inner 2x2 block of the 3x3 blocks
							!((x == 2 || x == 3 || x == 12 || x == 13) &&
							(y == 2 || y == 3 || y == 12 || y == 13) &&
							// With a quadruple exception of the outer 1x1 block of the 2x2 blocks
							!((x == 2 || x == 13) && (y == 2 || y == 13)) ))))
						{
							balls[num].map[x][y] = 0;
						}
						else
						{
							balls[num].map[x][y] = 1;
						}
					}
					else
					{
						balls[num].map[x][y] = 0;
					}
				}
			}
		}break;
	}
	return;
}

void LoseBall(int num) // Removes the given ball from the game
{
	// int x, y; // Counters
	int n; // Another Counter

	n = num; // Set the temporary counter to the ball being removed

	while(n < 4) // Cycle through the balls from the given ball up to the second to last
	{
		// If the next ball in the sequence exists...
		if(balls[n+1].size != -1)
		{
			// Copy the next ball over the current ball
			// Effectively overwriting the ball that was given
			balls[n].size = balls[n+1].size;
			AdjustBallSize(n,0); // Redraws the balls pixel map
			balls[n].x = balls[n+1].x;
			balls[n].y = balls[n+1].y;
			balls[n].speedX = balls[n+1].speedX;
			balls[n].speedY = balls[n+1].speedY;
			balls[n].speedMod = balls[n+1].speedMod;
			balls[n].stuck = balls[n+1].stuck;
			balls[n].bricks = balls[n+1].bricks;
			balls[n].greyBricks = balls[n+1].greyBricks;
			balls[n].explosive = balls[n+1].explosive;
			balls[n].fire = balls[n+1].fire;
			balls[n].noRebound = balls[n+1].noRebound;
			n++;
		}
		else // If the next ball in the sequence doesn't exist...
		{
			break; // Break the loop, leaving n equal to the ball that needs to be wiped
		}
	}
	
	// Wipe the last ball
	balls[n].size = -1;
	AdjustBallSize(n,0); // Resets the balls pixel map
	balls[n].x = 0;
	balls[n].y = 0;
	balls[n].speedX = 0;
	balls[n].speedY = 0;
	balls[n].speedMod = 0;
	balls[n].stuck = 0;
	balls[n].bricks = 0;
	balls[n].greyBricks = 0;
	balls[n].explosive = 0;
	balls[n].fire = 0;
	balls[n].noRebound = 0;

	if(balls[0].size == -1) // If there are no balls left...
	{
		UseLife(); // Lose a life
	}
}

void AddBall() // Adds a new ball if there is room
{
	int n; // Counter

	n = 0;
	while(n < 5) // Cycle through the ball array
	{
		if(balls[n].size == -1) // If there is an empty spot...
		{
			// Initilize and release the new ball
			balls[n].size = 4;
			AdjustBallSize(n,0); // Redraws the ball's pixel array
			balls[n].x = GetPaddlePosition() + (rand() % ((GetPaddleSize()+2)*8-TILESIZE)); // Random position on the paddle
			balls[n].y = 479 - 14 - (9+(balls[n].size));
			balls[n].speedX = 0;
			balls[n].speedY = 0;
			balls[n].speedMod = 0;
			balls[n].stuck = true;
			balls[n].fire = 0;
			balls[n].explosive = 0;
			balls[n].noRebound = 0;
			balls[n].bricks = 0;
			balls[n].greyBricks = 0;
			ReleaseBall(n); // Sends the ball in a direction based on its position on the paddle
			
			break; // Found an empty spot so break the loop
		}
		n++;
	}
}

void AddLife() // Adds a life if there aren't too many already
{
	if(livesRemaining < 5) // If the player has less then 5 extra lives...
	{
		livesRemaining++; // Add an extra life
	}
	return;
}

int GetLife() // Gives the number of extra lives a player has
{
	return livesRemaining;
}

void ResetNumBricks() // Resets the number of bricks left on a level to zero
{
	numBricks = 0; // Set the number of bricks left to zero
}

bool ChangeNumBricks(int num) // Changes the number of bricks left in the level
{
	numBricks += num; // Add the given change to the number of remaining bricks

	if(numBricks < 1) // If there are no bricks left to knock out...
	{
		ChangeLevel(1); // Go to the next level
		return true; // Report level changed
	}
	return false; // Report no change
}

int GetNumBricks() // Gives the number of bricks left to complete the level
{
	return numBricks;
}

int GetHelpColour() // Returns the numeric value for the brick colour used on the help screen
{
	return helpColour;
}

int GetHelpStyle() // Returns the numeric value for the style brick used on the help screen
{
	return helpStyle;
}

void ChangeHelpColour(int num) // Changes the value for the brick colour used on the help screen by value num
{
	helpColour += num; // Adjust the colour rotation by the number given
	while(helpColour > BRICKCOLOURS) // Cycle to the start of the rotation if the number goes too high
	{
		helpColour -= BRICKCOLOURS;
	}
	while(helpColour < 1) // Cycle to the end of the rotation if the number goes too low
	{
		helpColour += BRICKCOLOURS;
	}
}

void ChangeHelpStyle(int num) // Changes the value of the brick style used on the help screen
{
	helpStyle += num; // Adjust the style rotation by the number given
	while(helpStyle > brickStyles) // Cycle to the start of the rotation if the number goes too high
	{
		helpStyle -= brickStyles;
	}
	while(helpStyle < 1) // Cycle to the end of the rotation if the number goes too low
	{
		helpStyle += brickStyles;
	}
}

void PauseGame() // Increment the gamePaused counter
{
	gamePaused++; // Rotate throguh the help screens by one more
	while(gamePaused > HELPSCREENS) // Cycle back to the start of the help screens the number goes too high
	{
		gamePaused -= HELPSCREENS;
	}
}

void UnpauseGame() // Unpause the game
{
	gamePaused = 0; // Unpause the game
}

int GetPausedGame() // Gives the number of the help screen to be shown when paused
{
	return gamePaused;
}

int MyPower(int base, int power) // Gives base to the power (positive powers only)
{
	int temp = 1; // Working sum

	while(power > 0) // Multiply the working sum by the base, the number of times shown by the power
	{
		temp = temp*base;
		power--;
	}

	return temp;
}

int GetScore() // Gives the score
{
	return score;
}

void AddScore(int x, int y, int brickMultiplier) // Adds to the score based on the brick type, level and multipliers
{
	int scoreInc; // Score increase counter

	// Calculate the score increase
	scoreInc = x * (y + GetLevel()) * (brickMultiplier/5 + 1) * GetScoreMultiplier();

	score += scoreInc; // Add on the score increase
}

int GetScoreMultiplier() // Gives the current score mulitplier
{
	return scoreMultiplier;
}

void ChangeScoreMultiplier(int num) // Changes the score multiplier
{
	scoreMultiplier += num; // Add the given value to the score multiplier

	if(scoreMultiplier < 1) // If the score multiplier goes below one...
	{
		scoreMultiplier = 1; // The score multiplier is one
	}
	if(scoreMultiplier > 1000) // If the score multiplier goes above one thousand...
	{
		scoreMultiplier = 1000; // The score multiplier is one thousand
	}
}

void ClearCoins() // Clears all the falling coins
{
	int n = 0; // Counter

	while(n < 20) // Cycle through the coin array
	{
		coins[n].rotationPos = -1; // Remove all the coins
		n++;
	}
}

void DropCoins() // Move all the coin down and check for collision
{
	int n; // Counter
	int x, y; // More counters
	bool collectedCoin; // Set to true if the coin gets collected

	n = 0;
	while(n < 20) // Cycle through the coin array
	{
		collectedCoin = false; // This coin is not collected yet

		if(coins[n].rotationPos >= 0) // If the coin exists...
		{
			coins[n].rotationPos++; // Increase the rotation

			if(coins[n].rotationPos >= (8 * COINSPEED)) // If the coin has gone through all the rotations
			{
				coins[n].rotationPos = coins[n].rotationPos - (8 * COINSPEED); // Cycle back to the first rotation
			}

			if(coins[n].y + 2 >= GAMEHEIGHT*TILESIZE - 32) // If the coin is low enough to hit the paddle...
			{
				for(x = 0; x < 16; x++) // Check each column of the coins pixel map
				{
					y = 15;
					while(y >= 0) // Find the lowest active pixel
					{
						if(coinMap[coins[n].rotationPos/COINSPEED][x][y])
						{
							break;
						}
						y--;
					}

					if(y) // If a pixel was found in this column...
					{
						// Check for collisions at the paddles various pixel heights
						if(coins[n].x + x >= GetPaddlePosition() &&
							coins[n].x + x < GetPaddlePosition() + 8*(GetPaddleSize()+2))
						{
							if(coins[n].x + x >= GetPaddlePosition()+1 &&
								coins[n].x + x < GetPaddlePosition() + 8*(GetPaddleSize()+2)-1)
							{
								if(coins[n].x + x >= GetPaddlePosition()+3 &&
									coins[n].x + x < GetPaddlePosition() + 8*(GetPaddleSize()+2)-3)
								// If the coin pixel is in line with the longest segments of the paddle...
								{
									if(coins[n].y + y >= (GAMEHEIGHT*TILESIZE-1) - (TILESIZE*2 -2) && coins[n].y + y < (GAMEHEIGHT*TILESIZE-2) ||
										coins[n].y + (15-y) >= (GAMEHEIGHT*TILESIZE-1) - (TILESIZE*2 -2) && coins[n].y + (15-y) < GAMEHEIGHT*TILESIZE-2)
									// If the coin pixel, or is vertically opposite pixel touches these segments...
									{
										collectedCoin = true; // The coin will be collected
									}
								}

								// If the coin pixel is in line with the second longest segments of the paddle...

								if(coins[n].y + y >= (GAMEHEIGHT*TILESIZE-1) - (TILESIZE*2 -2) + 1 && coins[n].y + y < (GAMEHEIGHT*TILESIZE-3) ||
									coins[n].y + (15-y) >= (GAMEHEIGHT*TILESIZE-1) - (TILESIZE*2 -2) + 1 && coins[n].y + (15-y) < GAMEHEIGHT*TILESIZE-3)
								// If the coin pixel, or is vertically opposite pixel touches these slightly shorter segments...
								{
									collectedCoin = true; // The coin will be collected
								}
							}	

							// If the coin pixel is in line with the shortest segments of the paddle...

							if(coins[n].y + y >= (GAMEHEIGHT*TILESIZE-1) - (TILESIZE*2 -2) + 2 && coins[n].y + y < (GAMEHEIGHT*TILESIZE-4) ||
								coins[n].y + (15-y) >= (GAMEHEIGHT*TILESIZE-1) - (TILESIZE*2 -2) + 2 && coins[n].y + (15-y) < GAMEHEIGHT*TILESIZE-4)
							// If the coin pixel, or is vertically opposite pixel touches these shortest segments...
							{
								collectedCoin = true; // The coin will be collected
							}
						}
					}
				}
			}
		}
		
		if(collectedCoin) // If the coin is marked for collection...
		{
			AddSound(SOUND_COIN); // Play the coin collection sound
			GainPowerup(coins[n].powerup); // Give the player the coins powerup
			LoseCoin(n); // Remove the coin
		}
		else // If the coin was not marked for collection...
		{			
			if(coins[n].y + 2 > (GAMEHEIGHT * TILESIZE)) // If the coin is falling off the bottom of the board...
			{
				LoseCoin(n); // Remove the coin from the game
			}
			else // If the coin is not falling off the bottom of the board...
			{
				coins[n].y = coins[n].y + 2; // Drop the coin by two pixels
			}
		}
		n++;
	}
}

void LoseCoin(int num) // Remove the given coin from the game
{
	// int x, y; // Counters
	int n; // Another counter

	n = num; // Set n to the given coin number

	while(n < 19)	// Cycle through the coin array till you find an empty spot or get to the end
	{
		if(coins[n+1].rotationPos >= 0) // If another coin exists after the current coin...
		{
			// Overwrite the current coin witht he next coin
			coins[n].powerup = coins[n+1].powerup;
			coins[n].rotationPos = coins[n+1].rotationPos;
			coins[n].x = coins[n+1].x;
			coins[n].y = coins[n+1].y;
			n++;
		}
		else // If another coin does not exists after the current coin...
		{
			break; // Break the loop
		}
	}
	// Delete the final coin
	coins[n].powerup = 0;
	coins[n].rotationPos = -1;
	coins[n].x = 0;
	coins[n].y = 0;
}

void AddCoin(int powerup, int x, int y) // Add a new coin if there is room
{
	int n; // Counter

	n = 0;
	while(n < 20) // Find the last empty spot in the coin array
	{
		if(coins[n].rotationPos >= 0) // If the coin has a rotation position the it exists
		{
			n++; // Skip to the next coin
		}
		else // If the coin does not have a rotation position
		{
			break; // Found an empty spot
		}
	}
	if(n != 20) // If an empty spot was found...
	{
		// Create the new coin
		coins[n].powerup = powerup;
		coins[n].rotationPos = 0;
		coins[n].x = x;
		coins[n].y = y;
	}
}

void LoadCoinMap() // Load the coin pixel maps from file
{
	int x, y; // Counters
	char ch; // Place holder for each character as it's read from the file.
	int coinPos; // Rotation position
	FILE *coinMapFile;  // File to read the coin map data from
	
	// Variables for handling the characters retrieved from the file
	std::ostringstream fileNum;
	std::string strNum;

	coinMapFile = fopen("CoinMap.txt", "r"); // Open CoinMap.txt for reading

	if(coinMapFile != NULL) // Check that the file opened
	{
		do {
			ch = fgetc(coinMapFile); // Grab the first character in the file
			if(ch == 'P') // Found a position marker
			{
				fileNum.str(""); // Reset the data stream
				ch = fgetc(coinMapFile); // Retrieve the first digit
				
				while(ch != '\n' && ch != EOF) // Retrieve all the characters left on the line
				{
					fileNum << ch; // Concatenate each character retrieved into one variable
					ch = fgetc(coinMapFile); // Retrieve the next character
				}
				strNum = fileNum.str(); // Convert the coin poistion digits to a string
				coinPos = atoi(strNum.c_str()) - 1; // Convert the string to an integar

				// Retrieve the pixel information for the coin map
				for(y = 0; y < 16; y++)
				{
					for(x = 0; x < 16; x++)
					{
						fileNum.str(""); // Reset the data stream
						ch = fgetc(coinMapFile); // Retrieve the first digit
						while(ch != ',' && ch != '\n' && ch != EOF)
						{
							fileNum << ch; // Concatenate each character retrieved into one variable
							ch = fgetc(coinMapFile); // Retrieve the next character
						}

						strNum = fileNum.str(); // Convert the coin pixel digits to a string
						coinMap[coinPos][x][y] = atoi(strNum.c_str()); // Store the pixel state as an integar
					}
				}
			}
		} while (ch != EOF);

		fclose (coinMapFile); // Stop reading the file
	}
}

void GainPowerup(int num) // Apply a powerup
{
	int n; // Counter
	
	switch(num) // Apply the appropriate powerup
	{
	case INCPADDLESIZE:
		AdjustPaddleSize(1); // Increase paddle size
		AddMessage(INCPADDLESIZE); // Add the appropriate message
		AddSound(SOUND_PADDLESIZEINC); // Add the paddle size increase powerup sound
		return;
	case DECPADDLESIZE:
		AdjustPaddleSize(-1); // Decrease paddle size
		AddMessage(DECPADDLESIZE); // Add the appropriate message
		AddSound(SOUND_PADDLESIZEDEC); // Add the paddle size decrease powerup sound
		return;
	case INCPADDLESPEED:
		AdjustPaddleSpeed(1); // Increase paddle speed
		AddMessage(INCPADDLESPEED); // Add the appropriate message
		AddSound(SOUND_PADDLESPEEDINC); // Add the paddle speed increase powerup sound
		return;
	case DECPADDLESPEED:
		AdjustPaddleSpeed(-1); // Decrease paddle speed
		AddMessage(DECPADDLESPEED); // Add the appropriate message
		AddSound(SOUND_PADDLESPEEDDEC); // Add the paddle speed decrease powerup sound
		return;
	case INCBALLSIZE:
		n = 0;
		while(n < 5) // Cycle through all the balls
		{
			if(balls[n].size != -1)
			{
				AdjustBallSize(n, 1); // Increase the ball size
			}
			n++;
		}
		AddMessage(INCBALLSIZE); // Add the appropriate message
		AddSound(SOUND_BALLSIZEINC); // Add the ball size increase powerup sound
		return;
	case DECBALLSIZE:
		n = 0;
		while(n < 5) // Cycle through all the balls
		{
			if(balls[n].size != -1)
			{
				AdjustBallSize(n, -1); // Decrease the ball size
			}
			n++;
		}
		AddMessage(DECBALLSIZE); // Add the appropriate message
		AddSound(SOUND_BALLSIZEDEC); // Add the ball size decrease powerup sound
		return;
	case INCBALLSPEED: // Not used anymore
		AddMessage(INCBALLSPEED); // Add the appropriate message
		return;
	case DECBALLSPEED: // Not used anymore
		AddMessage(DECBALLSPEED); // Add the appropriate message
		return;
	case EXTRABALL:
		AddBall(); // Add another ball
		AddMessage(EXTRABALL); // Add the appropriate message
		AddSound(SOUND_EXTRABALL); // Add the extra ball powerup sound
		return;
	case EXTRALIFE:
		AddLife(); // Add an extra life
		AddMessage(EXTRALIFE); // Add the appropriate message
		AddSound(SOUND_EXTRALIFE); // Add the extra life powerup sound
		return;
	case MAGNETIC:
		magnetic += POWERUPTIME; // Add extra time to the magnetic powerup
		AddMessage(MAGNETIC); // Add the appropriate message
		AddSound(SOUND_MAGNETISM); // Add the magnetism powerup sound
		return;
	case FIREBALL:
		AddFire(); // Add extra time to the fire powerup
		AddMessage(FIREBALL); // Add the appropriate message
		AddSound(SOUND_FIREBALL); // Add the fireball powerup sound
		return;
	case GUNS:
		laser += POWERUPTIME; // Add extra time to the laser powerup
		AddMessage(GUNS); // Add the appropriate message
		AddSound(SOUND_GUNS); // Add the laser guns powerup sound
		return;
	case EXPLOSIVE:
		AddExplosive(); // Add extra time to the explosive powerup
		AddMessage(EXPLOSIVE); // Add the appropriate message
		AddSound(SOUND_EXPLOSIVE); // Add the explosive ball powerup sound
		return;
	}
}

void AddFire() // Add extra time to the fire powerup
{
	int n; // Counter

	n = 0;
	while(n < 5) // Cycle through the balls
	{
		if(balls[n].size) // If the ball exists...
		{
			balls[n].fire += POWERUPTIME; // Add extra time to the fire powerup
			balls[n].explosive = 0; // Remove any explosive powerup
		}
		n++;
	}

}

void AddExplosive() // Add extra time to the explosive powerup
{
	int n; // Counter

	n = 0;
	while(n < 5) // Cycle through the balls
	{
		if(balls[n].size) // If the ball exists...
		{
			balls[n].fire = 0; // Remove any fire powerup
			balls[n].explosive += POWERUPTIME; // Add extra time to the esplosive powerup
		}
		n++;
	}
}

void ExplodeBrick(int num, int x, int y) // Explode a brick
{
	int n, m; // counters
	bool knockOut = false; // To mark a block for deletion

	// Cycle through a 5x5 grid around the given brick
	for(n = -2; n <= 2; n++)
	{
		for(m = -2; m <= 2; m++)
		{
			knockOut = false; // Default to non-deletion
			if(balls[num].size < 3) // If the ball that knocked out the brick is small...
			{
				if( (n == 0 || m == 0) && (n != 2 && n != -2 && m != 2 && m != -2)  )
				{ // Knock out the adjacent bricks
					knockOut = true;
				}
			}
			if(balls[num].size > 2 && balls[num].size < 6) // If the ball is a medium size...
			{
				if( (n >= -1 && n <= 1) && (m >= -1 && m <= 1) )
				{ // Knock out the 3x3 grid of bricks around the given brick
					knockOut = true;
				}
			}
			if(balls[num].size > 5) // If the ball was large...
			{
				if( (n >= -1 && n <= 1) || (m >= -1 && m <= 1) )
				{ // Knock out the 3x3 grid and the bricks adjoining it
					knockOut = true;
				}
			}

			if(knockOut) // If the brick is marked to be knocked out...
			{
				if(levelMap[x+n][y+m][1] == 1) // If the brick is a grey brick...
				{
					if(balls[num].size >= KNOCKOUTBALLSIZE) // If the ball size big enough to knockout a grey brick...
					{ // Grey bricks are only knocked out by larger balls

						// Remove the brick from the game
						levelMap[x+n][y+m][0] = 0;
						levelMap[x+n][y+m][1] = 0;

						// If there was a powerup coin attached to the brick...
						// (Shouldn't trigger for grey bricks, but code is added incase this is changed later)
						if(levelMap[x+n][y+m][2])
						{
							// Add the coin to the game
							AddCoin(levelMap[x+n][y+m][2], (x+n)*BRICKSIZE, (y+m)*BRICKSIZE);
							levelMap[x+n][y+m][2] = 0; // Remove the coin data from the level map
						}
					}
					// If the ball is too small then nothing happens to grey bricks
				}
				else // If it is not a grey brick...
				{
					if(levelMap[x+n][y+m][0] && levelMap[x+n][y+m][1]) // If there is a block...
					{
						// Add points based on the type of brick that was knocked out and active multipliers
						AddScore(levelMap[x+n][y+m][0], levelMap[x+n][y+m][1], balls[num].bricks);
						if(ChangeNumBricks(-1)) // Reduce the number of bricks required to clear the level
						{
							return; // New level was initiated, abort exploding
						}

						// Remove the brick from the game
						levelMap[x+n][y+m][0] = 0;
						levelMap[x+n][y+m][1] = 0;

						// If there was a powerup coin attached to the brick...
						if(levelMap[x+n][y+m][2])
						{
							// Add the coin to the game
							AddCoin(levelMap[x+n][y+m][2], (x+n)*BRICKSIZE, (y+m)*BRICKSIZE);
							levelMap[x+n][y+m][2] = 0; // Remove the coin data from the level map
						}
						balls[num].bricks++; // Increase the brick score multiplier for the ball
						balls[num].greyBricks = 0; // Reset the grey brick counter
					}
				}
			}
		}
	}
	AddExplosion((x-2)*BRICKSIZE, (y-2)*BRICKSIZE, (balls[num].size/3) + 1);
}

void ResetExplosions() // Remove all explosions
{
	int n; // Counter

	n = 0;
	while(n < 25) // Set all explosion attributes to zero
	{
		explosions[n].x = 0;
		explosions[n].y = 0;
		explosions[n].size = 0;
		n++;
	}
}

void AddExplosion(int x, int y, int size) // Add an explosion to the game
{
	int n; // Counter

	n = 0;
	while(n < 25) // Cycle through the explosions
	{
		if(explosions[n].size == 0) // If an empty explosion is found...
		{
			// Create an explosion using the given details
			explosions[n].x = x;
			explosions[n].y = y;
			explosions[n].size = size*FRAMES;
			break;
		}
		n++;
	}
}

void RemoveExplosion(int num) // Remove and explosion
{
	int n; // Counter

	n = num;
	while(n < 24) // Cycle through all the explosions after the given explosion
	{
		if(explosions[n+1].size > 0) // If there is another explosion...
		{
			// Copy it over the current explosion
			explosions[n].x = explosions[n+1].x;
			explosions[n].y = explosions[n+1].y;
			explosions[n].size = explosions[n+1].size;
		}
		else
		{
			// There are no further explosions
			break;
		}
		n++;
	}
	
	// Delete the current explosion
	explosions[n].x = 0;
	explosions[n].y = 0;
	explosions[n].size = 0;
}

void ResetBullets()// Reset the bullets
{
	int n; // Counter

	n = 0;
	while(n < 20) // Cycle through the bullets
	{
		// Reset the bullets
		bullets[n].x = 0;
		bullets[n].y = 0;
		bullets[n].remove = false;
		n++;
	}
}

void AddBullets() // Add bullets to the game
{
	int n, m; // Counter

	n = 0;
	m = 0;
	while(m < 2) // Add 2 bullets
	{
		while(n < 20) // Cycle through the bullets
		{
			if(bullets[n].x == 0) // If there is room for a bullet...
			{
				if(m == 0) // Add the first bullet to the left laser
				{
					bullets[n].x = GetPaddlePosition() + 6;
					bullets[n].y = TILESIZE*GAMEHEIGHT-16;
					bullets[n].remove = false;
				}
				else // Add the second bullet to the right laser
				{
					bullets[n].x = GetPaddlePosition() + 8*(GetPaddleSize()+1);
					bullets[n].y = TILESIZE*GAMEHEIGHT-16;
					bullets[n].remove = false;
				}
				break;
			}
			n++;
		}
		m++;
	}

	AddSound(SOUND_LASERFIRE); // Add the laser fire sound
}

void RemoveBullet(int num) // Remove a bullet
{
	int n; // Counter

	n = num;
	while(n < 19) // Cycle through till the second to last bullet
	{
		if(bullets[n+1].x > 0) // If another bullet exists after the current one
		{
			bullets[n].x = bullets[n+1].x;
			bullets[n].y = bullets[n+1].y;
			bullets[n].remove = bullets[n+1].remove;
		}
		else // If no more bullets exist
		{
			break; // Stop overwriting bullets
		}
		n++;
	}

	// Remove the last bullet
	bullets[n].x = 0;
	bullets[n].y = 0;
	bullets[n].remove = false;
}

void MoveBullets() // Move the bullets and check for collisions
{
	int n; // Counter

	n = 0;
	while(n < 20) // Cycle through the bullets
	{
		if(bullets[n].remove) // If the bullet is marked for removal...
		{
			RemoveBullet(n);
		}
		else // If the bullet isn't marked for removal...
		{
			if(bullets[n].x > 0) // If the bullet exists...
			{
				// Check for collsion with the top border
				if(bullets[n].y - LASERSPEED <= 0) // If the bullet hits the top border...
				{
					bullets[n].remove = true; // Mark the bullet for removal
				}
				else // If the bullet doesn't hit the top border..
				{
					if(levelMap[bullets[n].x/BRICKSIZE][(bullets[n].y-LASERSPEED)/BRICKSIZE][0] > 0 ||
						levelMap[bullets[n].x/BRICKSIZE][(bullets[n].y-LASERSPEED)/BRICKSIZE][1] > 0)
					// If the bullet hits a brick...
					{
						 // If it isn't a grey brick
						if(levelMap[bullets[n].x/BRICKSIZE][(bullets[n].y-LASERSPEED)/BRICKSIZE][1] != 1)
						{
							// Add points based on the type of brick that was knocked out and active multipliers
							AddScore(levelMap[bullets[n].x/BRICKSIZE][(bullets[n].y-LASERSPEED)/BRICKSIZE][0],
									 levelMap[bullets[n].x/BRICKSIZE][(bullets[n].y-LASERSPEED)/BRICKSIZE][1],
									 1);
							
							ChangeNumBricks(-1); // Reduce the number of bricks required to clear the level
						}
						
						AddSound(SOUND_BRICKKO); // Play the sound for knocking out a brick
						// Remove the brick from the level map
						levelMap[bullets[n].x/BRICKSIZE][(bullets[n].y-LASERSPEED)/BRICKSIZE][0] = 0;
						levelMap[bullets[n].x/BRICKSIZE][(bullets[n].y-LASERSPEED)/BRICKSIZE][1] = 0;

						// If there was a powerup coin attached to the brick...
						if(levelMap[bullets[n].x/BRICKSIZE][(bullets[n].y-LASERSPEED)/BRICKSIZE][2])
						{
							// Create the powerup coin to the game in place of the brick
							AddCoin(levelMap[bullets[n].x/BRICKSIZE][(bullets[n].y-LASERSPEED)/BRICKSIZE][2],
								(bullets[n].x/BRICKSIZE)*BRICKSIZE,
								((bullets[n].y-LASERSPEED)/BRICKSIZE)*BRICKSIZE);

							// Remove the powerup coin data from the level map
							levelMap[bullets[n].x/BRICKSIZE][(bullets[n].y-LASERSPEED)/BRICKSIZE][2] = 0;
						}
						
						bullets[n].remove = true; // Mark the bullet for removal
					}

					bullets[n].y -= LASERSPEED; // Move the bullet					
				}
			}
			n++;
		}
	}
}

void AddMessage(int num) // Add a message to the queue
{
	int n; // Counter

	n = 0;
	while(n < 3) // Cycle through the messages
	{
		if(messages[n] == 0) // If there is room in the queue for a message...
		{
			messages[n] = num; // Add the new message to the queue
			break;
		}
		n++;
	}

	if(n == 0) // If this was the first message in the queue...
	{
		messageTimer = MESSAGETIME; // Show the message for 5s
	}

	if(n == 3) // If there was no room for a message...
	{
		RemoveMessage(); // Make room for a new message
		messages[2] = num; // Add the new message
	}
}

void RemoveMessage() // Remove the first message
{
	int n; // Counter

	n = 0;

	if(messages[n] == 0) // If no message exists...
	{
		messageTimer = -1; // Set the message timer to inactive
		return;
	}

	while(n < 2) // Cycle through the messages
	{
		if(messages[n+1] > 0) // If another message exists after the current message...
		{
			messages[n] = messages[n+1]; // Overwrite the current message with the next message
		}
		else // If no more messages exist
		{
			break;
		}
		n++;
	}

	messages[n] = 0; // Wipe the redundant message
	messageTimer = MESSAGETIME; // Show the next message for 1s
}

void ClearMessages() // Clear the message queue
{
	int n; // Counter

	n = 0;
	while(n < 3)
	{
		messages[n] = 0; // Clear the message
		n++;
	}

	messageTimer = -1;
}

void SetConfirmation(int num) // Set the confirmation box
{
	confirmationAction = 0; // Set the selected confirmation action to the first action

	if(num < 0) // If the confirmation request is less than 0...
	{
		confirmationBox = 0; // There is no confirmation box
		return;
	}
	if(num > CONFIRMATIONBOXES) // If the confirmation request does not exist...
	{
		confirmationBox = 0; // There is no confirmation box
		return;
	}
	confirmationBox = num; // Set the confirmation box

	// Set the number of available actions based on the active confirmation box
	switch(confirmationBox)
	{
	case 0:
		numConfirmationActions = 0;
		break;
	case CONFIRMQUIT:
		numConfirmationActions = 2;
		break;
	case CONFIRMNEW:		
		numConfirmationActions = 2;
		break;
	case CONFIRMEDITORSTART:
		numConfirmationActions = 2;
		break;
	case CONFIRMEDITORQUIT:
		numConfirmationActions = 3;
		break;
	case CONFIRMEDITORLEVELCHANGE:
		numConfirmationActions = 3;
		break;
	case CONFIRMRESTORE:
		numConfirmationActions = 2;
		break;
	}
}

void ChangeConfirmationAction(int num) // Change the confirmation action
{
	if(num > 0)
	{
		confirmationAction++; // Increase the confirmation action
	}
	if(num < 0)
	{
		confirmationAction--; // Decrease the confirmation action
	}

	if(confirmationAction < 0) // If the action is decrease below zero...
	{
		confirmationAction = numConfirmationActions - 1; // Set the confirmation action to the last action
	}
	if(confirmationAction >= numConfirmationActions) // If there are no further actions...
	{
		confirmationAction = 0; // Set the confirmation action to the first action
	}
}

void SelectConfirmationAction() // Select the current confirmation action
{
	switch(confirmationBox)
	{
	case CONFIRMQUIT: // Confirmed an action on the Quit confirmation box
		if(confirmationAction == 0) // If the first action (Yes) is selected...
		{
			QuitGame();
			return;
		}
		if(confirmationAction == 1) // If the second action (No) is selected...
		{
			SetConfirmation(0); // Clear the confirmation box
			return;
		}
		break;
	case CONFIRMNEW: // Confirmed an action on the New Game confirmation box
		if(confirmationAction == 0) // If the first action (Yes) is selected...
		{
			StartGame(); // Start a new game
			SetConfirmation(0); // Clear the confirmation box
			return;
		}
		if(confirmationAction == 1) // If the second action (No) is selected...
		{
			SetConfirmation(0); // Clear the confirmation box
			return;
		}
		break;
	case CONFIRMEDITORSTART: // Confirmed an action on the Start Editor confirmation box
		if(confirmationAction == 0) // If the first action (Yes) is selected...
		{
			StartEditor();
			SetConfirmation(0); // Clear the confirmation box
			return;
		}
		if(confirmationAction == 1) // If the second action (No) is selected...
		{
			SetConfirmation(0); // Clear the confirmation box
			return;
		}
		break;
	case CONFIRMEDITORQUIT: // Confirmed an action on the Quit Editor confirmation box
		if(confirmationAction == 0) // If the first action (Yes) is selected...
		{
			SaveLevel(); // Save the level
			QuitEditor(); // Quit the level Editor
			SetConfirmation(0); // Clear the confirmation box
			return;
		}
		if(confirmationAction == 1) // If the second action (No) is selected...
		{			
			QuitEditor(); // Quit the level Editor
			SetConfirmation(0); // Clear the confirmation box
			return;
		}
		if(confirmationAction == 2) // If the third action (Cancel) is selected...
		{
			SetConfirmation(0); // Clear the confirmation box
			return;
		}
		break;
	case CONFIRMEDITORLEVELCHANGE: // Confirmed an action on the Change Editor Level confirmation box
		if(confirmationAction == 0) // If the first action (Yes) is selected...
		{
			SaveLevel(); // Save the level
			ChangeSelectedLevel(levelChangeRequest); // Change the selected editor level
			SetConfirmation(0); // Clear the confirmation box
			levelChangeRequest = 0; // Reset the level change request
			return;
		}
		if(confirmationAction == 1) // If the second action (No) is selected...
		{
			ChangeSelectedLevel(levelChangeRequest); // Change the selected editor level
			SetConfirmation(0); // Clear the confirmation box
			levelChangeRequest = 0; // Reset the level change request
			return;
		}
		if(confirmationAction == 2) // If the thirf action (Cancel) is selected...
		{
			SetConfirmation(0); // Clear the confirmation box
			levelChangeRequest = 0; // Reset the level change request
			return;
		}
		break;		
	case CONFIRMRESTORE: // Confirmed an action on the Restore Levels confirmation box
		if(confirmationAction == 0) // If the first action (Yes) is selected...
		{
			RestoreLevels();
			SetConfirmation(0); // Clear the confirmation box
			return;
		}
		if(confirmationAction == 1) // If the second action (No) is selected...
		{
			SetConfirmation(0); // Clear the confirmation box
			return;
		}
		break;
	}
	SetConfirmation(0); // Clear the confirmation box
}

void QuitGame() // Releases memory and closes the game
{
	DestroyWindow(mainWindow); // Quit the game
}

// Level Editor Functions

void StartEditor() // Enter the level editor
{
	int x, y; // Counters
	
	delHeld = false; // Del key held is no longer tracked
	spaceHeld = false; // Space held is no longer tracked
	levelEditor = true; // Level editor mode is active
	levelEditorX = EDITORWIDTH/2; // Horizontal position of the cursor on the editor map
	levelEditorY = EDITORHEIGHT/2; // Vertical position of the cursor on the editor map
	levelEditorBrickColour = helpColour; // The currently selected block colour
	levelEditorBrickStyle = helpStyle; // The currently selected block colour
	levelEditorLevel = maxLevel+1; // The level being edited
	levelModified = false; // The current level hasn't been modified
	gameLost = true;
	
	// Start with a new, blank map
	for(x = 0; x < BGAMEWIDTH; x++)
	{
		for(y = 0; y < BGAMEHEIGHT; y++)
		{
			levelEditorMap[x][y][0] = 0;
			levelEditorMap[x][y][1] = 0;
		}
	}

	LoadBackground(levelEditorLevel); // Load the new background
}

void QuitEditor() // Quit the level editor
{
	delHeld = false; // Del key held is no longer tracked
	spaceHeld = false; // Space held is no longer tracked
	levelEditor = false; // Level editor mode is inactive
}

void DrawEditorBricks() // Draw the level editor bricks
{
	int x, y; // Counters

	// Varibles use to determine which block to draw
	int topSide, bottomSide, leftSide, rightSide;
	int tlCorner, trCorner, brCorner, blCorner;

	// The centre block (unshaded) in the bitmap for the block type and colour
	int centreX, centreY;
	
	// Step through the level editor map
	for(x = 0; x < BGAMEWIDTH; x++)
	{
		for(y = 0; y < BGAMEHEIGHT; y++)
		{
			if(levelEditorMap[x][y][0] || levelEditorMap[x][y][1]) // If a block exists then draw it
			{
				// Varibles used to determine which block to draw
				topSide = 0;
				bottomSide = 0;
				leftSide = 0;
				rightSide = 0;
				tlCorner = 0;
				trCorner = 0;
				brCorner = 0;
				blCorner = 0;
				
				// The centre block (unshaded) in the bitmap for the block type and colour
				centreX = (levelEditorMap[x][y][0]*10)-8;
				centreY = (levelEditorMap[x][y][1]*3)-2;

				// Check if there's a block of the same type and colour above this block
				if(y != 0)
				{
					if(levelEditorMap[x][y][0] == levelEditorMap[x][y-1][0] && levelEditorMap[x][y][1] == levelEditorMap[x][y-1][1])
					{
						topSide = 1;
					}
				}
				// Check if there's a block of the same type and colour below this block
				if(y != BGAMEHEIGHT-1)
				{
					if(levelEditorMap[x][y][0] == levelEditorMap[x][y+1][0] && levelEditorMap[x][y][1] == levelEditorMap[x][y+1][1])
					{
						bottomSide = 1;
					}
				}
				// Check if there's a block of the same type and colour left of this block
				if(x != 0)
				{
					if(levelEditorMap[x][y][0] == levelEditorMap[x-1][y][0] && levelEditorMap[x][y][1] == levelEditorMap[x-1][y][1])
					{
						leftSide = 1;
					}
				}
				// Check if there's a block of the same type and colour right of this block
				if(x != BGAMEWIDTH-1)
				{
					if(levelEditorMap[x][y][0] == levelEditorMap[x+1][y][0] && levelEditorMap[x][y][1] == levelEditorMap[x+1][y][1])
					{
						rightSide = 1;
					}
				}

				// Check for every combination of matches Top, Bottom, Left, Right and draw the appropriately shaded block.
				// Additional checks for diagonally adjacent mismatches when the corresponding adjacent matches are present
				// x and y checks also present to ensure there are not out of bounds errors on the levelMap array
				if(topSide)
				{
					if(bottomSide)
					{
						if(leftSide)
						{
							if(x != 0)
							{
								if(y != 0)
								{
									// Top-Left corner check (checking for not present)
									if(levelEditorMap[x][y][0] != levelEditorMap[x-1][y-1][0] || levelEditorMap[x][y][1] != levelEditorMap[x-1][y-1][1])
									{
										// Same bricks exist above and to the left, but not diagonally up and left
										tlCorner = 1; // Need to draw a shader in the top-left corner
									}
								}
								if(y != BGAMEHEIGHT-1)
								{				
									// Bottom-Left corner check (checking for not present)					
									if(levelEditorMap[x][y][0] != levelEditorMap[x-1][y+1][0] || levelEditorMap[x][y][1] != levelEditorMap[x-1][y+1][1])
									{
										// Same bricks exist below and to the left, but not diagonally down and left
										blCorner = 1; // Need to draw a shader in the bottom-left corner
									} 
								}
							}
							if(rightSide)
							{
								if(x != BGAMEWIDTH-1)
								{
									if(y != 0)
									{
										// Top-Right corner check (checking for not present)
										if(levelEditorMap[x][y][0] != levelEditorMap[x+1][y-1][0] || levelEditorMap[x][y][1] != levelEditorMap[x+1][y-1][1])
										{	
											// Same bricks exist above and to the right, but not diagonally up and right
											trCorner = 1; // Need to draw a shader in the top-right corner
										}
									}
									if(y != BGAMEHEIGHT-1)
									{
										// Bottom-Right corner check (checking for not present)
										if(levelEditorMap[x][y][0] != levelEditorMap[x+1][y+1][0] || levelEditorMap[x][y][1] != levelEditorMap[x+1][y+1][1])
										{	
											// Same bricks exist below and to the right, but not diagonally down and right
											brCorner = 1; // Need to draw a shader in the bottom-right corner
										}
									}
								}
								// All sides match
								DrawBrick(x, y, centreX, centreY); // Draw an unshaded block
							}
							else
							{
								// Top, Left and Bottom sides match
								DrawBrick(x, y, centreX+1, centreY); // Draw a block with only right side shaded.
							}
						}
						else
						{							
							if(rightSide)
							{								
								if(x != BGAMEWIDTH-1)
								{
									if(y != 0)
									{
										// Top-Right corner check (checking for not present)
										if(levelEditorMap[x][y][0] != levelEditorMap[x+1][y-1][0] || levelEditorMap[x][y][1] != levelEditorMap[x+1][y-1][1])
										{
											// Same bricks exist above and to the right, but not diagonally up and right
											trCorner = 1; // Need to draw a shader in the top-right corner
										}
									}
									if(y != BGAMEHEIGHT-1)
									{
										// Bottom-Right corner check (checking for not present)
										if(levelEditorMap[x][y][0] != levelEditorMap[x+1][y+1][0] || levelEditorMap[x][y][1] != levelEditorMap[x+1][y+1][1])
										{	
											// Same bricks exist below and to the right, but not diagonally down and right
											brCorner = 1; // Need to draw a shader in the bottom-right corner
										}
									}
								}
								// Top, Right and Bottom sides match
								DrawBrick(x, y, centreX-1, centreY); // Draw a block with only left side shaded
							}
							else
							{
								// Top and Bottom sides match
								DrawBrick(x, y, centreX+2, centreY-1); // Draw a brick with both left and right sides shaded
							}
						}
					}
					else
					{
						if(leftSide)
						{
							if(x != 0)
							{
								if(y != 0)
								{
									// Top-Left corner check (checking for not present)
									if(levelEditorMap[x][y][0] != levelEditorMap[x-1][y-1][0] || levelEditorMap[x][y][1] != levelEditorMap[x-1][y-1][1])
									{
										// Same bricks exist above and to the left, but not diagonally up and left
										tlCorner = 1; // Need to draw a shader in the top-left corner
									}
								}
							}							
							if(rightSide)
							{
								if(x != BGAMEWIDTH-1)
								{
									if(y != 0)
									{
										// Top-Right corner check (checking for not present)
										if(levelEditorMap[x][y][0] != levelEditorMap[x+1][y-1][0] || levelEditorMap[x][y][1] != levelEditorMap[x+1][y-1][1])
										{	
											// Same bricks exist above and to the right, but not diagonally up and right
											trCorner = 1; // Need to draw a shader in the top-right corner
										}
									}
								}
								// Top, left and Right sides match
								DrawBrick(x, y, centreX, centreY+1); // Draw a brick with only bottom shaded
							}
							else
							{
								// Top and Left sides match
								DrawBrick(x, y, centreX+1, centreY+1); // Draw a brick with right and bottom sides shaded
							}
						}
						else
						{							
							if(rightSide)
							{
								if(x != BGAMEWIDTH-1)
								{
									if(y != 0)
									{
										// Top-Right corner check (checking for not present)
										if(levelEditorMap[x][y][0] != levelEditorMap[x+1][y-1][0] || levelEditorMap[x][y][1] != levelEditorMap[x+1][y-1][1])
										{	
											// Same bricks exist above and to the right, but not diagonally up and right
											trCorner = 1; // Need to draw a shader in the top-right corner
										}
									}
								}
								// Top and Right sides match
								DrawBrick(x, y, centreX-1, centreY+1); // Draw a brick with left and bottom sides shaded
							}
							else
							{
								// Only Top side matches
								DrawBrick(x, y, centreX+3, centreY+1); // Draw a brick with Left, Right and Bottom sides shaded
							}
						}
					}
				}
				else
				{
					if(bottomSide)
					{
						if(leftSide)
						{
							if(x != 0)
							{
								if(y != BGAMEHEIGHT-1)
								{	
									// Bottom-Left corner check (checking for not present)								
									if(levelEditorMap[x][y][0] != levelEditorMap[x-1][y+1][0] || levelEditorMap[x][y][1] != levelEditorMap[x-1][y+1][1])
									{
										// Same bricks exist below and to the left, but not diagonally down and left
										blCorner = 1; // Need to draw a shader in the bottom-left corner
									}
								}
							}
							if(rightSide)
							{
								if(x != BGAMEWIDTH-1)
								{
									if(y != BGAMEHEIGHT-1)
									{
										// Bottom-Right corner check (checking for not present)
										if(levelEditorMap[x][y][0] != levelEditorMap[x+1][y+1][0] || levelEditorMap[x][y][1] != levelEditorMap[x+1][y+1][1])
										{	
											// Same bricks exist below and to the right, but not diagonally down and right
											brCorner = 1; // Need to draw a shader in the bottom-right corner
										}
									}
								}
								// Left, Right and Bottom sides match
								DrawBrick(x, y, centreX, centreY-1); // Draw a brick with only top side shaded
							}
							else
							{
								// Left and Bottom sides match
								DrawBrick(x, y, centreX+1, centreY-1); // Draw a brick with top and right sides shaded
							}
						}
						else
						{
							if(rightSide)
							{
								if(x != BGAMEWIDTH-1)
								{
									if(y != BGAMEHEIGHT-1)
									{
										// Bottom-Right corner check (checking for not present)
										if(levelEditorMap[x][y][0] != levelEditorMap[x+1][y+1][0] || levelEditorMap[x][y][1] != levelEditorMap[x+1][y+1][1])
										{	
											// Same bricks exist below and to the right, but not diagonally down and right
											brCorner = 1; // Need to draw a shader in the bottom-right corner
										}
									}
								}
								// Right and Bottom sides match
								DrawBrick(x, y, centreX-1, centreY-1); // Draw a brick with top and left sides shaded
							}
							else
							{
								// Only Bottom side matches
								DrawBrick(x, y, centreX+3, centreY-1); // Draw a brick with top, left and right sides shaded
							}
						}
					}
					else
					{
						if(leftSide)
						{
							if(rightSide)
							{
								// Left and Right sides match
								DrawBrick(x, y, centreX+2, centreY+1); // Draw a brick with top and bottom sides shaded
							}
							else
							{
								// Only Left side matches
								DrawBrick(x, y, centreX+4, centreY+1); // Draw a brick with top, right and bottom sides shaded
							}
						}
						else
						{
							if(rightSide)
							{
								// Onlt Right side matches
								DrawBrick(x, y, centreX+2, centreY); // Draw a brick with top, left and bottom sides shaded
							}
							else
							{
								// No sides match
								DrawBrick(x, y, centreX+3, centreY); // Draw a brick with all sides shaded
							}
						}
					}
				}

				// Overlay shades on the corners of the block based on the earlier diagonal checks
				if(tlCorner)
				{
					DrawBrickCorner(x, y, centreX+4, centreY-1); // Draw the inverse shade in the top-left corner
				}
				if(trCorner)
				{
					DrawBrickCorner(x, y, centreX+5, centreY-1); // Draw the inverse shade in the top-right corner
				}
				if(brCorner)
				{
					DrawBrickCorner(x, y, centreX+6, centreY-1); // Draw the inverse shade in the bottom-left corner
				}
				if(blCorner)
				{
					DrawBrickCorner(x, y, centreX+7, centreY-1); // Draw the inverse shade in the bottom-right corner
				}
			}
		}
	}
}

void DrawEditorCursors() // Draw the editor cursors
{
	int offset = (CURSORSIZE - BRICKSIZE)/2; // offset for the cursor
	int xPos, yPos; // Placement markers
	int frameSizeX = 212; // Horizontal frame size
	int frameSizeY = 92; // Vertical frame size

	xPos = GAMEWIDTH*TILESIZE/2 - frameSizeX/2; // Half game width - Half frame width
	yPos = GAMEHEIGHT*TILESIZE - frameSizeY - 31; // Game height - frame height and another 31

	// Draw the map cursor first
	// Mask first
	BitBlt(bmoBoard, BRICKSIZE*levelEditorX - offset, BRICKSIZE*levelEditorY - offset, CURSORSIZE, CURSORSIZE, bmoEditorCursor,
		CURSORSIZE*(cursorTimer/CURSORTIMING), CURSORSIZE, SRCAND);
	// Then image
	BitBlt(bmoBoard, BRICKSIZE*levelEditorX - offset, BRICKSIZE*levelEditorY - offset, CURSORSIZE, CURSORSIZE, bmoEditorCursor,
		CURSORSIZE*(cursorTimer/CURSORTIMING), 0, SRCPAINT);

	// Draw the brick selection cursor second
	// Mask first
	BitBlt(bmoBoard, xPos + (levelEditorBrickColour-1)*20 + 4, yPos + 24, CURSORSIZE, CURSORSIZE, bmoEditorCursor,
		CURSORSIZE*(cursorTimer/CURSORTIMING), CURSORSIZE, SRCAND);
	// Then image
	BitBlt(bmoBoard, xPos + (levelEditorBrickColour-1)*20 + 4, yPos + 24, CURSORSIZE, CURSORSIZE, bmoEditorCursor,
		CURSORSIZE*(cursorTimer/CURSORTIMING), 0, SRCPAINT);
}

void DrawBrickSelection() // Draw the frame with the available brick options
{
	int x, y, z; // Counters
	int xPos, yPos; // Placement markers
	int frameSizeX = 212; // Horizontal frame size
	int frameSizeY = 92; // Vertical frame size

	xPos = GAMEWIDTH*TILESIZE/2 - frameSizeX/2; // Half game width - Half frame width
	yPos = GAMEHEIGHT*TILESIZE - frameSizeY - 31; // Game height - frame height and another 31

	// Draw the frame in the calculated position
	// Mask First
	BitBlt(bmoBoard, xPos, yPos, frameSizeX, frameSizeY, bmoEditorFrames, 0, frameSizeY, SRCAND);
	// Then Image
	BitBlt(bmoBoard, xPos, yPos, frameSizeX, frameSizeY, bmoEditorFrames, 0, 0, SRCPAINT);

	// Draw the bricks
	x = levelEditorBrickStyle - 1; // Start one style back from the current style

	if(x < 1) // If x is before the first brick style...
	{
		x = brickStyles; // Wrap around to the last brick style
	}
	for(z = 0; z < 3; z++)
	{
		for(y = 1; y <= BRICKCOLOURS; y++) // Cycle through the colours
		{
			// Draw the brick
			
			// Mask first
			BitBlt(bmoBoard, xPos + (y-1)*20 + 8, yPos + z*20 + 8, BRICKSIZE, BRICKSIZE, bmoBricks, 0, 0, SRCAND);
			// Then image
			BitBlt(bmoBoard, xPos + (y-1)*20 + 8, yPos + z*20 + 8, BRICKSIZE, BRICKSIZE, bmoBricks, (x*10-5)*BRICKSIZE, (y*3-2)*BRICKSIZE, SRCPAINT);
		}

		// Cycle through 3 brickstyles
		x++;
		if(x > brickStyles) // If x is after the last brickstyle...
		{
			x = 1; // Wrap around to the first brick style
		}
	}

}

void DrawEditorMenu() // Draw the editor menu
{
	int xPos, yPos; // Placement markers
	int frameSizeX = 148; // Horizontal frame size
	int frameSizeY = 67; // Vertical frame size
	int bitmapX = 0; // Horizontal position of the frame in the bitmap
	int bitmapY = 184; // Vertical position of the frame in the bitmap

	xPos = 32; // Horizontal placement of the frame
	yPos = GAMEHEIGHT*TILESIZE - 123; // Vertical placement of the frame

	// Draw the frame in the provided position
	// Mask First
	BitBlt(bmoBoard, xPos, yPos, frameSizeX, frameSizeY, bmoEditorFrames, bitmapX, bitmapY, SRCAND);
	// Then Image
	BitBlt(bmoBoard, xPos, yPos, frameSizeX, frameSizeY, bmoEditorFrames, bitmapX, bitmapY, SRCPAINT);
}

void DrawEditorLevel() // Draw the editor level
{
	int xPos, yPos; // Placement markers
	int frameSizeX = 148; // Horizontal frame size
	int frameSizeY = 67; // Vertical frame size
	int bitmapX = 0; // Horizontal position of the frame in the bitmap
	int bitmapY = 251; // Vertical position of the frame in the bitmap	
	int temp1, temp2, offsetX, posLevel; // Variables for holding different numerals and place holders in the level

	xPos = GAMEWIDTH*TILESIZE - frameSizeX - 32; // Horizontal placement of the frame
	yPos = GAMEHEIGHT*TILESIZE - 123; // Vertical placement of the frame

	// Draw the frame in the provided position
	// Mask First
	BitBlt(bmoBoard, xPos, yPos, frameSizeX, frameSizeY, bmoEditorFrames, bitmapX, bitmapY + frameSizeY, SRCAND);
	// Then Image
	BitBlt(bmoBoard, xPos, yPos, frameSizeX, frameSizeY, bmoEditorFrames, bitmapX, bitmapY, SRCPAINT);

	// Calculate the number of level digits and the position
	temp1 = levelEditorLevel;
	temp2 = levelEditorLevel;
	posLevel = 0;
	bitmapX = 0; // Horizontal position of the frame in the bitmap
	bitmapY = 385; // Vertical position of the frame in the bitmap	

	// Count the digits in the level by dividing by 10 till theres no more result
	while(temp1/10)
	{
		posLevel++;
		temp1 = temp1/10;
	}

	offsetX = 4*(posLevel+1)-12; // Calculate where to start drawing the level number

	// Draw each digit in the score
	while(posLevel >= 0)
	{
		// Calculate and draw each digit
		temp1 = temp2/MyPower(10,posLevel); // Digit = temporary score / 10^posScore
		temp2 -= temp1*MyPower(10,posLevel); // Remove the printed digit from the temporary score

		// Mask first
		BitBlt(bmoBoard, xPos + frameSizeX/2 + offsetX - posLevel*8, yPos + 24, 8, 16, bmoEditorFrames, bitmapX + temp1*8, bitmapY+16, SRCAND);
		// Then image
		BitBlt(bmoBoard, xPos + frameSizeX/2 + offsetX - posLevel*8, yPos + 24, 8, 16, bmoEditorFrames, bitmapX + temp1*8, bitmapY, SRCPAINT);

		posLevel--;
	}
}

void LevelEditorAddBrick(int x, int y, int style, int colour) // Add a brick to the editor map
{
	if(x < 0 || x >= EDITORWIDTH || y < 0 || y >= EDITORHEIGHT) // If the brick position is outside the editor window...
	{
		return; // Do nothing
	}
	
	if(style < 0 || style > brickStyles) // If the style doesn't exist...	
	{
		return; // Do nothing
	}

	if(colour < 0 || colour > BRICKCOLOURS) // If the colour doesn't exist...
	{
		return; // Do nothing
	}

	// Add the brick to the map
	levelEditorMap[x][y][0] = style;
	levelEditorMap[x][y][1] = colour;

	levelModified = true; // Level has been edited
}

void MoveCursorHorizontally(int num) // Move the cursor horizontally
{
	if(levelEditorX + num < 0) // If the cursor will move out of bounds to the left...
	{
		levelEditorX = 0; // Stop at the left border
		return;
	}
	if(levelEditorX + num >= EDITORWIDTH) // If the cursor will move out of bounds to the right...
	{
		levelEditorX = EDITORWIDTH-1; // Stop at the right border
		return;
	}
	
	levelEditorX += num; // Move the cursor horizontally by the given amount
}

void MoveCursorVertically(int num) // Move the cursor vertically
{	
	if(levelEditorY + num < 0) // If the cursor will move out of bounds to the top...
	{
		levelEditorY = 0; // Stop at the left border
		return;
	}
	if(levelEditorY + num >= EDITORHEIGHT) // If the cursor will move out of bounds to the bottom...
	{
		levelEditorY = EDITORHEIGHT-1; // Stop at the right border
		return;
	}
	
	levelEditorY += num; // Move the cursor vertically by the given amount
}

void ChangeSelectedColour(int num) // Change the colour selected
{
	levelEditorBrickColour += num; // Change the brick colour

	while(levelEditorBrickColour > BRICKCOLOURS) // If the colour moved past the last available option...
	{
		levelEditorBrickColour -= BRICKCOLOURS; // Wrap back to the start
	}
	while(levelEditorBrickColour < 1) // If the colour moved past the first available option...
	{
		levelEditorBrickColour += BRICKCOLOURS; // Wrap back to the end
	}
}

void ChangeSelectedStyle(int num) // Change the style selected
{	
	levelEditorBrickStyle += num; // Change the brick colour

	while(levelEditorBrickStyle > brickStyles) // If the colour moved past the last available option...
	{
		levelEditorBrickStyle -= brickStyles; // Wrap back to the start
	}
	while(levelEditorBrickStyle < 1) // If the colour moved past the first available option...
	{
		levelEditorBrickStyle += brickStyles; // Wrap back to the end
	}
}

void ChangeSelectedLevel(int num) // Change the level selected
{	
	int x, y; // Counters

	levelEditorLevel += num; // Change the level selected

	while(levelEditorLevel > maxLevel + 1) // If the level selected moved past the last available level...
	{
		levelEditorLevel -= maxLevel + 1; // Wrap back to the start
	}
	while(levelEditorLevel < 1) // If the level selected moved past the first available level...
	{
		levelEditorLevel += maxLevel + 1; // Wrap back to the end
	}

	if(levelEditorLevel <= maxLevel) // If the selected level exists...
	{
		if(!LoadEditorLevel(levelEditorLevel)) // Attempt to load the new level and if it doesn't load...
		{
			levelEditorLevel = maxLevel + 1; // Set the editor to a new level

			for(x = 0; x < BGAMEWIDTH; x++) // Clear the editor map
			{
				for(y = 0; y < BGAMEHEIGHT; y++)
				{
					levelEditorMap[x][y][0] = 0;
					levelEditorMap[x][y][1] = 0;
				}
			}
		}
	}
	else // If the selected level doesn't exist...
	{
		for(x = 0; x < BGAMEWIDTH; x++) // Clear the editor map
		{
			for(y = 0; y < BGAMEHEIGHT; y++)
			{
				levelEditorMap[x][y][0] = 0;
				levelEditorMap[x][y][1] = 0;
			}
		}
	}

	LoadBackground(levelEditorLevel); // Load the new background
	levelModified = false; // The current level hasn't been modified
}

bool LoadEditorLevel(int num) // Load the level map from file.
{
	int x, y; // Counters
	int levelID = 0; // The level number retrieved from the levels file
	char ch; // Place holder for each character as it's read from the file.
	bool levelFound = false; // Check bit to see if the level desired was found or not
	FILE *levelFile; // File to read the level data from
	
	// Variables for handling the characters retrieved from the file
	std::ostringstream fileNum;
	std::string strNum;

	levelFile = fopen("Levels.txt", "r"); // Open Levels.txt for reading
	
	if(levelFile != NULL) // Check that the file opened
	{
		do {
			ch = fgetc(levelFile); // Grab the first character in the file
			if(ch == 'L') // Found a level marker now check if it's the level we want
			{
				fileNum.str(""); // Reset the data stream
				ch = fgetc(levelFile); // Retrieve the first digit
				while(ch != '\n' && ch != EOF) // Retrieve all the characters left on the line
				{
					fileNum << ch; // Concatenate each character retrieved into one variable
					ch = fgetc(levelFile); // Retrieve the next character
				}
				strNum = fileNum.str(); // Convert the level digits into a string
				levelID = atoi(strNum.c_str()); // Convert the string into an integar

				// Moving from searching to loading
				if(levelID == num) // If we found the right level
				{
					// Fill the values of the levelMap array with the level data
					y = 0;
					while(y < BGAMEHEIGHT)
					{
						x = 0;
						while(x < BGAMEWIDTH)
						{
							ch = fgetc(levelFile);
							while (ch != '(' && ch != EOF) // Skip through the file till the first open bracket
							{
								ch = fgetc(levelFile);
							}
							
							fileNum.str(""); // Reset the data stream
							ch = fgetc(levelFile); // Retrieve the first digit
							
							// Retrieve the first block value
							while(ch != ',' && ch != EOF) // Block values are seperated by commas
							{
								fileNum << ch; // Concatinate each digit of the value into one variable
								ch = fgetc(levelFile); // Reading the value one character at a time
							}
							strNum = fileNum.str(); // Convert the value into a string
							// Store the first value as an integar in the current position of the level map
							levelEditorMap[x][y][0] = atoi(strNum.c_str());
							
							fileNum.str(""); // Reset the data stream						
							ch = fgetc(levelFile); // Retrieve the first digit

							// Retrieve the second block value
							while(ch != ')' && ch != EOF) // Block definition ends with a closed bracket
							{
								fileNum << ch; // Concatinate each digit of the value into one variable
								ch = fgetc(levelFile); // Reading the value one character at a time
							}
							strNum = fileNum.str(); // Convert the value into a string
							// Store the first value as an integar in the current position of the level map
							levelEditorMap[x][y][1] = atoi(strNum.c_str());
							x++;
						}
						y++;
					}
					levelFound = true; // Level was found and loaded
					fclose(levelFile); // Stop reading the level file

					return levelFound; // Level was loaded
				}
			}
		} while (ch != EOF);
		fclose(levelFile); // Stop reading the level file
	}
	return levelFound; // Level wasn't loaded
}

void SaveLevel() // Save the current level
{
	
	int x, y; // Counters
	int levelID = 0;  // The level number retrieved from the levels file
	char ch; // Place holder for each character as it's read from the file.
	FILE *levelFile;  // File to read the level map data from
	FILE *newLevelFile; // File to write the new level map data to
	bool levelFound = false; // Check bit to mark if the level already existed or not
	
	// Variables for handling the characters retrieved from the file
	std::ostringstream fileNum;
	std::string strNum;
	char buffer[50];

	levelFile = fopen("Levels.txt", "r"); // Open Levels.txt for reading
	newLevelFile = fopen("newLevels.txt", "w"); // Open newLevels.txt for writing

	if(levelFile != NULL && newLevelFile != NULL) // If the files exist
	{
		ch = fgetc(levelFile);
		while(ch != EOF)
		{
			if(ch == 'M') // Look for the max Level marker
			{
				fputc(ch, newLevelFile); // Copy M to the new file
				if(levelEditorLevel > maxLevel) // If the level is higher than the current maximum
				{
					fileNum.str(""); // Reset the data stream
					ch = fgetc(levelFile); // Retrieve the first digit
					while(ch != '\n' && ch != EOF) // Retrieve all the characters left on the line
					{
						fileNum << ch; // Concatenate each character retrieved into one variable
						ch = fgetc(levelFile); // Retrieve the next character
					}
					strNum = fileNum.str(); // Convert the level digits into a string
					levelID = atoi(strNum.c_str()); // Convert the string into an integar

					levelID++; // Increase the maximum level

					_itoa(levelID, buffer, 10); // Convert the level number to a char array
					fputs(buffer, newLevelFile); // Write the new max level to the new file
				}
				else
				{
					ch = fgetc(levelFile); // Get the next character
				}
			}

			if(ch == 'L') // Look for the current level marker
			{
				fputc(ch, newLevelFile); // Copy L to the new file
				
				fileNum.str(""); // Reset the data stream
				ch = fgetc(levelFile); // Retrieve the first digit
				while(ch != '\n' && ch != EOF) // Retrieve all the characters left on the line
				{
					fileNum << ch; // Concatenate each character retrieved into one variable
					ch = fgetc(levelFile); // Retrieve the next character
				}
				strNum = fileNum.str(); // Convert the level digits into a string
				levelID = atoi(strNum.c_str()); // Convert the string into an integar

				if(levelID == levelEditorLevel) // Check if it is the selected level...
				{
					levelFound = true; // Found the level
					ch = fgetc(levelFile); // Get the next character
					while(ch != '\n' && ch != EOF) // Stop at a double and of line
					{
						ch = fgetc(levelFile); // Get the next character
						if(ch == '\n') // If end of line is found...
						{
							ch = fgetc(levelFile); // get another character
						}
					}
					
					_itoa(levelID, buffer, 10); // Convert the level number to a char array
					fputs(buffer, newLevelFile); // Write the new max level to the new file

					// Write the level
					fputc('\n', newLevelFile); // New line

					for(y = 0; y < BGAMEHEIGHT; y++) // Cycle through the level editor map
					{
						for(x = 0; x < BGAMEWIDTH; x++)
						{
							fputc('(', newLevelFile); // Insert a ( before each pair
							
							// Insert x brick number
							_itoa(levelEditorMap[x][y][0], buffer, 10);
							fputs(buffer, newLevelFile);

							fputc(',', newLevelFile); // Insert a , in the middle of each pair

							// Insert y brick number
							_itoa(levelEditorMap[x][y][1], buffer, 10);
							fputs(buffer, newLevelFile);

							fputc(')', newLevelFile); // Insert a ) after each pair
						}
						fputc('\n', newLevelFile);
					}
				}
				else // If it was not the selected level...
				{
					// Write the level number back
					_itoa(levelID, buffer, 10); // Convert the level number to a char array
					fputs(buffer, newLevelFile); // Write the new max level to the new file
				}
			}

			fputc(ch, newLevelFile); // Write the current character to the file

			ch = fgetc(levelFile);
		}

		if(!levelFound) // If the selected level did not already exist
		{
			// Write the level
			fputc('\n', newLevelFile); // New line
			fputc('L', newLevelFile); // Insert L

			// Insert the new levels number
			_itoa(levelEditorLevel, buffer, 10);
			fputs(buffer, newLevelFile);

			fputc('\n', newLevelFile); // New line
			for(y = 0; y < BGAMEHEIGHT; y++)
			{
				for(x = 0; x < BGAMEWIDTH; x++)
				{
					fputc('(', newLevelFile); // Insert a ( before each pair

					// Insert x brick number
					_itoa(levelEditorMap[x][y][0], buffer, 10);
					fputs(buffer, newLevelFile);

					fputc(',', newLevelFile); // Insert a , in the middle of each pair

					// Insert y brick number
					_itoa(levelEditorMap[x][y][1], buffer, 10);
					fputs(buffer, newLevelFile);

					fputc(')', newLevelFile); // Insert a ) after each pair
				}
				fputc('\n', newLevelFile);
			}
		}		

		fclose(levelFile); // Close the level file
		fclose(newLevelFile); // Close the new level file

		remove( "Levels.txt" ); // Delete the old level file
		rename("newLevels.txt" , "Levels.txt"); // Rename the new level file

		SetMaxLevel(); // Retrieves the new max level
	}
}

void RestoreLevels() // Restore the original levels
{
	char ch; // Place holder for each character as it's read from the file.
	FILE *originalFile;  // File to read the level map data from
	FILE *levelFile; // File to write the new level map data to

	originalFile = fopen("OriginalLevels.txt", "r"); // Open OriginalLevels.txt for reading
	levelFile = fopen("Levels.txt", "w"); // Open Levels.txt for writing

	if(originalFile != NULL && levelFile != NULL) // If the files exist
	{
		ch = fgetc(originalFile);
		while(ch != EOF) // Copy the originalFile to the levelFile
		{
			fputc(ch, levelFile);
			ch = fgetc(originalFile);
		}
	}
	SetMaxLevel(); // Retrieves the new max level
	levelEditorLevel = 1; // Set the selected level to 1
	LoadBackground(1); // Load the background
	LoadEditorLevel(levelEditorLevel); // Load the newly selected level
}

// Sound functions

void initSound() // Initialise the sound buffers and the sound queue
{
	int n; // Counter

	soundLoaded = true; // Sound is attempting to load

	// Attempt to load each sound and set sound loaded to false if any sound file fails to load
	
	if(!bufferBrickKO.loadFromFile(SOUND_BRICKKO))
	{
		MessageBox(NULL, "KO Sound Not Loaded", NULL, NULL);
		soundLoaded = false; // Sound failed to load
	}
	if(!bufferBrickRebound.loadFromFile(SOUND_BRICKREBOUND))
	{
		MessageBox(NULL, "Brick Rebound Sound Not Loaded", NULL, NULL);
		soundLoaded = false; // Sound failed to load
	}
	if(!bufferBorderRebound.loadFromFile(SOUND_BORDERREBOUND))
	{
		MessageBox(NULL, "Border Rebound Sound Not Loaded", NULL, NULL);
		soundLoaded = false; // Sound failed to load
	}
	if(!bufferPaddleRebound.loadFromFile(SOUND_PADDLEREBOUND))
	{
		MessageBox(NULL, "Paddle Rebound Sound Not Loaded", NULL, NULL);
		soundLoaded = false; // Sound failed to load
	}
	if(!bufferLoseLife.loadFromFile(SOUND_LOSELIFE))
	{
		MessageBox(NULL, "Lose Life Sound Not Loaded", NULL, NULL);
		soundLoaded = false; // Sound failed to load
	}
	if(!bufferGameOver.loadFromFile(SOUND_GAMEOVER))
	{
		MessageBox(NULL, "Game Over Sound Not Loaded", NULL, NULL);
		soundLoaded = false; // Sound failed to load
	}
	if(!bufferCoin1.loadFromFile("Coin1.WAV"))
	{
		MessageBox(NULL, "Coin1 Sound Not Loaded", NULL, NULL);
		soundLoaded = false; // Sound failed to load
	}
	if(!bufferCoin2.loadFromFile("Coin2.WAV"))
	{
		MessageBox(NULL, "Coin2 Sound Not Loaded", NULL, NULL);
		soundLoaded = false; // Sound failed to load
	}
	if(!bufferCoin3.loadFromFile("Coin3.WAV"))
	{
		MessageBox(NULL, "Coin3 Sound Not Loaded", NULL, NULL);
		soundLoaded = false; // Sound failed to load
	}
	if(!bufferMagnetism.loadFromFile(SOUND_MAGNETISM))
	{
		MessageBox(NULL, "Magnetism Sound Not Loaded", NULL, NULL);
		soundLoaded = false; // Sound failed to load
	}
	if(!bufferPaddleSizeInc.loadFromFile(SOUND_PADDLESIZEINC))
	{
		MessageBox(NULL, "Paddle Size Increase Sound Not Loaded", NULL, NULL);
		soundLoaded = false; // Sound failed to load
	}
	if(!bufferPaddleSizeDec.loadFromFile(SOUND_PADDLESIZEDEC))
	{
		MessageBox(NULL, "Paddle Size Decrease Sound Not Loaded", NULL, NULL);
		soundLoaded = false; // Sound failed to load
	}
	if(!bufferPaddleSpeedInc.loadFromFile(SOUND_PADDLESPEEDINC))
	{
		MessageBox(NULL, "Paddle Speed Increase Sound Not Loaded", NULL, NULL);
		soundLoaded = false; // Sound failed to load
	}
	if(!bufferPaddleSpeedDec.loadFromFile(SOUND_PADDLESPEEDDEC))
	{
		MessageBox(NULL, "Paddle Speed Decrease Sound Not Loaded", NULL, NULL);
		soundLoaded = false; // Sound failed to load
	}
	if(!bufferBallSizeInc.loadFromFile(SOUND_BALLSIZEINC))
	{
		MessageBox(NULL, "Ball Size Increase Sound Not Loaded", NULL, NULL);
		soundLoaded = false; // Sound failed to load
	}
	if(!bufferBallSizeDec.loadFromFile(SOUND_BALLSIZEDEC))
	{
		MessageBox(NULL, "Ball Size Decrease Sound Not Loaded", NULL, NULL);
		soundLoaded = false; // Sound failed to load
	}
	if(!bufferExtraBall.loadFromFile(SOUND_EXTRABALL))
	{
		MessageBox(NULL, "Extra Ball Sound Not Loaded", NULL, NULL);
		soundLoaded = false; // Sound failed to load
	}
	if(!bufferExtraLife.loadFromFile(SOUND_EXTRALIFE))
	{
		MessageBox(NULL, "Extra Life Sound Not Loaded", NULL, NULL);
		soundLoaded = false; // Sound failed to load
	}
	if(!bufferFireball.loadFromFile(SOUND_FIREBALL))
	{
		MessageBox(NULL, "Fireball Sound Not Loaded", NULL, NULL);
		soundLoaded = false; // Sound failed to load
	}
	if(!bufferGuns.loadFromFile(SOUND_GUNS))
	{
		MessageBox(NULL, "Laser Guns Sound Not Loaded", NULL, NULL);
		soundLoaded = false; // Sound failed to load
	}
	if(!bufferExplosive.loadFromFile(SOUND_EXPLOSIVE))
	{
		MessageBox(NULL, "Explosive Ball Sound Not Loaded", NULL, NULL);
		soundLoaded = false; // Sound failed to load
	}
	if(!bufferLaserFire.loadFromFile(SOUND_LASERFIRE))
	{
		MessageBox(NULL, "Laser Fire Sound Not Loaded", NULL, NULL);
		soundLoaded = false; // Sound failed to load
	}

	// Initialise all sound in the queue to null
	n = 0;
	while(n < SOUNDQUEUESIZE)
	{
		soundQueue[n].stop();
		n++;
	}
}

void AddSound(std::string sound) // Create a sound for the given sound buffer and add it to the queue
{
	int n; // Counter

	// Don't add sounds when the sound is turned off
	if(!soundOn)
	{
		return;
	}

	// Find the last free sound position in the queue
	n = 0;
	while(n < SOUNDQUEUESIZE)
	{
		if(soundQueue[n].getStatus() == sf::Sound::Status::Stopped) // Found an empty position
		{
			if(sound == SOUND_BRICKKO)
			{
				soundQueue[n].setBuffer(bufferBrickKO);
			}
			if(sound == SOUND_BRICKREBOUND)
			{
				soundQueue[n].setBuffer(bufferBrickRebound);
			}
			if(sound == SOUND_BORDERREBOUND)
			{
				soundQueue[n].setBuffer(bufferBorderRebound);
			}
			if(sound == SOUND_PADDLEREBOUND)
			{
				soundQueue[n].setBuffer(bufferPaddleRebound);
			}
			if(sound == SOUND_LOSELIFE)
			{
				soundQueue[n].setBuffer(bufferLoseLife);
			}
			if(sound == SOUND_GAMEOVER)
			{
				soundQueue[n].setBuffer(bufferGameOver);
			}
			if(sound == SOUND_COIN)
			{
				switch(rand() % 3)
				{
				case 0:
					soundQueue[n].setBuffer(bufferCoin1);
					break;
				case 1:
					soundQueue[n].setBuffer(bufferCoin2);
					break;
				case 2:
					soundQueue[n].setBuffer(bufferCoin3);
					break;
				}
			}
			if(sound == SOUND_PADDLESIZEINC)
			{
				soundQueue[n].setBuffer(bufferPaddleSizeInc);
			}			
			if(sound == SOUND_PADDLESIZEDEC)
			{
				soundQueue[n].setBuffer(bufferPaddleSizeDec);
			}
			if(sound == SOUND_PADDLESPEEDINC)
			{
				soundQueue[n].setBuffer(bufferPaddleSpeedInc);
			}
			if(sound == SOUND_PADDLESPEEDDEC)
			{
				soundQueue[n].setBuffer(bufferPaddleSpeedDec);
			}
			if(sound == SOUND_BALLSIZEINC)
			{
				soundQueue[n].setBuffer(bufferBallSizeInc);
			}		
			if(sound == SOUND_BALLSIZEDEC)
			{
				soundQueue[n].setBuffer(bufferBallSizeDec);
			}
			if(sound == SOUND_EXTRABALL)
			{
				soundQueue[n].setBuffer(bufferExtraBall);
			}
			if(sound == SOUND_MAGNETISM)
			{
				soundQueue[n].setBuffer(bufferMagnetism);
			}
			if(sound == SOUND_EXTRALIFE)
			{
				soundQueue[n].setBuffer(bufferExtraLife);
			}
			if(sound == SOUND_FIREBALL)
			{
				soundQueue[n].setBuffer(bufferFireball);
			}
			if(sound == SOUND_GUNS)
			{
				soundQueue[n].setBuffer(bufferGuns);
			}
			if(sound == SOUND_EXPLOSIVE)
			{
				soundQueue[n].setBuffer(bufferExplosive);
			}
			if(sound == SOUND_LASERFIRE)
			{
				soundQueue[n].setBuffer(bufferLaserFire);
			}
			soundQueue[n].play();
			break;
		}
		n++;
	}
}

void StopSound() // Turns the sound effects off
{
	int n = 0; // counter

	// Check all the sounds
	while(n < SOUNDQUEUESIZE)
	{
		if(soundQueue[n].getStatus() != sf::Sound::Status::Stopped) // If a sound is playing...
		{
			soundQueue[n].stop(); // Stop the sound
		}
		soundOn = false; // Sound effects are turned off
		n++;
	}
}

void StartSound() // Turns the sound effects on
{
	soundOn = true;
}

void StopMusic() // Turns the music off
{
}

void StartMusic() // Turns the music on
{
}
							
// For testing

// std::ostringstream temp1;
// std::string temp2;

// temp1 << blockID;
// temp2 = temp1.str();
// MessageBox(NULL, temp2.c_str(), NULL, NULL);

