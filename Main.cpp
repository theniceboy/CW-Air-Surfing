#include "DarkGDK.h"
#include <stdlib.h>
#include <string.h>

int js;
const int maxbjs = 10;

int scnw;
int scnh;

#define posscx 0
#define posscy 0
#define MaxRadius 2000
#define speed 0.12
#define playerspeed 5


#define boxx 50
#define boxy 10
#define boxz 50

float aga, agb;
int radius;

const int obc = 2;
const float jumph = 150.0f;

int nowon;

const int dbs = 1;

float bx, by, bz;
float cx, cy, cz;

DWORD white = RGB ( 255, 255, 255 );
DWORD gray = RGB ( 120, 120, 120 );
DWORD red = RGB ( 255, 0, 0 );

const float mx = 100.0f, my = 5000.0f, mz = 100.0f; //2000000000
//										     80000000
const int badr = 10;

const int bsum = 300;

struct block
{
    int x, y, z;
	int obx;
    bool bad,exist;
} b[bsum + maxbjs + 2];

float t0, t, baseh, toph, droph;
bool doControl, up, bj;

void setup ( )
{
	bx = 0.0f;
	by = -my + jumph;
	bz = 0.0f;
	droph = -my + jumph;
	nowon = 1;
	t = 0.0f;
	baseh = 0.0f;
	aga = 270;
	agb = 45;
	radius = 300;
	doControl = false;
	up = false;
	js = 0;
    dbRandomize ( dbTimer ( ) );

	
	dbLoadObject		( "media\\sky\\os.x" , 2 );
	dbPositionObject	( 2 , 0 , -my , 0 );
	dbSetObjectTexture	( 2 , 2 , 1 );
	dbSetObjectLight	( 2 , 0 );
	dbSetObjectCull		( 2 , 0 );
	dbYRotateObject		( 2 , 270 );
	dbScaleObject		( 2 , 250 , 250 , 250 );
}

bool check ( int x )
{
	if ( b[x].exist &&
		bx > b[x].x - 25 && bx < b[x].x + 25 &&
		bz > b[x].z - 25 && bz < b[x].z + 25 )
	{
		if ( b[x].bad )
		{
			++js;
			b[x].exist = false;
			dbDeleteObject ( b[x].obx );
		}
		up = true;
		droph = b[x].y + jumph;
		t = dbSqrt ( ( droph - b[x].y ) / 16.0f );
		nowon = x;

		return false;
	}
	else
		return true;
}

