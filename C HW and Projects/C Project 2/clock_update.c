#include "clock.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern int TIME_OF_DAY_SEC;
// Time of day in seconds. Tied to a hardware clock that automatically
// increments it 1 time per second starting at midnight. The
// simulatator provides a C global variable to emulate this part of
// the hardware. This variable is present when #include "clock.h" is
// used and should not be declared in any user code.

extern int32_t CLOCK_DISPLAY_PORT;
// Global variable used to control the LCD display on the
// clock. Making changes to this variable will change the clock
// time. Type ensures 32 bits.

int set_tod_from_secs(int time_of_day_sec, tod_t *tod){
	//check for out of bounds values
	if(time_of_day_sec < 0 || time_of_day_sec > 86400){
		return 1;
	}
	int seconds = time_of_day_sec % 60;
	//minutes left after seconds are removed
	int mins_left = (time_of_day_sec - seconds)/ 60; 
	int minutes =	(mins_left)%60;
	//hours left after seconds and minutes are removed
	int hours_left = (mins_left -minutes)/60;
	int hours = hours_left % 60;
	char ispm = 0;
	//if hours is 12, change ispm to 1
	if(hours == 12){
		ispm = 1;
	}
	//if hours is 0, this means it is 12am, so change hours to 12
	if(hours == 0){
		hours = 12;
	}
	//if hours is greater than 12, subtract 12 so that it 
	//is representing nonmilitary time
	if(hours > 12){
		hours -=12;
		ispm = 1;
	}
	//set fields of tod
	tod->hours = (short)hours;
	tod->minutes = (short)minutes;
	tod->seconds = (short)seconds;
	tod->ispm = ispm;
	return 0;
}

int set_display_bits_from_tod(tod_t tod, int *display){
	//check for out of bounds values in tod parameter
	if(tod.hours > 12 || tod.minutes > 59 || tod.seconds > 59){
		return 1;
	}
	//bits is an array that holds numbers that have the bit pattern
	//associated with printing the clock numbers (bit masks)
	int bits[] = {63,6,91,79,102,109,125,7,255,111,0};

	//get the bit pattern assoicated with digit
	int start_min_bit = bits[tod.minutes%10];
	int second_min_bit = bits[(tod.minutes-(tod.minutes%10))/10] << 7; //shift by 7
	int start_hour_bit = bits[tod.hours%10] << 14; //shift by 14
	int second_hour_bit;

	if((tod.hours - (tod.hours%10))/10 == 0){
		second_hour_bit = bits[10] << 21; //shift by 21
	}
	else{
		second_hour_bit = bits[(tod.hours - (tod.hours%10))/10] << 21; //shift by 21
	}
	int pmon;
	char a = 0;
	if(strcmp(&tod.ispm, &a) == 0){
		pmon = 1 << 28; //shift by 28
	}
	else{
		pmon = 2 <<28; //shift by 28
	}
	int disp = pmon | (second_hour_bit | (start_hour_bit | (second_min_bit | start_min_bit)));
	*display = disp; //dereference/set of the integer pointed to by the display parameter
	return 0;
}

int clock_update(){
	tod_t tod_y = {
		.hours = 0,
		.minutes = 0,
		.seconds = 0,
		.ispm = 0
	};
	tod_t *tod = &tod_y;
	//error checking on function return values	
	int result = set_tod_from_secs(TIME_OF_DAY_SEC, tod);
	if(result == 1){
		return 1;
	}
	int d = 0;
	int *display = &d;
	int res = set_display_bits_from_tod(*tod, display);
	//error checking on function return values
	if(res == 1){
		return 1;
	}
	CLOCK_DISPLAY_PORT = *display;
	return 0;
}
// Examines the TIME_OF_DAY_SEC global variable to determine hour,
// minute, and am/pm.  Sets the global variable CLOCK_DISPLAY_PORT bits
// to show the proper time.  If TIME_OF_DAY_SEC appears to be in error
// (to large/small) makes no change to CLOCK_DISPLAY_PORT and returns 1
// to indicate an error. Otherwise returns 0 to indicate success.
//
// Makes use of the set_tod_from_secs() and
// set_display_bits_from_tod() functions.
//
// CONSTRAINT: Does not allocate any heap memory as malloc() is NOT
// available on the target microcontroller.  Uses stack and global
// memory only.
