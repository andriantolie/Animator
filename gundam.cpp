// The Gundam model.
//Author:	LIE, Andrianto (alie@connect.ust.hk) - 20020408
//			ZENG, Xiong (zxengac@connect.ust.hk) - 20025056


#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include <FL/gl.h>
#include <math.h>
#include "modelerglobals.h"
#include "camera.h"
#include <iostream>
#include "particlesystem.h"
using namespace std;

#define PI 3.14159265

Mat4f getModelViewMatrix();
void SpawnParticles(Mat4f CameraTransforms);


//draw bezier curve and rotate it around specific axis
void drawRotatingCurve(float controlPoints[][3], double xRotationAxis, double yRotationAxis, double zRotationAxis){
	ModelerDrawState *mds = ModelerDrawState::Instance();
	switch (mds->m_drawMode)
	{
	case NORMAL:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel(GL_SMOOTH);
		break;
	case FLATSHADE:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel(GL_FLAT);
		break;
	case WIREFRAME:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glShadeModel(GL_FLAT);
	default:
		break;
	}

	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &controlPoints[0][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glColor3f(1.0, 0.0, 0.0);
	setDiffuseColor(COLOR_YELLOW);
	for (double angle = 0; angle < 360; angle += 0.1){
		glRotated(angle, xRotationAxis, yRotationAxis, zRotationAxis);
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i <= 30; i++){
			glEvalCoord1f((GLfloat)i / 30.0);
		}
		glEnd();
	}
	glFlush();
	glDisable(GL_MAP1_VERTEX_3);
};


// To make a GundamModel, we inherit off of ModelerView
class GundamModel : public ModelerView
{
public:
	GundamModel(int x, int y, int w, int h, char *label);

	virtual void draw();

private:
	int iterator;
	double upperBodySize[3];
	double lowerBodySize[3];
	double headSize[3];
	double hammerSize[3];

	double rightShoulderSize[3];
	double rightUpperArmSize[3];
	double rightLowerArmSize[3];
	double rightFistSize[3];
	double leftShoulderSize[3];
	double leftUpperArmSize[3];
	double leftLowerArmSize[3];
	double leftFistSize[3];

	double rightThighSize[3];
	double rightUpperLegSize[3];
	double rightLowerLegSize[3];
	double rightFootSize[3];
	double leftThighSize[3];
	double leftUpperLegSize[3];
	double leftLowerLegSize[3];
	double leftFootSize[3];

	double gundamHeight;

	int rightShoulderAngle;
	int rightUpperArmAngle;
	int rightLowerArmAngle;
	int leftShoulderAngle;
	int leftUpperArmAngle;
	int leftLowerArmAngle;
	int thighCrouchAngle;
	int legCrouchAngle;
	int animRightThigh;
	int animRightLowerLeg;
	int animLeftLowerLeg;
	int animLeftThigh;
	int animUpperBody;

	void drawUpperBody();
	void drawUpperBody2();
	void drawLowerBody();
	void drawLowerBody2();
	void drawHead();
	void drawHead2();
	void drawHammer();

	void drawRightShoulder();
	void drawRightShoulder2();
	void drawRightUpperArm();
	void drawRightLowerArm();
	void drawRightLowerArm2();
	void drawRightFist();

	void drawLeftShoulder();
	void drawLeftShoulder2();
	void drawLeftUpperArm();
	void drawLeftLowerArm();
	void drawLeftLowerArm2();
	void drawLeftFist();

	void drawRightThigh();
	void drawRightThigh2();
	void drawRightUpperLeg();
	void drawRightLowerLeg();
	void drawRightLowerLeg2();
	void drawRightFoot();

	void drawLeftThigh();
	void drawLeftThigh2();
	void drawLeftUpperLeg();
	void drawLeftLowerLeg();
	void drawLeftLowerLeg2();
	void drawLeftFoot();

	void animationIterator();
};

