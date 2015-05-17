

#include "D:\\Движки\freeglut-2.8.1\include\GL\freeglut.h"
#include "LogSystem.h"
#include "Font.h"
#include <sstream>


//#include <SFML\System.hpp>
//
//#ifdef _DEBUG
//      
//#pragma comment(lib,"sfml-system-d.lib")
//#else
//
//    #pragma comment(lib,"sfml-system.lib")
//  
//#endif

struct Vector2f
{
	float x;
	float y;
} ;

Font *ourtext;
Vector2f *pNewCoords;

int N=30,M=20;
int Scale=25;

int w = Scale*N;
int h = Scale*M;
bool turn(false);

int dir(1),num = 4;
int Score(0);
HDC  hDC;
const int StartTime(100);
int TimeDraw(0);
int lifes(0);
int level(1);
float speedSnake(0.9f);
float stepAnim(0.1f);
int FructOnLevel[2]={5,7};

 float static deltaTime(60);


const int LOGO = 1;
const int GAME = 2;
const int LEVELCOMPLETE = 3;
const int GAMEOVER = 4;
const int STARTMENU = 5;
const int ENDGAME   = 6;

bool StartGame(false);
bool GameOver(false);
int  State(LOGO);

struct 
{ int x;
  int y;}  s[100];


class Fructs
{ 

int x,y;
float scale;

public:

	Fructs()
	{
		scale = 25.0f;
		//this->New();
	}
	
	int GetX() const
	{
		return x;
	}
	int GetY() const
	{
		return y;
	}
	void SetX(const int x_)
	{
		this->x = x_;
	}
	void SetY(const int y_)
	{
		this->y = y_;
	}
    void New()
    {  
		this->SetX(rand() % 15);
		this->SetY(rand() % 15);

		/*for(int i=0; i<num; i++)
			if ( this->GetX() == s[i].x && this->GetY() == s[i].y) { this->New();}*/

	}
	
    void DrawApple()
   {   glColor4f(0,0.6,0,0.6);
       glRectf(x*scale,y*scale,(x+1)*scale,(y+1)*scale); 
	}

};

Fructs *pFruct;

void Tick()
 {
	

    for (int i = num; i>0; --i)
	 {
		 s[i].x = s[i-1].x;
	     s[i].y = s[i-1].y;
	}

	//CorectFrustPosition();

	if (dir==0) s[0].y+= (speedSnake + stepAnim);  
	if (dir==1) s[0].x-= (speedSnake + stepAnim);        
	if (dir==2) s[0].x+= (speedSnake + stepAnim);       
	if (dir==3) s[0].y-=  (speedSnake + stepAnim);       

  for (int i=0;i<10;i++)
	  if ((s[0].x == pFruct->GetX()) && ( s[0].y == pFruct->GetY())) 
      {
		  num++;
		  Score += 100;
		  pFruct->New();
		  //m[0].New();
	  }
 
	//if (s[0].x>N) {   }  if (s[0].x<0) {  }

  //if (s[0].y>M) dir=3;  if (s[0].y<0) dir=0;
 
	/* если столкнулись голова и хвост */
 for (int i=1;i<num;i++)
	 if (s[0].x==s[i].x && s[0].y==s[i].y)  {num=4; Score = 0;}
 }
void DrawSnake()
 {  
	 glColor4f(1.0f,1.0f,0.0f,0.9f);

  for (int i=0;i<num;i++)
		glRectf(s[i].x * Scale, s[i].y * Scale, (s[i].x + 0.9) * Scale, ( s[i].y + 0.9) * Scale);
 
 }
