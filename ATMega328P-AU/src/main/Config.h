/*
 * Config.h
 *
 *  Created on: 26.12.2025
 *      Author: martin
 */

#ifndef CONFIG_H_
#define CONFIG_H_
#include <Arduino.h>

#define VOL_MIN 0
#define VOL_MAX 10
#define SEN_MIN 0
#define SEN_MAX 5
#define UP_MIN 0
#define UP_MAX 400
#define DW_MIN 0
#define DW_MAX 400

class Config {
	uint8_t sensi_;
	uint8_t volume_;
	uint16_t up_100_;
	uint16_t down_100_;

public:
	Config();
	virtual ~Config();
	void save();
	uint8_t getSensi();
	uint8_t getVolume();
	uint16_t getUp100();
	uint16_t getDown100();
	void setSensi(uint8_t sensi);
	void setVolume(uint8_t volume);
	void setUp100(uint16_t up_100);
	void setDown100(uint16_t down_100);
};

#endif /* CONFIG_H_ */