//[0] is x-axis, [1] is y-axis, [2] is z-axis
GundamModel::GundamModel(int x, int y, int w, int h, char *label)
: ModelerView(x, y, w, h, label) {

	iterator = 0;

	upperBodySize[0] = 6;
	upperBodySize[1] = 4;
	upperBodySize[2] = 3;

	lowerBodySize[0] = 1.5;
	lowerBodySize[1] = 2.5;
	lowerBodySize[2] = 2;

	headSize[0] = 2;
	headSize[1] = 2;
	headSize[2] = 2;

	hammerSize[0] = 2;
	hammerSize[1] = 2;
	hammerSize[2] = 2;

	rightShoulderSize[0] = 1.5;
	rightShoulderSize[1] = 2;
	rightShoulderSize[2] = 2;

	rightUpperArmSize[0] = 1;
	rightUpperArmSize[1] = 1.5;
	rightUpperArmSize[2] = 1;

	rightLowerArmSize[0] = 1.5;
	rightLowerArmSize[1] = 2.5;
	rightLowerArmSize[2] = 2;

	rightFistSize[0] = 1;
	rightFistSize[1] = 0.75;
	rightFistSize[2] = 1;

	leftShoulderSize[0] = 1.5;
	leftShoulderSize[1] = 2;
	leftShoulderSize[2] = 2;

	leftUpperArmSize[0] = 1;
	leftUpperArmSize[1] = 1.5;
	leftUpperArmSize[2] = 1;

	leftLowerArmSize[0] = 1.5;
	leftLowerArmSize[1] = 2.5;
	leftLowerArmSize[2] = 2;

	leftFistSize[0] = 1;
	leftFistSize[1] = 0.75;
	leftFistSize[2] = 1;

	rightThighSize[0] = 1.75;
	rightThighSize[1] = 3.5;
	rightThighSize[2] = 2;

	rightUpperLegSize[0] = 1;
	rightUpperLegSize[1] = 1.5;
	rightUpperLegSize[2] = 1;

	rightLowerLegSize[0] = 1.5;
	rightLowerLegSize[1] = 2.5;
	rightLowerLegSize[2] = 1.5;

	rightFootSize[0] = 1.75;
	rightFootSize[1] = 0.5;
	rightFootSize[2] = 2;

	leftThighSize[0] = 1.75;
	leftThighSize[1] = 3.5;
	leftThighSize[2] = 2;

	leftUpperLegSize[0] = 1;
	leftUpperLegSize[1] = 1.5;
	leftUpperLegSize[2] = 1;

	leftLowerLegSize[0] = 1.5;
	leftLowerLegSize[1] = 2.5;
	leftLowerLegSize[2] = 1.75;

	leftFootSize[0] = 1.75;
	leftFootSize[1] = 0.5;
	leftFootSize[2] = 2;

	gundamHeight = headSize[1] + headSize[1] / 6 + upperBodySize[1] + lowerBodySize[1] / 3
		+ max(rightThighSize[1], leftThighSize[1])+ max(rightUpperLegSize[1], leftUpperLegSize[1]) +
		max(rightLowerLegSize[1],leftLowerLegSize[1]) + max(rightFootSize[1], leftFootSize[1]);
}

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createGundamModel(int x, int y, int w, int h, char *label)
{
	return new GundamModel(x, y, w, h, label);
}

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out GundamModel
void GundamModel::draw()
{

	//m_camera->frameAll();
	// This call takes care of a lot of the nasty projection 
	// matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...
	ModelerView::draw();

	//Set the lighting
	GLfloat lightColor[] = { VAL(LIGHT_INTENSITY) / 100.0, VAL(LIGHT_INTENSITY) / 100.0, VAL(LIGHT_INTENSITY) / 100.0, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor);
	setAmbientColor(1.0f, 1.0f, 1.0f);
	setDiffuseColor(COLOR_YELLOW);
	//	setSpecularColor(0.8f, 0.5f, 0.0f);

	// Get the camera matrix
	Mat4f CameraMatrix = getModelViewMatrix();

	// Start drawing the Gundam model
	glPushMatrix();
	glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));

	
	//draw Upper Body
	if (VAL(CROUCH))
		glTranslated(0, -(1 - cos(PI*VAL(CROUCH) * 2 / 180))*leftThighSize[1], 0);
	glRotated(VAL(ROTATE), 0.0, 1.0, 0.0);
	glRotated(VAL(ROTATE_UPPER_BODY), 0.0, 1.0, 0.0);

	VAL(UPPERBODY2) ? drawUpperBody2() : drawUpperBody();

	if (VAL(DETAIL) >= 1){
		//draw head
		glPushMatrix();
		glTranslated(0, upperBodySize[1], 0);
		if (VAL(ROTATE_HEAD_X))
			glRotated(VAL(ROTATE_HEAD_X), 1.0, 0.0, 0.0);
		if (VAL(ROTATE_HEAD_Y))
			glRotated(VAL(ROTATE_HEAD_Y), 0.0, 1.0, 0.0);
		if (VAL(ROTATE_HEAD_Z))
			glRotated(VAL(ROTATE_HEAD_Z), 0.0, 0.0, 1.0);
		VAL(HEAD2) ? drawHead2() : drawHead();
		glPushMatrix();
		glTranslated(headSize[0] / 2, headSize[1], 0.0);
		SpawnParticles(CameraMatrix);
		glPopMatrix();
		glTranslated(0.0, headSize[1] + headSize[1] / 6, 0.0);
		glPopMatrix();

		//draw right arm
		rightShoulderAngle = VAL(RIGHT_ARM_LINK_MOVEMENT);
		rightUpperArmAngle = VAL(RIGHT_ARM_LINK_MOVEMENT);
		rightLowerArmAngle = 1.5*VAL(RIGHT_ARM_LINK_MOVEMENT);
		glPushMatrix();
		//draw right shoulder
		glTranslated(upperBodySize[0] / 2, upperBodySize[1] - rightShoulderSize[0] / 2, 0);
		glRotated(-90, 0.0, 0.0, 1.0);
		glRotated(-(VAL(RAISE_RIGHT_ARM_X)), 0.0, 1.0, 0.0);
		glRotated(-rightShoulderAngle, 0.0, 1.0, 0.0); // For link movement
		glRotated(VAL(RAISE_RIGHT_ARM_Z), 0.0, 0.0, 1.0);
		VAL(SHOULDER2) ? drawRightShoulder2() : drawRightShoulder();
		glPushMatrix();
		glTranslated(-rightShoulderSize[0] / 2, rightShoulderSize[1]/2, 0.0);
		SpawnParticles(CameraMatrix);
		glPopMatrix();
		//draw right upper arm
		if (VAL(DETAIL) >= 2){
			glTranslated(rightShoulderSize[0] / 2, rightShoulderSize[1] / 2, 0);
			if (VAL(RIGHT_FOREARM_ROTATION))
				glRotated(VAL(RIGHT_FOREARM_ROTATION), 1.0, 0.0, 0.0);
			glRotated(rightUpperArmAngle, 1.0, 0.0, 0.0); // For link movement
			glRotated(-90, 0.0, 0.0, 1.0);
			drawRightUpperArm();
			//draw right lower arm
			if (VAL(DETAIL) >= 3){
				glTranslated(0.0, rightUpperArmSize[1], 0.0);
				glRotated(rightLowerArmAngle, 1.0, 0.0, 0.0); // For link movement
				VAL(LOWERARM2) ? drawRightLowerArm2() : drawRightLowerArm();
				//draw right Fist
				if (VAL(DETAIL) >= 4){
					glTranslated(0.0, rightLowerArmSize[1], 0.0);
					drawRightFist();
					if (VAL(DETAIL) >= 5){
						if (VAL(HAMMER)){
							glTranslated(0.0, rightFistSize[1], 0.0);
							glRotated(90, 1.0, 0.0, 0.0);
							drawHammer();
						}
					}
				}
			}
		}
		glPopMatrix();

		//draw left arm
		leftShoulderAngle = VAL(LEFT_ARM_LINK_MOVEMENT);
		leftUpperArmAngle = VAL(LEFT_ARM_LINK_MOVEMENT);
		leftLowerArmAngle = 1.5*VAL(LEFT_ARM_LINK_MOVEMENT);
		glPushMatrix();
		//draw left shoulder
		glTranslated(-upperBodySize[0] / 2, upperBodySize[1] - leftShoulderSize[0] / 2, 0);
		glRotated(90, 0.0, 0.0, 1.0);
		glRotated(VAL(RAISE_LEFT_ARM_X), 0.0, 1.0, 0.0);
		glRotated(leftShoulderAngle, 0.0, 1.0, 0.0); // For link movement
		glRotated(-VAL(RAISE_LEFT_ARM_Z), 0.0, 0.0, 1.0);
		VAL(SHOULDER2) ? drawLeftShoulder2() : drawLeftShoulder();
		glPushMatrix();
		glTranslated(leftShoulderSize[0] / 2, leftShoulderSize[1] / 2, 0.0);
		SpawnParticles(CameraMatrix);
		glPopMatrix();
		//draw left upper arm
		if (VAL(DETAIL) >= 2){
			glTranslated(-leftShoulderSize[0] / 2, leftShoulderSize[1] / 2, 0);
			if (VAL(LEFT_FOREARM_ROTATION))
				glRotated(VAL(LEFT_FOREARM_ROTATION), 1.0, 0.0, 0.0);
			glRotated(leftUpperArmAngle, 1.0, 0.0, 0.0); // For link movement
			glRotated(90, 0.0, 0.0, 1.0);
			drawLeftUpperArm();
			//draw left lower arm
			if (VAL(DETAIL) >= 3){
				glTranslated(0.0, leftUpperArmSize[1], 0.0);
				glRotated(leftLowerArmAngle, 1.0, 0.0, 0.0); // For link movement
				VAL(LOWERARM2) ? drawLeftLowerArm2() : drawLeftLowerArm();
				//draw left Fist
				if (VAL(DETAIL) >= 4){
					glTranslated(0.0, leftLowerArmSize[1], 0.0);
					drawLeftFist();
				}
			}

		}
		glPopMatrix();


		//draw lower body
		glRotated(-(VAL(ROTATE_UPPER_BODY)), 0.0, 1.0, 0.0); // To fix the position of lower body
		glPushMatrix();
		glRotated(180, 0.0, 0.0, 1.0);
		VAL(LOWERBODY2) ? drawLowerBody2() : drawLowerBody();
		glPopMatrix();

		
		if (VAL(DETAIL) >= 2){
			//draw right leg
			glPushMatrix();
			//draw right thigh
			glRotated(180, 0.0, 0.0, 1.0);
			glTranslated(-lowerBodySize[0] / 2, lowerBodySize[1] / 4, 0.0);
			glTranslated(-rightThighSize[0] / 2, 0.0, 0.0);
			if (VAL(CROUCH)) {
				thighCrouchAngle = 2 * VAL(CROUCH);
				glRotated(thighCrouchAngle, 1.0, 0.0, 0.0);
			}
			glRotated(VAL(RAISE_RIGHT_LEG_X), 1.0, 0.0, 0.0);
			glRotated(VAL(RAISE_RIGHT_LEG_Z), 0.0, 0.0, 1.0);
			VAL(THIGH2) ? drawRightThigh2() : drawRightThigh();
			//draw right upper leg
			if (VAL(DETAIL) >= 3){
				glTranslated(0.0, rightThighSize[1], 0.0);
				if (VAL(CROUCH)) {
					legCrouchAngle = 5 * VAL(CROUCH);
					glRotated(-legCrouchAngle, 1.0, 0.0, 0.0);
				}
				drawRightUpperLeg();
				//draw right lower leg
				if (VAL(DETAIL) >= 4){
					glTranslated(0.0, rightUpperLegSize[1], 0.0);

					VAL(LOWERLEG2) ? drawRightLowerLeg2() : drawRightLowerLeg();
					//draw right foot
					if (VAL(DETAIL) >= 5){
						glTranslated(0.0, rightLowerLegSize[1], 0.0);
						drawRightFoot();
					}
				}
			}
			glPopMatrix();

			//draw left leg
			glPushMatrix();
			//draw left thigh
			glRotated(180, 0.0, 0.0, 1.0);
			glTranslated(lowerBodySize[0] / 2, lowerBodySize[1] / 4, 0.0);
			glTranslated(leftThighSize[0] / 2, 0.0, 0.0);
			if (VAL(CROUCH)) {
				glRotated(thighCrouchAngle, 1.0, 0.0, 0.0);
			}
			glRotated(VAL(RAISE_LEFT_LEG_X), 1.0, 0.0, 0.0);
			glRotated(-VAL(RAISE_LEFT_LEG_Z), 0.0, 0.0, 1.0);
			VAL(THIGH2) ? drawLeftThigh2() : drawLeftThigh();
			//draw left upper leg
			if (VAL(DETAIL) >= 3){
				glTranslated(0.0, leftThighSize[1], 0.0);
				if (VAL(CROUCH)) {
					glRotated(-legCrouchAngle, 1.0, 0.0, 0.0);
				}
				drawLeftUpperLeg();
				//draw left lower leg
				if (VAL(DETAIL) >= 4){
					glTranslated(0.0, leftUpperLegSize[1], 0.0);
					VAL(LOWERLEG2) ? drawLeftLowerLeg2() : drawLeftLowerLeg();
					//draw left foot
					if (VAL(DETAIL) >= 5){
						glTranslated(0.0, leftLowerLegSize[1], 0.0);
						drawLeftFoot();
					}
				}
			}
			glPopMatrix();
		}
	}
	glPopMatrix();

}

//OpenGl command to draw upper body
void GundamModel::drawUpperBody(){
	glPushMatrix();
		glTranslated(-upperBodySize[0]/2, 0, -upperBodySize[2]/2);
		glScaled(upperBodySize[0], upperBodySize[1], upperBodySize[2]);
		if (VAL(TEXTURE_MAPPING))
			drawTextureBox(1, 1, 1);
		else
			drawBox(1, 1, 1);
	glPopMatrix();
}

//OpenGl command to draw upper body
void GundamModel::drawUpperBody2(){
	glPushMatrix();
	glRotated(-90, 1.0, 0.0, 0.0);
	drawCylinder(upperBodySize[1], upperBodySize[2]/2, upperBodySize[0]/2);
	glPopMatrix();
}

//OpenGl command to draw lower body
//TODO
void GundamModel::drawLowerBody(){
	glPushMatrix();
	setDiffuseColor(COLOR_YELLOW);
	glTranslated(-lowerBodySize[0] / 2, 0, -lowerBodySize[2] / 2);
	glScaled(lowerBodySize[0], lowerBodySize[1], lowerBodySize[2]);
	if (VAL(TEXTURE_MAPPING))
		drawTextureBox(1, 1, 1);
	else
		drawBox(1, 1, 1);
	glPopMatrix();
}

//OpenGl command to draw lower body
//TODO
void GundamModel::drawLowerBody2(){
	glPushMatrix();
	setDiffuseColor(COLOR_YELLOW);
	glRotated(-90, 1.0, 0.0, 0.0);
	drawCylinder(lowerBodySize[1], lowerBodySize[2]/6, lowerBodySize[0] / 2);
	glPopMatrix();
	glPopMatrix();
}

//OpenGl command to draw lower body
//TODO
void GundamModel::drawHead(){
	glPushMatrix();
	    //draw neck
		setDiffuseColor(COLOR_WHITE);
		glPushMatrix();
			glTranslated(-headSize[0] / 6, -1, -headSize[2] / 6);
			glScaled(headSize[0]/3, headSize[1], headSize[2]/3);
			//glRotated(-90, 1.0, 0.0, 0.0);
			if (VAL(TEXTURE_MAPPING))
				drawTextureBox(1, 1, 1);
			else
				drawBox(1, 1, 1);
		glPopMatrix();
		//draw head
		setDiffuseColor(COLOR_YELLOW);
		glPushMatrix();
			glTranslated(-headSize[0] / 2, headSize[1]/6, -headSize[2] / 2);
			glScaled(headSize[0], headSize[1], headSize[2]);
			if (VAL(TEXTURE_MAPPING))
				drawTextureBox(1, 1, 1);
			else
				drawBox(1, 1, 1);
		glPopMatrix();
	glPopMatrix();
}

