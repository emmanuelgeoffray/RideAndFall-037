/*
 *  ofxFern.h
 *  
 *
 *  Created by theo on 16/12/2009.
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

#include "ofxOpenCv.h"

#include "mcv.h"
#include "planar_pattern_detector_builder.h"
#include "template_matching_based_tracker.h"

class ofxFern{
	public:
		
		ofxFern();
		~ofxFern();
		
		void initTracker(string marker, int width, int height, bool bDrawTracking = false);
		void update(ofxCvGrayscaleImage & grayImg );

		void draw(float x, float y, float w, float h);
		void drawOverlay(float x, float y, float w, float h);
		
		void setMode(int key);
		
		vector <ofPoint> getTrackedCorners(){
			return pts;
		}
	
		bool last_frame_ok;

		
	protected:
	
		bool bDrawTrackingInfo;
				
		void draw_quadrangle(IplImage * frame, int u0, int v0, int u1, int v1, int u2, int v2, int u3, int v3, CvScalar color, int thickness = 1);
		void draw_detected_position(IplImage * frame, planar_pattern_detector * detector);
		void draw_initial_rectangle(IplImage * frame, template_matching_based_tracker * tracker);
		void draw_tracked_position(IplImage * frame, template_matching_based_tracker * tracker);
		void draw_tracked_locations(IplImage * frame, template_matching_based_tracker * tracker);
		void draw_detected_keypoints(IplImage * frame, planar_pattern_detector * detector);
		void draw_recognized_keypoints(IplImage * frame, planar_pattern_detector * detector);

		IplImage * detect(IplImage * frame);

		vector <ofPoint> pts;
		
		ofxCvGrayscaleImage img;

		int mode;
		bool show_tracked_locations;
		bool show_keypoints;
		planar_pattern_detector * detector;
		template_matching_based_tracker * tracker;

		
};