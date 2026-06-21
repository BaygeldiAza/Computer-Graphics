// *****************************************************************************************
// KXC354 - Computer Graphics & Animation - 2014
// Assignment 1 & 2
// 3D Engine Code
// *****************************************************************************************
//
// Author: Tony Gray
//
// platform.cpp
//
// contains MacOS and Windows implementations of platform-specific functions



// -----------------------------------------------------------------------------------------
// includes
// -----------------------------------------------------------------------------------------
#include "prefix.h"
#include "platform.h"
#include "constants.h"
#include "utility.h"



// -----------------------------------------------------------------------------------------
// local platform-specific "globals"
// -----------------------------------------------------------------------------------------
#ifdef __APPLE__ // Mac-specific includes
unsigned long		tStartTime;				// the time that the animation was started
unsigned long		tPauseStarted;			// the time the pause started (for the current pause only)
unsigned long		tPausedTime;			// the total time that the animation was paused
UnsignedWide		theTime;
#endif

#ifdef _WIN32
time_t				tStartTime;				// starting time of clock
time_t				tPauseStarted;			// the time the pause started (for the current pause only)
time_t				tPausedTime;			// the total time that the animation was paused
#endif


// irrklang sound engine globals
irrklang::ISoundEngine* irrkEngine;
map<long, string> soundFilenameMap;
map<long, irrklang::ISound*> soundPtrMap;




// -----------------------------------------------------------------------------------------
// external functions
// -----------------------------------------------------------------------------------------
extern void animateScene(float time, long frame);
extern void initScene();
extern void drawScene();




// -----------------------------------------------------------------------------------------
// external globals
// -----------------------------------------------------------------------------------------
extern ProgramMode gProgramMode;
extern long	gAnimatedFrameCounter;	
extern long	gRenderedFrameCounter;	
extern float gCurrentFrameTime;	




// -------------------------------------------------------------------------------------
// pathToResourceDirectory
// -------------------------------------------------------------------------------------
// returns the path to the resources directory as a C++ string type - this is
// where textures, music and other data files needed at runtime are stored
// for MacOS X, this is the Resources folder in the application bundle
// for Windows, this is the "resources" folder alongside the executable
// -------------------------------------------------------------------------------------

string pathToResourceDirectory()
{
#ifdef __APPLE__
	char path[MAXPATHLEN];
	string tempString;
	CFBundleRef bundle;
	CFURLRef resDirURL;

	path[0] = '\0';
	
	bundle = CFBundleGetMainBundle();
	if (!bundle) return string ("");

	resDirURL = CFBundleCopyResourcesDirectoryURL(bundle);
	if (!resDirURL) return string ("");

	CFURLGetFileSystemRepresentation(resDirURL, TRUE, (UInt8 *)path, MAXPATHLEN);
	CFRelease(resDirURL);
	
	// put a trailing slash on the path
	tempString = string(path);
	tempString.append("/");
	return tempString;
#endif

#ifdef _WIN32
	return string("resources/");
#endif
}





// -----------------------------------------------------------------------------------------
// platformInitialiase()
// -----------------------------------------------------------------------------------------
// performs any platform-specific initialisation when the program first starts up
// used to initialise irrKlang for sound playback
// -----------------------------------------------------------------------------------------

void platformInitialise(void)
{
    // start the sound engine with default parameters
    irrkEngine = irrklang::createIrrKlangDevice();
} 





// -----------------------------------------------------------------------------------------
// platformWindup()
// -----------------------------------------------------------------------------------------
// performs any platform-specific deinitialisation when the program terminates
// used to shut down irrKlang
// -----------------------------------------------------------------------------------------
void platformWindup(void)
{  
    // delete sound engine
    irrkEngine->drop();
} 





// -----------------------------------------------------------------------------------------
// resetAnimationTimer()
// -----------------------------------------------------------------------------------------
// sets the animation timer to zero seconds
// Microseconds is a MacOS function that returns the current time in microseconds
// as an unsigned wide value (an unsigned wide is a struct, that contains two longs)
//
// To reset the animation timer, we grab the low long, which is all that's really
// important to us) and save this in the "tStartTime" variable.  All timing functions
// are then based on the difference between this initial snagged value, and the current
// value returned by Microseconds.
// -----------------------------------------------------------------------------------------