void GundamModel::drawHead2(){
	glPushMatrix();
	//draw neck
	setDiffuseColor(COLOR_WHITE);
	glPushMatrix();
	glTranslated(-headSize[0] / 6, -1, -headSize[2] / 6);
	glScaled(headSize[0] / 3, headSize[1], headSize[2] / 3);
	//glRotated(-90, 1.0, 0.0, 0.0);
	drawBox(1, 1, 1);
	glPopMatrix();
	//draw head
	setDiffuseColor(COLOR_YELLOW);
	glPushMatrix();
	glTranslated(0.0, headSize[1] / 6 + headSize[1]/2, 0.0);
	drawSphere(headSize[1] / 2);
	glPopMatrix();
	glPopMatrix();
}

void GundamModel::drawHammer(){
	glPushMatrix();
	glTranslated(-0.5, 0.0, 0.0);
	//draw hammer handle
	setDiffuseColor(COLOR_WHITE);
	glRotated(-90, 1.0, 0.0, 0.0);
	drawCylinder(hammerSize[0] * 3, hammerSize[1] / 10, hammerSize[2] / 10);
	glTranslated(0.0, 0.0, hammerSize[0] * 3);
	//draw hammer head
	setDiffuseColor(COLOR_YELLOW);
	glTranslated(0.0, -hammerSize[1] / 2,0.0);
	float controlPoints[4][3] = {
		{ -hammerSize[0]/2, 0.0, 0.0 }, { -hammerSize[0]/3, hammerSize[1], 0.0 },
		{ hammerSize[0] / 3, 0.0, 0.0 }, { hammerSize[0] / 2, hammerSize[1], 0.0} };
	drawRotatingCurve(controlPoints, 0.0, 1.0, 0.0);
	glPopMatrix();
}

//OpenGl command to draw lower body
//TODO
void GundamModel::drawRightShoulder(){
	glPushMatrix();
	setDiffuseColor(COLOR_YELLOW);
		glTranslated(-rightShoulderSize[0] / 2 , 0.5, -rightShoulderSize[2] / 2);
		glScaled(rightShoulderSize[0], rightShoulderSize[1], rightShoulderSize[2]);
		if (VAL(TEXTURE_MAPPING))
			drawTextureBox(1, 1, 1);
		else
			drawBox(1, 1, 1);
	glPopMatrix();
}

void GundamModel::drawRightShoulder2(){
	glPushMatrix();
	setDiffuseColor(COLOR_YELLOW);
	rightShoulderSize[0] *= 1.5;
	rightShoulderSize[1] *= 1.5;
	rightShoulderSize[2] *= 1;
	glTranslated(0.0, 0.5, 0.0);
	glPushMatrix();
	//draw middle part
	drawTriangle(-rightShoulderSize[0] / 2, 0.0, -rightShoulderSize[2] / 2,
		-rightShoulderSize[0] / 2, rightShoulderSize[1], -rightShoulderSize[2] / 2,
		rightShoulderSize[0] / 2, 0.0, -rightShoulderSize[2] / 2);
	drawTriangle(rightShoulderSize[0] / 2, 0.0, rightShoulderSize[2] / 2,
		-rightShoulderSize[0] / 2, rightShoulderSize[1], rightShoulderSize[2] / 2,
		-rightShoulderSize[0] / 2, 0.0, rightShoulderSize[2] / 2);

	glTranslated(-rightShoulderSize[0] / 2, 0.0, -rightShoulderSize[2] / 2);
	drawBox(0.1f, rightShoulderSize[1], rightShoulderSize[2]);
	drawBox(rightShoulderSize[0], 0.1f, rightShoulderSize[2]);
	glTranslated(0.0, rightShoulderSize[1], 0.0);
	glRotated(-atan2(rightShoulderSize[1], rightShoulderSize[0]) * 180 / M_PI, 0.0, 0.0, 1.0);
	drawBox(sqrt(rightShoulderSize[0]*rightShoulderSize[0] + rightShoulderSize[1]*rightShoulderSize[1]), 0.1f, rightShoulderSize[2]);
	glPopMatrix();

	//draw upper part
	glPushMatrix();
	glTranslated(-rightShoulderSize[0] / 2, rightShoulderSize[1] / 4, 0.0);
	glRotated(90, 0.0, 0.0, 1.0);
	drawTriangle(-rightShoulderSize[0] / 4, 0.0, -rightShoulderSize[2] / 2,
		rightShoulderSize[0] / 4, rightShoulderSize[1] / 2, 0.0,
		rightShoulderSize[0] / 4, 0.0, -rightShoulderSize[2] / 2
		);
	drawTriangle(rightShoulderSize[0] / 4, 0.0, rightShoulderSize[2] / 2,
		rightShoulderSize[0] / 4, rightShoulderSize[1] / 2, 0.0,
		-rightShoulderSize[0] / 4, 0.0, rightShoulderSize[2] / 2);
	drawTriangle(-rightShoulderSize[0] / 4, 0.0, -rightShoulderSize[2] / 2,
		rightShoulderSize[0] / 4, rightShoulderSize[1] / 2, 0.0,
		- rightShoulderSize[0] / 4, 0.0, rightShoulderSize[2] / 2);
	drawTriangle(rightShoulderSize[0] / 4, 0.0, -rightShoulderSize[2] / 2,
		rightShoulderSize[0] / 4, rightShoulderSize[1] / 2, 0.0,
		rightShoulderSize[0] / 4, 0.0, rightShoulderSize[2] / 2);
	glTranslated(-rightShoulderSize[0] / 4, 0.0, -rightShoulderSize[2] / 2);
	drawBox(rightShoulderSize[0]/2, 0.1f, rightShoulderSize[2]);
	glPopMatrix();

	//draw lower part
	glPushMatrix();
	glTranslated(0.0, rightShoulderSize[1] / 2, 0.0);
	glRotated(-atan2(rightShoulderSize[1], rightShoulderSize[0]) * 180 / M_PI, 0.0, 0.0, 1.0);
	drawTriangle(-rightShoulderSize[0] / 4, 0.0, -rightShoulderSize[2] / 2,
		-rightShoulderSize[0] / 4, rightShoulderSize[1] / 2, 0.0,
		rightShoulderSize[0] / 4, 0.0, -rightShoulderSize[2] / 2

		);
	drawTriangle(rightShoulderSize[0] / 4, 0.0, rightShoulderSize[2] / 2,
		- rightShoulderSize[0] / 4, rightShoulderSize[1] / 2, 0.0,
		-rightShoulderSize[0] / 4, 0.0, rightShoulderSize[2] / 2);
	drawTriangle(-rightShoulderSize[0] / 4, rightShoulderSize[1] / 2, 0.0, 
		rightShoulderSize[0] / 4, 0.0, rightShoulderSize[2] / 2,
		rightShoulderSize[0] / 4, 0.0, -rightShoulderSize[2] / 2);
	drawTriangle(-rightShoulderSize[0] / 4, 0.0, rightShoulderSize[2] / 2,
		-rightShoulderSize[0] / 4, rightShoulderSize[1] / 2, 0.0,
		-rightShoulderSize[0] / 4, 0.0, -rightShoulderSize[2] / 2);
	glTranslated(-rightShoulderSize[0] / 4, 0.0, -rightShoulderSize[2] / 2);
	drawBox(rightShoulderSize[0] / 2, 0.1f, rightShoulderSize[2]);
	glPopMatrix();

	rightShoulderSize[0] /= 1.5;
	rightShoulderSize[1] /= 1.5;
	rightShoulderSize[2] /= 1;
	glPopMatrix();
}

//OpenGl command to draw lower body
//TODO
void GundamModel::drawRightUpperArm(){
	setDiffuseColor(COLOR_WHITE);
	glPushMatrix();
		glTranslated(-rightUpperArmSize[0] / 2 -0.5, 0, -rightUpperArmSize[2] / 2);
		glScaled(rightUpperArmSize[0], rightUpperArmSize[1], rightUpperArmSize[2]);
		if (VAL(TEXTURE_MAPPING))
			drawTextureBox(1, 1, 1);
		else
			drawBox(1, 1, 1);
	glPopMatrix();
}

//OpenGl command to draw lower body
//TODO
void GundamModel::drawRightLowerArm(){
	setDiffuseColor(COLOR_YELLOW);
	glPushMatrix();
		glTranslated(-rightLowerArmSize[0] / 2 -0.5, 0, -rightLowerArmSize[2] / 2);
		glScaled(rightLowerArmSize[0], rightLowerArmSize[1], rightLowerArmSize[2]);
		if (VAL(TEXTURE_MAPPING))
			drawTextureBox(1, 1, 1);
		else
			drawBox(1, 1, 1);
	glPopMatrix();
}

