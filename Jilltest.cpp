/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <SDL_ttf.h>
#include <sstream>
#include <iostream>
#include <fstream>
//#include <SpriteBox.h>
using namespace std;
//Level Dimensions
const int LEVEL_WIDTH = 5090;
const int LEVEL_HEIGHT = 2344;
//Screen dimension constants
const int SCREEN_WIDTH = 1300;
const int SCREEN_HEIGHT = 600;
const int DEBUG_X = 600;
const int DEBUG_Y = 20;
const int IDLE_FRAMES = 9;
const int RUN_FRAMES = 9;
const int JUMP_FRAMES = 12;
const int  SHOOT_FRAMES = 8;
const int LIGHT_KICK_FRAMES = 5;
const float JILL_SCALE = 1.5;
const float ENEMY_SCALE = 2.2;
const float BOX_SCALE = 1.5;
const float GRAVITY = 1;
const int FRAME_SIZE = 100;
const int ANIM_SPEED = 4;
const int JUMP_SPEED = 20;
const int NUM_BOXES = 3;
const int BULLET_SPEED = 15;
bool ControlState = false;
bool renderflag = false;
bool HitBoxFlag = false;
// Animation Variable.
// Note the sign Convention
struct BoxRect{
    float mX;
    float mY;
    float mW;
    float mH;
};
struct attackboxes{
    int Index;
    vector<BoxRect> AttackRects;
    };


vector<attackboxes>  CollisionBoxArray(string FileInput){
    vector<attackboxes> ReturnArray;
    attackboxes TempBoxes;
    BoxRect TempRect;
    ifstream InFile(FileInput);
    int temp = 0;
    int BoxIndex = 0;
    InFile >> temp;
    while(!InFile.eof()){
        InFile >> temp;
        if(temp != 1111){
            TempRect.mX = temp;
            for(int i = 0; i <3; i++){
                switch(i){
                case 0: InFile>> TempRect.mY; break;
                case 1: InFile>> TempRect.mH; break;
                case 2: InFile>> TempRect.mW; break;
                        }
                    }
                    TempBoxes.AttackRects.push_back(TempRect);
                }

        else{
            TempBoxes.Index = BoxIndex;
            BoxIndex++;
            ReturnArray.push_back(TempBoxes);
            TempBoxes.AttackRects.clear();

        }



            }
        return ReturnArray;
        }


enum Anim{
JILL_Walk,
JILL_Run,
JILL_Idle,
JILL_Dash,
Jill_Jump,
JILL_Shoot,
JILL_LIGHT_KICK,
Enemy_Idle,
Enemy_Shoot,
Enemy_Hit,
};

enum CollisionLineType{
    VERTICAL_LINE,
    HORIZONTAL_LINE,
    DIAGONAL_LINE
};

enum CollisionType{
UP_COLLISION,
DOWN_COLLISION,
RIGHT_COLLISION,
LEFT_COLLISION,

};

    enum Enemy_AI_State{
    AI_Walk_Left,
    AI_Walk_Right,
    AI_Walk_UP,
    AI_Walk_Down,
    AI_Idle
};


struct BulletCoord{
    float mX;
    float mY;
    bool flipped;
};

struct CollisionCoord{
    float mX;
    float mY;
};

class BulletHandler{
    public:
    BulletHandler();

    // Make and destroy functions
    // These will return true if they can succesfully create or destroy a bullet
    bool MakeBullet(float nX, float nY, bool IsFlipped);
    bool DestroyBullet(int index);
    vector<BulletCoord> GetBullets();
    BulletCoord GetBullet(int index);
    int NumBullets();
    void BulletX(int index, float deltaX);
   // void BulletY(int index, int deltaY);
    private:

    vector<BulletCoord> Bulletarray;
};

BulletHandler::BulletHandler(){


}

bool BulletHandler::MakeBullet(float nx, float ny, bool Flipped){
        //cout<<endl<<IsFlipped;
        BulletCoord TempBullet{nx,ny,Flipped};
        Bulletarray.push_back(TempBullet);


}

bool BulletHandler::DestroyBullet(int index){

        Bulletarray.erase(Bulletarray.begin()+index);

}


vector<BulletCoord> BulletHandler::GetBullets(){
        return Bulletarray;
}

BulletCoord BulletHandler::GetBullet(int index){

        return Bulletarray.at(index);

}

int BulletHandler::NumBullets(){

        return Bulletarray.size();

}

