#pragma once

#include "GLView.h"
#include "FontsWayPoints.h"
#include "NetModuleMessage.h"
#include <NetMessengerClient.h>

namespace Aftr
{
   class Camera;
   class WOFTGLString;
   class MGLFTGLString;
   class WOGUILabel;

/**
   \class GLViewFonts
   \author Scott Nykl 
   \brief A child of an abstract GLView. This class is the top-most manager of the module.

   Read \see GLView for important constructor and init information.

   \see GLView

    \{
*/

class GLViewFonts : public GLView
{

public:
   static GLViewFonts* New( const std::vector< std::string >& outArgs );
   virtual ~GLViewFonts();
   virtual void updateWorld(); ///< Called once per frame
   virtual void loadMap(); ///< Called once at startup to build this module's scene
   virtual void createFontsWayPoints();
   virtual void onResizeWindow( GLsizei width, GLsizei height );
   virtual void onMouseDown( const SDL_MouseButtonEvent& e );
   virtual void onMouseUp( const SDL_MouseButtonEvent& e );
   virtual void onMouseMove( const SDL_MouseMotionEvent& e );
   virtual void onKeyDown( const SDL_KeyboardEvent& key );
   virtual void onKeyUp( const SDL_KeyboardEvent& key );

   void updateMessage(std::string newMessage);
   void loadFontMessage();
   NetMessengerClient* getClient();

protected:
   GLViewFonts( const std::vector< std::string >& args );
   virtual void onCreate();  
   WOFTGLString* message;
   WOWayPointSpherical* wayPt;
   NetMessengerClient* client;
};

/** \} */

} //namespace Aftr
