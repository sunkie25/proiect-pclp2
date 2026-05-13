/*
    proiect SMECHER pt PCLP 2!!

    12.04.2026 (in gand, ca de apucat [serios] m-am apucat pe 27.04.2026 😤)

    așa-i că esti curios ce scria aici?? 😉
    de ce citesti???
    
    
    incă nu esti curios de cod?




    scria mai sus ce face codul dar nu avem voie cu comentarii despre cod 😤😤😤






    cel mai smecher proiect!!!
*/

#ifdef _WIN32
#include <windows.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/glut.h>
#include <GL/freeglut.h>

#define TEST_MODE

#define NOTIFICATION_TYPE_ERROR         (0)
#define NOTIFICATION_TYPE_SUCCESS       (1)
#define NOTIFICATION_TYPE_WARNING       (2)

#define GAME_WINDOW_WIDTH               (1152)
#define GAME_WINDOW_HEIGHT              (640)

#ifdef TEST_MODE
#define DEFAULT_CASH_BALANCE            (500)
#else
#define DEFAULT_CASH_BALANCE            (10000)
#endif

#define DEFAULT_BET                     (5)

#define ITEM_TYPE_TEST                  (0)
#define ITEM_TYPE_CASH                  (1)
#define ITEM_TYPE_DIAMOND               (2)
#define ITEM_TYPE_ROMB                  (3)
#define ITEM_TYPE_BAR                   (4)

#define SPIN_TIMER_DELAY                (150)

#define MAX_THEMES                      (3)

#define OFFSETS_FILE                    "offsets.txt"
#define LOGS_FILE                       "logs.txt"
#define THEME_FILE                      "theme.txt"

int cashBalance = DEFAULT_CASH_BALANCE;

int availableBets[4] = {5, 10, 50, 100};
int selectedBet = 3;

int notificationTimerId = 0;
int notificationType = NOTIFICATION_TYPE_ERROR;

int slotItemWinMultipliers[5] = {10, 2, 5, 7, 4};
int lineWinMultiplier[3] = {1, 5, 10};

struct slotItemDesign {
    float startX;
    float startY;
    float endX;
    float endY;
} slotItemDesigns[15];

int slotItemWon[15], displayedItems[15], finalItems[15];
int spinStep = 0;
int slotWinAmount = 0, lastBetAmount = 0;
int themePicked = 1;

char notificationMessage[64] = "";

void userLog(char message[128])
{
    FILE *logFile = fopen(LOGS_FILE, "a");
    if(logFile == NULL) return;

    fprintf(logFile, "%s\n", message);
    fclose(logFile);
}

void loadTheme()
{
    FILE *f_in = fopen(THEME_FILE, "r");
    if(f_in == NULL)    
    {
        printf("No theme file found!\n");
        return;
    }

    fscanf(f_in, "%d", &themePicked);
    fclose(f_in);
}

void saveTheme()
{
    FILE *f_out = fopen(THEME_FILE, "w");
    if(f_out == NULL)
    {
        printf("Error opening theme file for writing!\n");
        return;
    }

    fprintf(f_out, "%d", themePicked);
    fclose(f_out);
}

void createNotificationText()
{
    int i = 0;
    if(strlen(notificationMessage) == 0) return;

    if(notificationType == NOTIFICATION_TYPE_ERROR)
    {
        glColor3f(1.0f, 0.0f, 0.0f);
    }

    else if(notificationType == NOTIFICATION_TYPE_SUCCESS)
    {
        glColor3f(0.0f, 0.8f, 0.0f);
    }

    else if(notificationType == NOTIFICATION_TYPE_WARNING)
    {
        glColor3f(1.0f, 0.65f, 0.0f);
    }

    glRasterPos2f(-0.95f, -0.95f);

    for(i = 0; i < (int)strlen(notificationMessage); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, notificationMessage[i]);
    }
}

void hideNotification()
{
    notificationMessage[0] = '\0';
    glutPostRedisplay();
}

void hideNotificationDelayed(int value)
{
    if(value == notificationTimerId)
    {
        hideNotification();
    }
}

void showNotification(int type, char content[64])
{
    strcpy(notificationMessage, content);
    notificationType = type;
    notificationTimerId++;
    glutTimerFunc(5000, hideNotificationDelayed, notificationTimerId);
    glutPostRedisplay();
}

