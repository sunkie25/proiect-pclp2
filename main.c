/*
    proiect SMECHER pt PCLP 2!!

    12.04.2026 (in gand, ca de apucat [serios] m-am apucat pe 27.04.2026 😤)

    pacanele 3x5 ca nu suntem lenesi sa facem doar 3x3
    - 5 linii, cine sta sa faca 20 lmao
    - toate combinatiile posibile de linii sunt incarcate din .txt sa nu mai umplu memoria cu gunoaie degeaba
    - 1 si 0 folosit pt fiecare combinatie posibila (1 e win, 0 e lose)
    - fiecare element are plati diferite
    - pornesti cu 10000 balanta
    - poti juca 4 mize: 5, 10, 50, 100
*/

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glut.h>
#include <GL/freeglut.h>

#define DEFAULT_CASH_BALANCE        (10000)
#define DEFAULT_BET                 (5)

#define GAME_WINDOW_WIDTH           (1152)
#define GAME_WINDOW_HEIGHT          (640)

int cashBalance = DEFAULT_CASH_BALANCE;
int selectedBet = 1;

char slotItemTypes[5][2] = {
    "#", "@", "$", "%", "&"
};

void drawSlotMachineContainer() 
{
    int i = 0;
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
    // contur
    glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2f(-0.95f, -0.5f);
	glVertex2f( 0.95f, -0.5f);
	glVertex2f( 0.95f, 0.9f);
	glVertex2f(-0.95f, 0.9f);
	glEnd();

    // fundal
    glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(-0.94f, -0.48f);
	glVertex2f( 0.94f, -0.48f);
	glVertex2f( 0.94f, 0.88f);
	glVertex2f(-0.94f, 0.88f);
	glEnd();

    // linii intre... linii
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.95f, 0.45f);
    glVertex2f(0.95f, 0.45f);
    glVertex2f(0.95f, 0.435f);
    glVertex2f(-0.95f, 0.435f);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(-0.95f, -0.02f);
    glVertex2f(0.95f, -0.02f);
    glVertex2f(0.95f, -0.035f);
    glVertex2f(-0.95f, -0.035f);
    glEnd();

    // coloane
    for(i = 0; i < 4; i++)
    {
        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2f(-0.575f + i * 0.38f, -0.5f);
        glVertex2f(-0.575f + i * 0.38f + 0.01f, -0.5f);
        glVertex2f(-0.575f + i * 0.38f + 0.01f, 0.9f);
        glVertex2f(-0.575f + i * 0.38f, 0.9f);
        glEnd();
    }

    // balanta
    for(i = 0; i < 4; i++)
    {
        glBegin(GL_QUADS);
        glColor3f(0.7f, 0.02f, 0.0f);
        glVertex2f(-0.65f + i * 0.3f + 0.1f, -0.80f);
        glVertex2f(-0.35f + i * 0.3f, -0.80f);
        glVertex2f(-0.35f + i * 0.3f, -0.60f);
        glVertex2f(-0.65f + i * 0.3f + 0.1f, -0.60f);
        glEnd();
    }

    glBegin(GL_QUADS);
    glColor3f(0.05f, 0.92f, 0.0f);
    glVertex2f(0.65f, -0.80f);
    glVertex2f(0.9f, -0.80f);
    glVertex2f(0.9f, -0.60f);
    glVertex2f(0.65f, -0.60f);
    glEnd();

	glFlush();
}

void reshape(int width, int height) 
{
    if (width != GAME_WINDOW_WIDTH || height != GAME_WINDOW_HEIGHT) 
    {
        glutReshapeWindow(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);
    }
}

int main(int argc, char** argv) 
{
    glutInit(&argc, argv);
    glutInitWindowSize(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);

    int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    int windowPosX = (screenWidth - GAME_WINDOW_WIDTH) / 2;
    int windowPosY = (screenHeight - GAME_WINDOW_HEIGHT) / 2;

    glutInitWindowPosition(windowPosX, windowPosY);
    glutCreateWindow("Slot Machine");

    glutDisplayFunc(drawSlotMachineContainer);
    glutReshapeFunc(reshape); 
    glutMainLoop();
    return 0;
}

// int main()
// {
//     int i = 0, j = 0;
//     while(1)
//     {
//         printf("Balance: %d coins\n", cashBalance);
//         fflush(stdout);

//         printf("Choose your bet (5, 10, 50, 100): ");
//         scanf("%d", &selectedBet);

//         while(selectedBet != 5 && selectedBet != 10 && selectedBet != 50 && selectedBet != 100)
//         {
//             printf("Invalid bet. Please choose again (5, 10, 50, 100): ");
//             scanf("%d", &selectedBet);
//         }

//         if(selectedBet > cashBalance)
//         {
//             printf("Insufficient balance. Please choose a smaller bet.\n");
//             continue;
//         }

//         cashBalance -= selectedBet;
//         printf("Spinning the reels...\n");

//         showFinalSlotsPanel();
//     }

//     return 0;
// }