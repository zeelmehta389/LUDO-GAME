#include <graphics.h>
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
using namespace std;
/************************************/
/**  Forward Declaration of Class  **/
/************************************/
class ludo;

/*****************************/
/**     MOUSE FUNCTIONS     **/
/*****************************/
void getMouse(int*,int*);                           //Give coordinates of point where mouse was clicked

/*********************************************/
/**         USER DEFINED FUNCTIONS          **/
/*********************************************/
void gotoxy(int,int);                                   //move cursor on console screen
void getPlayers();                                      //Goes to console to take player names
void putPlayers();                                      //Print names on graphics screen
void first_page();                                      //first page of the game
int checkSelection4page1(int,int);                      //Options for page-1
void howtoplay();                                       //HOw to play rules page
void loading_page();                                    //page after colour selection.. loading page
void assc_ptrs();                                       //Associating func ptr with provided required functions
void initialising_ludos();                              //Initialising Ludos
void draw_all();                                        //static ludo board
void load_route_info(int);                              //Load route info
int listenclick(int,int);                              //Listen click on ludo piece
void redraw(int);                                       //redrawing complete structure after every dice throw
void redraw_d();
void ludo1(int col,int *ax,int *ay,ludo *node);         //assign values to all the data members and functions of the class LUDO
int check_ludo_move_request(int,ludo*);                //check if the ludo can move and returns value to func ptr of particular node/obj of ludo class
void move_ludo_to_new_pos(int,ludo*);                   //move ludo piece
int throwdice();                                       //Throwing dice with animation
void showDice();                                        //Show dice
void hideDice();                                        //Hides dice from end dice position
void messageout(char *message,int refresh=1);
void messageclear();
void winner();

/************************************/
/**         GLOBAL VARIABLES       **/
/************************************/
int flagturn=0; //turn of the player
int for6=0;//for 6 to come at regular intervals
int dice_value_color[4];
int check,a,b;
int colors[4]= {GREEN,BLUE,YELLOW,RED};
char names[4][20];
COORD coor= {0,0};
int cutChance=0;
int htp=0;

HANDLE hout= GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE outcon;

/*********************************/
/**      CLASS DEFINATION       **/
/*********************************/
class ludo
{
public:

    string name;
    int x[58];
    int y[58];
    int pos;
    int color;

public:

    void (*ptr_ludo1)(int,int*,int*,ludo *);
    int (*ptr_check_ludo_move_request)(int,ludo*);
    void (*ptr_move_ludo_to_new_pos)( int,ludo*);

} ply1[4],ply2[4],ply3[4],ply4[4];

void gotoxy(int x,int y)
{
    coor.X=x;
    coor.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coor);
}

