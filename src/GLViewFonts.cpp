#include "GLViewFonts.h"

#include "WorldList.h" //This is where we place all of our WOs
#include "ManagerOpenGLState.h" //We can change OpenGL State attributes with this
#include "Axes.h" //We can set Axes to on/off with this
#include "PhysicsEngineODE.h"

//Different WO used by this module
#include "WO.h"
#include "WOStatic.h"
#include "WOStaticPlane.h"
#include "WOStaticTrimesh.h"
#include "WOTrimesh.h"
#include "WOHumanCyborg.h"
#include "WOHumanCal3DPaladin.h"
#include "WOWayPointSpherical.h"
#include "WOLight.h"
#include "WOSkyBox.h"
#include "WOCar1970sBeater.h"
#include "Camera.h"
#include "CameraStandard.h"
#include "CameraChaseActorSmooth.h"
#include "CameraChaseActorAbsNormal.h"
#include "CameraChaseActorRelNormal.h"
#include "Model.h"
#include "ModelDataShared.h"
#include "ModelMesh.h"
#include "ModelMeshDataShared.h"
#include "ModelMeshSkin.h"
#include "WONVStaticPlane.h"
#include "WONVPhysX.h"
#include "WONVDynSphere.h"
#include "AftrGLRendererBase.h"

//If we want to use way points, we need to include this.
#include "FontsWayPoints.h"
#include <WOFTGLString.h>
#include <MGLFTGLString.h>

using namespace Aftr;

GLViewFonts* GLViewFonts::New( const std::vector< std::string >& args )
{
   GLViewFonts* glv = new GLViewFonts( args );
   glv->init( Aftr::GRAVITY, Vector( 0, 0, -1.0f ), "aftr.conf", PHYSICS_ENGINE_TYPE::petODE );
   glv->onCreate();
   return glv;
}


GLViewFonts::GLViewFonts( const std::vector< std::string >& args ) : GLView( args )
{
   //Initialize any member variables that need to be used inside of LoadMap() here.
   //Note: At this point, the Managers are not yet initialized. The Engine initialization
   //occurs immediately after this method returns (see GLViewFonts::New() for
   //reference). Then the engine invoke's GLView::loadMap() for this module.
   //After loadMap() returns, GLView::onCreate is finally invoked.

   //The order of execution of a module startup:
   //GLView::New() is invoked:
   //    calls GLView::init()
   //       calls GLView::loadMap() (as well as initializing the engine's Managers)
   //    calls GLView::onCreate()

   //GLViewFonts::onCreate() is invoked after this module's LoadMap() is completed.
}


void GLViewFonts::onCreate()
{
   //GLViewFonts::onCreate() is invoked after this module's LoadMap() is completed.
   //At this point, all the managers are initialized. That is, the engine is fully initialized.

   if( this->pe != NULL )
   {
      //optionally, change gravity direction and magnitude here
      //The user could load these values from the module's aftr.conf
      this->pe->setGravityNormalizedVector( Vector( 0,0,-1.0f ) );
      this->pe->setGravityScalar( Aftr::GRAVITY );
   }
   this->setActorChaseType( STANDARDEZNAV ); //Default is STANDARDEZNAV mode
   //this->setNumPhysicsStepsPerRender( 0 ); //pause physics engine on start up; will remain paused till set to 1
   
   std::string port = ManagerEnvironmentConfiguration::getVariableValue("NetServerListenPort");
   std::cout << "Listening on port " << port << std::endl;
   if (port != "12683") {
	   this->client = NetMessengerClient::New("127.0.0.1", "12683");
   }
   else {
	   this->client = NetMessengerClient::New("127.0.0.1", "12684");
   }
   loadFontMessage();
}


GLViewFonts::~GLViewFonts()
{
   //Implicitly calls GLView::~GLView()
}


void GLViewFonts::updateWorld()
{
   GLView::updateWorld(); //Just call the parent's update world first.
                          //If you want to add additional functionality, do it after
                          //this call.
   
}


void GLViewFonts::onResizeWindow( GLsizei width, GLsizei height )
{
   GLView::onResizeWindow( width, height ); //call parent's resize method.
}


void GLViewFonts::onMouseDown( const SDL_MouseButtonEvent& e )
{
   GLView::onMouseDown( e );
}


void GLViewFonts::onMouseUp( const SDL_MouseButtonEvent& e )
{
   GLView::onMouseUp( e );
}


void GLViewFonts::onMouseMove( const SDL_MouseMotionEvent& e )
{
   GLView::onMouseMove( e );
}


void GLViewFonts::onKeyDown( const SDL_KeyboardEvent& key )
{
   GLView::onKeyDown( key );
   if( key.keysym.sym == SDLK_0 )
      this->setNumPhysicsStepsPerRender( 1 );

   if( key.keysym.sym == SDLK_1 )
   {

   }
}


void GLViewFonts::onKeyUp( const SDL_KeyboardEvent& key )
{
   GLView::onKeyUp( key );
}


