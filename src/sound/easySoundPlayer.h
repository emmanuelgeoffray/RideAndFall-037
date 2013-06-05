/*
 *  easySoundPlayer.h
 *  ofxFernCameraDemo
 *
 *  Created by theo on 02/02/2010.
 *
 * Copyright Theodore Watson 2010
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */

#pragma once
#include "ofMain.h"

//TODO: should this be a pointer?
typedef std::pair<string, ofSoundPlayer> sndPair;

class easySoundPlayer{
	public:
	
	//-----------------
	void addSound(string filename, string uniqueKey, bool multiplay = false, bool looping = false, bool streaming = false);
	void play(string uniqueKey, float vol = 1.0, float pitch = 1.0, float pan = 0.0);
	void stop(string uniqueKey);
	void update();

	protected:
		
		map <string, ofSoundPlayer> snds;

};