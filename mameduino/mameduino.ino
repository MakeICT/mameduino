/*
	This software is written for the MakeICT Arcade Machine, which uses two
	Arduino Leonardo boards to convert button presses into USB keyboard codes.
	
	It is necessary to use a Leonardo (Micro, Due, or other HID compatible).
*/ 
#include <Keyboard.h>

/**
 * Options! You can modify these things
 **/

// Player 1 or player 2?
#define PLAYER 1

// Which pins are attached to buttons?
#define BUTTON_PINS 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12

// Which of those is connected to the START button?
// Pressing that will also simulate a coin insert
#define START_BUTTON_INDEX 0

// What keyboard codes should be generated when buttons are pressed?
#define PLAYER_1_KEYS 0x31, 0xDA, 0xD9, 0xD8, 0xD7, 0x80, 0x82, 0x20, 0x81, 0x7A, 0x78
#define PLAYER_2_KEYS 0x32, 0x72, 0x66, 0x64, 0x67, 0x61, 0x73, 0x71, 0x77, 0x63, 0x76
//                    START   UP  DOWN  LEFT  RIGHT FIRE1 FIRE2 FIRE3 FIRE4 FIRE5 FIRE6

#define PLAYER_1_MODK 0x00, 0x3D, 0x2D, 0xB3, 0xB3, 0x35, 0x37, 0x35, 0x00, 0x00, 0xB1
//                    NULL  VOL↑  VOL↓  MENU  MENU  COIN  COIN  COIN              ESCAPE     
#define PLAYER_2_MODK 0x00, 0xCA, 0xC9, 0xD1, 0xD1, 0x36, 0x38, 0x36, 0x00, 0x00, 0xB1
//                    NULL  SPD↑  SPD↓  SPD0  SPD0  COIN  COIN  COIN              ESCAPE     

// Which key is used for inserting coins?
#if PLAYER == 1
	#define COIN_KEY 0x35
#else
	#define COIN_KEY 0x36
#endif


// How many buttons are there?
#define BUTTON_COUNT 11

/**
 * Code! You should probably stop modifying things now
 **/
#if PLAYER == 1
	int keys[] = { PLAYER_1_KEYS, PLAYER_1_MODK };
#else
	int keys[] = { PLAYER_2_KEYS, PLAYER_2_MODK };
#endif


int pins[] = { BUTTON_PINS };
int states[2 * BUTTON_COUNT] = { false };

int getCompliementKeyIndex(int keyIndex){
	return (keyIndex + BUTTON_COUNT) % BUTTON_COUNT;
}

void setKeyState(int keyIndex, bool state){
	int key = keys[keyIndex];
	if(key != 0){
		if(state){
			Keyboard.press(key);
		}else{
			Keyboard.release(key);
		}
	}
	states[keyIndex] = state;
}

void setup() {
	Keyboard.begin();
	
	for(int i=0; i<11; i++){
		pinMode(pins[i], INPUT_PULLUP);
	}
}

void loop() {
	for(int buttonIndex=0; buttonIndex<BUTTON_COUNT; buttonIndex++){
		int state = 1-digitalRead(pins[buttonIndex]);
		
		int keyIndex = buttonIndex;
		// Use the START key as a modifier (like a shift key)
		if(states[START_BUTTON_INDEX] && buttonIndex != START_BUTTON_INDEX){
			keyIndex += BUTTON_COUNT;
		}
		
		// did the button state change?
		if(state != states[keyIndex]){
			if(state){
				// when the press the start button, pump a coin in first
				if(buttonIndex == START_BUTTON_INDEX){
					Keyboard.press(COIN_KEY);
					delay(50);
					Keyboard.release(COIN_KEY);
					delay(50);
				}
			}else{
				// when they release a key, also release the modded or unmodded compliment
				int complimentaryKeyIndex = getCompliementKeyIndex(keyIndex);
				if(states[complimentaryKeyIndex]){
					setKeyState(complimentaryKeyIndex, false);
				}
			}
			setKeyState(keyIndex, state);
		}
	}
	delay(3);
}