void Aftr::GLViewFonts::loadMap()
{
   this->worldLst = new WorldList(); //WorldList is a 'smart' vector that is used to store WO*'s
   this->actorLst = new WorldList();
   this->netLst = new WorldList();

   ManagerOpenGLState::GL_CLIPPING_PLANE = 1000.0;
   ManagerOpenGLState::GL_NEAR_PLANE = 0.1f;
   ManagerOpenGLState::enableFrustumCulling = false;
   Axes::isVisible = true;
   this->glRenderer->isUsingShadowMapping( false ); //set to TRUE to enable shadow mapping, must be using GL 3.2+

   this->cam->setPosition( 15,15,10 );

   std::string shinyRedPlasticCube( ManagerEnvironmentConfiguration::getSMM() + "/models/cube4x4x4redShinyPlastic_pp.wrl" );
   std::string wheeledCar( ManagerEnvironmentConfiguration::getSMM() + "/models/rcx_treads.wrl" );
   std::string grass( ManagerEnvironmentConfiguration::getSMM() + "/models/grassFloor400x400_pp.wrl" );
   std::string human( ManagerEnvironmentConfiguration::getSMM() + "/models/human_chest.wrl" );
   
   //SkyBox Textures readily available
   std::vector< std::string > skyBoxImageNames; //vector to store texture paths
   skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_lemon_lime+6.jpg" );

   float ga = 0.1f; //Global Ambient Light level for this module
   ManagerLight::setGlobalAmbientLight( aftrColor4f( ga, ga, ga, 1.0f ) );
   WOLight* light = WOLight::New();
   light->isDirectionalLight( true );
   light->setPosition( Vector( 0, 0, 100 ) );
   //Set the light's display matrix such that it casts light in a direction parallel to the -z axis (ie, downwards as though it was "high noon")
   //for shadow mapping to work, this->glRenderer->isUsingShadowMapping( true ), must be invoked.
   light->getModel()->setDisplayMatrix( Mat4::rotateIdentityMat( { 0, 1, 0 }, 90.0f * Aftr::DEGtoRAD ) );
   light->setLabel( "Light" );
   worldLst->push_back( light );

   //Create the SkyBox
   WO* wo = WOSkyBox::New( skyBoxImageNames.at( 0 ), this->getCameraPtrPtr() );
   wo->setPosition( Vector( 0,0,0 ) );
   wo->setLabel( "Sky Box" );
   wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
   worldLst->push_back( wo );

   ////Create the infinite grass plane (the floor)
   wo = WO::New( grass, Vector( 1, 1, 1 ), MESH_SHADING_TYPE::mstFLAT );
   wo->setPosition( Vector( 0, 0, 0 ) );
   wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
   ModelMeshSkin& grassSkin = wo->getModel()->getModelDataShared()->getModelMeshes().at( 0 )->getSkins().at( 0 );
   grassSkin.getMultiTextureSet().at( 0 )->setTextureRepeats( 5.0f );
   grassSkin.setAmbient( aftrColor4f( 0.4f, 0.4f, 0.4f, 1.0f ) ); //Color of object when it is not in any light
   grassSkin.setDiffuse( aftrColor4f( 1.0f, 1.0f, 1.0f, 1.0f ) ); //Diffuse color components (ie, matte shading color of this object)
   grassSkin.setSpecular( aftrColor4f( 0.4f, 0.4f, 0.4f, 1.0f ) ); //Specular color component (ie, how "shiney" it is)
   grassSkin.setSpecularCoefficient( 10 ); // How "sharp" are the specular highlights (bigger is sharper, 1000 is very sharp, 10 is very dull)
   wo->setLabel( "Grass" );
   worldLst->push_back( wo );

   createFontsWayPoints();
}


void GLViewFonts::createFontsWayPoints()
{
   // Create a waypoint with a radius of 3, a frequency of 1.5 seconds, activated by GLView's camera, and is visible.
   WayPointParametersBase params(this);
   params.frequency = 0010;
   params.useCamera = true;
   params.visible = true;
   this->wayPt = WOWP1::New( params, 3 );
   this->wayPt->setPosition( Vector( 50, 0, 3 ) );
   worldLst->push_back( this->wayPt );
}

void GLViewFonts::updateMessage(std::string newMessage) {
	this->message->setText(newMessage);
}

void GLViewFonts::loadFontMessage() {
	std::cout << "Loading font message..\n";
	std::string font("../mm/fonts/evogria/Evogria.ttf");
	this->message = WOFTGLString::New(font, 12);
	this->message->setText("Total targets hit: 0");
	this->message->getModelT<MGLFTGLString>()->setFontColor(aftrColor4f(0.0f, 1.0f, 1.0f, 1.0f));
	this->message->getModelT<MGLFTGLString>()->setSize(30, 10);
	Mat4 pose;
	pose = Mat4::rotateIdentityMat(Vector(1, 0, 0), Aftr::PI / 2);
	message->getModel()->setDisplayMatrix(pose);
	this->message->setPosition(0.0, 0.0, 25.0);
	worldLst->push_back(this->message);
}

NetMessengerClient* GLViewFonts::getClient() {
	return this->client;
}