void resetAnimationTimer(void)
{
#ifdef __APPLE__
	Microseconds (&theTime);			// get the current "time"
	tStartTime = theTime.lo;			// snag the low long
	tPausedTime = 0;					// so far, there have been no pauses
#endif

#ifdef _WIN32
	tStartTime = clock();				// snag the time
	tPausedTime = 0;					// so far, there have been no pauses
#endif
} 




// -----------------------------------------------------------------------------------------
// pauseAnimationTimer()
// -----------------------------------------------------------------------------------------
// to "pause" the timer, we record the current time in microseconds (in tPauseStarted)
// -----------------------------------------------------------------------------------------

void pauseAnimationTimer(void)
{
#ifdef __APPLE__
	UnsignedWide theTime;
	Microseconds (&theTime);
	tPauseStarted = theTime.lo;
#endif

#ifdef _WIN32
	tPauseStarted = clock();
#endif
} 




// -----------------------------------------------------------------------------------------
// unpauseAnimationTimer()
// -----------------------------------------------------------------------------------------
// to "unpause" the timer, we get the time in microseconds again, and subtract
// this from the time at which we started the pause - the result is the time
// that we were paused - this value is added to the overall paused amount
// -----------------------------------------------------------------------------------------

void unpauseAnimationTimer(void)
{
#ifdef __APPLE__
	UnsignedWide theTime;
	Microseconds (&theTime);
	unsigned long thisPause = (theTime.lo - tPauseStarted);
	tPausedTime += thisPause;
#endif

#ifdef _WIN32
	time_t thisPause = (clock() - tPauseStarted);
	tPausedTime += thisPause;
#endif
} 




// -----------------------------------------------------------------------------------------
// getTimeSinceAnimationStarted()
// -----------------------------------------------------------------------------------------
// Returns the time in seconds since the animation was started.
// We subtract the total paused time from this to account for paused time.
// Note that this final value is divided  by one million (Mac) or one thousand (Windows)
// so that the returned time is a float representing seconds.
// -----------------------------------------------------------------------------------------

float getTimeSinceAnimationStarted(void)
{
	static float retValue;
	
	if (gProgramMode == kpmFinished)					// if we're done, return the last animated timer
		return retValue;								// value (so the timer doesn't increment on a redraw)
		
#ifdef __APPLE__
	UnsignedWide theTime;
	Microseconds (&theTime);
	retValue = (theTime.lo - tStartTime - tPausedTime) / 1000000.0;
#endif

#ifdef _WIN32
	retValue = (clock() - tStartTime - tPausedTime) / 1000.0;
#endif

	return retValue;
} 





// -----------------------------------------------------------------------------------------
// platformLoadSoundFile()
// -----------------------------------------------------------------------------------------
// under QuickTime, this used to load a sound file and return a reference to it
// with irrKlang, it loads the sound "paused" and returns a long that represents the sound
//
// we keep two maps to the sound - one records the sound file names, the other
// records pointers to the matching irrklang::ISound object. (The latter may change from
// time to time, as irrKlang doesn't support restarting sounds - when they've stopped,
// they have to be recreated, and they may get a different object pointer).
// -----------------------------------------------------------------------------------------

long platformLoadSoundFile (string theFilename)
{
    static long nextSoundID = 0;
    
	// get the full path to the sound file
	string fileLocation;
	fileLocation = pathToResourceDirectory() + theFilename;
    
    // make a new unique sound ID
    nextSoundID++;
    
    // store the sound filename in a map, so we can retrieve it later
    soundFilenameMap[nextSoundID] = fileLocation;
    
    // load up the sound - nonlooped, startPaused, tracked, and keep a reference to it in the other map
    soundPtrMap[nextSoundID] = irrkEngine->play2D(fileLocation.c_str(), false, true, true);
    
	// return the reference to the sound
	return nextSoundID;

}





