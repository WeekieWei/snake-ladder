#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define TRUE 0
#define FALSE 1

/*Declaration of nested structure*/
struct Player
{
    char name[20];
    struct Snake
    {
        int player_num;
        int prev;
        int after;
        int temp;
        int turn;
    }snake;
    struct storeposition
    {
        char position[4];
        struct storeposition *nextnode;
    }*currentnode, *newnode, *headnode, *tailnode;
}player[5];

/*Global Declaration*/

char square[100][4]; //Creation of board
void instruction(), Game_Snake(), snakeboard(), Victory(struct Player []), refresh(), Search(), Dequeue(), Display();
int random(), random2(), initialize(), recursion(int, int);
int PL, die;

/*Main Console*/
int main()
{
    printf("                                     _____  ______          _____    __  __ ______ \n");
    printf("                                    |  __ `|  ____|   /`   |  __ `  |  `/  |  ____|\n");
    printf("                                    | |__) | |__     /  `  | |  | | | `  / | |__   \n");
    printf("                                    |  _  /|  __|   / /` ` | |  | | | |`/| |  __|  \n");
    printf("                                    | | ` \ | |____ / ____ `| |__| | | |  | | |___  \n");
    printf("                                    |_|  `_\|______/_/    `_`_____/  |_|  |_|______|\n\n\n\n");
    printf("         IMPORTANT!! BEFORE YOU BEGIN, YOU MUST MAXIMIZE THIS CONSOLE WINDOW OR MAKE IT FULLSCREEN\n\n         Press any key when it is already maximized");
    getch();system("cls");

    while(1)
    {
        system("cls");
        printf("        .d88888b                    dP                                                    dP    dP                       dP       dP                            \n");
        printf("        88.    ''                   88                                                    88    88                       88       88                            \n");
        printf("        `Y88888b. 88d888b. .d8888b. 88  .dP  .d8888b. .d8888b.    .d8888b. 88d888b. .d888b88    88        .d8888b. .d888b88 .d888b88 .d8888b. 88d888b. .d8888b. \n");
        printf("              `8b 88'  `88 88'  `88 88888'   88ooood8 Y8ooooo.    88'  `88 88'  `88 88'  `88    88        88'  `88 88'  `88 88'  `88 88ooood8 88'  `88 Y8ooooo. \n");
        printf("        d8'   .8P 88    88 88.  .88 88  `8b. 88.  ...       88    88.  .88 88    88 88.  .88    88        88.  .88 88.  .88 88.  .88 88.  ... 88             88 \n");
        printf("         Y88888P  dP    dP `88888P8 dP   `YP `88888P' `88888P'    `88888P8 dP    dP `88888P8    88888888P `88888P8 `88888P8 `88888P8 `88888P' dP       `88888P' \n");
        printf("        oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");


        int ch;
        printf("\nWelcome to Snakes and Ladders, it is a great game to play with your family!\n\nChoose one of the following:\n1. Start game\n2. Read game's rules & instructions\n3. Exit\nInput: ");
 select:scanf("%d", &ch);
        switch(ch)
        {
            case 1 : Game_Snake();refresh();break;
            case 2 : instruction();break;
            case 3 : printf("\n\n\nPlease play again sometime soon! We will be happy to see you again!\n\n\n\n\n");exit(0);break;
            default: printf("Invalid Input!!\nInput: ");goto select;
        }
    }
}

