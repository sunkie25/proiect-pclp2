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

#include <GL/glut.h>
#include <GL/freeglut.h>

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

#define DEFAULT_SELECTED_BET_INDEX      (3)

#define BET_BUTTON_COUNT                (4)
#define SLOT_GRID_ROWS                  (3)
#define SLOT_GRID_COLUMNS               (5)
#define SLOT_ITEM_COUNT                 (15)
#define SLOT_LINE_COUNT                 (5)
#define ITEM_TYPE_COUNT                 (5)

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

typedef struct SlotItemDesign {
    float startX;
    float startY;
    float endX;
    float endY;
} SlotItemDesign;

typedef struct ButtonBounds {
    int x1;
    int y1;
    int x2;
    int y2;
} ButtonBounds;

int cashBalance = DEFAULT_CASH_BALANCE;

const int availableBets[BET_BUTTON_COUNT] = {5, 10, 50, 100};
int selectedBet = DEFAULT_SELECTED_BET_INDEX;

int notificationTimerId = 0;
int notificationType = NOTIFICATION_TYPE_ERROR;

int slotItemWon[SLOT_ITEM_COUNT], displayedItems[SLOT_ITEM_COUNT], finalItems[SLOT_ITEM_COUNT];
int spinStep = 0;
int slotWinAmount = 0, lastBetAmount = 0;
int themePicked = 1;

char notificationMessage[64] = "";

void appendLogMessage(char *message)
{
    FILE *logFile;

    logFile = fopen(LOGS_FILE, "a");
    if(logFile == NULL) return;

    fprintf(logFile, "%s\n", message);
    fclose(logFile);
}

void loadSavedTheme()
{
    FILE *themeFile;

    themeFile = fopen(THEME_FILE, "r");
    if(themeFile == NULL)
    {
        printf("No theme file found!\n");
        return;
    }

    fscanf(themeFile, "%d", &themePicked);
    fclose(themeFile);
}

void saveSelectedTheme()
{
    FILE *themeFile;

    themeFile = fopen(THEME_FILE, "w");
    if(themeFile == NULL)
    {
        printf("Error opening theme file for writing!\n");
        return;
    }

    fprintf(themeFile, "%d", themePicked);
    fclose(themeFile);
}

void drawText(float x, float y, void *font, char *text)
{
    int i;
    int textLength;

    textLength = (int)strlen(text);
    glRasterPos2f(x, y);

    for(i = 0; i < textLength; i++)
    {
        glutBitmapCharacter(font, text[i]);
    }
}

void drawFilledRectangle(float left, float bottom, float right, float top)
{
    glBegin(GL_QUADS);
    glVertex2f(left, bottom);
    glVertex2f(right, bottom);
    glVertex2f(right, top);
    glVertex2f(left, top);
    glEnd();
}

void setNotificationColor()
{
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
}

void setBalanceTextColor()
{
    if(themePicked == 0) glColor3f(0.0f, 0.0f, 0.0f);
    else if(themePicked == 1) glColor3f(0.0f, 0.0f, 0.0f);
    else if(themePicked == 2) glColor3f(1.0f, 0.67f, 0.7f);
}

void setBetButtonFillColor(int isSelected)
{
    if(isSelected)
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
}

void setBetButtonTextColor()
{
    if(themePicked == 0) glColor3f(0.0f, 0.0f, 0.0f);
    else if(themePicked == 1) glColor3f(1.0f, 1.0f, 1.0f);
    else if(themePicked == 2) glColor3f(0.0f, 0.0f, 0.0f);
}

void setSlotCellBackgroundColor(int isWinningCell)
{
    if(isWinningCell)
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
}

void setMachineFrameColor()
{
    if(themePicked == 0) glColor3f(0.0f, 0.0f, 0.0f);
    else if(themePicked == 1) glColor3f(0.1f, 0.05f, 0.05f);
    else if(themePicked == 2) glColor3f(0.53f, 0.12f, 0.6f);
}

void setGridLineColor()
{
    if(themePicked == 0) glColor3f(0.0f, 0.0f, 0.0f);
    else if(themePicked == 1) glColor3f(0.0f, 0.0f, 0.0f);
    else if(themePicked == 2) glColor3f(0.53f, 0.12f, 0.6f);
}

void setSpinButtonFillColor()
{
    if(themePicked == 0) glColor3f(0.05f, 0.92f, 0.0f);
    else if(themePicked == 1) glColor3f(0.015f, 0.45f, 0.32f);
    else if(themePicked == 2) glColor3f(1.0f, 0.45f, 0.75f);
}

