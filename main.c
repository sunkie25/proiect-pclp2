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

#include <string.h>
#include <stdio.h>

#include <GL/glut.h>
#include <GL/freeglut.h>

#define DEFAULT_CASH_BALANCE        (1000)
#define DEFAULT_BET                 (5)

#define GAME_WINDOW_WIDTH           (1152)
#define GAME_WINDOW_HEIGHT          (640)

int cashBalance = DEFAULT_CASH_BALANCE;

int availableBets[4] = {5, 10, 50, 100};
int selectedBet = 2;

char slotItemTypes[5][2] = {
    "#", "@", "$", "%", "&"
};

char errorMessage[64] = "";

void createErrorText()
{
    int i = 0;
    if(strlen(errorMessage) == 0) return;

    glColor3f(1.0f, 0.0f, 0.0f);
    glRasterPos2f(-0.95f, -0.95f);

    for(i = 0; i < (int)strlen(errorMessage); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, errorMessage[i]);
    }
}

void hideError()
{
    errorMessage[0] = '\0';
    glutPostRedisplay();
}

void showError(char content[64])
{
    strcpy(errorMessage, content);
    glutPostRedisplay();
}

void updateCashBalanceUI()
{
    int i = 0;
    char balanceText[32];

    sprintf(balanceText, "BALANCE:");
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(-0.95f, -0.65f);

    for(i = 0; i < (int)strlen(balanceText); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, balanceText[i]);
    }

    balanceText[0] = '\0';
    sprintf(balanceText, "%d COINS", cashBalance);

    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(-0.95f, -0.75f);

    for(i = 0; i < (int)strlen(balanceText); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, balanceText[i]);
    }
}

void updateBetButtonsUI()
{
    int i = 0, j = 0;
    char betText[16];

    for(i = 0; i < 4; i++)
    {
        glBegin(GL_QUADS);
        if(selectedBet == i)
        {
            glColor3f(0.0f, 0.92f, 0.0f);
        }

        else
        {
            glColor3f(0.7f, 0.02f, 0.0f);
        }

        glVertex2f(-0.65f + i * 0.3f + 0.1f, -0.80f);
        glVertex2f(-0.35f + i * 0.3f, -0.80f);
        glVertex2f(-0.35f + i * 0.3f, -0.60f);
        glVertex2f(-0.65f + i * 0.3f + 0.1f, -0.60f);
        glEnd();

        betText[0] = '\0';
        sprintf(betText, "BET %d", availableBets[i]);

        glColor3f(0.0f, 0.0f, 0.0f);
        glRasterPos2f(-0.5f + (i * 0.3f), -0.72f);

        for(j = 0; j < (int)strlen(betText); j++)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, betText[j]);
        }
    }
}

void drawSlotMachineContainer() 
{
    int i = 0, j = 0;
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

    glBegin(GL_QUADS);
    glColor3f(0.05f, 0.92f, 0.0f);
    glVertex2f(0.65f, -0.80f);
    glVertex2f(0.9f, -0.80f);
    glVertex2f(0.9f, -0.60f);
    glVertex2f(0.65f, -0.60f);
    glEnd();

    // balanta
    updateCashBalanceUI();

    // BET
    updateBetButtonsUI();

    // SPIN
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(0.735f, -0.72f);

    char spinText[5] = "SPIN";
    for(i = 0; i < (int)strlen(spinText); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, spinText[i]);
    }

    // error text
    createErrorText();

	glFlush();
}

void handleSpinButton()
{   
    printf("\nSPIN BUTTON CLICKED!\n");
    if(cashBalance < availableBets[selectedBet])
    {
        showError("You don't have enough coins to select this bet!");
        return;
    }

    cashBalance -= availableBets[selectedBet];
    updateCashBalanceUI();
    glutPostRedisplay();
}

void onMouse(int button, int state, int x, int y) 
{
    if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) 
    {
        int clickedBetButton = 0;
        int betButtonCoords[][4] = {
            {261, 500, 371, 585},
            {433, 500, 543, 585},
            {606, 500, 725, 585},
            {780, 500, 890, 585}
        };

        int x1 = 0, x2 = 0, y1 = 0, y2 = 0, temp = 0;

        for(int i = 0; i < 4; i++)
        {
            x1 = betButtonCoords[i][0];
            y1 = betButtonCoords[i][1];
            x2 = betButtonCoords[i][2];
            y2 = betButtonCoords[i][3];

            if(x1 > x2) { temp = x1; x1 = x2; x2 = temp; }
            if(y1 > y2) { temp = y1; y1 = y2; y2 = temp; }

            if(x >= x1 && x <= x2 && y >= y1 && y <= y2)
            {
                printf("Clicked BET button %d\n", i);

                if(cashBalance < availableBets[i])
                {
                    showError("You don't have enough coins to select this bet!");
                    printf("n-ai bani boss\n");
                    break;
                }

                selectedBet = i;
                updateBetButtonsUI();
                glutPostRedisplay();
                clickedBetButton = 1;
                break;
            }
        }

        printf("\nclick la %d %d\n", x, y);

        if(!clickedBetButton)
        {
            // verifica dupa spin
            x1 = 950;
            x2 = 1093;
            y1 = 510;
            y2 = 580;

            if(x1 > x2) { temp = x1; x1 = x2; x2 = temp; }
            if(y1 > y2) { temp = y1; y1 = y2; y2 = temp; }

            if(x >= x1 && x <= x2 && y >= y1 && y <= y2)
            {
                handleSpinButton();
            }
        }
    }
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
    glutMouseFunc(onMouse);

    glutDisplayFunc(drawSlotMachineContainer);
    glutReshapeFunc(reshape); 
    glutMainLoop();
    return 0;
}