void BulletHandler::BulletX(int index, float deltaX){

    BulletCoord tempCoord{Bulletarray.at(index).mX+deltaX,Bulletarray.at(index).mY,Bulletarray.at(index).flipped};
   //cout<<endl<<"Bullet "<<index<<" has initial coord "<<Bulletarray.at(index).mX<<"it is changed to"<<tempCoord.mX;
    Bulletarray.erase(Bulletarray.begin()+index);
    Bulletarray.insert(Bulletarray.begin()+index,tempCoord );

}
//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		#ifdef _SDL_TTF_H
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );

		//Renders texture at given point
		void render( int x, int y, float scaler, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

class LTimer
{
    private:
    //The clock time when the timer started
    Uint32 mStartTicks;

    //The ticks stored when the timer was paused
    Uint32 mPausedTicks;

    //The timer status
    bool mPaused;
    bool mStarted;

    public:
    //Initializes variables
    LTimer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    Uint32 getTicks();

    //Checks the status of the timer
    bool isStarted();
    bool isPaused();
};


class RenderSprite{

public:

 virtual void render() = 0;
 virtual int getRenderPos() = 0;
float getPosX(){
        return mPosX;
    }
float getPosY(){
        return mPosY;
	}
protected:

//The X and Y offsets of the dot
		float mPosX, mPosY;

		//The velocity of the dot
		float mVelX, mVelY;

        //Vertical distance
        float VertDis;

        //Vertical Velocity
        float VertVel;

        int CposX;
        int CposY;


};

class CoverBox: public RenderSprite{

    public:

    CoverBox(float initX, float initY);

    void SetPos(float PosX, float PosY);

    void render();

    void SetCam( float cXpos, float cYpos);

    int getRenderPos(){

    return mPosY + 68;

    }

    private:

        SDL_Rect BoxRect;

};
class CollisionLine{
    public:
    // You declare the initial and Lengths of the Collision line
    // When you declare the class, you also specify if it is horizontal, vertical, or diagonal.

    CollisionLine(float BeginX, float BeginY, float InputLength,CollisionType InputType);

    CollisionLine(float BeginX, float BeginY, float InputLength, CollisionLineType InputLine, bool CollisionCondition);

    // Returns a bool if the line is out of bounds
    bool DidCollide(float CheckX, float CheckY, float CheckXNext, float CheckYNext);

    void RenderLine(SDL_Rect InputRect);

    CollisionType GetType();

    private:
    // The Beginning and ending coordinates

    CollisionCoord BeginCoord;
    // The type of line this is

    CollisionLineType ThisLine;

    // It can be either horizontal or diagonal
    CollisionType ThisType;
    // The Length of the line
    float LineLength;
    /* This boolean tells which side of the collision line you are
    For Vertical Lines
            True is right
            False is left
    For Diagonal Lines
            True is above
            False is Below
    For Horizontal Lines
            True is above
            False is below
    */
    bool CollisionCondition;

};


class Jill: public RenderSprite
{
    public:

        int framer();

		//The dimensions of the dot
		static constexpr  int JILL_WIDTH = 100;
		static constexpr   int JILL_HEIGHT = 100;

		//Maximum axis velocity of the dot
        float JILL_VEL = 10;

		//Initializes the variables
		Jill();


		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e,const  Uint8* KeyStates );

		//Moves the dot
		void move();
        // Checks for collisions

        CollisionType CollisionCheck(vector<CollisionLine*> InputCollisionVector);

        //
        void Logic();
		//Shows the dot on the screen
		void render();
        void HitBoxRender();
        void land();

		Anim GetAnim();

        void BoxCollide();

		// Position accessors in string format
        string getPosXString();
        string getPosYString();
        string getVertString();
//		getPosXString()

        //Set the Cameras
        void SetCam(float cXpos, float cYpos, SDL_Rect* InputRect);



        int getRenderPos(){
            return mPosY + JILL_HEIGHT;
        }

        void GetBoxes( CoverBox* InBoxes[] ){


        for (int i = 0; i<NUM_BOXES; i++){
        LevelBoxes[i] = InBoxes[i];
        }
        }


        vector<BulletCoord> GetJillBullets();

        void DestroyJillBullet(int index);

        private:

		/*
		//The X and Y offsets of the dot
		float mPosX, mPosY;

		//The velocity of the dot
		float mVelX, mVelY;

        //Vertical distance
        float VertDis;

        //Vertical Velocity
        float VertVel;
        */

        bool TriggerBool;

		// The Current Animation
		Anim JillAnim ;

		// FlipState
		bool IsFlipped;

        bool InAir;

		// Collision Detection
		SDL_Rect mCollider;

        SDL_Rect* Anim_Rect;
		// The animation frame
		int Frame;

        bool blinker;

        CoverBox* LevelBoxes[NUM_BOXES];

        SDL_Rect ShadowRect;

        float ShadowHeight;

        bool anim_indicator;

        BulletHandler JillBullets;

        vector<attackboxes> JillHitBoxes;


};



class Enemy: public RenderSprite
{
    public:

        int Framer();

		//The dimensions of the dot
		static constexpr  int ENEMY_WIDTH = 100;
		static constexpr   int ENEMY_HEIGHT = 100;

		//Maximum axis velocity of the dot
        const float ENEMY_WALK_VEL = 10;

		//Initializes the variables
		Enemy();


		//Takes key presses and adjusts the dot's velocity
	//	void handleEvent( SDL_Event& e,const  Uint8* KeyStates );

		//Moves the Enemy
		void move();

        void Logic();
		//Shows the dot on the screen
		void render();

		Anim GetAnim();

        //void BoxCollide();

		// Position accessors in string format
        string getPosXString();
        string getPosYString();
        string getVertString();
//		getPosXString()

        //Set the Cameras
        void SetCam(float cXpos, float cYpos, SDL_Rect* InputRect);



        int getRenderPos(){
            return mPosY + ENEMY_HEIGHT;
        }

        void GetBoxes( CoverBox* InBoxes[] ){


        for (int i = 0; i<NUM_BOXES; i++){
        LevelBoxes[i] = InBoxes[i];
        }
        }


        int EnemyLogic( vector<BulletCoord> PlayerBullets);

        void BoxCollide();
        private:

		/*
		//The X and Y offsets of the dot
		float mPosX, mPosY;

		//The velocity of the dot
		float mVelX, mVelY;

        //Vertical distance
        float VertDis;

        //Vertical Velocity
        float VertVel;
        */

        bool TriggerBool;

		// The Current Animation
		Anim EnemyAnim ;

		// FlipState
		bool IsFlipped;

        bool InAir;

		// Collision Detection
		SDL_Rect mCollider;

        SDL_Rect* Anim_Rect;
		// The animation frame
		int Frame;

        bool blinker;

        CoverBox* LevelBoxes[NUM_BOXES];

        SDL_Rect ShadowRect;

        Enemy_AI_State Current_AI_State;

        bool anim_indicator;

        BulletHandler EnemyBullets;

};

// This class will make make an object that will check if a coordinate has collided with it
// This will primarily be used so that sprites don't go outside the bounds of the level

void printF(char *c, int x, int y);

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL jmatthew@aip.org
void close();