void setThemeButtonFillColor()
{
    if(themePicked == 0) glColor3f(0.05f, 0.92f, 0.0f);
    else if(themePicked == 1) glColor3f(0.015f, 0.45f, 0.32f);
    else if(themePicked == 2) glColor3f(0.05f, 0.92f, 0.0f);
}

void setActionButtonTextColor()
{
    if(themePicked == 0) glColor3f(0.0f, 0.0f, 0.0f);
    else if(themePicked == 1) glColor3f(1.0f, 1.0f, 1.0f);
    else if(themePicked == 2) glColor3f(0.0f, 0.0f, 0.0f);
}

void setSlotSymbolColor(int type)
{
    switch(type)
    {
        case ITEM_TYPE_TEST:
        {
            if(themePicked == 0) glColor3f(1.0f, 0.0f, 0.0f);
            else if(themePicked == 1) glColor3f(0.09f, 0.36f, 0.095f);
            else if(themePicked == 2) glColor3f(1.0f, 0.0f, 0.0f);
            break;
        }

        case ITEM_TYPE_CASH:
        {
            if(themePicked == 0) glColor3f(0.0f, 0.8f, 0.1f);
            else if(themePicked == 1) glColor3f(0.05f, 0.07f, 0.55f);
            else if(themePicked == 2) glColor3f(0.0f, 0.8f, 0.1f);
            break;
        }

        case ITEM_TYPE_DIAMOND:
        {
            if(themePicked == 0) glColor3f(0.2f, 0.6f, 0.9f);
            else if(themePicked == 1) glColor3f(0.7f, 0.12f, 0.27f);
            else if(themePicked == 2) glColor3f(0.2f, 0.6f, 0.9f);
            break;
        }

        case ITEM_TYPE_ROMB:
        {
            if(themePicked == 0) glColor3f(0.9f, 0.1f, 0.9f);
            else if(themePicked == 1) glColor3f(0.97f, 0.6f, 0.03f);
            else if(themePicked == 2) glColor3f(0.97f, 0.1f, 0.51f);
            break;
        }

        case ITEM_TYPE_BAR:
        {
            if(themePicked == 0) glColor3f(1.0f, 0.8f, 0.0f);
            else if(themePicked == 1) glColor3f(0.7f, 0.8f, 0.0f);
            else if(themePicked == 2) glColor3f(1.0f, 0.8f, 0.0f);
            break;
        }
    }
}

void setBarTextColor()
{
    if(themePicked == 0) glColor3f(0.0f, 0.0f, 0.0f);
    else if(themePicked == 1) glColor3f(0.0f, 0.0f, 0.0f);
    else if(themePicked == 2) glColor3f(0.0f, 0.0f, 0.0f);
}

void drawNotificationMessage()
{
    if(strlen(notificationMessage) == 0) return;

    setNotificationColor();
    drawText(-0.95f, -0.95f, GLUT_BITMAP_HELVETICA_18, notificationMessage);
}

void clearNotificationMessage()
{
    notificationMessage[0] = '\0';
    glutPostRedisplay();
}

void clearNotificationAfterDelay(int value)
{
    if(value == notificationTimerId)
    {
        clearNotificationMessage();
    }
}

void showTimedNotification(int type, char *content)
{
    snprintf(notificationMessage, sizeof(notificationMessage), "%s", content);
    notificationType = type;
    notificationTimerId++;
    glutTimerFunc(5000, clearNotificationAfterDelay, notificationTimerId);
    glutPostRedisplay();
}

void drawCashBalance()
{
    char balanceText[32];

    sprintf(balanceText, "BALANCE:");
    setBalanceTextColor();
    drawText(-0.95f, -0.65f, GLUT_BITMAP_HELVETICA_18, balanceText);

    sprintf(balanceText, "%d COINS", cashBalance);
    setBalanceTextColor();
    drawText(-0.95f, -0.75f, GLUT_BITMAP_HELVETICA_18, balanceText);
}

void drawBetButton(int betIndex)
{
    char betText[16];
    float left;
    float right;
    int isSelected;

    left = -0.65f + betIndex * 0.3f + 0.1f;
    right = -0.35f + betIndex * 0.3f;
    isSelected = (selectedBet == betIndex);

    setBetButtonFillColor(isSelected);
    drawFilledRectangle(left, -0.80f, right, -0.60f);

    sprintf(betText, "BET %d", availableBets[betIndex]);
    setBetButtonTextColor();
    drawText(-0.5f + betIndex * 0.3f, -0.72f, GLUT_BITMAP_HELVETICA_18, betText);
}