//OpenGl command to draw lower body
//TODO
void GundamModel::drawRightLowerArm2(){
	setDiffuseColor(COLOR_YELLOW);
	glPushMatrix();
	int triangleNumber = 5;
	double triangleWidthX = rightLowerArmSize[0] / triangleNumber;
	double triangleWidthY = rightLowerArmSize[1] / triangleNumber;
	double triangleWidthZ = rightLowerArmSize[2] / triangleNumber;
	double triangleHeight = 0.2;
	glTranslated(-0.5, 0.0, 0.0);
	for (int j = 0; j < triangleNumber; j += 1){
		for (int i = 0; i < triangleNumber; i += 1){
			//draw front
			drawTriangle(
			-rightLowerArmSize[0]/2 + (i+1)*triangleWidthX, j*triangleWidthY, rightLowerArmSize[2]/2,
			-rightLowerArmSize[0]/2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY/2 + (j)*triangleWidthY,  rightLowerArmSize[2]/2 + triangleHeight,
			-rightLowerArmSize[0]/2 + (i)*triangleWidthX, j*triangleWidthY, rightLowerArmSize[2]/2);

			drawTriangle(
				-rightLowerArmSize[0]/2 + (i + 1)*triangleWidthX, (j+1)*triangleWidthY, rightLowerArmSize[2]/2,
				-rightLowerArmSize[0]/2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY,  rightLowerArmSize[2]/2 + triangleHeight,
				-rightLowerArmSize[0]/2 + (i + 1)*triangleWidthX, j*triangleWidthY, rightLowerArmSize[2]/2);

			drawTriangle(
				-rightLowerArmSize[0]/2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY,  rightLowerArmSize[2]/2 + triangleHeight,
				-rightLowerArmSize[0] / 2 + (i + 1)*triangleWidthX, (j + 1)*triangleWidthY, rightLowerArmSize[2] / 2,
				-rightLowerArmSize[0]/2 + (i)*triangleWidthX, (j+1)*triangleWidthY, rightLowerArmSize[2]/2);

			drawTriangle(
				-rightLowerArmSize[0]/2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY,  rightLowerArmSize[2]/2 + triangleHeight,
				-rightLowerArmSize[0] / 2 + (i)*triangleWidthX, (j + 1)*triangleWidthY, rightLowerArmSize[2] / 2,
				-rightLowerArmSize[0]/2 + (i)*triangleWidthX, j*triangleWidthY, rightLowerArmSize[2]/2);

			//draw back
			drawTriangle(	
				-rightLowerArmSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, -rightLowerArmSize[2] / 2 - triangleHeight,
				-rightLowerArmSize[0] / 2 + (i + 1)*triangleWidthX, j*triangleWidthY, -rightLowerArmSize[2] / 2,
				-rightLowerArmSize[0] / 2 + (i)*triangleWidthX, j*triangleWidthY, -rightLowerArmSize[2] / 2);

			drawTriangle(
				-rightLowerArmSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, -rightLowerArmSize[2] / 2 - triangleHeight,
				-rightLowerArmSize[0] / 2 + (i + 1)*triangleWidthX, (j + 1)*triangleWidthY, -rightLowerArmSize[2] / 2,
				-rightLowerArmSize[0] / 2 + (i + 1)*triangleWidthX, j*triangleWidthY, -rightLowerArmSize[2] / 2);

			drawTriangle(
				-rightLowerArmSize[0] / 2 + (i + 1)*triangleWidthX, (j + 1)*triangleWidthY, -rightLowerArmSize[2] / 2,
				-rightLowerArmSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, -rightLowerArmSize[2] / 2 - triangleHeight,
				-rightLowerArmSize[0] / 2 + (i)*triangleWidthX, (j + 1)*triangleWidthY, -rightLowerArmSize[2] / 2);

			drawTriangle(
				-rightLowerArmSize[0] / 2 + (i)*triangleWidthX, (j + 1)*triangleWidthY, -rightLowerArmSize[2] / 2,
				-rightLowerArmSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, -rightLowerArmSize[2] / 2 - triangleHeight,
				-rightLowerArmSize[0] / 2 + (i)*triangleWidthX, j*triangleWidthY, -rightLowerArmSize[2] / 2);

			//draw right
			drawTriangle(
				rightLowerArmSize[0] / 2, j*triangleWidthY, rightLowerArmSize[2] / 2 - (i + 1)*triangleWidthZ,
				rightLowerArmSize[0] / 2 + triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, rightLowerArmSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				rightLowerArmSize[0] / 2, j*triangleWidthY, rightLowerArmSize[2] / 2 - (i)*triangleWidthZ);

			drawTriangle(
				rightLowerArmSize[0] / 2, (j + 1)*triangleWidthY, rightLowerArmSize[2] / 2 - (i + 1)*triangleWidthZ,
				rightLowerArmSize[0] / 2 + triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, rightLowerArmSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				rightLowerArmSize[0] / 2, j*triangleWidthY, rightLowerArmSize[2] / 2 - (i + 1)*triangleWidthZ);

			drawTriangle(
				rightLowerArmSize[0] / 2 + triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, rightLowerArmSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				rightLowerArmSize[0] / 2, (j + 1)*triangleWidthY, rightLowerArmSize[2] / 2 - (i + 1)*triangleWidthZ,
				rightLowerArmSize[0] / 2, (j + 1)*triangleWidthY, rightLowerArmSize[2] / 2 - (i)*triangleWidthZ);

			drawTriangle(			
				rightLowerArmSize[0] / 2 + triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, rightLowerArmSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				rightLowerArmSize[0] / 2, (j + 1)*triangleWidthY, rightLowerArmSize[2] / 2 - (i)*triangleWidthZ,
				rightLowerArmSize[0] / 2, (j)*triangleWidthY, rightLowerArmSize[2] / 2 - (i)*triangleWidthZ);

			//draw left
			drawTriangle(
				-rightLowerArmSize[0] / 2 - triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, rightLowerArmSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				-rightLowerArmSize[0] / 2, j*triangleWidthY, rightLowerArmSize[2] / 2 - (i + 1)*triangleWidthZ,
				-rightLowerArmSize[0] / 2, j*triangleWidthY, rightLowerArmSize[2] / 2 - (i)*triangleWidthZ);

			drawTriangle(
				-rightLowerArmSize[0] / 2 - triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, rightLowerArmSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				-rightLowerArmSize[0] / 2, (j + 1)*triangleWidthY, rightLowerArmSize[2] / 2 - (i + 1)*triangleWidthZ,
				-rightLowerArmSize[0] / 2, j*triangleWidthY, rightLowerArmSize[2] / 2 - (i + 1)*triangleWidthZ);

			drawTriangle(
				-rightLowerArmSize[0] / 2, (j + 1)*triangleWidthY, rightLowerArmSize[2] / 2 - (i + 1)*triangleWidthZ,
				-rightLowerArmSize[0] / 2 - triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, rightLowerArmSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				-rightLowerArmSize[0] / 2, (j + 1)*triangleWidthY, rightLowerArmSize[2] / 2 - (i)*triangleWidthZ);

			drawTriangle(
				-rightLowerArmSize[0] / 2, (j + 1)*triangleWidthY, rightLowerArmSize[2] / 2 - (i)*triangleWidthZ,
				-rightLowerArmSize[0] / 2 - triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, rightLowerArmSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				-rightLowerArmSize[0] / 2, (j)*triangleWidthY, rightLowerArmSize[2] / 2 - (i)*triangleWidthZ);

		}
	}
	glTranslated(-rightLowerArmSize[0] / 2, 0.0, -rightLowerArmSize[2]/2);
	drawBox(rightLowerArmSize[0], 0.1f, rightLowerArmSize[2]);
	glTranslated(0.0, rightLowerArmSize[1], 0.0);
	drawBox(rightLowerArmSize[0], 0.1f, rightLowerArmSize[2]);
	glPopMatrix();
}


//OpenGl command to draw lower body
//TODO
void GundamModel::drawRightFist(){
	glPushMatrix();
	setDiffuseColor(COLOR_YELLOW);
		glTranslated(-rightFistSize[0] / 2 -0.5, 0.25, -rightFistSize[2] / 2);
		glScaled(rightFistSize[0], rightFistSize[1], rightFistSize[2]);
		if (VAL(TEXTURE_MAPPING))
			drawTextureBox(1, 1, 1);
		else
			drawBox(1, 1, 1);
	glPopMatrix();
}

//OpenGl command to draw lower body
//TODO
void GundamModel::drawLeftShoulder(){
	glPushMatrix();
	setDiffuseColor(COLOR_YELLOW);
		glTranslated(-leftShoulderSize[0] / 2, 0.5, -leftShoulderSize[2] / 2);
		glScaled(leftShoulderSize[0], leftShoulderSize[1], leftShoulderSize[2]);
		if (VAL(TEXTURE_MAPPING))
			drawTextureBox(1, 1, 1);
		else
			drawBox(1, 1, 1);
	glPopMatrix();
}