void Game ( )
{
	setup ( );

	int i, j, k, lx = 2 * mx + 1, ly = 2 * my + 1, lz = 2 * mz + 1;//, lz = 2 * mz + 1;
	int lowp, lp, rp;
	int t0, newbjs = maxbjs;
	float ls, gamemark = -my;
	block temp;
    
    dbSyncOn ( );
    dbSyncRate ( 260 );


	b[1].x = 0;
    b[1].y = -my;
    b[1].z = 0;
    b[1].bad = false;
	b[1].exist = true;
	b[1].obx = obc;

    dbMakeObjectBox ( obc, boxx, boxy, boxz );
	dbPositionObject ( obc, b[1].x, b[1].y, b[1].z );
	dbColorObject ( obc, white );

    for ( i = 2; i <= bsum; ++i )
    {
		b[i].exist = true;

        b[i].x = dbRnd ( lx ) - mx;
        b[i].y = dbRnd ( ly ) - my;
        b[i].z = dbRnd ( lz ) - mz;

		b[i].obx = i + obc;

        b[i].bad = ( bool )( dbRnd( badr ) == 0 );
		//b[i].bad = false;
		//dbColorObject ( i + obc, dbRGB ( dbRnd ( 255 ), dbRnd ( 255 ), dbRnd ( 255 ) ) );
        dbMakeObjectBox ( i + obc, boxx, boxy, boxz );
		dbPositionObject ( i + obc, b[i].x, b[i].y, b[i].z );
		dbSetObjectLight ( i + obc, 1 );


		//dbMakeLight ( i + obc );
		//dbPositionLight ( i + obc, b[i].x, b[i].y + 50, b[i].z );
		//dbPointLight ( i + obc, b[i].x, b[i].y, b[i].z );
		if ( b[i].bad )
			dbColorObject ( i + obc, red );
		else
			dbColorObject ( i + obc, white );
    }

	for ( i = 1; i < bsum; ++i )
	{
		for ( j = i + 1; j <= bsum; ++j )
		{
			if ( b[i].y > b[j].y )
			{
				temp = b[i];
				b[i] = b[j];
				b[j] = temp;
			}
		}
	}

	dbMakeObjectSphere ( 1, 20 );
	dbColorObject ( 1, white );
	dbMakeLight ( 1 );
	//dbPositionLight ( 1, bx, by, bz );

	dbMakeLight ( 1 );

	t0 = dbTimer ( );

    while ( LoopGDK ( ) )
    {
		if ( up )
			t -= speed;
		else
			t += speed;
 		by = droph - 16.0f * t * t;

		if ( by < baseh && baseh - by > 10 )
		{
			baseh = by;
			toph = baseh + jumph;
		}
		if ( by > toph && by - toph > 10 )
		{
			toph = by;
			baseh = toph - jumph;
		}

		//if ( by < -my && gamemark > my )
		//	gamemark = by;
		if ( by > gamemark )
			gamemark = by;

		if ( by < -my - 1000 )
			break;

		radius -= dbMouseMoveZ ( );
		if ( radius > MaxRadius )
			radius = MaxRadius;
		else if ( radius < -MaxRadius )
			radius =  -MaxRadius;

		if ( dbUpKey() || dbKeyState( 17 ) == 1 )
		{
			bx += dbCos ( aga + 180 ) * playerspeed;
			bz += dbSin ( aga + 180 ) * playerspeed;
			dbXRotateObject ( 1, dbCos ( aga + 180 ) * playerspeed );
			dbZRotateObject ( 1, dbSin ( aga + 180 ) * playerspeed );
		}
		if ( dbDownKey() || dbKeyState( 31 ) == 1 ) 
		{
			bx += dbCos ( aga ) * playerspeed;
			bz += dbSin ( aga ) * playerspeed;
		}
		
		if ( dbLeftKey() || dbKeyState( 30 ) == 1 )
		{
			bx += dbCos ( aga - 90 ) * playerspeed;
			bz += dbSin ( aga - 90 ) * playerspeed;
		}

		if ( dbRightKey() || dbKeyState( 32 ) == 1 )
		{
			bx += dbCos ( aga + 90 ) * playerspeed;
			bz += dbSin ( aga + 90 ) * playerspeed;
		}

		if ( dbSpaceKey ( ) && dbTimer ( ) - t0 > 1000 && newbjs > 0 )
		{
			t0 = dbTimer ( );
			--newbjs;
			up = true;
			droph = by + jumph;
			t = dbSqrt ( ( droph - by ) / 16.0f );
		}

		if ( t < 0 )
		{
			up = false;
			droph = by;
		}

		if ( up == false )
		{
			bj = true;
			for ( i = 1; i <= bsum && bj; ++i )
				if ( by >= b[i].y- 15 && by <= b[i].y + 25 )
					check ( i );


			/*
			lowp = bsum / 2;
			lp = 1;
			rp = bsum;
			bj = false;
			while ( lp < rp )
			{
				if ( b[lowp].y + 15 > by )//- 20 )
				{
					lp = lowp + 1;
					lowp = ( rp - lp ) / 2 + lp;
				}
				else if ( b[lowp].y - 25 < by )
				{
					rp = lowp - 1;
					lowp = ( rp - lp ) / 2 + lp;
				}
				else
				{
					bj = true;
					break;
				}
			}

			if ( bj )
			{
				i = lowp;
				bj = check ( i );
				if ( bj )
				{
					while ( i > 1 && b[--i].y <= by - 15 && bj )
						bj = check ( i );
					if ( bj )
					{
						i = lowp;
						while ( i < bsum && b[++i].y >= by + 25 && bj )
							bj = check ( i );
					}
				}
			}
			//*/
		}
		if ( nowon <= bsum )
			for ( i = 1; i < nowon - 10; ++i )
			{
				//dbDeleteObject ( b[i].obx );
				dbGhostObjectOn ( b[i].obx );
				b[i].exist = false;
			}

		if ( doControl == false && dbMouseClick ( ) == 1 )
		{
			doControl = true;
			lx = dbMouseX ( );
			ly = dbMouseY ( );
		}
		else if ( doControl == true )
		{
			aga = aga + lx - dbMouseX ( );
			agb = agb + dbMouseY ( ) - ly;

			if ( aga > 360 )
				aga -= 360;
			else if ( aga < 0 )
				aga += 360;
			if ( agb > 89 )
				agb = 89;
			else if ( agb < -89 )
				agb = -89;
			
			lx = dbMouseX ( );
			ly = dbMouseY ( );

			if ( dbMouseClick ( ) == 0 )
				doControl = false;
		}

		ls = radius * dbCos ( agb );
		cx = ls * dbCos ( aga ) + bx;
		cy = radius * dbSin ( agb ) + baseh;
		cz = ls * dbSin ( aga ) + bz;

		dbPositionCamera ( cx, cy, cz );
		dbPointCamera ( bx, baseh, bz );

		dbPositionObject ( 1, bx, by, bz );
		dbPositionLight ( 1, bx, by + 60, bz );
		dbPointLight ( 1, bx, by, bz );
		//dbPointLight ( 1, bx - dbCos ( aga ) * playerspeed, baseh + jumph - dbSin ( agb ) * playerspeed, bz - dbSin ( aga ) * playerspeed );

		//dbCenterText ( 50, 10, dbStr ( cx ) );
		//dbCenterText ( 50, 20, dbStr ( cy ) );
		//dbCenterText ( scnw - 120, 40, dbStr ( baseh ) );
		//dbCenterText ( scnw - 120, 70, dbStr ( toph ) );
		//dbCenterText ( scnw - 120, 100, dbStr ( by ) );
		//dbCenterText ( scnw - 120, 130, dbStr ( t ) );

		dbCenterText ( 80, 10, dbStr ( dbScreenFPS ( ) ) );
		dbCenterText ( 130, 10, "FPS" );

		dbCenterText ( 25, 50, dbStr ( newbjs ) );
		dbCenterText ( 130, 50, "Big Jump Left" );
		
		dbCenterText ( 120, 90, "Your Point :" );
		dbCenterText ( 120, 130, dbStr ( ( gamemark + my ) / 2.0f / my * 100.0f + (float) js ) );
        dbSync ( );
	//dbWaitKey ( );
    }

	for ( i = 1; i <= bsum + obc; ++i )
		dbDeleteObject ( i );

	dbCLS ( );
	dbCenterText ( scnw / 2 - 80, scnh / 2 - 80, "Add Point : Jump On" );
	dbCenterText ( scnw / 2 + 60, scnh / 2 - 80, dbStr ( js ) );
	dbCenterText ( scnw / 2 + 140, scnh / 2 - 80, "Bad Block" );
	dbCenterText ( scnw / 2, scnh / 2 - 40, "Your Point ( Full Marks: 100 ) :");
	dbCenterText ( scnw / 2, scnh / 2, dbStr ( ( gamemark + my ) / 2.0f / my * 100.0f + (float) js ) );
	dbSync ( );
	dbWaitKey ( );

    return ;
}

void Begin ( )
{
	dbSetTextSize ( 64 );
	dbCenterText ( scnw / 2, scnh / 2 - 80, "This Game Was Made By Chen Wei" );
	dbCenterText ( scnw / 2, scnh / 2, "In Class 3 Grade 2 , Beijing no.4 Middle School" );
	dbSetTextSize ( 32 );
	dbCenterText ( scnw / 2, scnh - 50, "Press Any Key To Play The Game" );
	dbWaitKey ( );
}


void DarkGDK ( void )
{
	dbSetWindowTitle("CW Air Surfing 0.1 beta 1 —— 陈为制作");
	scnw = GetSystemMetrics(SM_CXSCREEN);
	scnh = GetSystemMetrics(SM_CYSCREEN);
    dbSetWindowPosition ( posscx, posscy );
    dbSetDisplayMode ( scnw, scnh, dbScreenDepth ( ) );
	dbSetWindowOff ( );
	dbSetTextSize ( 32 );
	Begin ( );
    while ( !dbEscapeKey ( ) )
	{
		Game ( );
	}

	return ;
}
