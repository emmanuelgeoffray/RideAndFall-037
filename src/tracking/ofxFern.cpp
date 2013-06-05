/*
 *  ofxFern.cpp
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

#include "ofxFern.h"

//------------------------------------------------------------------------
ofxFern::ofxFern(){
	detector = NULL;
	tracker  = NULL;
	mode = 2;
	show_tracked_locations = true;
	show_keypoints = true;
	
	pts.assign(4, ofPoint());
}

//------------------------------------------------------------------------
ofxFern::~ofxFern(){
	if( detector != NULL){
		delete detector;
		detector = NULL;
	}
	if( tracker != NULL){
		delete tracker;
		tracker = NULL;
	}
}

//------------------------------------------------------------------------
void ofxFern::draw_quadrangle(IplImage * frame, int u0, int v0, int u1, int v1, int u2, int v2, int u3, int v3, CvScalar color, int thickness){
  cvLine(frame, cvPoint(u0, v0), cvPoint(u1, v1), color, thickness);
  cvLine(frame, cvPoint(u1, v1), cvPoint(u2, v2), color, thickness);
  cvLine(frame, cvPoint(u2, v2), cvPoint(u3, v3), color, thickness);
  cvLine(frame, cvPoint(u3, v3), cvPoint(u0, v0), color, thickness);
}

//------------------------------------------------------------------------
void ofxFern::draw_detected_position(IplImage * frame, planar_pattern_detector * detector){
	draw_quadrangle(frame,
		  detector->detected_u_corner[0], detector->detected_v_corner[0],
		  detector->detected_u_corner[1], detector->detected_v_corner[1],
		  detector->detected_u_corner[2], detector->detected_v_corner[2],
		  detector->detected_u_corner[3], detector->detected_v_corner[3],
		  cvScalar(255), 3);
}

//------------------------------------------------------------------------
void ofxFern::draw_initial_rectangle(IplImage * frame, template_matching_based_tracker * tracker){
  draw_quadrangle(frame,
		  tracker->u0[0], tracker->u0[1],
		  tracker->u0[2], tracker->u0[3],
		  tracker->u0[4], tracker->u0[5],
		  tracker->u0[6], tracker->u0[7],
		  cvScalar(128), 3);
}

//------------------------------------------------------------------------
void ofxFern::draw_tracked_position(IplImage * frame, template_matching_based_tracker * tracker){
  draw_quadrangle(frame,
		  tracker->u[0], tracker->u[1],
		  tracker->u[2], tracker->u[3],
		  tracker->u[4], tracker->u[5],
		  tracker->u[6], tracker->u[7],
		  cvScalar(255), 3);
}

//------------------------------------------------------------------------
void ofxFern::draw_tracked_locations(IplImage * frame, template_matching_based_tracker * tracker){
  for(int i = 0; i < tracker->nx * tracker->ny; i++) {
    int x1, y1;
    tracker->f.transform_point(tracker->m[2 * i], tracker->m[2 * i + 1], x1, y1);
    cvCircle(frame, cvPoint(x1, y1), 3, cvScalar(255, 255, 255), 1);
  }
}

//------------------------------------------------------------------------
void ofxFern::draw_detected_keypoints(IplImage * frame, planar_pattern_detector * detector){
  for(int i = 0; i < detector->number_of_detected_points; i++)
    cvCircle(frame,
	     cvPoint(detector->detected_points[i].fr_u(),
		     detector->detected_points[i].fr_v()),
	     16 * (1 << int(detector->detected_points[i].scale)),
	     cvScalar(100), 1);
}

//------------------------------------------------------------------------
void ofxFern::draw_recognized_keypoints(IplImage * frame, planar_pattern_detector * detector){
  for(int i = 0; i < detector->number_of_model_points; i++)
    if (detector->model_points[i].class_score > 0)
      cvCircle(frame,
		cvPoint(detector->model_points[i].potential_correspondent->fr_u(),
		       detector->model_points[i].potential_correspondent->fr_v()),
	       16 * (1 << int(detector->detected_points[i].scale)),
	       cvScalar(255, 255, 255), 1);
}

//------------------------------------------------------------------------
IplImage * ofxFern::detect(IplImage * frame){
	last_frame_ok=false;

	if (mode == 1 || ((mode==0) && last_frame_ok)) {
		bool ok = tracker->track(frame);
		last_frame_ok=ok;
		//printf("last_frame_ok=%i ?????????;\n", last_frame_ok);

		if (!ok) {
			if (mode==0) return detect(frame);
			else {
				if(bDrawTrackingInfo)draw_initial_rectangle(frame, tracker);
				tracker->initialize();
			}
		} else {
			if( bDrawTrackingInfo ){
				draw_tracked_position(frame, tracker);
				if (show_tracked_locations) draw_tracked_locations(frame, tracker);
			}
		}
		
		if( last_frame_ok ){
			//theo update our pts. 
			pts[0].set(tracker->u[0], tracker->u[1]);
			pts[1].set(tracker->u[2], tracker->u[3]);
			pts[2].set(tracker->u[4], tracker->u[5]);
			pts[3].set(tracker->u[6], tracker->u[7]);
		}
		
		//cvPutText(frame, "template-based 3D tracking", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
	} else {
		detector->detect(frame);

		if (detector->pattern_is_detected) {
			last_frame_ok=true;
			//printf("last_frame_ok=true;\n");

			tracker->initialize(detector->detected_u_corner[0], detector->detected_v_corner[0],
					detector->detected_u_corner[1], detector->detected_v_corner[1],
					detector->detected_u_corner[2], detector->detected_v_corner[2],
					detector->detected_u_corner[3], detector->detected_v_corner[3]);

			if (mode == 3 && tracker->track(frame)) {
				if( bDrawTrackingInfo ){
					if (show_keypoints) {
						draw_detected_keypoints(frame, detector);
						draw_recognized_keypoints(frame, detector);
					}
					draw_tracked_position(frame, tracker);
					if (show_tracked_locations) draw_tracked_locations(frame, tracker);

					;//cvPutText(frame, "detection+template-based 3D tracking", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
				}
			} else {
				if( bDrawTrackingInfo ){
					if (show_keypoints) {
						draw_detected_keypoints(frame, detector);
						draw_recognized_keypoints(frame, detector);
					}
					draw_detected_position(frame, detector);
					;//cvPutText(frame, "detection", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
				}
			}
			
			if( last_frame_ok ){
				//theo update our pts. 
				pts[0].set(tracker->u[0], tracker->u[1]);
				pts[1].set(tracker->u[2], tracker->u[3]);
				pts[2].set(tracker->u[4], tracker->u[5]);
				pts[3].set(tracker->u[6], tracker->u[7]);
			}
			
		} else {
			last_frame_ok=false;
			if (show_keypoints && bDrawTrackingInfo) draw_detected_keypoints(frame, detector);

			if (mode == 3)
				;//cvPutText(frame, "detection + template-based 3D tracking", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
			else
				;//cvPutText(frame, "detection", cvPoint(10, 30), &font, cvScalar(255, 255, 255));
		}
	}
	



	//cvShowImage("ferns-demo", frame);
}

//------------------------------------------------------------------------
void ofxFern::initTracker(string marker, int width, int height, bool drawTracking){

	bDrawTrackingInfo = drawTracking;

	img.allocate(width, height); 
	marker = ofToDataPath(marker);

	int mode = 2;
	bool show_tracked_locations = true;
	bool show_keypoints = true;

	string model_image			= marker;
	string sequence_format		= "";
	string video_file			= "";

	//TODO: check these values - the 10000 could br brought down maybe 5000 too?
	affine_transformation_range range;
//	detector = planar_pattern_detector_builder::build_with_cache(model_image.c_str(), &range,
//				 200, 2000, 0.0,32, 7, 4, 30, 12, 3000, 200);

	detector = planar_pattern_detector_builder::build_with_cache(model_image.c_str(), &range,
				 400, 5000, 0.0,32, 7, 4,30, 12, 10000, 200);

	setMode('5');

	if (!detector) {
	cerr << "Unable to build detector.\n";
	return ;
	}

	detector->set_maximum_number_of_points_to_detect(1000);

	tracker = new template_matching_based_tracker();
	string trackerfn = model_image + string(".tracker_data");
	if (!tracker->load(trackerfn.c_str())) {
	cout << "Training template matching..."<<endl;
	tracker->learn(detector->model_image,
		   5, // number of used matrices (coarse-to-fine)
		   80, // max motion in pixel used to train to coarser matrix
		   20, 20, // defines a grid. Each cell will have one tracked point.
		   detector->u_corner[0], detector->v_corner[1],
		   detector->u_corner[2], detector->v_corner[2],
		   40, 40, // neighbordhood for local maxima selection
		   10000 // number of training samples
		   );
	tracker->save(trackerfn.c_str());
	}

	printf("fern - initializing tracker\n");
	tracker->initialize();
	printf("fern - tracker initialized\n");

}

//------------------------------------------------------------------------
void ofxFern::update(ofxCvGrayscaleImage & grayImg ){
	if( grayImg.getWidth() != img.getWidth() ||  grayImg.getHeight() != img.getHeight() ){
		printf("ofxFern::update() image sizes do not match! Not going to process anything!\n");
		return;
	}
	
    img = grayImg;
	detect(img.getCvImage());
	img.flagImageChanged();
}

//------------------------------------------------------------------------
void ofxFern::draw(float x, float y, float w, float h){
	ofPushStyle();    
		ofSetColor(255, 255, 255, 255);
		img.draw(x,y,w,h);
		for(int i = 0; i < pts.size(); i++){
			ofSetColor(0xFF00FF);
			ofCircle(x + pts[i].x * w/(float)img.width, y + pts[i].y * h/(float)img.height, 10);
		}
	ofPopStyle();	
}

//------------------------------------------------------------------------
void ofxFern::drawOverlay(float x, float y, float w, float h){
	ofPushStyle();
		for(int i = 0; i < pts.size(); i++){
			ofSetColor(0xFF00FF);
			ofCircle(x + pts[i].x * w/(float)img.width, y + pts[i].y * h/(float)img.height, 10);
		}
	ofPopStyle();
}

//------------------------------------------------------------------------
//just for now - will be removed
void ofxFern::setMode(int modeIn ){

    if (modeIn >= 0) {
		switch(char(modeIn)) {
			case '0': mode = 0; break;
			case '1': mode = 1; break;
			case '2': mode = 2; break;
			case '3': mode = 3; break;
			case '4': show_tracked_locations = !show_tracked_locations; break;
			case '5': show_keypoints = !show_keypoints; break;
			default: ;
		}
		cout << "mode=" << mode << endl;
    }

}















