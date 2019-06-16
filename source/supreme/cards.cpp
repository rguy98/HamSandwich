#include "cards.h"
#include "mgldraw.h"
#include "control.h"
#include "display.h"
#include "player.h"
#include "music.h"
#include "dialogbits.h"
#include "textdialog.h"
#include "playlist.h"
#include "nameentry.h"
#include "yesnodialog.h"
#include "recordbook.h"
#include "shop.h"
#include "lsdir.h"
#if __linux__
#include <unistd.h>
#endif

static byte *backgd;
static sprite_set_t *plSpr;
static sprite_set_t *backSpr;
static sprite_set_t *frontSpr;
static char msBright,msDBright;
static int msx,msy;
static int page;

#define PBTN_HEIGHT	19
#define BTN_NEXTPG	1
#define BTN_PREVPG	2
#define BTN_EXITPG	3
#define MAX_PAGES	3

typedef struct cardButton_t
{
	int x,y;
	int wid;
	char txt[32];
	byte id;
} cardButton_t;

static cardButton_t btn[]={
	{20,72,122,"Next Page",BTN_NEXTPG},
	{20,102,122,"Previous Page",BTN_PREVPG},
	{20,132,122,"Exit",BTN_EXITPG},
};
#define NUM_CARD_BTNS	3

//init
void InitCardMenu(MGLDraw *mgl)
{
	int i;
	msBright=0;
	msDBright=1;

	mgl->LoadBMP("graphics/profmenu.bmp");
	backgd=(byte *)malloc(640*480);
	plSpr=new sprite_set_t("graphics/pause.jsp");
	backSpr=new sprite_set_t("expraphics/monsbacks.jsp");
	frontSpr=new sprite_set_t("expgraphics/monstercards.jsp");

	for(i=0;i<480;i++)
		memcpy(&backgd[i*640],&mgl->GetScreen()[i*mgl->GetWidth()],640);
}

//update
byte UpdateCardMenu(int *lastTime,MGLDraw *mgl)
{
	int i,j;
	char k;
	
	if (page==0)
	page=1;

	mgl->GetMouse(&msx,&msy);

	if(*lastTime>TIME_PER_FRAME*5)
		*lastTime=TIME_PER_FRAME*5;

	k=mgl->LastKeyPressed();

	while(*lastTime>=TIME_PER_FRAME)
	{
		msBright+=msDBright;
		if(msBright>10)
			msDBright=-1;
		if(msBright<-2)
			msDBright=1;

		mgl->Process();
		*lastTime-=TIME_PER_FRAME;
	}
	
	if(mgl->MouseTap())
	{
		for(i=0;i<NUM_CARD_BTNS;i++)
		{
			if(PointInRect(msx,msy,btn[i].x,btn[i].y,btn[i].x+btn[i].wid,btn[i].y+PBTN_HEIGHT))
			{
				MakeNormalSound(SND_MENUSELECT);
				switch(btn[i].id)
				{
					case BTN_NONE:
						break;
					case BTN_NEXTPG:
						if(page<MAX_PAGES)
						page++;
						break;
					case BTN_PREVPG:
						if(page>0)
						page--;
						break;
					case BTN_EXITPG:
						return 1;
						break;
				}
			}
		}
	}
	
	if(k==27)//pressing ESC exits to menu
	return 1;

	return 0;
}

//render buttons
void RenderCardButton(int x,int y,int wid,char *txt,MGLDraw *mgl)
{
	if(PointInRect(msx,msy,x,y,x+wid,y+PBTN_HEIGHT))
	{
		mgl->Box(x,y,x+wid,y+PBTN_HEIGHT,32+31);
		mgl->FillBox(x+1,y+1,x+wid-1,y+PBTN_HEIGHT-1,32+8);
	}
	else
		mgl->Box(x,y,x+wid,y+PBTN_HEIGHT,32+16);

	PrintGlow(x+2,y+3,txt,0,2);
}

//render the menu!
void RenderCardMenu(MGLDraw *mgl)
{
	int i;
	int msx2,msy2;
	char s[64];
	static char yesno[][5]={"Off","On"};
	
	for(i=0;i<480;i++)
		memcpy(&mgl->GetScreen()[i*mgl->GetWidth()],&backgd[i*640],640);

	for(i=0;i<NUM_CARD_BTNS;i++)
	{
			RenderCardButton(btn[i].x,btn[i].y,btn[i].wid,btn[i].txt,mgl);
	}
	

	PrintGlow(20,16,"Monster Cards!",0,0);
	//what page?
	sprintf(s,"Page %d/%d",page,MAX_PAGES);
	PrintGlow(320,48,s,0,2);
	
	SetSpriteConstraints(13,13,627,467);
	DrawBox(290,72,590,448,1*32+16);
	DrawBox(20,168,260,448,1*32+16);
	msx2=msx;
	msy2=msy;
	if(msx2<13)
		msx2=13;
	if(msy2<13)
		msy2=13;
	if(msx2>622)
		msx2=622;
	if(msy2>462)
		msy2=462;
	plSpr->GetSprite(0)->DrawBright(msx2,msy2,mgl,msBright/2);
	SetSpriteConstraints(0,0,639,479);
}

//exit
void ExitCardMenu(void)
{
	free(backgd);
	delete plSpr;
	delete frontSpr;
	delete backSpr;
}

void CardMenu(MGLDraw *mgl)
{
	byte done=0;
	int lastTime=1;

	InitCardMenu(mgl);
	PlaySongForce("SWC_07_foresthub.ogg");
	
	while(!done)
	{
		lastTime+=TimeLength();
		StartClock();
		done=UpdateCardMenu(&lastTime,mgl);
		RenderCardMenu(mgl);
		
		mgl->Flip();
	
		if(!mgl->Process())
		done=1;
		EndClock();
	}
	ExitCardMenu();
}