void drawBetButtons()
{
    int i;

    for(i = 0; i < BET_BUTTON_COUNT; i++)
    {
        drawBetButton(i);
    }
}

void copyFinalItemsAndClearWins(int copiedItems[])
{
    int i;

    for(i = 0; i < SLOT_ITEM_COUNT; i++)
    {
        slotItemWon[i] = 0;
        copiedItems[i] = finalItems[i];
    }
}

int getLineComboLength(int slotItems[], int linePositions[])
{
    if(slotItems[linePositions[0]] != slotItems[linePositions[1]]) return 0;
    if(slotItems[linePositions[1]] != slotItems[linePositions[2]]) return 0;
    if(slotItems[linePositions[2]] != slotItems[linePositions[3]]) return 3;
    if(slotItems[linePositions[3]] != slotItems[linePositions[4]]) return 4;
    return 5;
}

void markWinningLine(int linePositions[], int comboLength)
{
    int i;

    for(i = 0; i < comboLength; i++)
    {
        slotItemWon[linePositions[i]] = 1;
    }
}

int getLineWinAmount(int itemType, int comboLength, int betAmount)
{
    int slotItemWinMultipliers[ITEM_TYPE_COUNT] = {10, 2, 5, 7, 4};
    int lineWinMultiplier[3] = {1, 5, 10};

    return betAmount * slotItemWinMultipliers[itemType] * lineWinMultiplier[comboLength - 3];
}

void updateWinningItems()
{
    int copiedItems[SLOT_ITEM_COUNT];
    int lineCombos[SLOT_LINE_COUNT] = {0, 0, 0, 0, 0};
    int lineItems[SLOT_LINE_COUNT];
    int linePositions[SLOT_LINE_COUNT][SLOT_GRID_COLUMNS] = {
        {0, 3, 6, 9, 12},
        {1, 4, 7, 10, 13},
        {2, 5, 8, 11, 14},
        {0, 4, 8, 10, 12},
        {2, 4, 6, 10, 14}
    };
    int i;

    copyFinalItemsAndClearWins(copiedItems);
    slotWinAmount = 0;

    lineItems[0] = copiedItems[0];
    lineItems[1] = copiedItems[1];
    lineItems[2] = copiedItems[2];
    lineItems[3] = copiedItems[0];
    lineItems[4] = copiedItems[2];

    for(i = 0; i < SLOT_LINE_COUNT; i++)
    {
        lineCombos[i] = getLineComboLength(copiedItems, linePositions[i]);

        if(lineCombos[i] >= 3)
        {
            markWinningLine(linePositions[i], lineCombos[i]);
            slotWinAmount += getLineWinAmount(lineItems[i], lineCombos[i], lastBetAmount);
        }
    }
}

void shuffleVisibleSymbols()
{
    int i;

    for(i = 0; i < SLOT_ITEM_COUNT; i++)
    {
        slotItemWon[i] = 0;
        displayedItems[i] = rand() % ITEM_TYPE_COUNT;
    }
}

void generateSpinResult()
{
    int i;

    for(i = 0; i < SLOT_ITEM_COUNT; i++)
    {
        slotItemWon[i] = 0;
        finalItems[i] = rand() % ITEM_TYPE_COUNT;

        while(displayedItems[i] == finalItems[i])
        {
            displayedItems[i] = rand() % ITEM_TYPE_COUNT;
        }
    }
}

void drawTestSymbol(float x, float y)
{
    setSlotSymbolColor(ITEM_TYPE_TEST);
    drawFilledRectangle(x + 0.1f, y - 0.12f, x + 0.255f, y - 0.08f);

    setSlotSymbolColor(ITEM_TYPE_TEST);
    glBegin(GL_QUADS);
    glVertex2f(x + 0.255f, y - 0.11f);
    glVertex2f(x + 0.145f, y - 0.36f);
    glVertex2f(x + 0.115f, y - 0.36f);
    glVertex2f(x + 0.225f, y - 0.11f);
    glEnd();
}