/*Display rules and instructions of the game*/
void instruction()
{
    system("cls");
    printf("RULES:\n1. This game is playable with 1-4 players\n2. You SHALL NOT hold any key to automate the die rolling process, because the random number is generated through a pseudorandom number generator,\nwhich means the random number is calculated by an deterministic algorithm, and deterministic devices can never generate a random number, as a\npopular mathematician said..");
    printf("\n\n'Anyone who considers arithmetical methods of producing random digits is, of course, in a state of sin.'------Von Neumann");
    printf("\n\nINSTRUCTIONS:");
    printf("\n1. Each player roll 2 dies, the results of the 2 dices are multiplied together");
    printf("\n2. The player who has the highest final result will start the round first, then followed by the next player number\n\tEx. player 3 gets the highest, the turn goes from Player 3>4>1>2");
    printf("\n3. After the game starts, players take turn to roll accordingly");
    printf("\n4. If a player rolled number 6, the player takes an extra turn");
    printf("\n5. If a player lands at the bottom of a ladder, the player moves up to the top of the ladder. Ladder is listed as 'Ldx' where x = ladder number.");
    printf("\n6. If a player lands on the head of a snake, the player slides down to the bottom of the snake. Snake is listed as 'Snx' where x = snake number.");
    printf("\n7. If Player A moves to a position where Player B is currently at the position, Player A kicks Player B back to START");
    printf("\n8. Whichever player lands exactly on position 100 first wins the game(GOLD), then the subsequent ranking is based on other players current position(higher position the better)");
    printf("\n9. If a player rolls more than enough to land on position 100, the player bounces off position 100 and moves back. \nYou can only win by rolling the exact number needed to land on the position 100");
    printf("\n\nPress any key to return to the main screen..");
    getch();
}