// -----------------------------------------------------------------------------------------
// platformUnloadSound()
// -----------------------------------------------------------------------------------------
// under QuickTime, this disposed of the reference to the sound file
// with irrKlang, it drops the sound, and deletes all references to it from the maps
// -----------------------------------------------------------------------------------------

void platformUnloadSound (long theSoundID)
{
    soundPtrMap[theSoundID]->drop();
    soundPtrMap.erase(theSoundID);
    soundFilenameMap.erase(theSoundID);
}




// -----------------------------------------------------------------------------------------
// platformStartSound()
// -----------------------------------------------------------------------------------------
// starts playing a sound
// -----------------------------------------------------------------------------------------

void platformStartSound (long theSoundID)
{
    if (soundPtrMap[theSoundID]->isFinished())
    {
        // reload the sound in case it's needed again
        soundPtrMap[theSoundID]->drop();
        soundPtrMap[theSoundID] = irrkEngine->play2D(soundFilenameMap[theSoundID].c_str(), false, false, true);
    }
    else
    {
        soundPtrMap[theSoundID]->setIsPaused(false);
    }
}





// -----------------------------------------------------------------------------------------
// platformStopSound()
// -----------------------------------------------------------------------------------------

void platformStopSound (long theSoundID)
{
    if (soundPtrMap[theSoundID]->isFinished())
    {
        // reload the sound in case it's needed again
        soundPtrMap[theSoundID]->drop();
        soundPtrMap[theSoundID] = irrkEngine->play2D(soundFilenameMap[theSoundID].c_str(), false, true, true);
    }
    else
    {
        soundPtrMap[theSoundID]->setIsPaused(true);
    }
}




// -----------------------------------------------------------------------------------------
// platformSoundLevel()
// -----------------------------------------------------------------------------------------
// the first parameter is the sound reference number
// the second parameter is a floating point value between 0.0 and 1.0.  1.0 represents the
// maximum sound level
// -----------------------------------------------------------------------------------------

void platformSoundLevel (long theSoundID, float theLevel)
{
    if (!soundPtrMap[theSoundID]->isFinished())
    {
        soundPtrMap[theSoundID]->setVolume(theLevel);
    }
}




// -----------------------------------------------------------------------------------------
// platformSetSoundPlayPoint()
// -----------------------------------------------------------------------------------------
// the first parameter is the sound reference number
// the second parameter is a floating point value that repesents the time in seconds that
// the playpoint should be moved to
// -----------------------------------------------------------------------------------------

void platformSetSoundPlayPoint (long theSoundID, float theTime)
{
    if (soundPtrMap[theSoundID]->isFinished())
    {
        // reload the sound and assume it was playing
        soundPtrMap[theSoundID]->drop();
        soundPtrMap[theSoundID] = irrkEngine->play2D(soundFilenameMap[theSoundID].c_str(), false, false, true);
    }
    soundPtrMap[theSoundID]->setPlayPosition(theTime / 1000.0);
}




// -----------------------------------------------------------------------------------------
// platformProcessSound()
// -----------------------------------------------------------------------------------------
// called periodically when the animation is running to give time to sound processing
// -----------------------------------------------------------------------------------------

void platformProcessSound()
{
    irrkEngine->update();
}




// -----------------------------------------------------------------------------------------
// platformLoadTextureFile()
// -----------------------------------------------------------------------------------------
// loads a (file-based) image into a new OpenGL texture using the SOIL library
// -----------------------------------------------------------------------------------------

GLuint platformLoadTextureFile(string theFilename, bool generateMipmaps, int *width, int *height)
{
	GLuint					textureName;				// the "name" by which OpenGL knows the texture

	// get the full path to the texture file
	string fileLocation;
	fileLocation = pathToResourceDirectory() + theFilename;

    textureName = SOIL_load_OGL_texture(fileLocation.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_MULTIPLY_ALPHA);

    // pass the loaded texture size back out to the caller
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, (GLint *)width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, (GLint *)height);

	return textureName;
}