/*********************************/
/**     MAIN FUNCTION STARTS    **/
/*********************************/
int main()
{
    srand(time(NULL));
    assc_ptrs();
    int i,a,b,dice=0,playercolor;
    int colors[4]= {GREEN,BLUE,YELLOW,RED};
    initwindow(1364,725,"LUDO",-2,-24);
    int no_move_of_dice=0;
    int check;
    playercolor= colors[rand()%4];
    dice_value_color[flagturn]=playercolor;
    first_page();
    PlaySound("speech.wav", NULL, SND_ASYNC);
    delay(4500);

    start:
    first_page();
    a=123213,b=12323;

    while(1) //purpose is to get the mouse-clicks constantly from user until a box is selected
    {
        int flag=0;
        getMouse(&a,&b);
        if(1)
        {
            check=checkSelection4page1(a,b);
            switch(check)
            {
            case 1:
            {
                PlaySound("click.wav", NULL, SND_ASYNC);
                delay(400);
                cleardevice();
                closegraph();
                getPlayers();
                initwindow(1364,725,"LUDO",-2,-24);
                loading_page();
                draw_all();
                flag=1;
                break;
            }
            case 2:
                {
                    PlaySound("click.wav", NULL, SND_ASYNC);
                    delay(400);
                    cleardevice();
                    howtoplay();
                    goto start;
                    break;
                }
            case 9:
            {
                PlaySound("click.wav", NULL, SND_ASYNC);
                delay(400);
                exit(0);
                break;
            }
            }//end of switch
        }//end of if
        if (flag==1)
            break;
    }//end of while

    i=3;
    while(i)
    {
        if(colors[i]==playercolor)
            break;
        i--;
    }
    load_route_info(playercolor);
    int loop=i;
    while(1)
    {
        flagturn++;
        loop++;
        if(loop>=4)
            loop=0;
        load_route_info(colors[loop]);
        dice_value_color[flagturn]=colors[loop];
        if(flagturn==3)
            break;
    }
    initialising_ludos();

    flagturn=0;
    int tmp1=0,tmp2;
    //dice=6;

    /** MAIN LOGIC **/
    while(1)
    {
        int flag=0;

        ///PLAYER-1 TURN
        messageout("Throw the dice..!",1);
    player1:
        showDice();
        hideDice();
        a=0;
        b=0;
        no_move_of_dice=0;

        while(1)
        {
            a=0;
            b=0;
            getMouse(&a,&b);
            if(listenclick(a,b)==6)
            {
                dice=throwdice();
                if(ply1[0].pos==0 && ply1[2].pos==0 && ply1[1].pos==0 && ply1[3].pos==0 && dice!=6 && ply1[i].pos+dice<=57)
                {
                    no_move_of_dice=1;
                    setcolor(BLUE);
                    messageout("Sorry,you will have to skip this time.!");
                    delay(1500);
                }
                break;
            }
        }

        if(no_move_of_dice==1)
            flagturn=1;

        while(flagturn==0)
        {
            if(no_move_of_dice==1)
                break;

            while(1)
            {
                getMouse(&a,&b);
                tmp1=listenclick(a,b);
                if(tmp1>=0 && tmp1<=3)
                    break;
            }
            tmp2=ply1[tmp1].ptr_check_ludo_move_request(dice,&ply1[tmp1]);

            switch(tmp2)
            {
            case 1:
            {
                messageout("Proceeding..");
                ply1[tmp1].ptr_move_ludo_to_new_pos(dice,&ply1[tmp1]);
                delay(300);
                hideDice();
                flag=0;
                break;
            }
            case 2:
            {
                messageout("Choose correct ludo to proceed.!");
                delay(1500);
                flag=1;
                break;
            }
                /*case 3:
                {
                  messageout("Sorry,you will have to skip this time.!");
                  delay(1500);
                  break;
                }*/
            }//switch ends
            if(dice==6 || cutChance==1)
            {
                messageout("Throw Dice Again..!");
                flagturn=0;
                cutChance=0;
                goto player1;
            }
            if(flag!=1)
                flagturn=1;
        }//WHILE ENDS
        //END OF PLAYER-1 TURN

        ///PLAYER-2 TURN
        messageout("Throw the dice..!",1);

    player2:
        hideDice();
        showDice();
        a=0;
        b=0;
        no_move_of_dice=0;

        while(1)
        {
            a=0;
            b=0;
            getMouse(&a,&b);

            if(listenclick(a,b)==6)
            {
                dice=throwdice();
                if(ply2[0].pos==0 && ply2[2].pos==0 && ply2[1].pos==0 && ply2[3].pos==0 && dice!=6 && ply2[i].pos+dice<=57)
                {
                    no_move_of_dice=1;
                    setcolor(BLUE);
                    messageout("Sorry,you will have to skip this time.!");
                    delay(1500);
                }
                break;
            }
        }

        if(no_move_of_dice==1)
            flagturn=2;
        while(flagturn==1)
        {

            while(1)
            {
                getMouse(&a,&b);
                tmp1=listenclick(a,b);
                if(tmp1>=0 && tmp1<=3)
                    break;
            }

            tmp2=ply2[tmp1].ptr_check_ludo_move_request(dice,&ply2[tmp1]);

            switch(tmp2)
            {
            case 1:
            {
                messageout("Proceeding..");
                ply2[tmp1].ptr_move_ludo_to_new_pos(dice,&ply2[tmp1]);
                delay(300);
                hideDice();

                flag=0;
                break;
            }
            case 2:
            {
                messageout("Choose correct ludo to proceed.!");
                delay(1500);
                flag=2;
                break;
            }
                /*case 3:
                {
                    messageout("Sorry,you will have to skip this time.!");
                    delay(1500);
                    hideDice();

                  break;
                }*/
            }//switch ends
            if(dice==6 || cutChance==1)
            {
                messageout("Throw Dice Again..!");
                flagturn=1;
                cutChance=0;
                goto player2;
            }
            if(flag!=2)
                flagturn=2;
        }//PLAYER-2 TURN ENDS

        ///PLAYER-3 TURN
        messageout("Throw the dice..!");

    player3:
        hideDice();
        showDice();
        a=0;
        b=0;
        no_move_of_dice=0;
        while(1)
        {
            a=0;
            b=0;
            getMouse(&a,&b);

            if(listenclick(a,b)==6)
            {
                dice=throwdice();
                if(ply3[0].pos==0 && ply3[2].pos==0 && ply3[1].pos==0 && ply3[3].pos==0 && dice!=6 && ply3[i].pos+dice<=57)
                {
                    no_move_of_dice=1;
                    setcolor(BLUE);
                    messageout("Sorry,you will have to skip this time.!");
                    delay(1500);
                }
                break;
            }
        }

        if(no_move_of_dice==1)
            flagturn=3;
        while(flagturn==2)
        {

            while(1)
            {
                getMouse(&a,&b);
                tmp1=listenclick(a,b);
                if(tmp1>=0 && tmp1<=3)
                    break;
            }

            tmp2=ply3[tmp1].ptr_check_ludo_move_request(dice,&ply3[tmp1]);

            switch(tmp2)
            {
            case 1:
            {
                messageout("Proceeding..");
                ply3[tmp1].ptr_move_ludo_to_new_pos(dice,&ply3[tmp1]);
                delay(300);
                hideDice();

                flag=0;
                break;
            }
            case 2:
            {
                messageout("Choose correct ludo to proceed.!");
                delay(1500);
                flag=3;
                break;
            }
                /*case 3:
                {
                    messageout("Sorry,you will have to skip this time.!");
                    delay(1500);
                    hideDice();

                    break;
                }*/
            }//switch ends
            //if(dice!=6)
            if(dice==6 || cutChance==1)
            {
                messageout("Throw Dice Again..!");
                flagturn=2;
                cutChance=0;
                goto player3;
            }
            if(flag!=3)
                flagturn=3;
        }//Player-3 turn ends

        ///PLAYER-4 TURN
        messageout("Throw the dice..!",1);

    player4:
        hideDice();
        showDice();
        a=0;
        b=0;
        no_move_of_dice=0;

        while(1)
        {
            a=0;
            b=0;
            getMouse(&a,&b);

            if(listenclick(a,b)==6)
            {
                dice=throwdice();
                if(ply4[0].pos==0 && ply4[2].pos==0 && ply4[1].pos==0 && ply4[3].pos==0 && dice!=6 && ply4[i].pos+dice<=57)
                {
                    no_move_of_dice=1;
                    setcolor(BLUE);
                    messageout("Sorry,you will have to skip this time.!");
                    delay(1500);
                }
                break;
            }
        }

        if(no_move_of_dice==1)
            flagturn=0;
        while(flagturn==3)
        {

            while(1)
            {
                getMouse(&a,&b);
                tmp1=listenclick(a,b);
                if(tmp1>=0 && tmp1<=3)
                    break;
            }

            tmp2=ply4[tmp1].ptr_check_ludo_move_request(dice,&ply4[tmp1]);

            switch(tmp2)
            {
            case 1:
            {
                messageout("Proceeding..");
                ply4[tmp1].ptr_move_ludo_to_new_pos(dice,&ply4[tmp1]);
                delay(300);
                hideDice();

                flag=0;
                break;
            }
            case 2:
            {
                messageout("Choose correct ludo to proceed.!");
                delay(1500);
                flag=4;
                break;
            }
                /*case 3:
                {
                messageout("Sorry,you will have to skip this time.!");
                delay(1500);
                hideDice();
                break;
                }*/
            }//switch ends
            //if(dice!=6)
            if(dice==6 || cutChance==1)
            {
                messageout("Throw Dice Again..!");
                flagturn=3;
                cutChance=0;
                goto player4;
            }
            if(flag!=4)
                flagturn=0;
        }//Player-4 turn ends
    }
    getch();
    closegraph();
}

void assc_ptrs()
{
    for(int iter=0; iter<4; iter++)
    {
        ply1[iter].ptr_ludo1=ludo1;
        ply2[iter].ptr_ludo1=ludo1;
        ply3[iter].ptr_ludo1=ludo1;
        ply4[iter].ptr_ludo1=ludo1;

        ply1[iter].ptr_check_ludo_move_request=check_ludo_move_request;
        ply2[iter].ptr_check_ludo_move_request=check_ludo_move_request;
        ply3[iter].ptr_check_ludo_move_request=check_ludo_move_request;
        ply4[iter].ptr_check_ludo_move_request=check_ludo_move_request;

        ply1[iter].ptr_move_ludo_to_new_pos=move_ludo_to_new_pos;
        ply2[iter].ptr_move_ludo_to_new_pos=move_ludo_to_new_pos;
        ply3[iter].ptr_move_ludo_to_new_pos=move_ludo_to_new_pos;
        ply4[iter].ptr_move_ludo_to_new_pos=move_ludo_to_new_pos;
    }
}