void updateCashBalanceUI()
{
    int i = 0;
    char balanceText[32];

    sprintf(balanceText, "BALANCE:");
    if(themePicked == 0) glColor3f(0.0f, 0.0f, 0.0f);
    else if(themePicked == 1) glColor3f(0.0f, 0.0f, 0.0f);
    else if(themePicked == 2) glColor3f(1.0f, 0.67f, 0.7f);
    
    glRasterPos2f(-0.95f, -0.65f);

    for(i = 0; i < (int)strlen(balanceText); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, balanceText[i]);
    }

    balanceText[0] = '\0';
    sprintf(balanceText, "%d COINS", cashBalance);

    if(themePicked == 0) glColor3f(0.0f, 0.0f, 0.0f);
    else if(themePicked == 1) glColor3f(0.0f, 0.0f, 0.0f);
    else if(themePicked == 2) glColor3f(1.0f, 0.67f, 0.7f);

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
            if(themePicked == 0) glColor3f(0.0f, 0.92f, 0.0f);
            else if(themePicked == 1) glColor3f(0.02f, 0.45f, 0.31f);
            else if(themePicked == 2) glColor3f(0.81f, 0.2f, 0.4f);
        }

        else
        {
            if(themePicked == 0) glColor3f(0.7f, 0.02f, 0.0f);
            else if(themePicked == 1) glColor3f(0.7f, 0.08f, 0.16f);
            else if(themePicked == 2) glColor3f(1.0f, 0.67f, 0.7f);
        }

        glVertex2f(-0.65f + i * 0.3f + 0.1f, -0.80f);
        glVertex2f(-0.35f + i * 0.3f, -0.80f);
        glVertex2f(-0.35f + i * 0.3f, -0.60f);
        glVertex2f(-0.65f + i * 0.3f + 0.1f, -0.60f);
        glEnd();

        betText[0] = '\0';
        sprintf(betText, "BET %d", availableBets[i]);

        if(themePicked == 0) glColor3f(0.0f, 0.0f, 0.0f);
        else if(themePicked == 1) glColor3f(1.0f, 1.0f, 1.0f);
        else if(themePicked == 2) glColor3f(0.0f, 0.0f, 0.0f);

        glRasterPos2f(-0.5f + (i * 0.3f), -0.72f);

        for(j = 0; j < (int)strlen(betText); j++)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, betText[j]);
        }
    }
}