//Box collision detector
bool checkCollision( SDL_Rect a, SDL_Rect b );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gJillTexture;
LTexture gShadowTexture;
LTexture gEnemyTexture;
LTexture gBoxTexture;
LTexture gBGTexture;
LTexture gTextTexture;
// SDL_Surface *DebugText = NULL;
//SDL_Color textColor = { 0, 0, 0 };
const int JILL_ANIMATION_FRAMES = IDLE_FRAMES + RUN_FRAMES + JUMP_FRAMES + SHOOT_FRAMES+LIGHT_KICK_FRAMES;
const int ENEMY_ANIMATION_FRAMES = 15;
SDL_Rect gJillClips[ JILL_ANIMATION_FRAMES+1];
SDL_Rect gEnemyClips[ ENEMY_ANIMATION_FRAMES+1];
SDL_Rect gBoxClips;
//Globally used font
TTF_Font *gFont = NULL;

//Surface

SDL_Surface* fontSurface;
SDL_Surface* screen;
SDL_Color TextColor = { 0, 0, 0, 0xFF };

SDL_Rect fontRect;
LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0xFF, 0, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}


	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y,float scaler, SDL_Rect* clip , double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w*scaler;
		renderQuad.h = clip->h*scaler;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

Enemy::Enemy()
{
    //Initialize the offsets
    mPosX = 1000;
    mPosY = 500;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    mCollider.w = ENEMY_WIDTH;
	mCollider.h = ENEMY_HEIGHT;

    //Initialize The Aniamtion
    EnemyAnim = Enemy_Idle;

    // Set the Flip state and the other booleans
    IsFlipped = false;

    InAir = false;

    Frame = 0;

            //Vertical distance
    VertDis = 0;

        //Vertical Velocity
    VertVel = 0;

    blinker = true;

      ShadowRect.x =   0;
      ShadowRect.y =  400;
      ShadowRect.w =  100;
      ShadowRect.h = 50;
    TriggerBool = false;


    anim_indicator = false;

    Current_AI_State = AI_Idle;
}
void Enemy::BoxCollide(){
   // bool onBox = false;
    float tempX = 0;
    float tempY = 0;
    int F_x1 = 0;
    int F_x2 = 0;
    int F_x12 = 0;
    int F_x22 = 0;
    int F_x13 = 0;
    int F_x23 = 0;
    int checkPosX = mPosX - CposX;
    int checkPosY = mPosY - CposY+150;
    int Width = 69*1.5;
    int Height = 37*1.5;
    for (int k = 0; k<NUM_BOXES; k++){
    tempX = LevelBoxes[k]->getPosX()-CposX;
    tempY = LevelBoxes[k]->getPosY()-CposY+67*1.5;

                        tempX = LevelBoxes[k]->getPosX()-CposX;
                        tempY = LevelBoxes[k]->getPosY()-CposY+67*1.5;
                        F_x1 = checkPosX+tempY-tempX;
                        F_x2 = checkPosX+tempY-tempX-Width;
                        F_x12 = checkPosX+150+tempY-tempX;
                        F_x22 = checkPosX+150+tempY-tempX-Width;
                        F_x13 = checkPosX+75+tempY-tempX;
                        F_x23 = checkPosX+75+tempY-tempX-Width;



					if((checkPosY<F_x1&&checkPosY>F_x2&&checkPosY>tempY&&checkPosY<tempY+Height||(checkPosY<F_x12&&checkPosY>F_x22&&checkPosY>tempY&&checkPosY<tempY+Height)||(checkPosY<F_x13&&checkPosY>F_x23&&checkPosY>tempY&&checkPosY<tempY+Height))){

                    mPosY -= mVelY;
                    mPosX -= mVelX+.71*mVelY;


                    return;
					}



    }

}
void Enemy::SetCam( float cXpos, float cYpos, SDL_Rect* InputRect){

    CposX = cXpos;
    CposY = cYpos;
    Anim_Rect = InputRect;
    blinker = !blinker;
}

void Enemy::render()
{
    int rendY = mPosY - VertDis - CposY;





if(blinker){
    gShadowTexture.render( mPosX - CposX+27, mPosY-CposY+110,1.5, &ShadowRect );

}


 /*   if(JillAnim == Jill_Jump){
        rendY -=30;
    }
*/

    if(IsFlipped == false){
     /*       if(JillAnim == Jill_Jump){
            gKunoTexture.render( mPosX - CposX, mPosY - CposY,JILL_SCALE, clip );
            }
            else{  */
            gEnemyTexture.render( mPosX - CposX, rendY,ENEMY_SCALE, Anim_Rect );

    }
    else if(IsFlipped == true){
    gEnemyTexture.render( mPosX - CposX, rendY,ENEMY_SCALE, Anim_Rect,0.0,NULL, SDL_FLIP_HORIZONTAL );
    }


/*
    if(anim_indicator){
      SDL_Rect fillRect = { 500, 100, 50, 50};
      SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );

      SDL_RenderFillRect( gRenderer, &fillRect );
    }
*/

        //render the bullets

    for(int i = 0 ; i < EnemyBullets.NumBullets(); i++){
                  SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
                  SDL_Rect fillRect = { EnemyBullets.GetBullet(i).mX-CposX, EnemyBullets.GetBullet(i).mY-CposY, 10, 10};
                  SDL_RenderFillRect( gRenderer, &fillRect );
    //              if(blinker){
                 // SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x67 );
                //  fillRect = { JillBullets.GetBullet(i).mX-CposX, JillBullets.GetBullet(i).mY-CposY+100, 5, 5};
       //           SDL_RenderFillRect( gRenderer, &fillRect );
 //   }

    }


    }