void getMouse(int* xcord,int* ycord)
{
    //int count=0;
    DWORD screenWidth=GetSystemMetrics(SM_CXSCREEN);
    DWORD screenHeight=GetSystemMetrics(SM_CYSCREEN);
    //initwindow(1000,600,"Graphics Screen",-2,-24);

    POINT cursorPosition;
    int mX,mY;

    //while(1)
    {
        GetCursorPos(&cursorPosition);

        mX=cursorPosition.x;
        mY=cursorPosition.y;

        if(GetAsyncKeyState(VK_LBUTTON))
        {
            *xcord=mX;
            *ycord=mY;
            //count++;
            delay(300);
            //cout<<*xcord<<','<<*ycord<<endl;
            //if(count==1)
            //  break;
        }
    }
}

void draw_all()
{
    int x=100;
    int y=80;
    int colour[4] = {2,4,1,14};

    putPlayers();

    {
        ///Centre-Square
        setcolor(LIGHTGRAY);
        rectangle(x+225,y+225,x+345,y+345);
        line(x+225,y+225,x+345,y+345);
        line(x+225,y+345,x+345,y+225);

        setfillstyle(1,BLUE);
        floodfill(x+227,y+226,LIGHTGRAY);

        setfillstyle(1,RED);
        floodfill(x+227,y+344,LIGHTGRAY);

        setfillstyle(1,GREEN);
        floodfill(x+226,y+227,LIGHTGRAY);

        setfillstyle(1,YELLOW);
        floodfill(x+344,y+343,LIGHTGRAY);
    }


    ///roads between houses
    {
        //between green and blue
        {
            for(int q=0; q<=6; q++)
            {
                setcolor(LIGHTGRAY);
                line(x+225,y-15,x+345,y-15);
                y=y+40;
            }
        }
        x=100;
        y=80;

        //between green and red
        {
            for(int q=0; q<=6; q++)
            {
                setcolor(LIGHTGRAY);
                line(x-15,y+225,x-15,y+345);
                x=x+40;
            }
        }
        x=100;
        y=80;

        //between red and yellow
        {
            for(int q=0; q<=6; q++)
            {
                setcolor(LIGHTGRAY);
                line(x+225,y+345,x+345,y+345);
                y=y+40;
            }
        }
        x=100;
        y=80;

        //between yellow and green
        {
            for(int q=0; q<=6; q++)
            {
                setcolor(LIGHTGRAY);
                line(x+345,y+225,x+345,y+345);
                x=x+40;
            }
        }
        x=100;
        y=80;

        //vertical lines between green and blue
        {
            for(int q=0; q<=3; q++)
            {
                setcolor(LIGHTGRAY);
                line(x+225,y-15,x+225,y+225);
                x=x+40;
            }
        }
        x=100;
        y=80;

        //horizontal lines between green and red
        {
            for(int q=0; q<=3; q++)
            {
                setcolor(LIGHTGRAY);
                line(x-15,y+225,x+225,y+225);
                y=y+40;
            }
        }
        x=100;
        y=80;

        //vertical lines between red and yellow
        {
            for(int q=0; q<=3; q++)
            {
                setcolor(LIGHTGRAY);
                line(x+225,y+345,x+225,y+585);
                x=x+40;
            }
        }
        x=100;
        y=80;

        //horizontal lines between blue and yellow
        {
            for(int q=0; q<=3; q++)
            {
                setcolor(LIGHTGRAY);
                line(x+345,y+225,x+585,y+225);
                y=y+40;
            }
        }
        x=100;
        y=80;
    }

    ///Colouring roads
    {

        setcolor(7);
        setfillstyle(1,BLUE);
        floodfill(x+306,y+26,7);
        floodfill(x+266,y+26,7);
        floodfill(x+266,y+70,7);
        floodfill(x+266,y+107,7);
        floodfill(x+266,y+147,7);
        floodfill(x+266,y+187,7);

        setfillstyle(1,GREEN);
        floodfill(x+26,y+226,7);
        floodfill(x+26,y+266,7);
        floodfill(x+66,y+266,7);
        floodfill(x+106,y+266,7);
        floodfill(x+146,y+266,7);
        floodfill(x+186,y+266,7);

        setfillstyle(1,RED);
        floodfill(x+226,y+506,7);
        floodfill(x+266,y+506,7);
        floodfill(x+266,y+466,7);
        floodfill(x+266,y+426,7);
        floodfill(x+266,y+386,7);
        floodfill(x+266,y+346,7);

        setfillstyle(1,YELLOW);
        floodfill(x+509,y+318,7);
        floodfill(x+506,y+266,7);
        floodfill(x+466,y+266,7);
        floodfill(x+426,y+266,7);
        floodfill(x+386,y+266,7);
        floodfill(x+346,y+266,7);
    }

    for(int p=1; p<=4; p++)
    {
        ///White Broad-border like Box
        setcolor(LIGHTGRAY);
        rectangle(x-15,y-15,x+225,y+225);
        setfillstyle(1,WHITE);
        floodfill(x-9,y-9,LIGHTGRAY);

        ///House Colour
        setcolor(LIGHTGRAY);
        rectangle(x,y,x+210,y+210);
        setfillstyle(1,colour[p-1]);
        floodfill(x+10,y+10,LIGHTGRAY);

        //Circle-1
        circle(x+55,y+55,30);
        setfillstyle(1,WHITE);
        floodfill(x+55,y+55,LIGHTGRAY);

        //Circle-2
        circle(x+55,y+155,30);
        setfillstyle(1,WHITE);
        floodfill(x+55,y+155,LIGHTGRAY);

        //Circle-3
        circle(x+155,y+55,30);
        setfillstyle(1,WHITE);
        floodfill(x+155,y+55,LIGHTGRAY);

        //Circle-4
        circle(x+155,y+155,30);
        setfillstyle(1,WHITE);
        floodfill(x+155,y+155,LIGHTGRAY);

        if (p%2==0)
        {
            y=y-360;
            x=x+360;
        }
        else
            y=y+360;
    }

    //Green Borders
    setcolor(7);
    rectangle(20,20,getmaxx()-20,getmaxy()-20);
    rectangle(25,25,getmaxx()-25,getmaxy()-25);
    setfillstyle(9,7);
    floodfill(21,21,7);

    //comment box
    x=100;
    y=80;
    setcolor(LIGHTGRAY);
    rectangle(x+650,65,getmaxx()-80,185);

    //exit and main menu boxes
    x=100;
    y=80;
    setcolor(WHITE);
    rectangle(x+800,620,x+980,665);
    settextstyle(10,HORIZ_DIR,3);
    outtextxy(x+815,633,"HOW TO PLAY");

    rectangle(x+1040,620,getmaxx()-80,665);
    outtextxy(x+1085,633,"EXIT");

}