void updateWonItems()
{
    int i = 0, x[15];
    for(i = 0; i < 15; i++)
    {
        slotItemWon[i] = 0;
        x[i] = finalItems[i];
    }

    slotWinAmount = 0;
    int lineCombos[5] = {0, 0, 0, 0, 0};

    int lineItems[5] = {x[0], x[1], x[2], x[0], x[2]};

    if(x[0] == x[3] && x[3] == x[6] && x[6] != x[9]) slotItemWon[0] = slotItemWon[3] = slotItemWon[6] = 1, lineCombos[0] = 3;
    else if(x[0] == x[3] && x[3] == x[6] && x[6] == x[9] && x[9] != x[12]) slotItemWon[0] = slotItemWon[3] = slotItemWon[6] = slotItemWon[9] = 1, lineCombos[0] = 4;
    else if(x[0] == x[3] && x[3] == x[6] && x[6] == x[9] && x[9] == x[12]) slotItemWon[0] = slotItemWon[3] = slotItemWon[6] = slotItemWon[9] = slotItemWon[12] = 1, lineCombos[0] = 5;

    if(x[1] == x[4] && x[4] == x[7] && x[7] != x[10]) slotItemWon[1] = slotItemWon[4] = slotItemWon[7] = 1, lineCombos[1] = 3;
    else if(x[1] == x[4] && x[4] == x[7] && x[7] == x[10] && x[10] != x[13]) slotItemWon[1] = slotItemWon[4] = slotItemWon[7] = slotItemWon[10] = 1, lineCombos[1] = 4;
    else if(x[1] == x[4] && x[4] == x[7] && x[7] == x[10] && x[10] == x[13]) slotItemWon[1] = slotItemWon[4] = slotItemWon[7] = slotItemWon[10] = slotItemWon[13] = 1, lineCombos[1] = 5;

    if(x[2] == x[5] && x[5] == x[8] && x[8] != x[11]) slotItemWon[2] = slotItemWon[5] = slotItemWon[8] = 1, lineCombos[2] = 3;
    else if(x[2] == x[5] && x[5] == x[8] && x[8] == x[11] && x[11] != x[14]) slotItemWon[2] = slotItemWon[5] = slotItemWon[8] = slotItemWon[11] = 1, lineCombos[2] = 4;
    else if(x[2] == x[5] && x[5] == x[8] && x[8] == x[11] && x[11] == x[14]) slotItemWon[2] = slotItemWon[5] = slotItemWon[8] = slotItemWon[11] = slotItemWon[14] = 1, lineCombos[2] = 5;

    if(x[0] == x[4] && x[4] == x[8] && x[8] != x[10]) slotItemWon[0] = slotItemWon[4] = slotItemWon[8] = 1, lineCombos[3] = 3;
    else if(x[0] == x[4] && x[4] == x[8] && x[8] == x[10] && x[10] != x[12]) slotItemWon[0] = slotItemWon[4] = slotItemWon[8] = slotItemWon[10] = 1, lineCombos[3] = 4;
    else if(x[0] == x[4] && x[4] == x[8] && x[8] == x[10] && x[10] == x[12]) slotItemWon[0] = slotItemWon[4] = slotItemWon[8] = slotItemWon[10] = slotItemWon[12] = 1, lineCombos[3] = 5;

    if(x[2] == x[4] && x[4] == x[6] && x[6] != x[10]) slotItemWon[2] = slotItemWon[4] = slotItemWon[6] = 1, lineCombos[4] = 3;
    else if(x[2] == x[4] && x[4] == x[6] && x[6] == x[10] && x[10] != x[14]) slotItemWon[2] = slotItemWon[4] = slotItemWon[6] = slotItemWon[10] = 1, lineCombos[4] = 4;
    else if(x[2] == x[4] && x[4] == x[6] && x[6] == x[10] && x[10] == x[14]) slotItemWon[2] = slotItemWon[4] = slotItemWon[6] = slotItemWon[10] = slotItemWon[14] = 1, lineCombos[4] = 5;
    
    for(i = 0; i < 5; i++)
    {
        if(lineCombos[i] >= 3)
        {
            slotWinAmount += lastBetAmount * slotItemWinMultipliers[lineItems[i]] * lineWinMultiplier[lineCombos[i] - 3];
        }
    }
}

void shuffleSlotItems()
{
    int i = 0;
    for(i = 0; i < 15; i++)
    {
        slotItemWon[i] = 0;
        displayedItems[i] = rand() % 5;
    }
}

void generateFinalItems()
{
    int i = 0;
    for(i = 0; i < 15; i++)
    {
        slotItemWon[i] = 0;
        finalItems[i] = rand() % 5;

        while(displayedItems[i] == finalItems[i])
        {
            displayedItems[i] = rand() % 5;
        }
    }
}