void DrawField()
  {
   glColor3f(0.0,0.7,0.0);
   glBegin(GL_LINES);

    for (int i=0; i<w; i+=Scale) 
      {
		  glVertex2f(i,0); glVertex2f(i,h);
	  }
    for (int j=0; j<h; j+=Scale)  
      {
		  glVertex2f(0,j); glVertex2f(w,j);
	  }

    glEnd();
}
void ResetGame()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0,0,0.6,0.3);
	//glColor4f(0,0,1,0);
}
bool IsOutBorder()
{
	const int leftWall   = 0;
	const int rightWall  = 30;
	const int topWall    = 20;
	const int bottomWall = 0;

	if ( s[0].x < leftWall   ) 
	{ 
		dir = 2;
		return true;
	}
	if ( s[0].x > rightWall  )  
	{ 
		dir = 1;
		return true;
	}
	if ( s[0].y > topWall    ) 
	{ 
		dir = 3;
		return true; 
	}
	if ( s[0].y < bottomWall ) 
	{
		dir = 0;
		return true;
	}

	return false;
}
std::string GetStr(const char* mess,const float value,const int valueInt=0)
{
	std::string ScoreFloat="";
	std::stringstream StrStream;

	StrStream<<value;
	StrStream>>ScoreFloat;
	ScoreFloat = mess + ScoreFloat ;
	 
	return ScoreFloat;
}
void DrawLife(const int life)
{

	
	
     glColor4f(0.5f,0.0f,0.9f,1.9f);

	 std::string lev = GetStr("Time: ",deltaTime);
	
	 ourtext->put(550, 460, 1.0f, lev.c_str());

	 std::string score = GetStr("Score: ",Score);
	
	 ourtext->put(550, 435, 1.0f, score.c_str());

	 std::string life_ = GetStr("Life: ",life);
	
	 ourtext->put(550, 410, 1.0f, life_.c_str());

	       /* std::string str_fps = GetStr(" ",abs((num-4)-FructOnLevel[0]));
		    glColor4f(1.0f,1.0f,0.3f,0.9f);
	        ourtext->put(350, 485, 1.0f, str_fps.c_str());*/

	/* if (level == 2)
	 {
		    std::string str_fps = GetStr(" ",abs((num-4)-FructOnLevel[1]));
		    glColor4f(1.0f,1.0f,0.3f,0.9f);
	        ourtext->put(350, 485, 1.0f, str_fps.c_str());
	 }*/
}
void DrawFruct()
{

	pFruct->DrawApple();
   
}
void ShowGameOver()
{
	 ResetGame();
     glColor4f(1.0f,0.0f,0.0f,0.9f);

	 std::string ScoreStr = GetStr("Last Score: ",Score);
	 ourtext->put(300, 350, 1.0f, ScoreStr.c_str());
	
	 ourtext->put(300, 300, 1.0f, "End Level, retry again");

	 glColor4f(0.0f,1.0f,0.0f,0.9f);
	 ourtext->put(300, 250, 1.0f, "Press <g> to Start or <Esc> to Exit...");

	 deltaTime = 60;
	 
	
	
}
inline GLvoid box(float x1, float y1, float x2, float y2)
{
	glBegin(GL_QUADS);	
        	glTexCoord2f(0.0f, 0.0f); glVertex2f(x1,y1);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(x2,y1);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(x2,y2);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(x1,y2);
			
	glEnd();
}
void ShowMenu()
{
	glEnable(GL_BLEND);
	float FonMenuX(200.0f),FonMenuY(90.0f),FonMenuX2(550.0f),FonMenuY2(450.0f);
	float WidthMenu(350.0f);
	float HeightMenu(360.0f);
	float StartButtonX(220.0f); float StartButtonY(350.0f); float StartButtonX2(530.0f); float StartButtonY2(400.0f);
	float WidthButtonStart(310.0f); float HeightButtonStart(50.0f);

    	/*-------------------
	       фон рамка меню
	       ------------------*/
	glColor4f(0.5f,0.5f,0.5f,0.5f);
	glRectf(FonMenuX, FonMenuY, FonMenuX + WidthMenu, FonMenuY + HeightMenu);

	/*glColor3f(0.0f,0.0f,0.0f);
	std::string ScoreStr = GetStr("Last Score: ",Score);
	ourtext->put(290,420,1.0f,ScoreStr.c_str());*/

	/*-------------------
	       кнопка старт
	       ------------------*/

	glColor4f(0.0f,1.0f,0.0f,0.3f);
	glRectf(StartButtonX, StartButtonY-20, StartButtonX + WidthButtonStart, StartButtonY + HeightButtonStart); 

	/*-------------------
	       кнопка старт
	       ------------------*/


	/*-------------------
	       текст для кнопки старт
	       ------------------*/
	
     glColor4f(0.0f,0.0f,0.3f,0.9f);
	
     ourtext->put(290, 365, 1.0f, "Start Game");
	 
     
}
bool TargetInButton(int x, int y)
{
	float xmin(220.0f); float xmax(530.0f); float ymin(350.0f); float ymax(400.0f);

	if ( x >= xmin && x <= xmax) 
		{
			if ( ((y+260.0f)  >= ymin) && ((y +260.0f) <= ymax))
			{
               return true;
			}
	}

	return false;
}
bool IsLevelComplete()
{
	if ( (num - 4) == FructOnLevel[0]  )
	{
      // State = LEVELCOMPLETE;
	   level += 1;
	   //FructOnLevel[0] = 7;
	   TimeDraw = 30;
	   glColor3f(0.0f,1.0f,0.0f);
	   ourtext->put(300,300,1.0f,"Level is complete...");
	   //LastScore = FructOnLevel[0] * 100;
	   FructOnLevel[0] = rand() % 10;
	 //  ShadowFructs = FructOnLevel[0];
       return true;
	   
	}

	//if ( (num - 4) == FructOnLevel[1]  )
	//{
 //     // State = LEVELCOMPLETE;
	//   level = 3;
	//   //FructOnLevel[0] = 7;
	//     glColor3f(0.0f,1.0f,0.0f);
	//   ourtext->put(300,300,1.0f,"Level is complete...");
	//   TimeDraw = 80;
	//   LastScore = FructOnLevel[1] * 100;
 //      return true;
	//   
	//}
	return false;
}
void display() 
{
	

			ResetGame();

			if (deltaTime <= 0.1f) { State = GAMEOVER; ShowGameOver(); }   /* поставаить делта тайм опять на 60сек*/ 

	        if (State == GameOver) { ResetGame();  ShowGameOver();   }
		//	else { IsLevelComplete();}

			if (State == GameOver) { ShowGameOver(); }
	       
			if ( State == GAME)
			{
               DrawLife(lifes);
               Tick();
               DrawSnake(); 
	           DrawFruct();
			}

	        if (IsOutBorder())
			{ 
				if (lifes == 0 )
				{
			         State = GAMEOVER;
				     ResetGame();
					 ShowGameOver();                          
				}									
													                                   
			}  

			if (IsOutBorder()) 
			{ 
				if (lifes != 0 ) { lifes -= 1;  Score = 0; TimeDraw = StartTime; }
			}
               
			

			if (State == STARTMENU) { ShowMenu();}
         glutSwapBuffers();	
}
void ProcessStates()
{
	switch (State)
	{
	//case LOGO :      ShowLogo();     break;
	case GAME :      display();      break;
	case GAMEOVER :  ShowGameOver(); break;
	case STARTMENU : ShowMenu();     break;
	
	}

	glutSwapBuffers();
}
void getkeys_down(unsigned char key,int x,int y)
{
	switch ( key )
	{

		// нужно использовать русскую раскладку проще действительно переключить раскладку
	case 'w' : if (dir != 3 && turn) {dir = 0; turn = false;}  break; //ClearScreen(); //Data::id = 1; glutTimerFunc(Data::IntervalTimer,glutTimer,Data::id); break; 
	case 'a' : if (dir != 2 && turn) {dir = 1; turn = false;}  break; //ClearScreen(); //Data::id = 1; glutTimerFunc(Data::IntervalTimer,glutTimer,Data::id); break; 
	case 'd' : if (dir != 1 && turn) {dir = 2; turn = false;}  break; //ClearScreen(); //Data::id = 1; glutTimerFunc(Data::IntervalTimer,glutTimer,Data::id); break; 
	case 's' : if (dir != 0 && turn) {dir = 3; turn = false;}  break; 
	case 'g' : State = GAME; lifes = 3; break;
	case 'm' : State = STARTMENU; ShowMenu(); break;
	case '1' : glBlendFunc(GL_SRC_ALPHA, GL_ONE); break;
	case '2' : glBlendFunc(GL_ONE, GL_ONE); break;
	case '3' : glBlendFunc(GL_DST_COLOR, GL_ZERO); break;
	case '4' : glBlendFunc(GL_SRC_COLOR, GL_ONE); break;
	case '5' : glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE); break;
    case 'b' : glEnable(GL_BLEND); glBlendFunc(GL_ZERO,  GL_SRC_ALPHA_SATURATE); break;
	case 't' : TimeDraw -= 10.0f; break;

	case 27  : exit(0); break; //State = STARTMENU; ResetGame(); lifes = 3; break;  //exit(0); break;
}
}
void Mouse(int state,int button,int x,int y)
{
	using namespace LogSpace;
	CLog logFile;

	if ( button == GLUT_LEFT_BUTTON  && state == GLUT_DOWN)
	{
      if (TargetInButton(x, y))
	{
                    // logFile<< LogSpace::color("gray") << LogSpace::color("green") << "Is target in button!!! " <<"x "<< x<<"y "<<y << LogSpace::endl;
			  
		State = GAME;
	}

	}

	
}
void Drawtimer(int = 1)
{
	turn = true;
	deltaTime -= 0.1f;
	glutPostRedisplay();
	//GetCoordsTime(pNewCoords);
	glutTimerFunc(TimeDraw, Drawtimer, 1);
 
}
void StateTimer(int = 1 )
{
	ProcessStates();
	glutTimerFunc(1,StateTimer,1);
}

HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;
HINSTANCE hPrevInstance;

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR pCmdLine,int nShowCmd)
{
	int argc=0; char** argv=0;

	srand(time(0));

	pFruct = new Fructs();
	pFruct->New();
	pNewCoords = new Vector2f;
	
	 s[0].x=10;
	 s[0].y=10;

	 State = STARTMENU;
	 lifes = 3;
	 TimeDraw = StartTime;

glutInit(&argc, argv);
glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_RGB );
glutInitWindowSize (w, h);
glutCreateWindow ("Snake v 1.0"); 
hWnd = FindWindow(L"GLUT",L"OpenGL Application");
hDC = GetDC(hWnd);
ourtext = new Font(hDC,"times",10,15,30);

glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(0,w,0,h);

glutKeyboardFunc(getkeys_down);
glutDisplayFunc (display);

glutMouseFunc(Mouse);

glutTimerFunc(TimeDraw,Drawtimer,1);


glutMainLoop();
}