void initialising_ludos()
{
    int colour[4] = {2,4,1,14};
    int x=100,y=80;
    for(int p=1; p<=4; p++)
    {
        setcolor(0);
        setfillstyle(1,colour[p-1]);
        circle(x+55,y+55,8);
        floodfill(x+55,y+55,0);
        circle(x+55,y+155,8);
        floodfill(x+55,y+155,0);
        circle(x+155,y+55,8);
        floodfill(x+155,y+55,0);
        circle(x+155,y+155,8);
        floodfill(x+155,y+155,0);

        if (p%2==0)
        {
            y=y-360;
            x=x+360;
        }
        else y=y+360;
    }
}

void first_page()
{
    //prior to ludo main screen
    int i=getmaxx();
    int j=getmaxy();
    setcolor(RED);
    settextstyle(10,HORIZ_DIR,9);
    outtextxy(400,80,"LET'S LUDO");
    settextstyle(10,HORIZ_DIR,5);
    setcolor(LIGHTBLUE);
    outtextxy(630,305,"PLAY");
    outtextxy(550,406,"HOW TO PLAY");
    outtextxy(630,505,"EXIT");

    rectangle(500,300,864,350);
    rectangle(500,400,864,450);
    rectangle(500,500,864,550);

    //border
    setcolor(GREEN);
    rectangle(20,20,i-20,j-20);
    rectangle(25,25,i-25,j-25);
    setfillstyle(9,2);
    floodfill(21,21,GREEN);
}

int checkSelection4page1(int a,int b)
{
    if(a>=500&&a<=864&&b>=300&&b<=350)
        return 1; //PlayButton - ALl the bes screen
    else if(a>=500&&a<=864&&b>=400&&b<=450)
        return 2; //Rules OR How to play button
    else if(a>=500&&a<=864&&b>=500&&b<=550)
        return 9; //exit button
}

void load_route_info(int pcolour)
{
    int arrx[58],arry[58];
    switch(pcolour)
    {
    case GREEN:
    {
        int arrx[58]= {155,145,185,225,265,305,345,345,345,345,345,345,385,425,425,425,425,425,425,465,505,545,585,625,665,665,665,625,585,545,505,465,425,425,425,425,425,425,385,345,345,345,345,345,345,305,265,225,185,145,105,105,145,185,225,265,305,345}; //start index remaining
        int arry[58]= {135,325,325,325,325,325,285,245,205,165,125,85,85,85,125,165,205,245,285,325,325,325,325,325,325,365,405,405,405,405,405,405,445,485,525,565,605,645,645,645,605,565,525,485,445,405,405,405,405,405,405,365,365,365,365,365,365,365}; //start index remaining
        for(int tmp=0; tmp<=3; tmp++)
        {
            switch(tmp)
            {
            case 0:
            {
                arrx[0]=155;
                arry[0]=135;
                break;
            }
            case 1:
            {
                arrx[0]=255;
                arry[0]=135;
                break;
            }
            case 2:
            {
                arrx[0]=155;
                arry[0]=235;
                break;
            }
            case 3:
            {
                arrx[0]=255;
                arry[0]=235;
                break;
            }
            }//end of switch-tmp

            switch(flagturn)
            {
            case 0:
            {
                ply1[tmp].ptr_ludo1(GREEN,arrx,arry,&ply1[tmp]);
                break;
            }
            case 1:
            {
                ply2[tmp].ptr_ludo1(GREEN,arrx,arry,&ply2[tmp]);
                break;
            }
            case 2:
            {
                ply3[tmp].ptr_ludo1(GREEN,arrx,arry,&ply3[tmp]);
                break;
            }
            case 3:
            {
                ply4[tmp].ptr_ludo1(GREEN,arrx,arry,&ply4[tmp]);
                break;
            }
            }//end of switch-flagturn
        }//end of for loop
        break;
    }//case green ends
    case BLUE:
    {
        int arrx[58]= {515,425,425,425,425,425,465,505,545,585,625,665,665,665,625,585,545,505,465,425,425,425,425,425,425,385,345,345,345,345,345,345,305,265,225,185,145,105,105,105,145,185,225,265,305,345,345,345,345,345,345,385,385,385,385,385,385,385};
        int arry[58]= {135,125,165,205,245,285,325,325,325,325,325,325,365,405,405,405,405,405,405,445,485,525,565,605,645,645,645,605,565,525,485,445,405,405,405,405,405,405,365,325,325,325,325,325,325,285,245,205,165,125,85,85,125,165,205,245,285,325};
        for(int tmp=0; tmp<=3; tmp++)
        {
            switch(tmp)
            {
            case 0:
            {
                arrx[0]=515;
                arry[0]=135;
                break;
            }
            case 1:
            {
                arrx[0]=615;
                arry[0]=135;
                break;
            }
            case 2:
            {
                arrx[0]=515;
                arry[0]=235;
                break;
            }
            case 3:
            {
                arrx[0]=615;
                arry[0]=235;
                break;
            }
            }//end of switch-tmp

            switch(flagturn)
            {
            case 0:
            {
                ply1[tmp].ptr_ludo1(BLUE,arrx,arry,&ply1[tmp]);
                break;
            }
            case 1:
            {
                ply2[tmp].ptr_ludo1(BLUE,arrx,arry,&ply2[tmp]);
                break;
            }
            case 2:
            {
                ply3[tmp].ptr_ludo1(BLUE,arrx,arry,&ply3[tmp]);
                break;
            }
            case 3:
            {
                ply4[tmp].ptr_ludo1(BLUE,arrx,arry,&ply4[tmp]);
                break;
            }
            }//end of switch-flagturn
        }//end of for loop
        break;
    }//case blude ends
    case YELLOW:
    {
        int arrx[58]= {515,625,585,545,505,465,425,425,425,425,425,425,385,345,345,345,345,345,345,305,265,225,185,145,105,105,105,145,185,225,265,305,345,345,345,345,345,345,385,425,425,425,425,425,425,465,505,545,585,625,665,665,625,585,545,505,465,425};
        int arry[58]= {515,405,405,405,405,405,445,485,525,565,605,645,645,645,605,565,525,485,445,405,405,405,405,405,405,365,325,325,325,325,325,325,285,245,205,165,125,85,85,85,125,165,205,245,285,325,325,325,325,325,325,365,365,365,365,365,365,365};
        for(int tmp=0; tmp<=3; tmp++)
        {
            switch(tmp)
            {
            case 0:
            {
                arrx[0]=515;
                arry[0]=495;
                break;
            }
            case 1:
            {
                arrx[0]=615;
                arry[0]=495;
                break;
            }
            case 2:
            {
                arrx[0]=515;
                arry[0]=595;
                break;
            }
            case 3:
            {
                arrx[0]=615;
                arry[0]=595;
                break;
            }
            }//end of switch-tmp

            switch(flagturn)
            {
            case 0:
            {
                ply1[tmp].ptr_ludo1(YELLOW,arrx,arry,&ply1[tmp]);
                break;
            }
            case 1:
            {
                ply2[tmp].ptr_ludo1(YELLOW,arrx,arry,&ply2[tmp]);
                break;
            }
            case 2:
            {
                ply3[tmp].ptr_ludo1(YELLOW,arrx,arry,&ply3[tmp]);
                break;
            }
            case 3:
            {
                ply4[tmp].ptr_ludo1(YELLOW,arrx,arry,&ply4[tmp]);
                break;
            }
            }//end of switch-flagturn
        }//end of for loop
        break;
    }//case yellow ends

    case RED:
    {
        int arrx[58]= {155,345,345,345,345,345,305,265,225,185,145,105,105,105,145,185,225,265,305,345,345,345,345,345,345,385,425,425,425,425,425,425,465,505,545,585,625,665,665,665,625,585,545,505,465,425,425,425,425,425,425,385,385,385,385,385,385,385};
        int arry[58]= {515,605,565,525,485,445,405,405,405,405,405,405,365,325,325,325,325,325,325,285,245,205,165,125,85,85,85,125,165,205,245,285,325,325,325,325,325,325,365,405,405,405,405,405,405,445,485,525,565,605,645,645,605,565,525,485,445,405};
        for(int tmp=0; tmp<=3; tmp++)
        {
            switch(tmp)
            {
            case 0:
            {
                arrx[0]=155;
                arry[0]=495;
                break;
            }
            case 1:
            {
                arrx[0]=255;
                arry[0]=495;
                break;
            }
            case 2:
            {
                arrx[0]=155;
                arry[0]=595;
                break;
            }
            case 3:
            {
                arrx[0]=255;
                arry[0]=595;
                break;
            }
            }//end of switch-tmp

            switch(flagturn)
            {
            case 0:
            {
                ply1[tmp].ptr_ludo1(RED,arrx,arry,&ply1[tmp]);
                break;
            }
            case 1:
            {
                ply2[tmp].ptr_ludo1(RED,arrx,arry,&ply2[tmp]);
                break;
            }
            case 2:
            {
                ply3[tmp].ptr_ludo1(RED,arrx,arry,&ply3[tmp]);
                break;
            }
            case 3:
            {
                ply4[tmp].ptr_ludo1(RED,arrx,arry,&ply4[tmp]);
                break;
            }
            }//end of switch-flagturn
        }//end of for loop
        break;
    }//case red ends

    }//switch for player ends
}