void drawSlotItem(int type, float x, float y)
{
    int i = 0;
    switch(type)
    {
        case ITEM_TYPE_TEST:
        {
            // {-0.94f, 0.88f, -0.57f, 0.45f},
            
            glBegin(GL_QUADS);
            
            if(themePicked == 0) glColor3f(1.0f, 0.0f, 0.0f);
            else if(themePicked == 1) glColor3f(0.09f, 0.36f, 0.095f);
            else if(themePicked == 2) glColor3f(1.0f, 0.0f, 0.0f);

            glVertex2f(x + 0.1f, y - 0.08f);
            glVertex2f(x + 0.255f, y - 0.08f);
            glVertex2f(x + 0.255f, y - 0.12f);
            glVertex2f(x + 0.1f, y - 0.12f);
            glEnd();

            glBegin(GL_QUADS);

            if(themePicked == 0) glColor3f(1.0f, 0.0f, 0.0f);
            else if(themePicked == 1) glColor3f(0.09f, 0.36f, 0.095f);
            else if(themePicked == 2) glColor3f(1.0f, 0.0f, 0.0f);

            glVertex2f(x + 0.255f, y - 0.11f);
            glVertex2f(x + 0.145f, y - 0.36f);
            glVertex2f(x + 0.115f, y - 0.36f);
            glVertex2f(x + 0.225f, y - 0.11f);
            glEnd();

            break;
        }

        case ITEM_TYPE_CASH:
        {
            for(i = 0; i < 3; i++)
            {
                glBegin(GL_QUADS);
                
                if(themePicked == 0) glColor3f(0.0f, 0.8f, 0.1f);
                else if(themePicked == 1) glColor3f(0.05f, 0.07f, 0.55f);
                else if(themePicked == 2) glColor3f(0.0f, 0.8f, 0.1f);

                glVertex2f(x + 0.1f, y - 0.08f - i * 0.11f);
                glVertex2f(x + 0.255f, y - 0.08f - i * 0.11f);
                glVertex2f(x + 0.255f, y - 0.12f - i * 0.11f);
                glVertex2f(x + 0.1f, y - 0.12f - i * 0.11f);
                glEnd();
            }

            glBegin(GL_QUADS);

            if(themePicked == 0) glColor3f(0.0f, 0.8f, 0.1f);
            else if(themePicked == 1) glColor3f(0.05f, 0.07f, 0.55f);
            else if(themePicked == 2) glColor3f(0.0f, 0.8f, 0.1f);

            glVertex2f(x + 0.1f, y - 0.08f);
            glVertex2f(x + 0.125f, y - 0.08f);
            glVertex2f(x + 0.125f, y - 0.22f);
            glVertex2f(x + 0.1f, y - 0.22f);
            glEnd();

            glBegin(GL_QUADS);

            if(themePicked == 0) glColor3f(0.0f, 0.8f, 0.1f);
            else if(themePicked == 1) glColor3f(0.05f, 0.07f, 0.55f);
            else if(themePicked == 2) glColor3f(0.0f, 0.8f, 0.1f);

            glVertex2f(x + 0.23f, y - 0.20f);
            glVertex2f(x + 0.255f, y - 0.20f);
            glVertex2f(x + 0.255f, y - 0.34f);
            glVertex2f(x + 0.23f, y - 0.34f);
            glEnd();

            glBegin(GL_QUADS);

            if(themePicked == 0) glColor3f(0.0f, 0.8f, 0.1f);
            else if(themePicked == 1) glColor3f(0.05f, 0.07f, 0.55f);
            else if(themePicked == 2) glColor3f(0.0f, 0.8f, 0.1f);

            glVertex2f(x + 0.17f, y - 0.05f);
            glVertex2f(x + 0.195f, y - 0.05f);
            glVertex2f(x + 0.195f, y - 0.37f);
            glVertex2f(x + 0.17f, y - 0.37f);
            glEnd();
            break;
        }
    
        case ITEM_TYPE_DIAMOND:
        {
            glBegin(GL_TRIANGLES);

            if(themePicked == 0) glColor3f(0.2f, 0.6f, 0.9f);
            else if(themePicked == 1) glColor3f(0.7f, 0.12f, 0.27f);
            else if(themePicked == 2) glColor3f(0.2f, 0.6f, 0.9f);

            glVertex2f(x + 0.175f, y - 0.33f);
            glVertex2f(x + 0.1f, y - 0.13f);
            glVertex2f(x + 0.25f, y - 0.13f);
            glEnd();

            glBegin(GL_POLYGON);

            if(themePicked == 0) glColor3f(0.2f, 0.6f, 0.9f);
            else if(themePicked == 1) glColor3f(0.7f, 0.12f, 0.27f);
            else if(themePicked == 2) glColor3f(0.2f, 0.6f, 0.9f);

            glVertex2f(x + 0.1f, y - 0.13f);
            glVertex2f(x + 0.125f, y - 0.08f);
            glVertex2f(x + 0.225f, y - 0.08f);
            glVertex2f(x + 0.25f, y - 0.13f);
            glEnd();
            break;
        }
   
        case ITEM_TYPE_ROMB:
        {
            glBegin(GL_POLYGON);
            if(themePicked == 0) glColor3f(0.9f, 0.1f, 0.9f);
            else if(themePicked == 1) glColor3f(0.97f, 0.6f, 0.03f);
            else if(themePicked == 2) glColor3f(0.97f, 0.1f, 0.51f);
            glVertex2f(x + 0.175f, y - 0.10f);
            glVertex2f(x + 0.125f, y - 0.22f);
            glVertex2f(x + 0.175f, y - 0.34f);
            glVertex2f(x + 0.225f, y - 0.22f);
            glEnd();
            break;
        }
   
        case ITEM_TYPE_BAR:
        {
            glBegin(GL_QUADS);
            if(themePicked == 0) glColor3f(1.0f, 0.8f, 0.0f);
            else if(themePicked == 1) glColor3f(0.7f, 0.8f, 0.0f);
            else if(themePicked == 2) glColor3f(1.0f, 0.8f, 0.0f);
            glVertex2f(x + 0.10f, y - 0.1f);
            glVertex2f(x + 0.255f, y - 0.1f);
            glVertex2f(x + 0.255f, y - 0.3f);
            glVertex2f(x + 0.10f, y - 0.3f);
            glEnd();

            if(themePicked == 0) glColor3f(0.0f, 0.0f, 0.0f);
            else if(themePicked == 1) glColor3f(0.0f, 0.0f, 0.0f);
            else if(themePicked == 2) glColor3f(0.0f, 0.0f, 0.0f);
            glRasterPos2f(x + 0.145f, y - 0.22f);

            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'B');
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'A');
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'R');
            break;
        }
    }
}