/*Main System of Snake and Ladder game*/
void Game_Snake()
{
    int doubleturn=FALSE, climb, fall, overlap, i, j, k, l;

    printf("\nEnter number of players(1-4): ");
    scanf("%d", &PL);

    while(PL<1||PL>4)
    {
        printf("\nInvalid Number of players!\n\nEnter number of players(1-4): ");
        scanf("%d", &PL);
    }

    /*Initialize players required data*/
    for(i=0;i<PL;i++)
    {
        printf("\nEnter player name for player number %d: ", i+1);
        fflush(stdin);
        scanf("%[^\n]s", &player[i].name);
        player[i].snake.player_num=i+1;
        player[i].snake.turn=0;
        player[i].snake.prev=-1;
        player[i].snake.after=-1;
        player[i].snake.temp=-1;
        player[i].headnode=NULL;

        /*Enqueue 1st node using linked list*/
        player[i].newnode=(struct storeposition*)malloc(sizeof(struct storeposition));
        strcpy((player[i].newnode->position), "SRT");
        player[i].headnode=player[i].newnode;
        player[i].tailnode=player[i].newnode;
        player[i].newnode->nextnode=NULL;
    }
    if(PL==1)
        i=0;
    else
        i=initialize();
    while(1)
    {
        doubleturn=FALSE;
        overlap=FALSE;
        climb=FALSE;
        fall=FALSE;

        system("cls");
        printf("\n\n\tSnake Board\n\n");
        printf("\nPlayer %d(%s) is rolling a die for turn %d...\nPress any key to roll the die!",i+1, player[i].name, player[i].snake.turn+1);
        getch();
        system("cls");
        die=random(); //Rolling die using pseudorandom algorithm
        //printf("\nEnter die num: ");scanf("%d", &die); //Enter die number manually
        if(die==6)
        {
            doubleturn=TRUE;
        }
        player[i].snake.prev=player[i].snake.after;
        player[i].snake.after=player[i].snake.prev+die;
        player[i].snake.temp=player[i].snake.prev+die;

        if(player[i].snake.after>99)
        {
            player[i].snake.after-=99;
            player[i].snake.after=99-player[i].snake.after;
            player[i].snake.temp=player[i].snake.after;
        }

        /*Check if the player lands on a snake*/
        switch(player[i].snake.after)
        {
            case 96 : player[i].snake.after=77;fall=TRUE;break;
            case 94 : player[i].snake.after=55;fall=TRUE;break;
            case 87 : player[i].snake.after=23;fall=TRUE;break;
            case 47 : player[i].snake.after=25;fall=TRUE;break;
            case 35 : player[i].snake.after=5;fall=TRUE;break;
            case 31 : player[i].snake.after=9;fall=TRUE;break;
        }

        /*Check if the player lands on a ladder*/
        switch(player[i].snake.after)
        {
            case 0 : player[i].snake.after=37;climb=TRUE;break;
            case 3 : player[i].snake.after=13;climb=TRUE;break;
            case 7 : player[i].snake.after=29;climb=TRUE;break;
            case 20 : player[i].snake.after=41;climb=TRUE;break;
            case 27 : player[i].snake.after=75;climb=TRUE;break;
            case 49 : player[i].snake.after=66;climb=TRUE;break;
            case 70 : player[i].snake.after=91;climb=TRUE;break;
            case 79 : player[i].snake.after=98;climb=TRUE;break;
        }

        /*Enqueue subsequent nodes using linked list*/
        player[i].newnode=(struct storeposition*)malloc(sizeof(struct storeposition));
        refresh();
        strcpy((player[i].newnode->position), (square[player[i].snake.after]));
        player[i].currentnode=player[i].headnode;
        player[i].tailnode->nextnode=player[i].newnode;
        player[i].tailnode=player[i].newnode;
        player[i].newnode->nextnode=NULL;

        /*Verify if the landing position is empty*/
        k=-1;
        for(j=0;j<PL;j++)
        {
            if((player[i].snake.after==player[j].snake.after)&&(i!=j))
            {
                player[j].snake.after=-1;
                k=j;
                overlap=TRUE;
                player[k].newnode=(struct storeposition*)malloc(sizeof(struct storeposition));
                refresh();
                strcpy((player[k].newnode->position), "SRT");
                player[k].currentnode=player[k].headnode;
                while(player[k].currentnode->nextnode!=NULL)
                {
                    player[k].currentnode=player[k].currentnode->nextnode;
                }
                player[k].currentnode->nextnode=player[k].newnode;
                player[k].newnode->nextnode=NULL;
            }
        }

        if(player[i].snake.after==99)
        {
            Victory(player);
        }

        /*Announcement*/
        refresh();
        if(player[i].snake.prev==-1)
        {
            strcpy(square[player[i].snake.prev],"SRT");
        }


        if(player[i].snake.after==player[i].snake.prev)
        {
            printf("\nPlayer %d(%s) has rolled: %d!!!\n...and moved to the same location\n", i+1, player[i].name, die);

        }
        /*Normal Movement Announcement*/
        else if(doubleturn==TRUE&&fall==FALSE&&climb==FALSE)
        {
            printf("\nPlayer %d(%s) is lucky! because he/she has just rolled: %d!!!\n...and moved from %s to %s\n\nHe/she can roll again!", i+1, player[i].name, die, square[player[i].snake.prev], square[player[i].snake.after]);
        }
        else if(doubleturn==FALSE&&fall==FALSE&&climb==FALSE)
        {
            printf("\nPlayer %d(%s) has rolled: %d!!!\n...and moved from %s to %s", i+1, player[i].name, die, square[player[i].snake.prev], square[player[i].snake.after]);
        }

        /*Snake bitten announcement*/
        else if(doubleturn==TRUE&&fall==TRUE&&climb==FALSE)
        {
            printf("\nPlayer %d(%s) is kind of lucky! because he/she has just rolled: %d!!!\n...and moved from %s to %s, then fall to %s because got awfully bitten by a snake :(\n\nHe/she can roll again!", i+1, player[i].name, die, square[player[i].snake.prev], square[player[i].snake.temp], square[player[i].snake.after]);
        }
        else if(doubleturn==FALSE&&fall==TRUE&&climb==FALSE)
        {
            printf("\nPlayer %d(%s) has rolled: %d!!!\n...and moved from %s to %s, then fall to %s because got awfully bitten by a snake :(", i+1, player[i].name, die, square[player[i].snake.prev], square[player[i].snake.temp], square[player[i].snake.after]);
        }

        /*Climb ladder annoucement*/
        else if(doubleturn==TRUE&&fall==FALSE&&climb==TRUE)
        {
            printf("\nPlayer %d(%s) is super lucky! because he/she has just rolled: %d!!!\n...and moved from %s to %s, then climb to %s because found a ladder :)\n\nHe/she can roll again!", i+1, player[i].name, die, square[player[i].snake.prev], square[player[i].snake.temp], square[player[i].snake.after]);
        }
        else if(doubleturn==FALSE&&fall==FALSE&&climb==TRUE)
        {
            printf("\nPlayer %d(%s) has rolled: %d!!!\n...and moved from %s to %s, then climb to %s because found a ladder :)", i+1, player[i].name, die, square[player[i].snake.prev], square[player[i].snake.temp], square[player[i].snake.after]);
        }
        if(overlap==TRUE)
        {
            printf(", and kicked Player %d back to initial place :D",k+1);
        }

        /*Data Management*/
        snakeboard();
        player[i].snake.turn++;
        if(doubleturn==FALSE)
        {
            i=(i+1)%PL;
        }
        printf("\n\n Press any key to continue...");
        getch();
    }
}