void drawCashSymbol(float x, float y)
{
    int i;

    setSlotSymbolColor(ITEM_TYPE_CASH);

    for(i = 0; i < 3; i++)
    {
        drawFilledRectangle(x + 0.1f, y - 0.12f - i * 0.11f, x + 0.255f, y - 0.08f - i * 0.11f);
    }

    drawFilledRectangle(x + 0.1f, y - 0.22f, x + 0.125f, y - 0.08f);
    drawFilledRectangle(x + 0.23f, y - 0.34f, x + 0.255f, y - 0.20f);
    drawFilledRectangle(x + 0.17f, y - 0.37f, x + 0.195f, y - 0.05f);
}

void drawDiamondSymbol(float x, float y)
{
    setSlotSymbolColor(ITEM_TYPE_DIAMOND);

    glBegin(GL_TRIANGLES);
    glVertex2f(x + 0.175f, y - 0.33f);
    glVertex2f(x + 0.1f, y - 0.13f);
    glVertex2f(x + 0.25f, y - 0.13f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x + 0.1f, y - 0.13f);
    glVertex2f(x + 0.125f, y - 0.08f);
    glVertex2f(x + 0.225f, y - 0.08f);
    glVertex2f(x + 0.25f, y - 0.13f);
    glEnd();
}

void drawRombSymbol(float x, float y)
{
    setSlotSymbolColor(ITEM_TYPE_ROMB);

    glBegin(GL_POLYGON);
    glVertex2f(x + 0.175f, y - 0.10f);
    glVertex2f(x + 0.125f, y - 0.22f);
    glVertex2f(x + 0.175f, y - 0.34f);
    glVertex2f(x + 0.225f, y - 0.22f);
    glEnd();
}

void drawBarSymbol(float x, float y)
{
    setSlotSymbolColor(ITEM_TYPE_BAR);
    drawFilledRectangle(x + 0.10f, y - 0.3f, x + 0.255f, y - 0.1f);

    setBarTextColor();
    drawText(x + 0.145f, y - 0.22f, GLUT_BITMAP_HELVETICA_18, "BAR");
}

void drawSlotSymbol(int type, float x, float y)
{
    switch(type)
    {
        case ITEM_TYPE_TEST:
        {
            drawTestSymbol(x, y);
            break;
        }

        case ITEM_TYPE_CASH:
        { 
            drawCashSymbol(x, y);
            break;
        }

        case ITEM_TYPE_DIAMOND:
        {
            drawDiamondSymbol(x, y);
            break;
        }

        case ITEM_TYPE_ROMB:
        {
            drawRombSymbol(x, y);
            break;
        }

        case ITEM_TYPE_BAR:
        { 
            drawBarSymbol(x, y);
            break;
        }
    }
}

int loadSlotItemDesigns(SlotItemDesign slotItemDesigns[])
{
    int i;
    FILE *offsetsFile;

    i = 0;
    offsetsFile = fopen(OFFSETS_FILE, "r");
    if(offsetsFile == NULL)
    {
        printf("Error opening offsets file!\n");
        return 0;
    }

    while(i < SLOT_ITEM_COUNT && fscanf(offsetsFile, "%f %f %f %f", &slotItemDesigns[i].startX, &slotItemDesigns[i].startY, &slotItemDesigns[i].endX, &slotItemDesigns[i].endY) == 4)
    {
        i++;
    }

    fclose(offsetsFile);
    return i == SLOT_ITEM_COUNT;
}

void drawSlotCellBackground(SlotItemDesign *slotItemDesign, int isWinningCell)
{
    setSlotCellBackgroundColor(isWinningCell);
    drawFilledRectangle(slotItemDesign->startX, slotItemDesign->endY, slotItemDesign->endX, slotItemDesign->startY);
}

void drawAllSlotCells()
{
    SlotItemDesign slotItemDesigns[SLOT_ITEM_COUNT];
    int row;
    int column;
    int itemIndex;

    if(!loadSlotItemDesigns(slotItemDesigns)) return;

    for(row = 0; row < SLOT_GRID_ROWS; row++)
    {
        for(column = 0; column < SLOT_GRID_COLUMNS; column++)
        {
            itemIndex = row * SLOT_GRID_COLUMNS + column;
            drawSlotCellBackground(&slotItemDesigns[itemIndex], slotItemWon[itemIndex]);
            drawSlotSymbol(displayedItems[itemIndex], slotItemDesigns[itemIndex].startX, slotItemDesigns[itemIndex].startY);
        }
    }
}