int listenclick(int p,int q)
{
    int i;
    delay(50);
    if(p>=900 && p<=1080 && q>=620 && q<=665 && htp!=1)
        {
            howtoplay();
            draw_all();
            redraw(1);
            showDice();
            messageout("Throw the dice..!");
            return 4;
        }//how to play
    else if(p>=1100 && p<=getmaxx()-80 && q>=620 && q<=665)
    {
        PlaySound("click.wav", NULL, SND_ASYNC);
        delay(400);
        exit(0);   //exit
    }
    else if(p>=780 && p<=835 && q>=310 && q<=365)
        return 6;   //dice rolling

    else if(flagturn==0)
    {
        for(i=0; i<=3; i++)
        {
            if( abs(p - (ply1[i].x[ply1[i].pos])) < 8  &&  abs(q - (ply1[i].y[ply1[i].pos])) < 8 )
                return i;
        }
    }

    else if(flagturn ==1)
    {
        for(i=0; i<=3; i++)
        {
            if( abs(p - (ply2[i].x[ply2[i].pos])) < 8  &&  abs(q - (ply2[i].y[ply2[i].pos])) < 8 )
                return i;
        }
    }
    else if(flagturn==2)
    {
        for(i=0; i<=3; i++)
        {
            if( abs(p - (ply3[i].x[ply3[i].pos])) < 8  &&  abs(q - (ply3[i].y[ply3[i].pos])) < 8 )
                return i;
        }
    }
    else if(flagturn==3)
    {
        for(i=0; i<=3; i++)
        {
            if( abs(p - (ply4[i].x[ply4[i].pos])) < 8  &&  abs(q - (ply4[i].y[ply4[i].pos])) < 8 )
                return i;
        }
    }
}

void messageout(char *message,int refresh)
{
    if(refresh)
        messageclear();
    setcolor(dice_value_color[flagturn]);
    settextstyle(10,HORIZ_DIR,2);
    outtextxy(780,115,message);
}

void messageclear()
{
    setfillstyle(1,0);
    floodfill(770,90,LIGHTGRAY);
}

void redraw(int tmp)
{
    draw_all();

    for(int i=0; i<4; i++)
    {
        //if(tmp==ply1[0].color)
        {
            //setcolor(0);
            setcolor(BLACK);
            circle(ply1[i].x[ply1[i].pos],ply1[i].y[ply1[i].pos],8);
            setfillstyle(1,ply1[i].color);
            floodfill(ply1[i].x[ply1[i].pos],ply1[i].y[ply1[i].pos],BLACK);
        }
        //else if(tmp==ply2[0].color)
        {
            //setcolor(ply2[i].color);
            setcolor(BLACK);
            circle(ply2[i].x[ply2[i].pos],ply2[i].y[ply2[i].pos],8);
            setfillstyle(1,ply2[i].color);
            floodfill(ply2[i].x[ply2[i].pos],ply2[i].y[ply2[i].pos],BLACK);
        }
        //else if(tmp==ply3[0].color)
        {
            //setcolor(ply3[i].color);
            setcolor(BLACK);
            circle(ply3[i].x[ply3[i].pos],ply3[i].y[ply3[i].pos],8);
            setfillstyle(1,ply3[i].color);
            floodfill(ply3[i].x[ply3[i].pos],ply3[i].y[ply3[i].pos],BLACK);
        }
        //else if(tmp==ply4[0].color)
        {
            //setcolor(ply4[i].color);
            setcolor(BLACK);
            circle(ply4[i].x[ply4[i].pos],ply4[i].y[ply4[i].pos],8);
            setfillstyle(1,ply4[i].color);
            floodfill(ply4[i].x[ply4[i].pos],ply4[i].y[ply4[i].pos],BLACK);
        }
    }//for ends
}

void redraw_d()
{
        redraw(ply1[0].color);
        redraw(ply1[1].color);
        redraw(ply1[2].color);
        redraw(ply1[3].color);

        redraw(ply2[0].color);
        redraw(ply2[1].color);
        redraw(ply2[2].color);
        redraw(ply2[3].color);

        redraw(ply3[0].color);
        redraw(ply3[1].color);
        redraw(ply3[2].color);
        redraw(ply3[3].color);

        redraw(ply4[0].color);
        redraw(ply4[1].color);
        redraw(ply4[2].color);
        redraw(ply4[3].color);


}