void GundamModel::drawLeftShoulder2(){
	glPushMatrix();
	setDiffuseColor(COLOR_YELLOW);
	leftShoulderSize[0] *= 1.5;
	leftShoulderSize[1] *= 1.5;
	leftShoulderSize[2] *= 1;
	glTranslated(0.0, 0.5, 0.0);
	glPushMatrix();
	//draw middle part
	drawTriangle(-leftShoulderSize[0] / 2, 0.0, -leftShoulderSize[2] / 2,
		leftShoulderSize[0] / 2, leftShoulderSize[1], -leftShoulderSize[2] / 2,
		leftShoulderSize[0] / 2, 0.0, -leftShoulderSize[2] / 2);
	drawTriangle(leftShoulderSize[0] / 2, 0.0, leftShoulderSize[2] / 2,
		leftShoulderSize[0] / 2, leftShoulderSize[1], leftShoulderSize[2] / 2,
		-leftShoulderSize[0] / 2, 0.0, leftShoulderSize[2] / 2);

	glTranslated(-leftShoulderSize[0] / 2, 0.0, -leftShoulderSize[2] / 2);
	drawBox(leftShoulderSize[0], 0.1f, leftShoulderSize[2]);
	glTranslated(leftShoulderSize[0], 0.0, 0.0);
	drawBox(0.1f, leftShoulderSize[1], leftShoulderSize[2]);
	glTranslated(-leftShoulderSize[0], 0.0, 0.0);
	glRotated(atan2(leftShoulderSize[1], leftShoulderSize[0]) * 180 / M_PI, 0.0, 0.0, 1.0);
	drawBox(sqrt(leftShoulderSize[0] * leftShoulderSize[0] + leftShoulderSize[1] * leftShoulderSize[1]), 0.1f, leftShoulderSize[2]);
	glPopMatrix();

	//draw upper part
	glPushMatrix();
	glTranslated(leftShoulderSize[0] / 2, leftShoulderSize[1] / 4, 0.0);
	glRotated(-90, 0.0, 0.0, 1.0);
	drawTriangle(-leftShoulderSize[0] / 4, leftShoulderSize[1] / 2, 0.0,
		leftShoulderSize[0] / 4, 0.0, -leftShoulderSize[2] / 2,
		-leftShoulderSize[0] / 4, 0.0, -leftShoulderSize[2] / 2);
	drawTriangle(leftShoulderSize[0] / 4, 0.0, leftShoulderSize[2] / 2,
		-leftShoulderSize[0] / 4, leftShoulderSize[1] / 2, 0.0,
		-leftShoulderSize[0] / 4, 0.0, leftShoulderSize[2] / 2);
	drawTriangle(leftShoulderSize[0] / 4, 0.0, -leftShoulderSize[2] / 2, 
		-leftShoulderSize[0] / 4, leftShoulderSize[1] / 2, 0.0,
		leftShoulderSize[0] / 4, 0.0, leftShoulderSize[2] / 2);
	drawTriangle(-leftShoulderSize[0] / 4, 0.0, -leftShoulderSize[2] / 2,
		-leftShoulderSize[0] / 4, 0.0, leftShoulderSize[2] / 2,
		-leftShoulderSize[0] / 4, leftShoulderSize[1] / 2, 0.0);
	glTranslated(-leftShoulderSize[0] / 4, 0.0, -leftShoulderSize[2] / 2);
	drawBox(leftShoulderSize[0] / 2, 0.1f, leftShoulderSize[2]);
	glPopMatrix();

	//draw lower part
	glPushMatrix();
	glTranslated(0.0, leftShoulderSize[1] / 2, 0.0);
	glRotated(atan2(leftShoulderSize[1], leftShoulderSize[0]) * 180 / M_PI, 0.0, 0.0, 1.0);
	drawTriangle(leftShoulderSize[0] / 4, leftShoulderSize[1] / 2, 0.0,
		leftShoulderSize[0] / 4, 0.0, -leftShoulderSize[2] / 2,
		-leftShoulderSize[0] / 4, 0.0, -leftShoulderSize[2] / 2);
	drawTriangle(leftShoulderSize[0] / 4, 0.0, leftShoulderSize[2] / 2,
		leftShoulderSize[0] / 4, leftShoulderSize[1] / 2, 0.0,
		-leftShoulderSize[0] / 4, 0.0, leftShoulderSize[2] / 2);
	drawTriangle(-leftShoulderSize[0] / 4, 0.0, leftShoulderSize[2] / 2,
		leftShoulderSize[0] / 4, leftShoulderSize[1] / 2, 0.0,
		-leftShoulderSize[0] / 4, 0.0, -leftShoulderSize[2] / 2);
	drawTriangle(leftShoulderSize[0] / 4, leftShoulderSize[1] / 2, 0.0,
		leftShoulderSize[0] / 4, 0.0, leftShoulderSize[2] / 2,
		leftShoulderSize[0] / 4, 0.0, -leftShoulderSize[2] / 2);
	glTranslated(-leftShoulderSize[0] / 4, 0.0, -leftShoulderSize[2] / 2);
	drawBox(leftShoulderSize[0] / 2, 0.1f, leftShoulderSize[2]);
	glPopMatrix();

	leftShoulderSize[0] /= 1.5;
	leftShoulderSize[1] /= 1.5;
	leftShoulderSize[2] /= 1;
	glPopMatrix();
}

//OpenGl command to draw lower body
//TODO
void GundamModel::drawLeftUpperArm(){
	setDiffuseColor(COLOR_WHITE);
	glPushMatrix();
		glTranslated(-leftUpperArmSize[0] / 2 + 0.5 , 0, -leftUpperArmSize[2] / 2);
		glScaled(leftUpperArmSize[0], leftUpperArmSize[1], leftUpperArmSize[2]);
		if (VAL(TEXTURE_MAPPING))
			drawTextureBox(1, 1, 1);
		else
			drawBox(1, 1, 1);
	glPopMatrix();
}

//OpenGl command to draw lower body
//TODO
void GundamModel::drawLeftLowerArm(){
	setDiffuseColor(COLOR_YELLOW);
	glPushMatrix();
		glTranslated(-leftLowerArmSize[0] / 2 + 0.5 , 0, -leftLowerArmSize[2] / 2);
		glScaled(leftLowerArmSize[0], leftLowerArmSize[1], leftLowerArmSize[2]);
		if (VAL(TEXTURE_MAPPING))
			drawTextureBox(1, 1, 1);
		else
			drawBox(1, 1, 1);
	glPopMatrix();
}

void GundamModel::drawLeftLowerArm2(){
	setDiffuseColor(COLOR_YELLOW);
	glPushMatrix();
	int triangleNumber = 5;
	double triangleWidthX = leftLowerArmSize[0] / triangleNumber;
	double triangleWidthY = leftLowerArmSize[1] / triangleNumber;
	double triangleWidthZ = leftLowerArmSize[2] / triangleNumber;
	double triangleHeight = 0.2;
	glTranslated(0.5, 0.0, 0.0);
	for (int j = 0; j < triangleNumber; j += 1){
		for (int i = 0; i < triangleNumber; i += 1){
			//draw front
			drawTriangle(
				-leftLowerArmSize[0] / 2 + (i + 1)*triangleWidthX, j*triangleWidthY, leftLowerArmSize[2] / 2,
				-leftLowerArmSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerArmSize[2] / 2 + triangleHeight,
				-leftLowerArmSize[0] / 2 + (i)*triangleWidthX, j*triangleWidthY, leftLowerArmSize[2] / 2);

			drawTriangle(
				-leftLowerArmSize[0] / 2 + (i + 1)*triangleWidthX, (j + 1)*triangleWidthY, leftLowerArmSize[2] / 2,
				-leftLowerArmSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerArmSize[2] / 2 + triangleHeight,
				-leftLowerArmSize[0] / 2 + (i + 1)*triangleWidthX, j*triangleWidthY, leftLowerArmSize[2] / 2);

			drawTriangle(
				-leftLowerArmSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerArmSize[2] / 2 + triangleHeight,
				-leftLowerArmSize[0] / 2 + (i + 1)*triangleWidthX, (j + 1)*triangleWidthY, leftLowerArmSize[2] / 2,
				-leftLowerArmSize[0] / 2 + (i)*triangleWidthX, (j + 1)*triangleWidthY, leftLowerArmSize[2] / 2);

			drawTriangle(
				-leftLowerArmSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerArmSize[2] / 2 + triangleHeight,
				-leftLowerArmSize[0] / 2 + (i)*triangleWidthX, (j + 1)*triangleWidthY, leftLowerArmSize[2] / 2,
				-leftLowerArmSize[0] / 2 + (i)*triangleWidthX, j*triangleWidthY, leftLowerArmSize[2] / 2);

			//draw back
			drawTriangle(
				-leftLowerArmSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, -leftLowerArmSize[2] / 2 - triangleHeight,
				-leftLowerArmSize[0] / 2 + (i + 1)*triangleWidthX, j*triangleWidthY, -leftLowerArmSize[2] / 2,
				-leftLowerArmSize[0] / 2 + (i)*triangleWidthX, j*triangleWidthY, -leftLowerArmSize[2] / 2);

			drawTriangle(
				-leftLowerArmSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, -leftLowerArmSize[2] / 2 - triangleHeight,
				-leftLowerArmSize[0] / 2 + (i + 1)*triangleWidthX, (j + 1)*triangleWidthY, -leftLowerArmSize[2] / 2,
				-leftLowerArmSize[0] / 2 + (i + 1)*triangleWidthX, j*triangleWidthY, -leftLowerArmSize[2] / 2);

			drawTriangle(
				-leftLowerArmSize[0] / 2 + (i + 1)*triangleWidthX, (j + 1)*triangleWidthY, -leftLowerArmSize[2] / 2,
				-leftLowerArmSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, -leftLowerArmSize[2] / 2 - triangleHeight,
				-leftLowerArmSize[0] / 2 + (i)*triangleWidthX, (j + 1)*triangleWidthY, -leftLowerArmSize[2] / 2);

			drawTriangle(
				-leftLowerArmSize[0] / 2 + (i)*triangleWidthX, (j + 1)*triangleWidthY, -leftLowerArmSize[2] / 2,
				-leftLowerArmSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, -leftLowerArmSize[2] / 2 - triangleHeight,
				-leftLowerArmSize[0] / 2 + (i)*triangleWidthX, j*triangleWidthY, -leftLowerArmSize[2] / 2);

			//draw left
			drawTriangle(
				leftLowerArmSize[0] / 2, j*triangleWidthY, leftLowerArmSize[2] / 2 - (i + 1)*triangleWidthZ,
				leftLowerArmSize[0] / 2 + triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerArmSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				leftLowerArmSize[0] / 2, j*triangleWidthY, leftLowerArmSize[2] / 2 - (i)*triangleWidthZ);

			drawTriangle(
				leftLowerArmSize[0] / 2, (j + 1)*triangleWidthY, leftLowerArmSize[2] / 2 - (i + 1)*triangleWidthZ,
				leftLowerArmSize[0] / 2 + triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerArmSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				leftLowerArmSize[0] / 2, j*triangleWidthY, leftLowerArmSize[2] / 2 - (i + 1)*triangleWidthZ);

			drawTriangle(
				leftLowerArmSize[0] / 2 + triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerArmSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				leftLowerArmSize[0] / 2, (j + 1)*triangleWidthY, leftLowerArmSize[2] / 2 - (i + 1)*triangleWidthZ,
				leftLowerArmSize[0] / 2, (j + 1)*triangleWidthY, leftLowerArmSize[2] / 2 - (i)*triangleWidthZ);

			drawTriangle(
				leftLowerArmSize[0] / 2 + triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerArmSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				leftLowerArmSize[0] / 2, (j + 1)*triangleWidthY, leftLowerArmSize[2] / 2 - (i)*triangleWidthZ,
				leftLowerArmSize[0] / 2, (j)*triangleWidthY, leftLowerArmSize[2] / 2 - (i)*triangleWidthZ);

			//draw left
			drawTriangle(
				-leftLowerArmSize[0] / 2 - triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerArmSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				-leftLowerArmSize[0] / 2, j*triangleWidthY, leftLowerArmSize[2] / 2 - (i + 1)*triangleWidthZ,
				-leftLowerArmSize[0] / 2, j*triangleWidthY, leftLowerArmSize[2] / 2 - (i)*triangleWidthZ);

			drawTriangle(
				-leftLowerArmSize[0] / 2 - triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerArmSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				-leftLowerArmSize[0] / 2, (j + 1)*triangleWidthY, leftLowerArmSize[2] / 2 - (i + 1)*triangleWidthZ,
				-leftLowerArmSize[0] / 2, j*triangleWidthY, leftLowerArmSize[2] / 2 - (i + 1)*triangleWidthZ);

			drawTriangle(
				-leftLowerArmSize[0] / 2, (j + 1)*triangleWidthY, leftLowerArmSize[2] / 2 - (i + 1)*triangleWidthZ,
				-leftLowerArmSize[0] / 2 - triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerArmSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				-leftLowerArmSize[0] / 2, (j + 1)*triangleWidthY, leftLowerArmSize[2] / 2 - (i)*triangleWidthZ);

			drawTriangle(
				-leftLowerArmSize[0] / 2, (j + 1)*triangleWidthY, leftLowerArmSize[2] / 2 - (i)*triangleWidthZ,
				-leftLowerArmSize[0] / 2 - triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerArmSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				-leftLowerArmSize[0] / 2, (j)*triangleWidthY, leftLowerArmSize[2] / 2 - (i)*triangleWidthZ);

		}
	}
	glTranslated(-leftLowerArmSize[0] / 2, 0.0, -leftLowerArmSize[2] / 2);
	drawBox(leftLowerArmSize[0], 0.1f, leftLowerArmSize[2]);
	glTranslated(0.0, leftLowerArmSize[1], 0.0);
	drawBox(leftLowerArmSize[0], 0.1f, leftLowerArmSize[2]);
	glPopMatrix();
}
//OpenGl command to draw lower body
//TODO
void GundamModel::drawLeftFist(){
	glPushMatrix();
	setDiffuseColor(COLOR_YELLOW);
		glTranslated(-leftFistSize[0] / 2 + 0.5 , 0.25, -leftFistSize[2] / 2);
		glScaled(leftFistSize[0], leftFistSize[1], leftFistSize[2]);
		if (VAL(TEXTURE_MAPPING))
			drawTextureBox(1, 1, 1);
		else
			drawBox(1, 1, 1);
	glPopMatrix();
}