int Enemy::EnemyLogic( vector<BulletCoord> PlayerBullets){


    for(int i = 0; i< PlayerBullets.size(); i++){

                    // ATTN:
                    // Adjust the bullet collision here
                    if((PlayerBullets.at(i).mX+10>mPosX+10)&&(PlayerBullets.at(i).mX<mPosX+100)&&((PlayerBullets.at(i).mY>mPosY-5)&&(PlayerBullets.at(i).mY<mPosY+35))){

                      EnemyAnim = Enemy_Hit;
                        return i;
                    }

    }
  //  cout<<endl;
    return 9999;
}
int Enemy::Framer(){


       if(EnemyAnim == Enemy_Idle){

            Frame = 0;
       }
       else if( EnemyAnim == Enemy_Shoot){
       Frame++;
                if(Frame/ANIM_SPEED>3||(Frame/ANIM_SPEED)<1){
                        Frame =1*ANIM_SPEED;
                        }
             if(Frame/ANIM_SPEED == 3){
                    EnemyAnim = Enemy_Idle;

       }
        }
       else if( EnemyAnim == Enemy_Hit){
            Frame++;
             if(Frame/ANIM_SPEED>5||(Frame/ANIM_SPEED)<4){
                        Frame =4*ANIM_SPEED;
                        }
             if(Frame/ANIM_SPEED == 5){
                    EnemyAnim = Enemy_Idle;

       }

}
return Frame/ANIM_SPEED;
}
void Enemy::move(){

    switch(Current_AI_State){

       case AI_Walk_Down: mVelY += ENEMY_WALK_VEL; break;
       case AI_Walk_UP: mVelY -= ENEMY_WALK_VEL; break;
       case AI_Walk_Left: mVelX -= ENEMY_WALK_VEL; break;
       case AI_Walk_Right: mVelX += ENEMY_WALK_VEL; break;

    }


}

Jill::Jill()
{
    //Initialize the offsets
    mPosX = 30;
    mPosY = 2000;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    mCollider.w = JILL_WIDTH;
	mCollider.h = JILL_HEIGHT;

    //Initialize The Aniamtion
    JillAnim = JILL_Idle;

    // Set the Flip state and the other booleans
    IsFlipped = false;

    InAir = false;

    Frame = 0;

            //Vertical distance
    VertDis = 0;

        //Vertical Velocity
    VertVel = 0;

    blinker = true;

      ShadowRect.x =   0;
      ShadowRect.y =  400;
      ShadowRect.w =  100;
      ShadowRect.h = 50;
    TriggerBool = false;
    ShadowHeight = 0;

    anim_indicator = false;

    JillHitBoxes = CollisionBoxArray("input.txt");
}

int Jill::framer(){
                // This function determines how to animate Jill
                // If  you want to set the location of the animation
                // You must use the location* anim_speed

                if(JillAnim == JILL_Walk){


                Frame ++;
                if(Frame/ANIM_SPEED>17||(Frame/ANIM_SPEED)<9){
                    Frame =9*ANIM_SPEED;
                    }
				}
				else if(JillAnim == JILL_Idle){
                    Frame++;

                    if(Frame/ANIM_SPEED>8){
                        Frame = 0;
                    }
				}
				else if(JillAnim == Jill_Jump){
                    Frame++;
                    if((Frame/ANIM_SPEED)<18){
                    Frame =18*ANIM_SPEED;
                    }
				}
                else if (JillAnim == JILL_Shoot){
                        Frame++;
                        if(Frame/ANIM_SPEED>37||(Frame/ANIM_SPEED)<30){
                        Frame =30*ANIM_SPEED;
                        }

                        if(Frame/ANIM_SPEED == 36){
                    JillAnim = JILL_Idle;
                }
                }
                    else if (JillAnim == JILL_LIGHT_KICK){
                        Frame++;
                        if(Frame/ANIM_SPEED>41||(Frame/ANIM_SPEED)<37){
                        Frame =37*ANIM_SPEED;
                        }

                        if(Frame/ANIM_SPEED == 41){
                    JillAnim = JILL_Idle;
                    Frame = 0;
                }
            }
                      /*  else if(Frame/ANIM_SPEED>=29){
                        land();
                    }*/


				return Frame;

}

void Jill::Logic(){
      //create a bullet once the animation shows Jill Shooting
      if(Frame == 34*ANIM_SPEED ){

       // anim_indicator = true;
       if(!IsFlipped){
        JillBullets.MakeBullet(mPosX+150,mPosY+20,false);

       }
       else{
            JillBullets.MakeBullet(mPosX,mPosY+20,true);

       }
      }

       for(int i = 0; i < JillBullets.GetBullets().size();i++){

        if(JillBullets.GetBullet(i).mX>2000||JillBullets.GetBullet(i).mX<0){

            JillBullets.DestroyBullet(i);
        }


       }

}


void Jill::handleEvent( SDL_Event& e ,const Uint8* KeyStates  )
{

/*
    if (KeyStates[SDL_SCANCODE_DOWN]){

        mPosY += KUNO_VEL;
    }
*/


    //If a key was pressed

	if( e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= JILL_VEL; break;
            case SDLK_DOWN: mVelY += JILL_VEL; break;
            case SDLK_LEFT: mVelX -= JILL_VEL;if(mVelX<=0){IsFlipped = true;}   break;
            case SDLK_RIGHT: mVelX += JILL_VEL; if(mVelX>=0){IsFlipped = false;}  break;
            case SDLK_SPACE: if(JillAnim != Jill_Jump){JillAnim = Jill_Jump; VertVel = JUMP_SPEED; InAir = true;} break;
            case SDLK_s: if (JillAnim != JILL_Shoot){JillAnim = JILL_Shoot;} break;
            case SDLK_d: if (JillAnim != JILL_LIGHT_KICK){JillAnim = JILL_LIGHT_KICK;} break;
        }

    }


    //If a key was released


    if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {

            case SDLK_UP: mVelY += JILL_VEL; break;
            case SDLK_DOWN: mVelY -= JILL_VEL; break;
            case SDLK_LEFT: mVelX += JILL_VEL; break;
            case SDLK_RIGHT: mVelX -= JILL_VEL; break;


        }
    //}
    }

}