/*Algorithm to display snake board*/
void snakeboard()
{
    int y, z;
    printf("\n\n[Current player position]");
    for(y=0;y<PL;y++)
    {
        if(player[y].snake.after==-1)
            printf("\nPlayer %d: SRT", y+1);
        else
            printf("\nPlayer %d: %s", y+1, square[player[y].snake.after]);
    }
    strcpy(square[61],"Sn1");strcpy(square[17],"Sn1");strcpy(square[96],"Sn2");strcpy(square[77],"Sn2");strcpy(square[94],"Sn3");strcpy(square[55],"Sn3");
    strcpy(square[87],"Sn4");strcpy(square[23],"Sn4");strcpy(square[35],"Sn5");strcpy(square[5], "Sn5");strcpy(square[47],"Sn6");strcpy(square[25],"Sn6");
    strcpy(square[31],"Sn7");strcpy(square[9], "Sn7");strcpy(square[98],"Ld1");strcpy(square[79],"Ld1");strcpy(square[91],"Ld2");strcpy(square[70],"Ld2");
    strcpy(square[75],"Ld3");strcpy(square[27],"Ld3");strcpy(square[66],"Ld4");strcpy(square[49],"Ld4");strcpy(square[29],"Ld5");strcpy(square[7],"Ld5");
    strcpy(square[13],"Ld6");strcpy(square[3],"Ld6");strcpy(square[37],"Ld7");strcpy(square[0],"Ld7");strcpy(square[41],"Ld8");strcpy(square[20],"Ld8");
    for(y=0;y<PL;y++)
    {
        switch(y)
        {
            case 0 : strcpy(square[player[y].snake.after],"PL1");break;
            case 1 : strcpy(square[player[y].snake.after],"PL2");break;
            case 2 : strcpy(square[player[y].snake.after],"PL3");break;
            case 3 : strcpy(square[player[y].snake.after],"PL4");break;
        }
    }
    printf("\n        _____________________________________________________________________\n");
    printf("       |      |      |      |      |      |      |      |      |      |      |\n");
    printf("       |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |\n",square[99],square[98],square[97],square[96],square[95],square[94],square[93],square[92],square[91],square[90]);
    printf("       |______|______|______|______|______|______|______|______|______|______|\n");
    printf("       |      |      |      |      |      |      |      |      |      |      |\n");
    printf("       |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |\n",square[80],square[81],square[82],square[83],square[84],square[85],square[86],square[87],square[88],square[89]);
    printf("       |______|______|______|______|______|______|______|______|______|______|\n");
    printf("       |      |      |      |      |      |      |      |      |      |      |\n");
    printf("       |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |\n",square[79],square[78],square[77],square[76],square[75],square[74],square[73],square[72],square[71],square[70]);
    printf("       |______|______|______|______|______|______|______|______|______|______|\n");
    printf("       |      |      |      |      |      |      |      |      |      |      |\n");
    printf("       |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |\n",square[60],square[61],square[62],square[63],square[64],square[65],square[66],square[67],square[68],square[69]);
    printf("       |______|______|______|______|______|______|______|______|______|______|\n");
    printf("       |      |      |      |      |      |      |      |      |      |      |\n");
    printf("       |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |\n",square[59],square[58],square[57],square[56],square[55],square[54],square[53],square[52],square[51],square[50]);
    printf("       |______|______|______|______|______|______|______|______|______|______|\n");
    printf("       |      |      |      |      |      |      |      |      |      |      |\n");
    printf("       |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |\n",square[40],square[41],square[42],square[43],square[44],square[45],square[46],square[47],square[48],square[49]);
    printf("       |______|______|______|______|______|______|______|______|______|______|\n");
    printf("       |      |      |      |      |      |      |      |      |      |      |\n");
    printf("       |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |\n",square[39],square[38],square[37],square[36],square[35],square[34],square[33],square[32],square[31],square[30]);
    printf("       |______|______|______|______|______|______|______|______|______|______|\n");
    printf("       |      |      |      |      |      |      |      |      |      |      |\n");
    printf("       |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |\n",square[20],square[21],square[22],square[23],square[24],square[25],square[26],square[27],square[28],square[29]);
    printf("       |______|______|______|______|______|______|______|______|______|______|\n");
    printf("       |      |      |      |      |      |      |      |      |      |      |\n");
    printf("       |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |\n",square[19],square[18],square[17],square[16],square[15],square[14],square[13],square[12],square[11],square[10]);
    printf(" ______|______|______|______|______|______|______|______|______|______|______|\n");
    printf("|      |      |      |      |      |      |      |      |      |      |      |\n");
    printf("|Start |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |  %s |\n",square[0],square[1],square[2],square[3],square[4],square[5],square[6],square[7],square[8],square[9]);
    printf("|______|______|______|______|______|______|______|______|______|______|______|\n");

    refresh();
}