//OpenGl command to draw lower body
//TODO
void GundamModel::drawRightThigh(){
	glPushMatrix();
	setDiffuseColor(COLOR_YELLOW);
		glTranslated(-rightThighSize[0] / 2, 0, -rightThighSize[2] / 2);
		glScaled(rightThighSize[0], rightThighSize[1], rightThighSize[2]);
		if (VAL(TEXTURE_MAPPING))
			drawTextureBox(1, 1, 1);
		else
			drawBox(1, 1, 1);
	glPopMatrix();
}

//OpenGl command to draw lower body
//TODO
void GundamModel::drawRightThigh2(){
	glPushMatrix();

	setDiffuseColor(COLOR_YELLOW);

	glRotated(-90, 1.0, 0.0, 0.0);
	drawCylinder(rightThighSize[1], (rightThighSize[0] + rightThighSize[2]) / 4, (rightThighSize[0] + rightThighSize[2]) / 4);

	glPopMatrix();
}
//OpenGl command to draw lower body
//TODO
void GundamModel::drawRightUpperLeg(){
	setDiffuseColor(COLOR_WHITE);
	glPushMatrix();
		glTranslated(-rightUpperLegSize[0] / 2, 0, -rightUpperLegSize[2] / 2);
		glScaled(rightUpperLegSize[0], rightUpperLegSize[1], rightUpperLegSize[2]);
		if (VAL(TEXTURE_MAPPING))
			drawTextureBox(1, 1, 1);
		else
			drawBox(1, 1, 1);
	glPopMatrix();
}

//OpenGl command to draw lower body
//TODO
void GundamModel::drawRightLowerLeg(){
	setDiffuseColor(COLOR_YELLOW);
	glPushMatrix();
		glTranslated(-rightLowerLegSize[0] / 2, 0, -rightLowerLegSize[2] / 2);
		glScaled(rightLowerLegSize[0], rightLowerLegSize[1], rightLowerLegSize[2]);
		if (VAL(TEXTURE_MAPPING))
			drawTextureBox(1, 1, 1);
		else
			drawBox(1, 1, 1);
	glPopMatrix();
}
//OpenGl command to draw lower body
//TODO
void GundamModel::drawRightLowerLeg2(){
	setDiffuseColor(COLOR_YELLOW);
	glPushMatrix();
	int triangleNumber = 5;
	double triangleWidthX = rightLowerLegSize[0] / triangleNumber;
	double triangleWidthY = rightLowerLegSize[1] / triangleNumber;
	double triangleWidthZ = rightLowerLegSize[2] / triangleNumber;
	double triangleHeight = 0.2;
	for (int j = 0; j < triangleNumber; j += 1){
		for (int i = 0; i < triangleNumber; i += 1){
			//draw front
			drawTriangle(
				-rightLowerLegSize[0] / 2 + (i + 1)*triangleWidthX, j*triangleWidthY, rightLowerLegSize[2] / 2,
				-rightLowerLegSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, rightLowerLegSize[2] / 2 + triangleHeight,
				-rightLowerLegSize[0] / 2 + (i)*triangleWidthX, j*triangleWidthY, rightLowerLegSize[2] / 2);

			drawTriangle(
				-rightLowerLegSize[0] / 2 + (i + 1)*triangleWidthX, (j + 1)*triangleWidthY, rightLowerLegSize[2] / 2,
				-rightLowerLegSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, rightLowerLegSize[2] / 2 + triangleHeight,
				-rightLowerLegSize[0] / 2 + (i + 1)*triangleWidthX, j*triangleWidthY, rightLowerLegSize[2] / 2);

			drawTriangle(
				-rightLowerLegSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, rightLowerLegSize[2] / 2 + triangleHeight,
				-rightLowerLegSize[0] / 2 + (i + 1)*triangleWidthX, (j + 1)*triangleWidthY, rightLowerLegSize[2] / 2,
				-rightLowerLegSize[0] / 2 + (i)*triangleWidthX, (j + 1)*triangleWidthY, rightLowerLegSize[2] / 2);

			drawTriangle(
				-rightLowerLegSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, rightLowerLegSize[2] / 2 + triangleHeight,
				-rightLowerLegSize[0] / 2 + (i)*triangleWidthX, (j + 1)*triangleWidthY, rightLowerLegSize[2] / 2,
				-rightLowerLegSize[0] / 2 + (i)*triangleWidthX, j*triangleWidthY, rightLowerLegSize[2] / 2);

			//draw back
			drawTriangle(
				-rightLowerLegSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, -rightLowerLegSize[2] / 2 - triangleHeight,
				-rightLowerLegSize[0] / 2 + (i + 1)*triangleWidthX, j*triangleWidthY, -rightLowerLegSize[2] / 2,
				-rightLowerLegSize[0] / 2 + (i)*triangleWidthX, j*triangleWidthY, -rightLowerLegSize[2] / 2);

			drawTriangle(
				-rightLowerLegSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, -rightLowerLegSize[2] / 2 - triangleHeight,
				-rightLowerLegSize[0] / 2 + (i + 1)*triangleWidthX, (j + 1)*triangleWidthY, -rightLowerLegSize[2] / 2,
				-rightLowerLegSize[0] / 2 + (i + 1)*triangleWidthX, j*triangleWidthY, -rightLowerLegSize[2] / 2);

			drawTriangle(
				-rightLowerLegSize[0] / 2 + (i + 1)*triangleWidthX, (j + 1)*triangleWidthY, -rightLowerLegSize[2] / 2,
				-rightLowerLegSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, -rightLowerLegSize[2] / 2 - triangleHeight,
				-rightLowerLegSize[0] / 2 + (i)*triangleWidthX, (j + 1)*triangleWidthY, -rightLowerLegSize[2] / 2);

			drawTriangle(
				-rightLowerLegSize[0] / 2 + (i)*triangleWidthX, (j + 1)*triangleWidthY, -rightLowerLegSize[2] / 2,
				-rightLowerLegSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, -rightLowerLegSize[2] / 2 - triangleHeight,
				-rightLowerLegSize[0] / 2 + (i)*triangleWidthX, j*triangleWidthY, -rightLowerLegSize[2] / 2);

			//draw right
			drawTriangle(
				rightLowerLegSize[0] / 2, j*triangleWidthY, rightLowerLegSize[2] / 2 - (i + 1)*triangleWidthZ,
				rightLowerLegSize[0] / 2 + triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, rightLowerLegSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				rightLowerLegSize[0] / 2, j*triangleWidthY, rightLowerLegSize[2] / 2 - (i)*triangleWidthZ);

			drawTriangle(
				rightLowerLegSize[0] / 2, (j + 1)*triangleWidthY, rightLowerLegSize[2] / 2 - (i + 1)*triangleWidthZ,
				rightLowerLegSize[0] / 2 + triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, rightLowerLegSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				rightLowerLegSize[0] / 2, j*triangleWidthY, rightLowerLegSize[2] / 2 - (i + 1)*triangleWidthZ);

			drawTriangle(
				rightLowerLegSize[0] / 2 + triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, rightLowerLegSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				rightLowerLegSize[0] / 2, (j + 1)*triangleWidthY, rightLowerLegSize[2] / 2 - (i + 1)*triangleWidthZ,
				rightLowerLegSize[0] / 2, (j + 1)*triangleWidthY, rightLowerLegSize[2] / 2 - (i)*triangleWidthZ);

			drawTriangle(
				rightLowerLegSize[0] / 2 + triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, rightLowerLegSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				rightLowerLegSize[0] / 2, (j + 1)*triangleWidthY, rightLowerLegSize[2] / 2 - (i)*triangleWidthZ,
				rightLowerLegSize[0] / 2, (j)*triangleWidthY, rightLowerLegSize[2] / 2 - (i)*triangleWidthZ);

			//draw left
			drawTriangle(
				-rightLowerLegSize[0] / 2 - triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, rightLowerLegSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				-rightLowerLegSize[0] / 2, j*triangleWidthY, rightLowerLegSize[2] / 2 - (i + 1)*triangleWidthZ,
				-rightLowerLegSize[0] / 2, j*triangleWidthY, rightLowerLegSize[2] / 2 - (i)*triangleWidthZ);

			drawTriangle(
				-rightLowerLegSize[0] / 2 - triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, rightLowerLegSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				-rightLowerLegSize[0] / 2, (j + 1)*triangleWidthY, rightLowerLegSize[2] / 2 - (i + 1)*triangleWidthZ,
				-rightLowerLegSize[0] / 2, j*triangleWidthY, rightLowerLegSize[2] / 2 - (i + 1)*triangleWidthZ);

			drawTriangle(
				-rightLowerLegSize[0] / 2, (j + 1)*triangleWidthY, rightLowerLegSize[2] / 2 - (i + 1)*triangleWidthZ,
				-rightLowerLegSize[0] / 2 - triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, rightLowerLegSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				-rightLowerLegSize[0] / 2, (j + 1)*triangleWidthY, rightLowerLegSize[2] / 2 - (i)*triangleWidthZ);

			drawTriangle(
				-rightLowerLegSize[0] / 2, (j + 1)*triangleWidthY, rightLowerLegSize[2] / 2 - (i)*triangleWidthZ,
				-rightLowerLegSize[0] / 2 - triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, rightLowerLegSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				-rightLowerLegSize[0] / 2, (j)*triangleWidthY, rightLowerLegSize[2] / 2 - (i)*triangleWidthZ);

		}
	}
	glTranslated(-rightLowerLegSize[0] / 2, 0.0, -rightLowerLegSize[2] / 2);
	drawBox(rightLowerLegSize[0], 0.1f, rightLowerLegSize[2]);
	glTranslated(0.0, rightLowerLegSize[1], 0.0);
	drawBox(rightLowerLegSize[0], 0.1f, rightLowerLegSize[2]);
	glPopMatrix();
}
//OpenGl command to draw lower body
//TODO
void GundamModel::drawRightFoot(){
	glPushMatrix();
	setDiffuseColor(COLOR_YELLOW);
		glTranslated(-rightFootSize[0] / 2, 0, -rightFootSize[2] / 2);
		glScaled(rightFootSize[0], rightFootSize[1], rightFootSize[2]);
		if (VAL(TEXTURE_MAPPING))
			drawTextureBox(1, 1, 1);
		else
			drawBox(1, 1, 1);
	glPopMatrix();
}