void ludo1(int col,int *ax,int *ay,ludo *node)
{
    int i;

    for(i=0; i<58; i++)
    {
        node->x[i]=*(ax);
        node->y[i]=*(ay);
        ax++;
        ay++;
    }
    node->color=col;
    node->pos=0;
}//end of ludo1

int check_ludo_move_request(int dice,ludo *node)
{
    /*return 1 move valid i.e proceed to move to pos,0 if not valid 2 if player cannot move any ludo*/
    int i,count=0;

    if( (node->pos<=57-dice && node->pos!=0)  || (node->pos==0 && dice==6))
        return(1);              // ludo can move

    if(flagturn==0)
    {
        for(i=0; i<=3; i++)
        {
            if( (ply1[i].pos+dice<=57 && ply1[i].pos!=0) || (ply1[i].pos==0 && dice==6) )
                count=1;                    //there is at least one ludo which can move with current dice value
        }
        if(count>0)
            return(2);              //choose the correct ludo that can move
        else
            return(3);              //no ludo can move at current dice value and player has to skip chance
        cout<<"IN Here";
    }

    else if(flagturn==1)
    {
        for(i=0; i<=3; i++)
        {
            if( (ply2[i].pos+dice<=57 && ply2[i].pos!=0) || (ply2[i].pos==0 && dice==6) )
                count=1;                    //there is at least one ludo which can move with current dice value
        }
        if(count>0)
            return(2);              //choose the correct ludo that can move
        else
            return(3);              //no ludo can move at current dice value and player has to skip chance
    }

    else if(flagturn==2)
    {
        for(i=0; i<=3; i++)
        {
            if( (ply3[i].pos+dice<=57 && ply3[i].pos!=0) || (ply3[i].pos==0 && dice==6) )
                count=1;                    //there is at least one ludo which can move with current dice value
        }
        if(count>0)
            return(2);              //choose the correct ludo that can move
        else
            return(3);              //no ludo can move at current dice value and player has to skip chance
    }

    else if(flagturn==3)
    {
        for(i=0; i<=3; i++)
        {
            if( (ply4[i].pos+dice<=57 && ply4[i].pos!=0) || (ply4[i].pos==0 && dice==6) )
                count=1;                    //there is at least one ludo which can move with current dice value
        }
        if(count>0)
            return(2);              //choose the correct ludo that can move
        else
            return(3);              //no ludo can move at current dice value and player has to skip chance
    }
}

void move_ludo_to_new_pos(int dice,ludo *node)
{
    if(node->pos==0 && dice==6)                 //taking out of house on dice giving 6
    {
        messageclear();
        dice=1;
    }

    dice=dice-1;
    int opos;
    opos=node->pos;
    int i;
    for(i=opos; i<=dice+opos; i++)
    {
        setcolor(0);
        circle(node->x[i],node->y[i],8);
        setfillstyle(1,0);
        floodfill(node->x[i],node->y[i],7);
        delay(30);
        {
            node->pos++;
            redraw(node->color);
        }
        setcolor(WHITE);
        circle(node->x[i+1],node->y[i+1],8);
        setfillstyle(1,node->color);
        floodfill(node->x[i+1],node->y[i+1],WHITE);
        PlaySound("move.wav", NULL, SND_ASYNC);
        delay(400);
    }

    ///Logic for cutting ludo piece of another colour
    for(i=0; i<=3; i++)
    {
        if(node->x[node->pos]==ply1[i].x[ply1[i].pos] && node->y[node->pos]==ply1[i].y[ply1[i].pos] )
        {
            if(node->color!=ply1[i].color && ply1[i].pos!=1)
            {
                ply1[i].pos=0;
                redraw(ply1[i].color);
                break;
            }
        }
        if(node->x[node->pos]==ply2[i].x[ply2[i].pos] && node->y[node->pos]==ply2[i].y[ply2[i].pos] )
        {
            if(node->color!=ply2[i].color && ply2[i].pos!=1)
            {
                ply2[i].pos=0;
                redraw(ply2[i].color);
                break;
            }
        }
        if(node->x[node->pos]==ply3[i].x[ply3[i].pos] && node->y[node->pos]==ply3[i].y[ply3[i].pos] )
        {
            if(node->color!=ply3[i].color && ply3[i].pos!=1)
            {
                ply3[i].pos=0;
                redraw(ply3[i].color);
                break;
            }
        }
        if(node->x[node->pos]==ply4[i].x[ply4[i].pos] && node->y[node->pos]==ply4[i].y[ply4[i].pos] )
        {
            if(node->color!=ply4[i].color && ply4[i].pos!=1)
            {
                ply4[i].pos=0;
                redraw(ply4[i].color);
                break;
            }

        }

        // redrawing new positions of all ludos
//        redraw(ply1[0].color);
//        redraw(ply2[0].color);
//        redraw(ply3[0].color);
//        redraw(ply4[0].color);

        /* to check who is the winner */
        static int number_of_ply1_ludo_home=0;
        static int number_of_ply2_ludo_home=0;
        static int number_of_ply3_ludo_home=0;
        static int number_of_ply4_ludo_home=0;
        if(node->pos==57 && flagturn==0)
            number_of_ply1_ludo_home++;
        if(node->pos==57 && flagturn==1)
            number_of_ply2_ludo_home++;
        if(node->pos==57 && flagturn==2)
            number_of_ply3_ludo_home++;
        if(node->pos==57 && flagturn==3)
            number_of_ply4_ludo_home++;
        if(number_of_ply1_ludo_home==4)
            winner();
        if(number_of_ply2_ludo_home==4)
            winner();
        if(number_of_ply3_ludo_home==4)
            winner();
        if(number_of_ply4_ludo_home==4)
            winner();

    }// end of for
    //redraw_d();
}

