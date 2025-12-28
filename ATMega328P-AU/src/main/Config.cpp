/*
 * Config.cpp
 *
 *  Created on: 26.12.2025
 *      Author: martin
 */

#include "Config.h"
#include <EEPROM.h>

template< typename T >
void readData(T &ref, int addr, int up, int dw){
	T val;
	EEPROM.get(addr, val);
	if(val >= up && val <= dw) {
		ref = val;
	}
}

template< typename T >
void writeData(T &ref, int addr, int up, int dw){
	T val;
	EEPROM.get(addr, val);
	if(val != ref && ref >= up && ref <= dw) {
		EEPROM.put(addr, ref);
	}
}


Config::Config() : volume_(3), sensi_(0x03), up_100_(25), down_100_(100) {
	readData(volume_,   0, VOL_MIN, VOL_MAX);
	readData(sensi_,    1, SEN_MIN, SEN_MAX);
	readData(up_100_,   2, UP_MIN,  UP_MAX);
	readData(down_100_, 4, DW_MIN,  DW_MAX);
}


Config::~Config() {
}


void Config::save() {
	writeData(volume_,   0, VOL_MIN, VOL_MAX);
	writeData(sensi_,    1, SEN_MIN, SEN_MAX);
	writeData(up_100_,   2, UP_MIN,  UP_MAX);
	writeData(down_100_, 4, DW_MIN,  DW_MAX);
}

uint8_t Config::getSensi(){
	return sensi_;
}
uint8_t Config::getVolume(){
	return volume_;
}
uint16_t Config::getUp100(){
	return up_100_;
}
uint16_t Config::getDown100(){
	return down_100_;
}
void Config::setSensi(uint8_t sensi){
	sensi_= sensi;
}
void Config::setVolume(uint8_t volume){
	volume_ = volume;
}
void Config::setUp100(uint16_t up_100){
	up_100_ = up_100;
}
void Config::setDown100(uint16_t down_100){
	down_100_ = down_100;
}