void drawSlotItems()
{
    int i = 0, j = 0;
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 5; j++)
        {
            glBegin(GL_QUADS);
            
            if(slotItemWon[i * 5 + j])
            {
                if(themePicked == 0) glColor3f(0.0f, 0.92f, 0.0f);
                else if(themePicked == 1) glColor3f(0.0f, 0.92f, 0.0f);
                else if(themePicked == 2) glColor3f(0.4f, 0.8f, 0.56f);
            }

            else
            {
                if(themePicked == 0) glColor3f(0.94f, 0.6f, 0.5f);
                else if(themePicked == 1) glColor3f(0.999f, 0.764f, 0.5f);
                else if(themePicked == 2) glColor3f(0.8f, 0.33f, 0.64f);
            }

            glVertex2f(slotItemDesigns[i * 5 + j].startX, slotItemDesigns[i * 5 + j].startY);
            glVertex2f(slotItemDesigns[i * 5 + j].endX, slotItemDesigns[i * 5 + j].startY);
            glVertex2f(slotItemDesigns[i * 5 + j].endX, slotItemDesigns[i * 5 + j].endY);
            glVertex2f(slotItemDesigns[i * 5 + j].startX, slotItemDesigns[i * 5 + j].endY);
            glEnd();

            drawSlotItem(displayedItems[i * 5 + j], slotItemDesigns[i * 5 + j].startX, slotItemDesigns[i * 5 + j].startY);
        }
    }
}

void onSpinStop()
{
    spinStep = 0;

    #ifdef TEST_MODE
        printf("\nSPIN STOPPED!!!!");
    #endif

    updateWonItems();
    glutPostRedisplay();

    char message[64];
    if(slotWinAmount > 0)
    {
        sprintf(message, "(+) You won %d coins!", slotWinAmount);
        showNotification(NOTIFICATION_TYPE_SUCCESS, message);
        cashBalance += slotWinAmount;
        updateCashBalanceUI();

        sprintf(message, "[WIN] +%d coins (bet: %d, new balance: %d)", slotWinAmount, availableBets[selectedBet], cashBalance);
        userLog(message);
    }

    else
    {
        sprintf(message, "(-) You lost %d coins!", availableBets[selectedBet]);
        showNotification(NOTIFICATION_TYPE_ERROR, message);

        sprintf(message, "[LOSE] -%d coins (bet: %d, new balance: %d)", availableBets[selectedBet], availableBets[selectedBet], cashBalance);
        userLog(message);
    }
}

void spinTimer(int value)
{
    value = 0; // unused parameter
    
    if(spinStep >= 15)
    {
        onSpinStop();
        return;
    }

    displayedItems[spinStep] = finalItems[spinStep];
    spinStep++;

    glutPostRedisplay();

    if(spinStep < 15)
    {
        glutTimerFunc(SPIN_TIMER_DELAY, spinTimer, 0);
    }
    else
    {
        onSpinStop();
    }
}

void handleSpin()
{
    if(spinStep > 0) return;

    glutTimerFunc(SPIN_TIMER_DELAY, spinTimer, 0);
}