/*Deterministic random number generator*/
int random()
{
    int n=0;

    /* Initializes random number generator */
    srand(time(NULL));

    /* Produce a random number from 0 to 6 */
    n=rand()%6+1;

    return n;
}

/*Refresh snake board to default*/
void refresh()
{
    strcpy(square[0],"001");strcpy(square[1],"002");strcpy(square[2],"003");strcpy(square[3],"004");
    strcpy(square[4],"005");strcpy(square[5],"006");strcpy(square[6],"007");strcpy(square[7],"008");
    strcpy(square[8],"009");strcpy(square[9],"010");strcpy(square[10],"011");strcpy(square[11],"012");
    strcpy(square[12],"013");strcpy(square[13],"014");strcpy(square[14],"015");strcpy(square[15],"016");
    strcpy(square[16],"017");strcpy(square[17],"018");strcpy(square[18],"019");strcpy(square[19],"020");
    strcpy(square[20],"021");strcpy(square[21],"022");strcpy(square[22],"023");strcpy(square[23],"024");
    strcpy(square[24],"025");strcpy(square[25],"026");strcpy(square[26],"027");strcpy(square[27],"028");
    strcpy(square[28],"029");strcpy(square[29],"030");strcpy(square[30],"031");strcpy(square[31],"032");
    strcpy(square[32],"033");strcpy(square[33],"034");strcpy(square[34],"035");strcpy(square[35],"036");
    strcpy(square[36],"037");strcpy(square[37],"038");strcpy(square[38],"039");strcpy(square[39],"040");
    strcpy(square[40],"041");strcpy(square[41],"042");strcpy(square[42],"043");strcpy(square[43],"044");
    strcpy(square[44],"045");strcpy(square[45],"046");strcpy(square[46],"047");strcpy(square[47],"048");
    strcpy(square[48],"049");strcpy(square[49],"050");strcpy(square[50],"051");strcpy(square[51],"052");
    strcpy(square[52],"053");strcpy(square[53],"054");strcpy(square[54],"055");strcpy(square[55],"056");
    strcpy(square[56],"057");strcpy(square[57],"058");strcpy(square[58],"059");strcpy(square[59],"060");
    strcpy(square[60],"061");strcpy(square[61],"062");strcpy(square[62],"063");strcpy(square[63],"064");
    strcpy(square[64],"065");strcpy(square[65],"066");strcpy(square[66],"067");strcpy(square[67],"068");
    strcpy(square[68],"069");strcpy(square[69],"070");strcpy(square[70],"071");strcpy(square[71],"072");
    strcpy(square[72],"073");strcpy(square[73],"074");strcpy(square[74],"075");strcpy(square[75],"076");
    strcpy(square[76],"077");strcpy(square[77],"078");strcpy(square[78],"079");strcpy(square[79],"080");
    strcpy(square[80],"081");strcpy(square[81],"082");strcpy(square[82],"083");strcpy(square[83],"084");
    strcpy(square[84],"085");strcpy(square[85],"086");strcpy(square[86],"087");strcpy(square[87],"088");
    strcpy(square[88],"089");strcpy(square[89],"090");strcpy(square[90],"091");strcpy(square[91],"092");
    strcpy(square[92],"093");strcpy(square[93],"094");strcpy(square[94],"095");strcpy(square[95],"096");
    strcpy(square[96],"097");strcpy(square[97],"098");strcpy(square[98],"099");strcpy(square[99],"100");
}