//OpenGl command to draw lower body
//TODO
void GundamModel::drawLeftThigh(){
	glPushMatrix();
	setDiffuseColor(COLOR_YELLOW);
		glTranslated(-leftThighSize[0] / 2, 0, -leftThighSize[2] / 2);
		glScaled(leftThighSize[0], leftThighSize[1], leftThighSize[2]);
		if (VAL(TEXTURE_MAPPING))
			drawTextureBox(1, 1, 1);
		else
			drawBox(1, 1, 1);
	glPopMatrix();
}


//OpenGl command to draw lower body
//TODO
void GundamModel::drawLeftThigh2(){
	glPushMatrix();
	setDiffuseColor(COLOR_YELLOW);
	glRotated(-90, 1.0, 0.0, 0.0);
	drawCylinder(leftThighSize[1], (leftThighSize[0] + leftThighSize[2]) / 4, (leftThighSize[0] + leftThighSize[2]) / 4);

	glPopMatrix();
}
//OpenGl command to draw lower body
//TODO
void GundamModel::drawLeftUpperLeg(){
	setDiffuseColor(COLOR_WHITE);
	glPushMatrix();
		glTranslated(-leftUpperLegSize[0] / 2, 0, -leftUpperLegSize[2] / 2);
		glScaled(leftUpperLegSize[0], leftUpperLegSize[1], leftUpperLegSize[2]);
		if (VAL(TEXTURE_MAPPING))
			drawTextureBox(1, 1, 1);
		else
			drawBox(1, 1, 1);
	glPopMatrix();
}


//OpenGl command to draw lower body
//TODO
void GundamModel::drawLeftLowerLeg(){
	setDiffuseColor(COLOR_YELLOW);
	glPushMatrix();
		glTranslated(-leftLowerLegSize[0] / 2, 0, -leftLowerLegSize[2] / 2);
		glScaled(leftLowerLegSize[0], leftLowerLegSize[1], leftLowerLegSize[2]);
		if (VAL(TEXTURE_MAPPING))
			drawTextureBox(1, 1, 1);
		else
			drawBox(1, 1, 1);
	glPopMatrix();
}
void GundamModel::drawLeftLowerLeg2(){
	setDiffuseColor(COLOR_YELLOW);
	glPushMatrix();
	int triangleNumber = 5;
	double triangleWidthX = leftLowerLegSize[0] / triangleNumber;
	double triangleWidthY = leftLowerLegSize[1] / triangleNumber;
	double triangleWidthZ = leftLowerLegSize[2] / triangleNumber;
	double triangleHeight = 0.2;
	for (int j = 0; j < triangleNumber; j += 1){
		for (int i = 0; i < triangleNumber; i += 1){
			//draw front
			drawTriangle(
				-leftLowerLegSize[0] / 2 + (i + 1)*triangleWidthX, j*triangleWidthY, leftLowerLegSize[2] / 2,
				-leftLowerLegSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerLegSize[2] / 2 + triangleHeight,
				-leftLowerLegSize[0] / 2 + (i)*triangleWidthX, j*triangleWidthY, leftLowerLegSize[2] / 2);

			drawTriangle(
				-leftLowerLegSize[0] / 2 + (i + 1)*triangleWidthX, (j + 1)*triangleWidthY, leftLowerLegSize[2] / 2,
				-leftLowerLegSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerLegSize[2] / 2 + triangleHeight,
				-leftLowerLegSize[0] / 2 + (i + 1)*triangleWidthX, j*triangleWidthY, leftLowerLegSize[2] / 2);

			drawTriangle(
				-leftLowerLegSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerLegSize[2] / 2 + triangleHeight,
				-leftLowerLegSize[0] / 2 + (i + 1)*triangleWidthX, (j + 1)*triangleWidthY, leftLowerLegSize[2] / 2,
				-leftLowerLegSize[0] / 2 + (i)*triangleWidthX, (j + 1)*triangleWidthY, leftLowerLegSize[2] / 2);

			drawTriangle(
				-leftLowerLegSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerLegSize[2] / 2 + triangleHeight,
				-leftLowerLegSize[0] / 2 + (i)*triangleWidthX, (j + 1)*triangleWidthY, leftLowerLegSize[2] / 2,
				-leftLowerLegSize[0] / 2 + (i)*triangleWidthX, j*triangleWidthY, leftLowerLegSize[2] / 2);

			//draw back
			drawTriangle(
				-leftLowerLegSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, -leftLowerLegSize[2] / 2 - triangleHeight,
				-leftLowerLegSize[0] / 2 + (i + 1)*triangleWidthX, j*triangleWidthY, -leftLowerLegSize[2] / 2,
				-leftLowerLegSize[0] / 2 + (i)*triangleWidthX, j*triangleWidthY, -leftLowerLegSize[2] / 2);

			drawTriangle(
				-leftLowerLegSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, -leftLowerLegSize[2] / 2 - triangleHeight,
				-leftLowerLegSize[0] / 2 + (i + 1)*triangleWidthX, (j + 1)*triangleWidthY, -leftLowerLegSize[2] / 2,
				-leftLowerLegSize[0] / 2 + (i + 1)*triangleWidthX, j*triangleWidthY, -leftLowerLegSize[2] / 2);

			drawTriangle(
				-leftLowerLegSize[0] / 2 + (i + 1)*triangleWidthX, (j + 1)*triangleWidthY, -leftLowerLegSize[2] / 2,
				-leftLowerLegSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, -leftLowerLegSize[2] / 2 - triangleHeight,
				-leftLowerLegSize[0] / 2 + (i)*triangleWidthX, (j + 1)*triangleWidthY, -leftLowerLegSize[2] / 2);

			drawTriangle(
				-leftLowerLegSize[0] / 2 + (i)*triangleWidthX, (j + 1)*triangleWidthY, -leftLowerLegSize[2] / 2,
				-leftLowerLegSize[0] / 2 + triangleWidthX / 2 + (i)*triangleWidthX, triangleWidthY / 2 + (j)*triangleWidthY, -leftLowerLegSize[2] / 2 - triangleHeight,
				-leftLowerLegSize[0] / 2 + (i)*triangleWidthX, j*triangleWidthY, -leftLowerLegSize[2] / 2);

			//draw left
			drawTriangle(
				leftLowerLegSize[0] / 2, j*triangleWidthY, leftLowerLegSize[2] / 2 - (i + 1)*triangleWidthZ,
				leftLowerLegSize[0] / 2 + triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerLegSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				leftLowerLegSize[0] / 2, j*triangleWidthY, leftLowerLegSize[2] / 2 - (i)*triangleWidthZ);

			drawTriangle(
				leftLowerLegSize[0] / 2, (j + 1)*triangleWidthY, leftLowerLegSize[2] / 2 - (i + 1)*triangleWidthZ,
				leftLowerLegSize[0] / 2 + triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerLegSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				leftLowerLegSize[0] / 2, j*triangleWidthY, leftLowerLegSize[2] / 2 - (i + 1)*triangleWidthZ);

			drawTriangle(
				leftLowerLegSize[0] / 2 + triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerLegSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				leftLowerLegSize[0] / 2, (j + 1)*triangleWidthY, leftLowerLegSize[2] / 2 - (i + 1)*triangleWidthZ,
				leftLowerLegSize[0] / 2, (j + 1)*triangleWidthY, leftLowerLegSize[2] / 2 - (i)*triangleWidthZ);

			drawTriangle(
				leftLowerLegSize[0] / 2 + triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerLegSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				leftLowerLegSize[0] / 2, (j + 1)*triangleWidthY, leftLowerLegSize[2] / 2 - (i)*triangleWidthZ,
				leftLowerLegSize[0] / 2, (j)*triangleWidthY, leftLowerLegSize[2] / 2 - (i)*triangleWidthZ);

			//draw left
			drawTriangle(
				-leftLowerLegSize[0] / 2 - triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerLegSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				-leftLowerLegSize[0] / 2, j*triangleWidthY, leftLowerLegSize[2] / 2 - (i + 1)*triangleWidthZ,
				-leftLowerLegSize[0] / 2, j*triangleWidthY, leftLowerLegSize[2] / 2 - (i)*triangleWidthZ);

			drawTriangle(
				-leftLowerLegSize[0] / 2 - triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerLegSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				-leftLowerLegSize[0] / 2, (j + 1)*triangleWidthY, leftLowerLegSize[2] / 2 - (i + 1)*triangleWidthZ,
				-leftLowerLegSize[0] / 2, j*triangleWidthY, leftLowerLegSize[2] / 2 - (i + 1)*triangleWidthZ);

			drawTriangle(
				-leftLowerLegSize[0] / 2, (j + 1)*triangleWidthY, leftLowerLegSize[2] / 2 - (i + 1)*triangleWidthZ,
				-leftLowerLegSize[0] / 2 - triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerLegSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				-leftLowerLegSize[0] / 2, (j + 1)*triangleWidthY, leftLowerLegSize[2] / 2 - (i)*triangleWidthZ);

			drawTriangle(
				-leftLowerLegSize[0] / 2, (j + 1)*triangleWidthY, leftLowerLegSize[2] / 2 - (i)*triangleWidthZ,
				-leftLowerLegSize[0] / 2 - triangleHeight, triangleWidthY / 2 + (j)*triangleWidthY, leftLowerLegSize[2] / 2 - triangleWidthZ / 2 - (i)*triangleWidthZ,
				-leftLowerLegSize[0] / 2, (j)*triangleWidthY, leftLowerLegSize[2] / 2 - (i)*triangleWidthZ);

		}
	}
	glTranslated(-leftLowerLegSize[0] / 2, 0.0, -leftLowerLegSize[2] / 2);
	drawBox(leftLowerLegSize[0], 0.1f, leftLowerLegSize[2]);
	glTranslated(0.0, leftLowerLegSize[1], 0.0);
	drawBox(leftLowerLegSize[0], 0.1f, leftLowerLegSize[2]);
	glPopMatrix();
}
//OpenGl command to draw lower body
//TODO
void GundamModel::drawLeftFoot(){
	glPushMatrix();
	setDiffuseColor(COLOR_YELLOW);
		glTranslated(-leftFootSize[0] / 2, 0, -leftFootSize[2] / 2);
		glScaled(leftFootSize[0], leftFootSize[1], leftFootSize[2]);
		if (VAL(TEXTURE_MAPPING))
			drawTextureBox(1, 1, 1);
		else
			drawBox(1, 1, 1);
	glPopMatrix();
}