void drawSlotMachineContainer() 
{
    int i = 0;
    
    if(themePicked == 0) glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    else if(themePicked == 1) glClearColor(0.66f, 0.89f, 0.898f, 1.0f);
    else if(themePicked == 2) glClearColor(0.32f, 0.18f, 0.5f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);
	
    glBegin(GL_QUADS);
    
    if(themePicked == 0) glColor3f(0.0f, 0.0f, 0.0f);
    else if(themePicked == 1) glColor3f(0.1f, 0.05f, 0.05f);
    else if(themePicked == 2) glColor3f(0.53f, 0.12f, 0.6f);

	glVertex2f(-0.95f, -0.5f);
	glVertex2f( 0.95f, -0.5f);
	glVertex2f( 0.95f, 0.9f);
	glVertex2f(-0.95f, 0.9f);
	glEnd();

    // glBegin(GL_QUADS);
	// glColor3f(0.94f, 0.6f, 0.5f);
	// glVertex2f(-0.94f, -0.48f);
	// glVertex2f( 0.94f, -0.48f);
	// glVertex2f( 0.94f, 0.88f);
	// glVertex2f(-0.94f, 0.88f);
	// glEnd();

    drawSlotItems();

    glBegin(GL_QUADS);

    if(themePicked == 0) glColor3f(0.0f, 0.0f, 0.0f);
    else if(themePicked == 1) glColor3f(0.0f, 0.0f, 0.0f);
    else if(themePicked == 2) glColor3f(0.53f, 0.12f, 0.6f);

    glVertex2f(-0.95f, 0.45f);
    glVertex2f(0.95f, 0.45f);
    glVertex2f(0.95f, 0.435f);
    glVertex2f(-0.95f, 0.435f);
    glEnd();

    glBegin(GL_QUADS);

    if(themePicked == 0) glColor3f(0.0f, 0.0f, 0.0f);
    else if(themePicked == 1) glColor3f(0.0f, 0.0f, 0.0f);
    else if(themePicked == 2) glColor3f(0.53f, 0.12f, 0.6f);
    
    glVertex2f(-0.95f, -0.02f);
    glVertex2f(0.95f, -0.02f);
    glVertex2f(0.95f, -0.035f);
    glVertex2f(-0.95f, -0.035f);
    glEnd();

    for(i = 0; i < 4; i++)
    {
        glBegin(GL_QUADS);

        if(themePicked == 0) glColor3f(0.0f, 0.0f, 0.0f);
        else if(themePicked == 1) glColor3f(0.0f, 0.0f, 0.0f);
        else if(themePicked == 2) glColor3f(0.53f, 0.12f, 0.6f);

        glVertex2f(-0.575f + i * 0.38f, -0.5f);
        glVertex2f(-0.575f + i * 0.38f + 0.01f, -0.5f);
        glVertex2f(-0.575f + i * 0.38f + 0.01f, 0.9f);
        glVertex2f(-0.575f + i * 0.38f, 0.9f);
        glEnd();
    }

    glBegin(GL_QUADS);

    if(themePicked == 0) glColor3f(0.05f, 0.92f, 0.0f);
    else if(themePicked == 1) glColor3f(0.015f, 0.45f, 0.32f);
    else if(themePicked == 2) glColor3f(1.0f, 0.45f, 0.75f);
    
    glVertex2f(0.65f, -0.80f);
    glVertex2f(0.9f, -0.80f);
    glVertex2f(0.9f, -0.60f);
    glVertex2f(0.65f, -0.60f);
    glEnd();

    updateCashBalanceUI();

    updateBetButtonsUI();

    if(themePicked == 0) glColor3f(0.0f, 0.0f, 0.0f);
    else if(themePicked == 1) glColor3f(1.0f, 1.0f, 1.0f);
    else if(themePicked == 2) glColor3f(0.0f, 0.0f, 0.0f);

    glRasterPos2f(0.735f, -0.72f);

    char tempText[7] = "SPIN";
    for(i = 0; i < (int)strlen(tempText); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, tempText[i]);
    }

    glBegin(GL_QUADS);
    if(themePicked == 0) glColor3f(0.05f, 0.92f, 0.0f);
    else if(themePicked == 1) glColor3f(0.015f, 0.45f, 0.32f);
    else if(themePicked == 2) glColor3f(0.05f, 0.92f, 0.0f);

    glVertex2f(-0.94f, -0.87f);
    glVertex2f(-0.82f, -0.87f);
    glVertex2f(-0.82f, -0.80f);
    glVertex2f(-0.94f, -0.80f);
    glEnd();

    if(themePicked == 0) glColor3f(0.0f, 0.0f, 0.0f);
    else if(themePicked == 1) glColor3f(1.0f, 1.0f, 1.0f);
    else if(themePicked == 2) glColor3f(0.0f, 0.0f, 0.0f);

    glRasterPos2f(-0.92f, -0.85f);
    strcpy(tempText, "THEME");

    for(i = 0; i < (int)strlen(tempText); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, tempText[i]);
    }
    
    createNotificationText();

	glFlush();
}