/*Algorithm after a winner is found*/
void Victory(struct Player player[4])
{
    char buffer[5], selection;

    /*Declaration of nodes*/
    struct sortwinner
    {
        char sortname[20];
        int sortplnum;
        int sortposition;
        struct sortwinner *nextptr;
        struct sortwinner *prevptr;
    }*headptr, *tailptr, *currentptr, *newptr, *walker;

    struct hold
    {
        int storeposition;
        char storename[20];
        int storeplnum;
    }store;

    int i;
    headptr=NULL;
    for(i=0;i<PL;i++)
    {
        /*create and insert data into the created node*/
        newptr=(struct sortwinner*)malloc(sizeof(struct sortwinner));
        strcpy(newptr->sortname,player[i].name);
        newptr->sortplnum=i+1;
        newptr->sortposition=player[i].snake.after+1;

        if(headptr==NULL)
        {
            headptr=newptr;
            newptr->prevptr=NULL;
        }
        else
        {
            tailptr->nextptr=newptr;
            newptr->prevptr=tailptr;
        }
        tailptr=newptr;
        newptr->nextptr=NULL;
    }

    /*SORTING - BUBBLE SORT*/
    currentptr=tailptr;

    while(currentptr!=NULL)
    {
        walker=headptr;
        while(walker->nextptr!=NULL)
        {
            if(walker->sortposition<walker->nextptr->sortposition)
            {
                store.storeposition=walker->nextptr->sortposition;
                store.storeplnum=walker->nextptr->sortplnum;
                strcpy(store.storename,walker->nextptr->sortname);
                walker->nextptr->sortposition=walker->sortposition;
                walker->nextptr->sortplnum=walker->sortplnum;
                strcpy(walker->nextptr->sortname,walker->sortname);
                walker->sortposition=store.storeposition;
                walker->sortplnum=store.storeplnum;
                strcpy(walker->sortname,store.storename);
            }
            walker=walker->nextptr;
        }
        currentptr=currentptr->prevptr;
    }
    system("cls");
    currentptr=headptr;
    printf("THE WINNER IS FOUND!!\n\n\n");
    i=1;
    while(currentptr!=NULL)
    {
        switch(i)
        {
            case 1 : printf("[1st/GOLD]\nPlayer %d\nName: %s\nPosition: %d\n\n", currentptr->sortplnum, currentptr->sortname, currentptr->sortposition);break;
            case 2 : printf("[2nd/SILVER]\nPlayer %d\nName: %s\nPosition: %d\n\n", currentptr->sortplnum, currentptr->sortname, currentptr->sortposition);break;
            case 3 : printf("[3rd/BRONZE]\nPlayer %d\nName: %s\nPosition: %d\n\n", currentptr->sortplnum, currentptr->sortname, currentptr->sortposition);break;
            case 4 : printf("[4th/BRICK]\nPlayer %d\nName: %s\nPosition: %d\n", currentptr->sortplnum, currentptr->sortname, currentptr->sortposition);break;
        }
        currentptr=currentptr->nextptr;
        i++;
    }
    printf("\n\n\nType EXIT to continue to the final screen\n");
    while(strcmp(buffer,"EXIT")!=0)
    {
        fflush(stdin);
        scanf("%s", &buffer);
        strupr(buffer);
    }
    while(selection!='X')
    {
        system("cls");
        printf("THANKS FOR PLAYING SNAKES AND LADDERS\n\nNow, you may do the following\n");
        printf("S: Search a player moved position\nQ: Dequeue a player movement history\nD: Display a player movement history\nX: Exit the game\nInput: ");
        fflush(stdin);
        scanf("%c", &selection);
        switch(toupper(selection))
        {
            case 'S' : Search();break;
            case 'Q' : Dequeue();break;
            case 'D' : Display();break;
            case 'X' : printf("\n\n\nPlease play again sometime soon! We will be happy to see you again!\n\n\n\n\n\n\n\n");exit(0);break;
            default  : printf("Invalid Input!!");
        }

    }
}