void GundamModel::animationIterator() {
	if (iterator==60) {
		iterator = 0;
	}
//	printf("%d \n", iterator);
	if (iterator < 15) {
		animRightThigh = (iterator + 1) * 2;
		animRightLowerLeg = iterator + 1;
		animLeftThigh = -(iterator + 1) * 2;
		animLeftLowerLeg = 0;
		animUpperBody = - (iterator + 1);
		++iterator;
		return;
	}
	else if (iterator < 30) {
		animRightThigh = (30 - iterator) * 2;
		animRightLowerLeg = 30 - iterator;
		animLeftThigh = -(30 - iterator) * 2;
		animUpperBody = -(30 - iterator);
		++iterator;
		return;
	}
	else if (iterator < 45) {
		animLeftThigh = (iterator - 29) * 2;
		animLeftLowerLeg = iterator - 29;
		animRightThigh = -(iterator - 29) * 2;
		animRightLowerLeg = 0;
		animUpperBody = iterator - 29;
		++iterator;
		return;
	}
	else if (iterator < 60) {
		animLeftThigh = (60 - iterator) * 2;
		animLeftLowerLeg = 60 - iterator;
		animRightThigh = -(60 - iterator) * 2;
		animUpperBody = 60 - iterator;
		++iterator;
		return;
	}
	
	
}

Mat4f getModelViewMatrix() {
	GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	Mat4f matMV(m[0], m[1], m[2], m[3],
		m[4], m[5], m[6], m[7],
		m[8], m[9], m[10], m[11],
		m[12], m[13], m[14], m[15]);
	return matMV.transpose();
}

void SpawnParticles (Mat4f CameraMatrix) {
	Mat4f CurrentModelViewMatrix = getModelViewMatrix();
	CurrentModelViewMatrix.transpose();
	Mat4f WorldMatrix = CameraMatrix.inverse() * CurrentModelViewMatrix;
	Vec4f WorldPoint = WorldMatrix * (Vec4f(0, 0, 0, 1));
	ParticleSystem* ps = ModelerApplication::Instance()->GetParticleSystem();
	float t = ModelerApplication::Instance()->GetTime();

	// apply the inverse transforation matrix
	float* glTransformationMatrix = new float[16];
	WorldMatrix.inverse().getGLMatrix(glTransformationMatrix);
	glMultMatrixf(glTransformationMatrix);
	glTranslated(WorldPoint[0], WorldPoint[1], WorldPoint[2]);
	if (ps) {
		ps->computeForcesAndUpdateParticles(t);
		ps->drawParticles(t);
	}
	delete [] glTransformationMatrix;
}

int main()
{
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
	ModelerControl controls[NUMCONTROLS];
	controls[DETAIL] = ModelerControl("Level of Details", 0, 5, 1, 5);
	controls[XPOS] = ModelerControl("X Position", -20, 20, 0.1f, 0);
	controls[YPOS] = ModelerControl("Y Position", 0, 20, 0.1f, 0);
	controls[ZPOS] = ModelerControl("Z Position", -20, 20, 0.1f, 0);
	controls[ROTATE] = ModelerControl("Rotate", -135, 135, 1, 0);
	controls[ROTATE_UPPER_BODY] = ModelerControl("Rotate upper body Y", 90, -90, 1, 0);
	controls[RAISE_RIGHT_ARM_X] = ModelerControl("Raise right arm X", -75, 90, 1, 0);
	controls[RAISE_LEFT_ARM_X] = ModelerControl("Raise left arm X", -75, 90, 1, 0);
	controls[RAISE_RIGHT_ARM_Z] = ModelerControl("Raise right arm Z", 0, 60, 1, 0);
	controls[RAISE_LEFT_ARM_Z] = ModelerControl("Raise left arm Z", 0, 60, 1 ,0);
	controls[RAISE_RIGHT_LEG_X] = ModelerControl("Raise right leg X", -75, 90, 1, 0);
	controls[RAISE_LEFT_LEG_X] = ModelerControl("Raise left leg X", -75, 90, 1, 0);
	controls[RAISE_RIGHT_LEG_Z] = ModelerControl("Raise right leg Z", 0, 60, 1, 0);
	controls[RAISE_LEFT_LEG_Z] = ModelerControl("Raise left leg Z", 0, 60, 1, 0);
	controls[ROTATE_HEAD_X] = ModelerControl("Rotate head X", -45, 45, 1, 0);
	controls[ROTATE_HEAD_Y] = ModelerControl("Rotate head Y", -45, 45, 1, 0);
	controls[ROTATE_HEAD_Z] = ModelerControl("Rotate head Z", -45, 45, 1, 0);
	controls[LEFT_FOREARM_ROTATION] = ModelerControl("Rotate left forearm", -45, 45, 1, 0);
	controls[RIGHT_FOREARM_ROTATION] = ModelerControl("Rotate right forearm", -45, 45, 1, 0);
	controls[LEFT_ARM_LINK_MOVEMENT] = ModelerControl("Left arm link movement", 0, 40, 1, 0);
	controls[RIGHT_ARM_LINK_MOVEMENT] = ModelerControl("Right arm link movement", 0, 40, 1, 0);
	controls[CROUCH] = ModelerControl("Crouch", 0, 30, 1, 0);
	controls[LIGHT_INTENSITY] = ModelerControl("Light intensity", 0, 200, 5, 100);
	controls[SHOULDER2] = ModelerControl("Shoulder Type 2?", 0, 1, 1, 0);
	controls[LOWERARM2] = ModelerControl("Lower Arm Type 2?", 0, 1, 1, 0);
	controls[HEAD2] = ModelerControl("Head Type 2?", 0, 1, 1, 0);
	controls[UPPERBODY2] = ModelerControl("Upper Body Type 2?", 0, 1, 1, 0);
	controls[LOWERBODY2] = ModelerControl("Lower Body Type 2?", 0, 1, 1, 0);
	controls[THIGH2] = ModelerControl("Thigh Type 2?", 0, 1, 1, 0);
	controls[LOWERLEG2] = ModelerControl("Lower Leg Type 2?", 0, 1, 1, 0);
	controls[HAMMER] = ModelerControl("Use Hammer?", 0, 1, 1, 0);
	controls[TEXTURE_MAPPING] = ModelerControl("Texture mapping option", 0, 1, 1, 0);
	controls[FRAMEALL] = ModelerControl("Frame All?", 0, 1, 1, 0);
	controls[TENSION] = ModelerControl("Adjust tension", 0.1, 1, 0.1, 0.5);

	ParticleSystem* ps = new ParticleSystem();
	ModelerApplication::Instance()->SetParticleSystem(ps);
	ModelerApplication::Instance()->Init(&createGundamModel, controls, NUMCONTROLS);
	return ModelerApplication::Instance()->Run();
}