void handleSpinButton()
{   
    #ifdef TEST_MODE
        printf("\nSPIN BUTTON CLICKED!\n");
    #endif

    if(spinStep > 0) return;
    if(selectedBet < 0)
    {
        showNotification(NOTIFICATION_TYPE_WARNING, "Please select a bet before spinning!");
        return;
    }


    if(cashBalance < availableBets[selectedBet])
    {
        showNotification(NOTIFICATION_TYPE_ERROR, "You don't have enough coins to select this bet!");

        selectedBet = -1;
        updateBetButtonsUI();
        return;
    }

    cashBalance -= availableBets[selectedBet];
    lastBetAmount = availableBets[selectedBet];
    updateCashBalanceUI();

    showNotification(NOTIFICATION_TYPE_SUCCESS, "Spinning the reels... Good luck!");
    
    shuffleSlotItems();
    generateFinalItems();

    handleSpin();

    glutPostRedisplay();
}

void handleThemeButton()
{
    if(themePicked < MAX_THEMES - 1) themePicked++;
    else themePicked = 0;

    saveTheme();
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
                #ifdef TEST_MODE
                    printf("Clicked BET button %d\n", i);
                #endif

                if(spinStep > 0)
                {
                    showNotification(NOTIFICATION_TYPE_WARNING, "You can't change your bet while the reels are spinning!");
                    return;
                }

                if(cashBalance < availableBets[i])
                {
                    showNotification(NOTIFICATION_TYPE_ERROR, "You don't have enough coins to select this bet!");

                    selectedBet = -1;

                    #ifdef TEST_MODE
                        printf("n-ai bani boss\n");
                    #endif
                }

                else
                {
                    selectedBet = i;

                    #ifdef TEST_MODE
                        printf("selected bet: %d\n", availableBets[selectedBet]);
                    #endif
                }

                updateBetButtonsUI();
                glutPostRedisplay();
                clickedBetButton = 1;
                break;
            }
        }

        #ifdef TEST_MODE
            printf("\nclick la %d %d\n", x, y);
        #endif

        if(!clickedBetButton)
        {
            int action = 0;
            x1 = 950;
            x2 = 1093;
            y1 = 510;
            y2 = 580;

            if(x1 > x2) { temp = x1; x1 = x2; x2 = temp; }
            if(y1 > y2) { temp = y1; y1 = y2; y2 = temp; }

            if(x >= x1 && x <= x2 && y >= y1 && y <= y2)
            {
                handleSpinButton();
                action = 1;
            }

            if(!action)
            {
                x1 = 35;
                x2 = 102;
                y1 = 576;
                y2 = 697;

                if(x1 > x2) { temp = x1; x1 = x2; x2 = temp; }
                if(y1 > y2) { temp = y1; y1 = y2; y2 = temp; }

                if(x >= x1 && x <= x2 && y >= y1 && y <= y2)
                {
                    handleThemeButton();
                    action = 1;
                }
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

void loadSlotItemOffsets()
{
    FILE *f_in = fopen(OFFSETS_FILE, "r");
    if(f_in == NULL)    
    {
        printf("Error opening offsets file!\n");
        return;
    }

    int i = 0;
    while(i < 15 && !feof(f_in))
    {
        fscanf(f_in, "%f %f %f %f", &slotItemDesigns[i].startX, &slotItemDesigns[i].startY, &slotItemDesigns[i].endX, &slotItemDesigns[i].endY);
        i++;
    }

    fclose(f_in);
}

int main(int argc, char** argv) 
{
    loadSlotItemOffsets();
    loadTheme();
    userLog("New slots session!!!!");

    glutInit(&argc, argv);
    glutInitWindowSize(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);

    int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    int windowPosX = (screenWidth - GAME_WINDOW_WIDTH) / 2;
    int windowPosY = (screenHeight - GAME_WINDOW_HEIGHT) / 2;

    glutInitWindowPosition(windowPosX, windowPosY);
    glutCreateWindow("Slot Machine");
    glutMouseFunc(onMouse);
    shuffleSlotItems();

    glutDisplayFunc(drawSlotMachineContainer);
    glutReshapeFunc(reshape); 
    glutMainLoop();
    return 0;
}
