#ifndef _MODELER_GLOBALS_H
#define _MODELER_GLOBALS_H

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502
#endif

// This is a list of the controls for the SampleModel
// We'll use these constants to access the values 
// of the controls from the user interface.
enum SampleModelControls
{ 
	DETAIL,
	XPOS, YPOS, ZPOS, ROTATE, ROTATE_UPPER_BODY, 
	RAISE_RIGHT_ARM_X, RAISE_LEFT_ARM_X, RAISE_RIGHT_ARM_Z, RAISE_LEFT_ARM_Z,
	RAISE_RIGHT_LEG_X, RAISE_LEFT_LEG_X, RAISE_RIGHT_LEG_Z, RAISE_LEFT_LEG_Z,
	ROTATE_HEAD_X, ROTATE_HEAD_Y, ROTATE_HEAD_Z, 
	LEFT_FOREARM_ROTATION, RIGHT_FOREARM_ROTATION, 
	LEFT_ARM_LINK_MOVEMENT, RIGHT_ARM_LINK_MOVEMENT, CROUCH,
	LIGHT_INTENSITY,
	HEAD2, UPPERBODY2, LOWERBODY2,
	HAMMER,
	SHOULDER2, LOWERARM2,
	THIGH2, LOWERLEG2, 
	TEXTURE_MAPPING,
	FRAMEALL,
	NUMCONTROLS
};

// Colors
#define COLOR_RED		1.0f, 0.0f, 0.0f
#define COLOR_YELLOW		1.0f, 0.65f, 0.0f
#define COLOR_GREEN		0.0f, 1.0f, 0.0f
#define COLOR_BLUE		0.0f, 0.0f, 1.0f
#define COLOR_WHITE		1.0f, 1.0f, 1.0f
#define COLOR_BLACK		0.0f, 0.0f, 0.0f
#define COLOR_BROWN		204.0/255.0 , 102.0/255.0, 0.0f

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))

#endif