void Jill::move()
{

    //Move the dot left or right
    if(ControlState == true){
    mPosX += mVelX;//+mVelY*0.70710678118;
    mPosY += mVelY;//*0.70710678118;
    }
    else if (ControlState == false){
    mPosX += mVelX+mVelY*0.70710678118;
    mPosY += mVelY*0.70710678118;
    }
    //If the dot went too far to the left or right

    if( ( mPosX < 0 ) || ( mPosX + JILL_WIDTH > LEVEL_WIDTH ) )
    {
        //Move back
        mPosX -= mVelX;
    }


    //If the dot went too far up or down


    if( ( mPosY < 300 ) || ( mPosY + JILL_HEIGHT > LEVEL_HEIGHT ) )
    {

    //    onGround = true;

        //Move back
        mPosY -= mVelY;
        /*
        mVelY = 0;
        onGround = true;
        mVelX = 0;
    */
    }
    if(InAir){
    VertVel-=GRAVITY;
    }
    VertDis+=VertVel;






    if(VertDis<0){
            land();

    }




    if(((mVelX)!= 0||(mVelY)!= 0)&&!(InAir)&&JillAnim!=JILL_LIGHT_KICK){
        JillAnim = JILL_Walk;
    }
    //NOTE! This function will need to be changed everytime a new animation is added.
    // I meed to figure out a more elegant way to do this.
    else if((!InAir)&&(JillAnim!=JILL_Shoot)&&JillAnim!=JILL_LIGHT_KICK){
        JillAnim = JILL_Idle;
    }

    BoxCollide();
    // Iterate the bullets
    for(int i = 0; i<JillBullets.NumBullets(); i++){
        if(JillBullets.GetBullet(i).flipped == false){
        JillBullets.BulletX(i,BULLET_SPEED);

        }
        else if(JillBullets.GetBullet(i).flipped == true){
        JillBullets.BulletX(i,-BULLET_SPEED);

        }

    }



}
CollisionType Jill::CollisionCheck(vector<CollisionLine*> InputCollisionVector){

    // Must finish this later

    /*
    for(int i = 0; i<InputCollisionVector.size();i++){
    if(InputCollisionVector.at(0)->DidCollide(mPosX,mPosY+135,mVelX+mVelY*0.70710678118,mVelY*0.70710678118)){
    cout<<"jill is within the bounds of the line"<<endl;
            return.
        }
    }
    */
    return UP_COLLISION;
}
void Jill::render()
{

    // Set the Render Distance. This Takes into account the vertical offset (from jumping)

    int rendY = mPosY - VertDis - CposY;
    //this is a correction for the light kick animation
    int XOffset = 0;

    if(JillAnim == JILL_LIGHT_KICK && IsFlipped == true){
        XOffset = 43;
    }
    else{
        XOffset = 0;
    }


//blinker is a boolean that goes on and off
//This function renders the shadow
if(blinker){
    gJillTexture.render( mPosX - CposX, mPosY-CposY+110-ShadowHeight,JILL_SCALE, &ShadowRect );

}

//Offset for the jumping animation
    if(JillAnim == Jill_Jump){
        rendY -=30;
    }


    if(IsFlipped == false){
     /*       if(JillAnim == Jill_Jump){
            gKunoTexture.render( mPosX - CposX, mPosY - CposY,JILL_SCALE, clip );
            }
            else{  */
            gJillTexture.render( mPosX - CposX, rendY,JILL_SCALE, Anim_Rect );

    }
    else if(IsFlipped == true){
    gJillTexture.render( mPosX - CposX-XOffset, rendY,JILL_SCALE, Anim_Rect,0.0,NULL, SDL_FLIP_HORIZONTAL );
    }




        //render the bullets

    for(int i = 0 ; i < JillBullets.NumBullets(); i++){
                  SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
                  SDL_Rect fillRect = { JillBullets.GetBullet(i).mX-CposX, JillBullets.GetBullet(i).mY-CposY, 10, 10};
                  SDL_RenderFillRect( gRenderer, &fillRect );
    //              if(blinker){
                 // SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x67 );
                //  fillRect = { JillBullets.GetBullet(i).mX-CposX, JillBullets.GetBullet(i).mY-CposY+100, 5, 5};
       //           SDL_RenderFillRect( gRenderer, &fillRect );
 //   }

    }
    if(HitBoxFlag){

    HitBoxRender();
    }


    }

void Jill::HitBoxRender(){
      int ScreenPosX = mPosX-CposX;
      int ScreenPosY = mPosY-CposY;
      float BoxX = 0;
      float BoxY = 0;
      float BoxW = 0;
      float BoxH = 0;
      int BoxFrame = Frame/ANIM_SPEED;
      for(int i = 0; i<JillHitBoxes.at(BoxFrame).AttackRects.size();i++){
      if (IsFlipped == false){
      //cout<<Frame/ANIM_SPEED<<endl;
      BoxX = JillHitBoxes.at(BoxFrame).AttackRects.at(i).mX*JILL_SCALE+ScreenPosX;
      BoxY = JillHitBoxes.at(BoxFrame).AttackRects.at(i).mY*JILL_SCALE+ScreenPosY;
      BoxW = JillHitBoxes.at(BoxFrame).AttackRects.at(i).mW*JILL_SCALE;
      BoxH = JillHitBoxes.at(BoxFrame).AttackRects.at(i).mH*JILL_SCALE;
      }
      else if(IsFlipped == true){
      BoxX = ScreenPosX+150-JillHitBoxes.at(BoxFrame).AttackRects.at(i).mX*JILL_SCALE-JillHitBoxes.at(BoxFrame).AttackRects.at(0).mW*JILL_SCALE;
      BoxY = JillHitBoxes.at(BoxFrame).AttackRects.at(i).mY*JILL_SCALE+ScreenPosY;
      BoxW = JillHitBoxes.at(BoxFrame).AttackRects.at(i).mW*JILL_SCALE;
      BoxH = JillHitBoxes.at(BoxFrame).AttackRects.at(i).mH*JILL_SCALE;
      }

      SDL_Rect fillRect = { BoxX, BoxY-VertDis, BoxH, BoxW};
      SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
      SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0x7D );

      SDL_RenderFillRect( gRenderer, &fillRect );
      }
}
Anim Jill::GetAnim(){

return JillAnim;

}