/*Searching algorithm*/
void Search()
{
    int i, k=FALSE, l, c=FALSE;
    char j[4];
    system("cls");
    refresh();
    printf("Enter which player number movement to search(1-%d): ", PL);
    scanf("%d", &i);
    while(i<1||i>4)
    {
        printf("\nPlayer %d will never exist!!!", i);
        printf("\nEnter which player number movement to search(1-%d): ", PL);
        scanf("%d", &i);
    }
    i--;
    if(player[i].headnode==NULL)
    {
        printf("\nPlayer %d movement history is empty, there is nothing to search for", i+1);
    }
    else
    {
        printf("Enter the position you would like to search(SRT, 001-100): ");
        fflush(stdin);
        scanf("%s", &j);
        while(k==FALSE)
        {
            l=0;
            while(k==FALSE)
            {
                if((strcmp(strupr(j),square[l])==0)||((strcmp(strupr(j),"SRT")==0)))
                {
                    k=TRUE;
                    goto endloop;
                }
                l++;
                if(l==100)
                    goto endloop;
            }
            endloop:
            if(k==FALSE)
            {
                printf("\nInvalid Input!! There are only SRT(START), Position 1 - 100 on the snake board");
                printf("\nEnter the position you would like to search(SRT, 001-100): ");
                scanf("%s", &j);
            }
        }
        player[i].currentnode=player[i].headnode;
        k=1;
        while(k!=0&&player[i].currentnode!=NULL)
        {
            k=strcmp(player[i].currentnode->position,j);
            if(k!=0)
            player[i].currentnode=player[i].currentnode->nextnode;
        }
        if(player[i].currentnode!=NULL)
            printf("\n\nBINGO!!Position '%s' found in Player %d movement history, Player %d has indeed moved into position %s in the last match",j,i+1,i+1,j);
        else
        {
            printf("\nThe system could not find Position '%s' in Player %d movement history, Player %d did not move into position %s in the last match", j,i+1,i+1,j);
        }
    }
    printf("\nPress any key to continue..");
    getch();
}

/*Dequeue Algorithm*/
void Dequeue()
{
    int i;
    system("cls");
    printf("Enter which player number movement history you would like to dequeue(1-%d): ", PL);
    scanf("%d", &i);
    while(i<1||i>4)
    {
        printf("\nPlayer %d will never exist!!!", i);
        printf("\nEnter which player number movement history you would like to dequeue(1-%d): ", PL);
        scanf("%d", &i);
    }
    i--;
    if(player[i].headnode==NULL)
        printf("\n\nThere is no movement history for Player %d", i+1);
    else
    {
        player[i].currentnode=player[i].headnode;
        player[i].headnode=player[i].headnode->nextnode;
        printf("\n'%s' has been dequeued from Player %d movement history", player[i].currentnode->position, i+1);
        free(player[i].currentnode);
        printf("\n\nPress any key to see Player %d current movement history..", i+1);
        getch();
        system("cls");
        printf("Player %d current movement history:\n", i+1);
        player[i].currentnode=player[i].headnode;
        while(player[i].currentnode!=NULL)
        {
            printf("%s ",player[i].currentnode->position);
            player[i].currentnode=player[i].currentnode->nextnode;
            if(player[i].currentnode!=NULL)
                printf("\n-> ");
        }
    }
    printf("\n\nPress any key to continue..");
    getch();
}