void winner()
{
    //WHITE Borders
        setcolor(7);
        rectangle(20,20,getmaxx()-20,getmaxy()-20);
        rectangle(25,25,getmaxx()-25,getmaxy()-25);
        setfillstyle(9,3);
        floodfill(21,21,7);

    for(int x=450,y=0;y<getmaxy()-100;y++)
    {
        setcolor(WHITE);
        rectangle(x,y-100,920,y);
        rectangle(x+8,y-92,912,y-8);
        setfillstyle(9,LIGHTCYAN);
        settextstyle(10,HORIZ_DIR,5);
        outtextxy(x+163,y-70,"WINNER");
        floodfill(x+4,y-98,WHITE);
        setcolor(BLACK);
        outtextxy(x+163,y-70,"WINNER");
        rectangle(x,y-100,920,y);
        rectangle(x+8,y-92,912,y-8);

    }
        int x=450,y=getmaxy()-100;
        setcolor(WHITE);
        rectangle(x,y-100,920,y);
        rectangle(x+8,y-92,912,y-8);
        settextstyle(10,HORIZ_DIR,5);
        outtextxy(x+163,y-70,"WINNER");
        setfillstyle(9,LIGHTCYAN);
        floodfill(x+4,y-98,WHITE);

    for(x=480,y=0;y<getmaxy()-200;y++)
    {
        setcolor(WHITE);
        rectangle(x,y-100,890,y);
        rectangle(x+8,y-92,882,y-8);
        setfillstyle(9,LIGHTCYAN);
        settextstyle(10,HORIZ_DIR,5);
        outtextxy(x+120,y-70,*(names));
        floodfill(x+4,y-98,WHITE);
        setcolor(BLACK);
        outtextxy(x+120,y-70,"88888888");
        rectangle(x,y-100,890,y);
        rectangle(x+8,y-92,882,y-8);
    }

        setcolor(WHITE);
        rectangle(x,y-100,890,y);
        rectangle(x+8,y-92,882,y-8);
        setfillstyle(9,LIGHTCYAN);
        settextstyle(10,HORIZ_DIR,5);
        outtextxy(x+120,y-70,"ZEEYANK");
        floodfill(x+4,y-98,WHITE);

        //WHITE Borders
        setcolor(7);
        rectangle(20,20,getmaxx()-20,getmaxy()-20);
        rectangle(25,25,getmaxx()-25,getmaxy()-25);
        setfillstyle(9,3);
        floodfill(21,21,7);
        //setfillstyle(9,3);

        //flag
        for(x=686,y=getmaxy()-300;y>getmaxy()-650;y--)
        {
            setcolor(WHITE);
            line(x+4,y,x+4,y-1);
            line(x+15,y,x+15,y-1);
            delay(2);
        }

        x=686;y=getmaxy()-650;
        line(x+4,y,x+15,y);

        //triangle of the flag
        setfillstyle(1,colors[flagturn]);
        line(x+15,y,x+170,y+65);
        line(x+15,y+130,x+170,y+65);
        floodfill(x+17,y+4,WHITE);
}
int throwdice()
{
    srand(time(NULL));
    PlaySound("dice.wav", NULL, SND_ASYNC);
    int x=780,y=310;
    int a,b;
    int rn;
    char num[2];
    int flag=0;

    if(1)
    {
        flag=1;
        for(x=780,y=310; y<=getmaxy()-350; x=x+15,y=y+10)
        {
            setcolor(BLACK);
            outtextxy(780,330,"2222");

            rn=(rand()%6+1);
            num[0]=rn+48;
            setcolor(dice_value_color[flagturn]);
            rectangle(x,y,x+55,y+55);
            rectangle(x+4,y+4,x+51,y+51);
            setfillstyle(1,BLACK);
            floodfill(x+3,y+3,dice_value_color[flagturn]);
            settextstyle(1,HORIZ_DIR,1);
            outtextxy(x+20,y+20,&num[0]);
            delay(100);
            setcolor(0);
            rectangle(x,y,x+55,y+55);
            rectangle(x+4,y+4,x+51,y+51);
            outtextxy(x+20,y+20,&num[0]);
        }

        for(x,y; y>=getmaxy()-400; x=x+15,y=y-10)
        {
            rn=(rand()%6+1);
            num[0]=rn+48;
            setcolor(dice_value_color[flagturn]);
            rectangle(x,y,x+55,y+55);
            rectangle(x+4,y+4,x+51,y+51);
            outtextxy(x+20,y+20,&num[0]);
            delay(100);
            setcolor(0);
            rectangle(x,y,x+55,y+55);
            rectangle(x+4,y+4,x+51,y+51);
            outtextxy(x+20,y+20,&num[0]);
        }

        for(x,y; y<=getmaxy()-350; x=x+15,y=y+10)
        {
            rn=(rand()%6+1);
            num[0]=rn+48;
            setcolor(dice_value_color[flagturn]);
            rectangle(x,y,x+55,y+55);
            rectangle(x+4,y+4,x+51,y+51);
            outtextxy(x+20,y+20,&num[0]);
            delay(100);
            setcolor(0);
            rectangle(x,y,x+55,y+55);
            rectangle(x+4,y+4,x+51,y+51);
            outtextxy(x+20,y+20,&num[0]);
        }

        setcolor(dice_value_color[flagturn]);
        rectangle(x,y,x+55,y+55);
        rectangle(x+4,y+4,x+51,y+51);
        outtextxy(x+20,y+20,&num[0]);
        htp=1;
        messageout("Move your token...!");
        return num[0]-48;
    }
}

void showDice()
{
    //constant dice
    int x=780,y=310;
    setcolor(dice_value_color[flagturn]);
    rectangle(x,y,x+55,y+55);
    rectangle(x+4,y+4,x+51,y+51);
    setcolor(dice_value_color[flagturn]);
    settextstyle(10,HORIZ_DIR,1);
    outtextxy(x+6,y+19,"DICE");
    //messageout("THROW DICE",1);
    htp=0;
}

void hideDice()
{
    setcolor(BLACK);
    int  x=1065,y=380;
    rectangle(x,y,x+55,y+55);
    rectangle(x+4,y+4,x+51,y+51);
    setfillstyle(1,BLACK);
    floodfill(1068,384,BLACK);
    setcolor(BLACK);
    outtextxy(1085,400,"2222");
}

void loading_page()
{
    //borders
    setcolor(LIGHTGRAY);
    rectangle(20,20,getmaxx()-20,getmaxy()-20);
    rectangle(25,25,getmaxx()-25,getmaxy()-25);
    setfillstyle(9,2);
    floodfill(21,21,LIGHTGRAY);

    setcolor(LIGHTGRAY);
    rectangle(20,20,getmaxx()-20,getmaxy()-20);
    rectangle(25,25,getmaxx()-25,getmaxy()-25);
    setfillstyle(9,2);
    floodfill(21,21,LIGHTGRAY);

    setcolor(LIGHTCYAN);
    settextstyle(10,HORIZ_DIR,5);
    outtextxy(500,600,"HAVE FUN DICING!!");
    setcolor(RED);
    settextstyle(10,HORIZ_DIR,8);
    outtextxy(400,100,"LET'S   LUDO");
    setcolor(WHITE);
    settextstyle(10,HORIZ_DIR,2);
    outtextxy(300,300,"LOADING...");
    for(int i=300,j=330; i<=1000; i=i+1)
    {
        setcolor(WHITE);
        setfillstyle(1,WHITE);
        rectangle(i,j,1000,350);
        floodfill(i+1,j,WHITE);
        delay(7);

        setcolor(CYAN);
        settextstyle(10,HORIZ_DIR,3);
        if(i==350)
        {
            setcolor(BLACK);
            rectangle(290,352,850,500);
            setfillstyle(1,BLACK);
            floodfill(293,354,BLACK);
            setcolor(CYAN);
            outtextxy(300,400,"GETTING LUDO BOARD READY..");
        }
        else if(i==700)
        {
            setcolor(BLACK);
            rectangle(290,352,850,500);
            setfillstyle(1,BLACK);
            floodfill(293,354,BLACK);
            setcolor(CYAN);
            outtextxy(300,400,"LUDO PIECES READY TO FIGHT..");
        }
    }
    cleardevice();
}