void finishSpin()
{
    char message[64];

    spinStep = 0;

    #ifdef TEST_MODE
        printf("\nSPIN STOPPED!!!!");
    #endif

    updateWinningItems();
    glutPostRedisplay();

    if(slotWinAmount > 0)
    {
        sprintf(message, "(+) You won %d coins!", slotWinAmount);
        showTimedNotification(NOTIFICATION_TYPE_SUCCESS, message);
        cashBalance += slotWinAmount;
        drawCashBalance();

        sprintf(message, "[WIN] +%d coins (bet: %d, new balance: %d)", slotWinAmount, availableBets[selectedBet], cashBalance);
        appendLogMessage(message);
    }
    else
    {
        sprintf(message, "(-) You lost %d coins!", lastBetAmount);
        showTimedNotification(NOTIFICATION_TYPE_ERROR, message);

        sprintf(message, "[LOSE] -%d coins (bet: %d, new balance: %d)", lastBetAmount, availableBets[selectedBet], cashBalance);
        appendLogMessage(message);
    }
}

void revealNextSpinSymbol(int value)
{
    (void)value;

    if(spinStep >= SLOT_ITEM_COUNT)
    {
        finishSpin();
        return;
    }

    displayedItems[spinStep] = finalItems[spinStep];
    spinStep++;

    glutPostRedisplay();

    if(spinStep < SLOT_ITEM_COUNT)
    {
        glutTimerFunc(SPIN_TIMER_DELAY, revealNextSpinSymbol, 0);
    }
    else
    {
        finishSpin();
    }
}

void startSpinAnimation()
{
    if(spinStep > 0) return;

    glutTimerFunc(SPIN_TIMER_DELAY, revealNextSpinSymbol, 0);
}

void drawMachineFrame()
{
    setMachineFrameColor();
    drawFilledRectangle(-0.95f, -0.5f, 0.95f, 0.9f);
}

void drawGridLines()
{
    int i;

    setGridLineColor();
    drawFilledRectangle(-0.95f, 0.435f, 0.95f, 0.45f);

    setGridLineColor();
    drawFilledRectangle(-0.95f, -0.035f, 0.95f, -0.02f);

    for(i = 0; i < 4; i++)
    {
        setGridLineColor();
        drawFilledRectangle(-0.575f + i * 0.38f, -0.5f, -0.575f + i * 0.38f + 0.01f, 0.9f);
    }
}

void drawSpinButton()
{
    setSpinButtonFillColor();
    drawFilledRectangle(0.65f, -0.80f, 0.9f, -0.60f);

    setActionButtonTextColor();
    drawText(0.735f, -0.72f, GLUT_BITMAP_HELVETICA_18, "SPIN");
}

void drawThemeButton()
{
    setThemeButtonFillColor();
    drawFilledRectangle(-0.94f, -0.87f, -0.82f, -0.80f);

    setActionButtonTextColor();
    drawText(-0.92f, -0.85f, GLUT_BITMAP_HELVETICA_12, "THEME");
}

void drawGameScreen()
{
    if(themePicked == 0) glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    else if(themePicked == 1) glClearColor(0.66f, 0.89f, 0.898f, 1.0f);
    else if(themePicked == 2) glClearColor(0.32f, 0.18f, 0.5f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);

    drawMachineFrame();
    drawAllSlotCells();
    drawGridLines();
    drawSpinButton();
    drawCashBalance();
    drawBetButtons();
    drawThemeButton();
    drawNotificationMessage();

    glFlush();
}

void handleSpinButtonClick()
{
    #ifdef TEST_MODE
        printf("\nSPIN BUTTON CLICKED!\n");
    #endif

    if(spinStep > 0) return;

    if(selectedBet < 0)
    {
        showTimedNotification(NOTIFICATION_TYPE_WARNING, "Please select a bet before spinning!");
        return;
    }

    if(cashBalance < availableBets[selectedBet])
    {
        showTimedNotification(NOTIFICATION_TYPE_ERROR, "You don't have enough coins to select this bet!");
        selectedBet = -1;
        drawBetButtons();
        return;
    }

    cashBalance -= availableBets[selectedBet];
    lastBetAmount = availableBets[selectedBet];
    drawCashBalance();

    showTimedNotification(NOTIFICATION_TYPE_SUCCESS, "Spinning the reels... Good luck!");

    shuffleVisibleSymbols();
    generateSpinResult();
    startSpinAnimation();

    glutPostRedisplay();
}

void handleThemeButtonClick()
{
    if(themePicked < MAX_THEMES - 1) themePicked++;
    else themePicked = 0;

    saveSelectedTheme();
    glutPostRedisplay();
}