/*Display player movement history algorithm*/
void Display()
{
    int i;
    system("cls");
    printf("Enter which player number movement history you would like to see(1-%d): ", PL);
    scanf("%d", &i);
    while(i<1||i>4)
    {
        printf("\nPlayer %d will never exist!!!", i);
        printf("\nEnter which player number movement history you would like to see(1-%d): ", PL);
        scanf("%d", &i);
    }
    i--;
    if(player[i].headnode==NULL)
        printf("\n\nThere is no movement history for Player %d", i+1);
    else
    {
        printf("\n\nPlayer %d Position History: \n", i+1);
        player[i].currentnode=player[i].headnode;
        while(player[i].currentnode!=NULL)
        {
            printf("%s ",player[i].currentnode->position);
            player[i].currentnode=player[i].currentnode->nextnode;
            if(player[i].currentnode!=NULL)
                printf("\n-> ");
        }
    }
    printf("\n\nPress any key to continue..");
    getch();
}

/*Deciding which player starts the turn before game starts*/
int initialize()
{
    int play[PL], dies[2], total[PL], a, b, c, d, compare=0;

    system("cls");
    for(a=0;a<PL;a++)
    {
        printf("[Player %d(%s) is rolling dies]\n1st roll: [PRESS ANY KEY TO ROLL]\n2nd roll: \n", a+1, player[a].name);
        getch();system("cls");
        dies[0]=random();
        printf("[Player %d(%s) is rolling dies]\n1st roll: %d\n2nd roll: [PRESS ANY KEY TO ROLL]\n", a+1, player[a].name, dies[0]);
        getch();system("cls");
        dies[1]=random();
        printf("[Player %d(%s) is rolling dies]\n1st roll: %d\n2nd roll: %d", a+1, player[a].name, dies[0], dies[1]);
        total[a]=recursion(dies[0], dies[1]);
        printf("\n\nTotal scored : %d * %d = %d", dies[0], dies[1], total[a]);
        for(d=0;d<PL;d++)
        {
            if((total[a]==total[d])&&(a!=d))
            {
                printf(", but unfortunately you got the same score with Player %d, so you will have to re-reroll your dies again!",d+1);
                a--;
                goto quit;
            }
        }
    quit:printf("\nPress any key to continue..");
        getch();
        system("cls");
    }
    printf("[Final Score]\n");
    for(a=0;a<PL;a++)
    {
        printf("Player %d: %d\n",a+1, total[a]);
    }
    b=0;
    for(a=0;a<PL;a++)
    {
        if(total[a]>compare)
        {
            compare=total[a];
            b=a;
            c=a;
        }
    }
    printf("\nThe highest score goes to Player %d(%d)",b+1,total[b]);
    printf("\nThe game will start with:");
    for(a=0;a<PL;a++)
    {
        if(b==PL)
            printf("\n%d. Player %d(%s)",a+1, (b+1)%PL, player[b%PL].name);
        else if(b>PL)
            printf("\n%d. Player %d(%s)",a+1, b+1-PL, player[b-PL].name);
        else
            printf("\n%d. Player %d(%s)",a+1, b+1, player[b].name);
        b++;
    }
    printf("\n\nPress any key to start the game..");
    getch();

    return c;
}

int recursion(int one, int two)
{
    if(two==1)
    {
        return one;
    }
    else
    {
        return one+recursion(one,two-1);
    }
}