void getPlayers()
{
    system("MODE CON COLS=169 LINES=58");
    SetConsoleTextAttribute(hout,LIGHTCYAN);
    gotoxy(2,1);
    for(int x=1; x<84; x++)
        cout<<"* ";
    for(int y=1; y<54; y++)
    {
        gotoxy(2,y);
        cout<<"*";
        gotoxy(166,y);
        cout<<"*";
    }
    gotoxy(2,54);

    for(int x=1; x<84; x++)
        cout<<"* ";

    gotoxy(4,30);

    string print[9]= {"* *           * * * * * *   * * * * * *   * * *   * * * * * *       * *           * *     * *   * * * *       * * * * * *",
                      "* *           * * * * * *   * * * * * *   * * *   * * * * * *       * *           * *     * *   * * * * *     * * * * * *",
                      "* *           * *               * *         * *   * *               * *           * *     * *   * *    * *    * *     * *",
                      "* *           * * * *           * *         *     * * * * * *       * *           * *     * *   * *     * *   * *     * *",
                      "* *           * * * *           * *        *      * * * * * *       * *           * *     * *   * *     * *   * *     * *",
                      "* *           * *               * *                       * *       * *           * *     * *   * *    * *    * *     * *",
                      "* * * * * *   * * * * * *       * *               * * * * * *       * * * * * *   * * * * * *   * * * * *     * * * * * *",
                      "* * * * * *   * * * * * *       * *               * * * * * *       * * * * * *   * * * * * *   * * * *       * * * * * *"
                     };

    for(int i=0; i<9; i++)
    {
        gotoxy(24,9+i);
        SetConsoleTextAttribute(hout,LIGHTCYAN);
        cout<<print[i];
    }

    int x=350,y=200;
    SetConsoleTextAttribute(outcon,4);

    SetConsoleTextAttribute(hout,GREEN);
    gotoxy(55,30);
    cout<<"Enter Player Name: ";

    SetConsoleTextAttribute(hout,BLUE);
    gotoxy(55,33);
    cout<<"Enter Player Name: ";

    SetConsoleTextAttribute(hout,YELLOW);
    gotoxy(55,36);
    cout<<"Enter Player Name: ";

    SetConsoleTextAttribute(hout,RED);
    gotoxy(55,39);
    cout<<"Enter Player Name: ";

    gotoxy(74,30);
    PlaySound("enter_names.wav", NULL, SND_SYNC);
    for(int i=0,n=30; i<4; i++)
    {
        switch(i)
        {
        case 0:PlaySound("green.wav", NULL, SND_ASYNC);break;
        case 1:PlaySound("blue.wav", NULL, SND_ASYNC);break;
        case 2:PlaySound("yellow.wav", NULL, SND_ASYNC);break;
        case 3:PlaySound("red.wav", NULL, SND_ASYNC);break;
        }
        SetConsoleTextAttribute(hout,colors[i]);
        gotoxy(74,n);
        fgets(*(names+i),20,stdin);
        n=n+3;
    }
    gotoxy(168,57);
    return;
}

void putPlayers()
{
    setcolor(colors[0]);
    outtextxy(90,40,*names);
    setcolor(colors[1]);
    outtextxy(460,40,*(names+1));
    setcolor(colors[2]);
    outtextxy(460,670,*(names+2));
    setcolor(colors[3]);
    outtextxy(90,670,*(names+3));
}

void howtoplay()
{
    initwindow(1364,725,"HOW TO PLAY",-2,-24);
    //Green Borders
    setcolor(GREEN);
    rectangle(20,20,getmaxx()-20,getmaxy()-20);
    rectangle(25,25,getmaxx()-25,getmaxy()-25);
    setfillstyle(9,GREEN);
    floodfill(21,21,GREEN);

    //rule box
    setcolor(WHITE);
    rectangle(80,250,getmaxx()-80,getmaxy()-180);
    rectangle(78,248,getmaxx()-78,getmaxy()-178);

    //i got it
    rectangle(550,getmaxy()-130,getmaxx()-550,getmaxy()-75);
    rectangle(548,getmaxy()-132,getmaxx()-548,getmaxy()-73);

    setcolor(RED);
    settextstyle(10,HORIZ_DIR,9);
    outtextxy(400,60,"LET'S LUDO");
    setcolor(WHITE);
    settextstyle(10,HORIZ_DIR,6);
    outtextxy(100,180,"HOW TO PLAY");

    settextstyle(10,HORIZ_DIR,2);
    setfillstyle(1,WHITE);

    pieslice(100,270,0,360,5);
    outtextxy(110,260,"Rule 1");
    outtextxy(120,280,"LUDO GAME IS PLAYED BETWEEN 4 PLAYERS. EACH PLAYER HAS 4 TOKENS. WHOEVER GETS ALL 4 TOKENS TO ");
    outtextxy(120,300,"TO THE CENTRAL HOME FIRST, IS THE WINNER.");

    pieslice(100,335,0,360,5);
    outtextxy(110,325,"Rule 2");
    outtextxy(120,345,"A TOKEN CAN MOVE OUT OF THE HOUSE ONLY IF THE PLAYER ROLLES A SIX.");

    pieslice(100,380,0,360,5);
    outtextxy(110,370,"Rule 3");
    outtextxy(120,390,"WHEN A PLAYER ROLLS A SIX,HE GETS ANOTHER TURN TO ROLL DICE.");

    pieslice(100,425,0,360,5);
    outtextxy(110,415,"Rule 4");
    outtextxy(120,435,"THE TOKENS MUST MOVE OUT AND TRAVEL AROUND THE BOARD TO MAKE IT TO CENTRAL HOME.");

    pieslice(100,470,0,360,5);
    outtextxy(110,460,"Rule 5");
    outtextxy(120,480,"IF A TOKEN CAPTURES ANOTHER TOKEN,TOKEN OF THE LATER PLAYER HAS TO MOVE BACK TO ITS HOME");
    outtextxy(120,500,"POSITION.");

    settextstyle(10,HORIZ_DIR,4);
    outtextxy(600,getmaxy()-117,"I GOT IT");

    a=0;b=0;   //initializes the mouse clicks again for "i got it".
    while(1)
    {
        getMouse(&a,&b);
        if(a>=550 && a<=getmaxx()-550 && b>=getmaxy()-130 && getmaxy()-75)
        {
            PlaySound("click.wav", NULL, SND_ASYNC);
            closegraph();
            initwindow(1364,725,"LUDO",-2,-24);
            return;
        }
    }
}
