/*
 *  easySoundPlayer.cpp
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

#include "easySoundPlayer.h"

//-----------------
void easySoundPlayer::addSound(string filename, string uniqueKey, bool multiplay, bool looping, bool streaming){			
	snds.insert(sndPair(uniqueKey, ofSoundPlayer()));
	snds[uniqueKey].loadSound(filename, streaming);
	snds[uniqueKey].setMultiPlay(multiplay);
	snds[uniqueKey].setLoop(looping);
}

void easySoundPlayer::play(string uniqueKey, float vol, float pitch, float pan){
	if( snds[uniqueKey].isLoaded() == false )return;

	snds[uniqueKey].setPan(pan);	 	
	snds[uniqueKey].play();
	snds[uniqueKey].setVolume(vol);
	snds[uniqueKey].setSpeed(pitch);
	snds[uniqueKey].setPan(pan);	 
}

void easySoundPlayer::stop(string uniqueKey){
	if( snds[uniqueKey].isLoaded() == false )return;	
	snds[uniqueKey].stop();
}	

//-----------------	
void easySoundPlayer::update(){
	ofSoundUpdate();
}