void normalizeButtonBounds(ButtonBounds *buttonBounds)
{
    int temp;

    temp = 0;

    if(buttonBounds->x1 > buttonBounds->x2)
    {
        temp = buttonBounds->x1;
        buttonBounds->x1 = buttonBounds->x2;
        buttonBounds->x2 = temp;
    }

    if(buttonBounds->y1 > buttonBounds->y2)
    {
        temp = buttonBounds->y1;
        buttonBounds->y1 = buttonBounds->y2;
        buttonBounds->y2 = temp;
    }
}

int isPointInsideButton(int x, int y, ButtonBounds buttonBounds)
{
    normalizeButtonBounds(&buttonBounds);
    return x >= buttonBounds.x1 && x <= buttonBounds.x2 && y >= buttonBounds.y1 && y <= buttonBounds.y2;
}

int getClickedBetButton(int x, int y)
{
    ButtonBounds betButtonBounds[BET_BUTTON_COUNT] = {
        {261, 500, 371, 585},
        {433, 500, 543, 585},
        {606, 500, 725, 585},
        {780, 500, 890, 585}
    };
    int i;

    for(i = 0; i < BET_BUTTON_COUNT; i++)
    {
        if(isPointInsideButton(x, y, betButtonBounds[i]))
        {
            return i;
        }
    }

    return -1;
}

void handleBetButtonClick(int betIndex)
{
    if(spinStep > 0)
    {
        showTimedNotification(NOTIFICATION_TYPE_WARNING, "You can't change your bet while the reels are spinning!");
        return;
    }

    if(cashBalance < availableBets[betIndex])
    {
        showTimedNotification(NOTIFICATION_TYPE_ERROR, "You don't have enough coins to select this bet!");
        selectedBet = -1;

        #ifdef TEST_MODE
            printf("n-ai bani boss\n");
        #endif
    }
    else
    {
        selectedBet = betIndex;

        #ifdef TEST_MODE
            printf("selected bet: %d\n", availableBets[selectedBet]);
        #endif
    }

    drawBetButtons();
    glutPostRedisplay();
}

void handleActionButtonClick(int x, int y)
{
    ButtonBounds spinButtonBounds;
    ButtonBounds themeButtonBounds;

    spinButtonBounds.x1 = 950;
    spinButtonBounds.x2 = 1093;
    spinButtonBounds.y1 = 510;
    spinButtonBounds.y2 = 580;

    themeButtonBounds.x1 = 35;
    themeButtonBounds.x2 = 102;
    themeButtonBounds.y1 = 576;
    themeButtonBounds.y2 = 697;

    if(isPointInsideButton(x, y, spinButtonBounds))
    {
        handleSpinButtonClick();
        return;
    }

    if(isPointInsideButton(x, y, themeButtonBounds))
    {
        handleThemeButtonClick();
    }
}

void mouseCallback(int button, int state, int x, int y)
{
    int clickedBetButton;

    if(state != GLUT_DOWN || button != GLUT_LEFT_BUTTON) return;

    clickedBetButton = getClickedBetButton(x, y);

    if(clickedBetButton >= 0)
    {
        #ifdef TEST_MODE
            printf("Clicked BET button %d\n", clickedBetButton);
        #endif

        handleBetButtonClick(clickedBetButton);
        return;
    }

    #ifdef TEST_MODE
        printf("\nclick la %d %d\n", x, y);
    #endif

    handleActionButtonClick(x, y);
}

void keepWindowFixedSize(int width, int height)
{
    if(width != GAME_WINDOW_WIDTH || height != GAME_WINDOW_HEIGHT)
    {
        glutReshapeWindow(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);
    }
}

int main(int argc, char** argv)
{
    int screenWidth;
    int screenHeight;
    int windowPosX;
    int windowPosY;

    loadSavedTheme();
    appendLogMessage("New slots session!!!!");

    glutInit(&argc, argv);
    glutInitWindowSize(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);

    screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    screenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    windowPosX = (screenWidth - GAME_WINDOW_WIDTH) / 2;
    windowPosY = (screenHeight - GAME_WINDOW_HEIGHT) / 2;

    glutInitWindowPosition(windowPosX, windowPosY);
    glutCreateWindow("Slot Machine");
    glutMouseFunc(mouseCallback);
    shuffleVisibleSymbols();

    glutDisplayFunc(drawGameScreen);
    glutReshapeFunc(keepWindowFixedSize);
    glutMainLoop();

    return 0;
}