string Jill::getPosXString(){

        char Jbuffer[20];
        int temp = mPosX;
        snprintf(Jbuffer, 20, "%d", temp);
        return Jbuffer;
}

string Jill::getPosYString(){

        char Jbuffer[20];
        int temp = mPosY;
        snprintf(Jbuffer, 20, "%d", temp);

        return Jbuffer;
}

string Jill::getVertString(){

        char Jbuffer[20];
        int temp = VertDis;
        snprintf(Jbuffer, 20, "%d", temp);

        return Jbuffer;
}


void Jill::land(){
    VertDis = 0;
    VertVel = 0;
   // JillAnim = JILL_Idle;
    InAir = false;

}

void Jill::SetCam( float cXpos, float cYpos, SDL_Rect* InputRect){

    CposX = cXpos;
    CposY = cYpos;
    Anim_Rect = InputRect;

}

void Jill::BoxCollide(){
   // bool onBox = false;
blinker = !blinker;
    float tempX = 0;
    float tempY = 0;
    int F_x1 = 0;
    int F_x2 = 0;
    int F_x12 = 0;
    int F_x22 = 0;
    int F_x13 = 0;
    int F_x23 = 0;
    int checkPosX = mPosX - CposX;
    int checkPosY = mPosY - CposY+150;
    int Width = 69*1.5;
    int Height = 37*1.5;
    for (int k = 0; k<NUM_BOXES; k++){
    tempX = LevelBoxes[k]->getPosX()-CposX;
    tempY = LevelBoxes[k]->getPosY()-CposY+67*1.5;
    	/*	for( int j = tempY; j < tempY+Height; j++)
				{
					for(int i = tempX; i<Height+tempX+Width; i++){
                        F_x1 = i+tempY-tempX;
                        F_x2 = i+tempY-tempX-Width;
					if(j<F_x1&&j>F_x2&&j>tempY&&j<tempY+Height){

					SDL_RenderDrawPoint( gRenderer, i, j );


					}
					}
				} */
                        tempX = LevelBoxes[k]->getPosX()-CposX;
                        tempY = LevelBoxes[k]->getPosY()-CposY+67*1.5;
                        F_x1 = checkPosX+tempY-tempX;
                        F_x2 = checkPosX+tempY-tempX-Width;
                        F_x12 = checkPosX+150+tempY-tempX;
                        F_x22 = checkPosX+150+tempY-tempX-Width;
                        F_x13 = checkPosX+75+tempY-tempX;
                        F_x23 = checkPosX+75+tempY-tempX-Width;



					if((checkPosY<F_x1&&checkPosY>F_x2&&checkPosY>tempY&&checkPosY<tempY+Height||(checkPosY<F_x12&&checkPosY>F_x22&&checkPosY>tempY&&checkPosY<tempY+Height)||(checkPosY<F_x13&&checkPosY>F_x23&&checkPosY>tempY&&checkPosY<tempY+Height))){
             //       SDL_Rect fillRect = { 500, 100, 50, 50};
			//	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );

			//	SDL_RenderFillRect( gRenderer, &fillRect );

                   // return true;
                    if(VertDis<67){
                    mPosY -= mVelY;
                    mPosX -= mVelX+.71*mVelY;

                            }
                    if(VertDis>67){

                        ShadowHeight = 85;
                    }



                    if(VertDis>67&&(VertDis+VertVel)<67){
                        VertDis-=VertVel;
                        VertVel = 0;
                        InAir = false;
                        }

                    //return;
                    return;
					}

                    //    InAir = true;


    }
    ShadowHeight = 0;
    InAir = true;
}

vector<BulletCoord> Jill::GetJillBullets(){

   return JillBullets.GetBullets();

}
CoverBox::CoverBox(float initX, float initY){

    BoxRect.x = 0;
    BoxRect.y = 0;
    BoxRect.h = 107;
    BoxRect.w = 107;

    mPosX = initX;
    mPosY = initY;
    VertDis = 0;
    VertVel = 0;
    mVelX = 0;
    mVelY = 0;
    CposX = 0;
    CposY =0;
}

void Jill::DestroyJillBullet(int index){

       if(index !=9999){
            JillBullets.DestroyBullet(index);
       }
}

void CoverBox::render(){

//gBoxTexture.render( mPosX - CposX, mPosY - CposY,BOX_SCALE, &BoxRect,0.0,NULL, SDL_FLIP_HORIZONTAL );


//if((0<(mPosY - CposY))&&((mPosY - CposY)<SCREEN_HEIGHT)&&(0<(mPosX - CposX))&&((mPosX - CposX)<SCREEN_WIDTH+107)){
gBoxTexture.render(mPosX - CposX, mPosY - CposY, BOX_SCALE, &BoxRect);

//}
}

void CoverBox::SetCam( float cXpos, float cYpos){

    CposX = cXpos;
    CposY = cYpos;

}



LTimer::LTimer()
{
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}

void LTimer::start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void LTimer::stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void LTimer::pause()
{
    //If the timer is running and isn't already paused
    if( mStarted && !mPaused )
    {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
    }
}

void LTimer::unpause()
{
    //If the timer is running and paused
    if( mStarted && mPaused )
    {
        //Unpause the timer
        mPaused = false;

        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;

        //Reset the paused ticks
        mPausedTicks = 0;
    }
}

Uint32 LTimer::getTicks()
{
	//The actual timer time
	Uint32 time = 0;

    //If the timer is running
    if( mStarted )
    {
        //If the timer is paused
        if( mPaused )
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

bool LTimer::isStarted()
{
	//Timer is running and paused or unpaused
    return mStarted;
}

bool LTimer::isPaused()
{
	//Timer is running and paused
    return mPaused && mStarted;
}

 CollisionLine::CollisionLine(float BeginX, float BeginY, float InputLength,CollisionType InputType){

ThisType = InputType;

LineLength = InputLength;

BeginCoord.mX = BeginX;
BeginCoord.mY = BeginY;

 }

 CollisionLine::CollisionLine(float BeginX, float BeginY, float InputLength, CollisionLineType InputLine, bool CollisionCondition){

ThisLine = InputLine;

LineLength = InputLength;

BeginCoord.mX = BeginX;
BeginCoord.mY = BeginY;

 }





bool CollisionLine::DidCollide(float CheckX, float  CheckY, float CheckXNext, float CheckYNext){
/*
if(ThisLine == HORIZONTAL_LINE && CollisionCondition  == true){

    if(BeginCoord.mX<CheckX && CheckX < (BeginCoord.mX + LineLength) && CheckY < BeginCoord.mY){

        if
        return true;
    }

}

if(ThisLine == HORIZONTAL_LINE && CollisionCondition  == false){
    if(BeginCoord.mX<CheckX && CheckX < (BeginCoord.mX + LineLength) && CheckY > BeginCoord.mY){

        return true;
    }

    return false;
}

if(ThisLine == DIAGONAL_LINE &&CollisionCondition == true){

}
if(ThisLine == DIAGONAL_LINE &&CollisionCondition == false){

}
*/

return false;
}



void CollisionLine::RenderLine(SDL_Rect InputRect){
  SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x67 );
  if(ThisLine == HORIZONTAL_LINE){
 SDL_RenderDrawLine(gRenderer,(BeginCoord.mX - InputRect.x  ),(BeginCoord.mY - InputRect.y ),(BeginCoord.mX - InputRect.x + LineLength),(BeginCoord.mY - InputRect.y ));
  }
  else if (ThisLine == DIAGONAL_LINE){
 SDL_RenderDrawLine(gRenderer,(BeginCoord.mX - InputRect.x  ),(BeginCoord.mY - InputRect.y ),(BeginCoord.mX+LineLength*.7071067- InputRect.x),(BeginCoord.mY+LineLength*.7071067 - InputRect.y ));
  }

}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Enable VSync
		if( !SDL_SetHint( SDL_HINT_RENDER_VSYNC, "1" ) )
		{
			printf( "Warning: VSync not enabled!" );
		}

		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Jill2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
						 //Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{

	//Loading success flag
	bool success = true;


	// Kuno addition: load the kunoichi walk strip instead of the dot
	if( !gJillTexture.loadFromFile( "./png/JillSprites.png" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}
		if( !gBGTexture.loadFromFile( "./png/BackGround.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}

        if( !gBoxTexture.loadFromFile( "./png/Box.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
        if(!gShadowTexture.loadFromFile("./png/Shadow.png")){
        printf( "Failed to load background texture!\n" );
		success = false;
        }
        if(!gEnemyTexture.loadFromFile("./png/Enemy.png")){
        printf( "Failed to load background texture!\n" );
		success = false;
        }
    else
	{
    //    int Frame_Subtotal = 0;
		//Set sprite clips
		// This is done in an array.
		for(int i = 0; i<9;i++){
		gJillClips[ i ].x =   i*FRAME_SIZE;
		gJillClips[ i ].y =   0;
		gJillClips[ i ].w =  FRAME_SIZE;
		gJillClips[ i ].h = FRAME_SIZE;

		}
		//Frame_Subtotal +=  IDLE_FRAMES;
		for(int i = 9; i < 18; i++){
          gJillClips[ i ].x =   (i-9)*FRAME_SIZE;
          gJillClips[ i ].y =   FRAME_SIZE;
          gJillClips[ i ].w =  FRAME_SIZE;
          gJillClips[ i ].h = FRAME_SIZE;
		}
        for(int i = 18; i <30; i++){
          gJillClips[ i ].x =   (i-18)*FRAME_SIZE;
          gJillClips[ i ].y =   2*FRAME_SIZE;
          gJillClips[ i ].w =  FRAME_SIZE;
          gJillClips[ i ].h = FRAME_SIZE+20;
        }
        for(int i = 30; i <37 ;i++){
          gJillClips[ i ].x =   (i-30)*FRAME_SIZE;
          gJillClips[ i ].y =   500;
          gJillClips[ i ].w =  FRAME_SIZE;
          gJillClips[ i ].h = FRAME_SIZE;
            }
        for(int i = 37; i <41 ;i++){
          gJillClips[ i ].x =   (i-37)*130;
          gJillClips[ i ].y =   600;
          gJillClips[ i ].w =  FRAME_SIZE+30;
          gJillClips[ i ].h = FRAME_SIZE;
            }
          //Define the rectangles for the Boxes
          gBoxClips.x = 0;
          gBoxClips.y = 0;
          gBoxClips.w = 107;
          gBoxClips.h = 107;
          //Define the rectangles for the Enemy
          gEnemyClips[0].x = 0;
          gEnemyClips[0].y = 0;
          gEnemyClips[0].w = 80;
          gEnemyClips[0].h = 72;

          gEnemyClips[1].x = 0;
          gEnemyClips[1].y = 80;
          gEnemyClips[1].w = 80;
          gEnemyClips[1].h = 72;

          gEnemyClips[2].x = 80;
          gEnemyClips[2].y = 80; SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x67 );
          gEnemyClips[2].w = 90;
          gEnemyClips[2].h = 72;

          gEnemyClips[3].x =165;
          gEnemyClips[3].y = 80;
          gEnemyClips[3].w = 80;
          gEnemyClips[3].h = 72;

          gEnemyClips[4].x = 0;
          gEnemyClips[4].y = 180;
          gEnemyClips[4].w = 80;
          gEnemyClips[4].h = 66;

          gEnemyClips[5].x =80;
          gEnemyClips[5].y = 180;
          gEnemyClips[5].w = 90;
          gEnemyClips[5].h = 66;
	}
	return success;
}

bool checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

void close()
{
	//Free loaded images
	gJillTexture.free();
    gBGTexture.free();
    gBoxTexture.free();
    gTextTexture.free();
    	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
    bool debug = false;
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{


			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

            SDL_Color TextColor = { 0, 0, 0, 0xFF };

         //   gTextTexture.loadFromRenderedText( "geese", TextColor );

           // int frame = 11;

			//The jill that will be moving around on the screen
			Jill jill;
            // the Enemy
            Enemy enemy1;
            // Our Collision lines
            CollisionLine  LineOne(0,2093,1620,HORIZONTAL_LINE, false);
            CollisionLine  LineTwo(390,863,1740,DIAGONAL_LINE, true);
            CollisionLine  LineThree(390,863,1132,HORIZONTAL_LINE, true);
            CollisionLine  LineFour(1038,375,690,DIAGONAL_LINE,true);
            CollisionLine  LineFive(1038,375,585,HORIZONTAL_LINE,true);
            CollisionLine  LineSix(1615,375,690,DIAGONAL_LINE,true);
            CollisionLine  LineSeven(2101,863,1550,HORIZONTAL_LINE, true);
            // The vector that stores our collision lines
            vector<CollisionLine*> CollisionVector;
            CollisionVector.push_back(&LineOne);
            CollisionVector.push_back(&LineTwo);
            CollisionVector.push_back(&LineThree);
            CollisionVector.push_back(&LineFour);
            CollisionVector.push_back(&LineFive);
            CollisionVector.push_back(&LineSix);
            CollisionVector.push_back(&LineSeven);

            //
            int BulletToDestroy;
            CoverBox Box1(400,350);
            CoverBox Box2(800,450);
            CoverBox Box3(600,550);
            // Our array of sprites pointers
            vector<RenderSprite*> RenderVector;
            RenderVector.push_back(&jill);
            RenderVector.push_back(&Box1);
            RenderVector.push_back(&Box2);
            RenderVector.push_back(&Box3);
            RenderVector.push_back(&enemy1);http://www.bbc.com/news/uk-29223984
            int NumRenderSprites = RenderVector.size();
            RenderSprite* RenderArray[RenderVector.size()];
            for(int i = 0; i<NumRenderSprites; i++){
                RenderArray[i] = RenderVector.at(i);
            }
            CoverBox* BoxArray[NUM_BOXES];

            BoxArray[0] = &Box1;
            BoxArray[1] = &Box2;
            BoxArray[2] = &Box3;
            jill.GetBoxes( BoxArray);
            //SDL camera
            SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };


			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
                    else if( e.type == SDL_KEYDOWN)
					{
						//Handle backspace
						if(  e.key.keysym.sym == SDLK_d && SDL_GetModState() & KMOD_CTRL )
						{

                        debug = !debug;

						}
							if(  e.key.keysym.sym == SDLK_f && SDL_GetModState() & KMOD_CTRL )
						{

                        ControlState = !ControlState;

						}
						if(  e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
						{

                        HitBoxFlag = !HitBoxFlag;

						}

					}
					//Handle input for the dot
					// The kunoichi

					const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );

					jill.handleEvent( e, currentKeyStates );
				}

            //  collision handling
             /* if(  LineOne.DidCollide(jill.getPosX(), jill.getPosY()+130)){


              }
              else{
                cout<<"jill is not within the bounds of the line"<<endl;
              }
				*/
                jill.CollisionCheck(CollisionVector);
				jill.move();

				jill.Logic();


                    jill.DestroyJillBullet(enemy1.EnemyLogic(jill.GetJillBullets()));

                //Center the camera over the dot
				camera.x = ( jill.getPosX() + Jill::JILL_WIDTH / 2 ) - SCREEN_WIDTH / 2;
				camera.y = ( jill.getPosY() + Jill::JILL_WIDTH/ 2 ) - SCREEN_HEIGHT / 2;

				//Keep the camera in bounds
				if( camera.x < 0 )
				{
					camera.x = 0;
				}
				if( camera.y < 0 )
				{
					camera.y = 0;
				}
				if( camera.x > LEVEL_WIDTH - camera.w )
				{
					camera.x = LEVEL_WIDTH - camera.w;
				}
				if( camera.y > LEVEL_HEIGHT - camera.h )
				{
					camera.y = LEVEL_HEIGHT - camera.h;
				}

                  // Display infor... I don't know if I should be loading every frame



                jill.SetCam(camera.x, camera.y, &gJillClips[jill.framer()/ANIM_SPEED]);
                Box1.SetCam(camera.x, camera.y);
                Box2.SetCam(camera.x, camera.y);
                Box3.SetCam(camera.x, camera.y);
                enemy1.SetCam(camera.x, camera.y, &gEnemyClips[enemy1.Framer()]);
				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );
               //  cout<<camera.y<<endl;
				//Render objects
                gBGTexture.render( 0, 0,1, &camera );



                for(int i = 0; i< 5; i++){
                    for(int j = 0; j<5; j++){
                            int I = RenderArray[i]->getRenderPos();
                            int J = RenderArray[j]->getRenderPos();
                        //if((RectsP[j]->getPosY())<(RectsP[j]->getPosY())){
                            if(J>I){
                        RenderSprite* temp = RenderArray[j];
                        RenderArray[j] = RenderArray[i];
                        RenderArray[i] = temp;
                        //std::cout<<"must switch";
                        }

                    }
                }

                for(int i = 0; i<5 ; i ++){
                RenderArray[i]->render();

                }
           //     enemy1.render();
                //Box1.render();

                        if(debug){

                gFont = TTF_OpenFont( "./png/lazy.ttf", 20 );
                gTextTexture.loadFromRenderedText(jill.getPosXString()+" "+jill.getPosYString()+" "+jill.getVertString(), TextColor);
                gTextTexture.render( DEBUG_X, DEBUG_Y,1 );

                        }
                        else{
                            gFont = NULL;
                        }

                if(true){


                // SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x67 );
                    /* SDL_Rect fillRect = { 500, 100, 50, 50};
                  SDL_RenderFillRect( gRenderer, &fillRect );

                    */

                //LineOne.RenderLine(camera);
                for(int i = 0; i<CollisionVector.size();i++){
                    CollisionVector.at(i)->RenderLine(camera);
                }
                }


				//Update screen
				SDL_RenderPresent( gRenderer );




			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
