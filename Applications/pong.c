#include "../Kernel/system.h"
#include "../Kernel/Drivers/vga.h"
#include "../Kernel/Drivers/double_buffer.h"

unsigned char gameRunning = 0;

int playerX, playerY;
unsigned int cpuX, cpuY;

int ballX, ballY;

unsigned char playerSpeed = 10;
unsigned char ballSpeed = 8;
signed char trajectoryX, trajectoryY;
unsigned char count = 0;

signed char calculateSpeeds(int x, int y) {
    unsigned char MAX_ANGLE = 75;  // The max angle we can have is 75 degrees
    unsigned char MAX_SPEED = 10;

    char realativePositionY = (y + (70/2)) - ballY;  // Finding where the ball is realative to the paddle

    float normalizedRealativePositionY = realativePositionY/(70/2);  // Normalize the position
    count++;
    //return ((realativePositionY * (trajectoryY/0.1)) * -0.1);
    return (1 - 2 * (ballX - x) / 70) * MAX_SPEED;
    // char bounceAngle = normalizedRealativePositionY * MAX_ANGLE;  // Get the bounce angle by multiplying the normalized position by the Max angle
    // return bounceAngle;
}  

void logic() {

    // Detect collision for the ball
    /*
    For ball movement:
        Treat it's trajectory as a line, and move it by its slope. Here, the slope of the line is trajectoryX over trajectoryY
    */
    ballX = ballX + trajectoryX;
    ballY = ballY + trajectoryY;

    // Calculate movement of cpu
    if (cpuY+70/2 < ballY) { cpuY = cpuY + playerSpeed; }
    else if (cpuY+70/2 > ballY) { cpuY = cpuY - playerSpeed; }
    else { cpuY = cpuY; }

    // Ball collision with cpu or player
    if (ballY > cpuY && ballY < cpuY+70 && ballX >= cpuX-8) { trajectoryX = -1 * ballSpeed; trajectoryY = calculateSpeeds(cpuX, cpuY); }  // Hit cpu
    else if (ballY > playerY && ballY < playerY+70 && ballX <= playerX+4) { trajectoryX = ballSpeed; trajectoryY = calculateSpeeds(playerX, playerY); }  // Hit player

    // Ball collision with walls
    if (ballY <= 1 || ballY >= 473) { trajectoryY =  trajectoryY * -1; }

    // Win/loose conditions
    if (ballX >= 630) { ballX = 630; }
    else if (ballX <= 0) { ballX = 0; }

    if (count % 3 == 0) { ballSpeed++; count++; }

}

void draw() {
    clearBuffer();
    unsigned int c = 0xffffff;
    // plotPixle(3, 30, c);
    // plotPixle(4, 30, c);
    // plotPixle(5, 30, c);
    // plotPixle(6, 30, c);
    // plotPixle(7, 30, c);

    drawRectangleBuffer(playerX, playerY, 4, 70, c);
    drawRectangleBuffer(cpuX, cpuY, 4, 70, c);
    drawRectangleBuffer(ballX, ballY, 5, 5, c);

}

void mainLoop() {
    while (gameRunning == 1) {
        logic();
        draw();
        //it = it + 3;

        swapBuffers();
        gameRunning = 1;
    }

}

// Handle a keypress
void pongHandler(char c) {
    
    if (c == 'w') { playerY = playerY - playerSpeed; }
    else if (c == 's') { playerY = playerY + playerSpeed; }

    // Detect collision for the player
    if (playerY < 0) { playerY = 0; }
    else if (playerY > 410) { playerY = 410; }

}

void loadPong() {
    gameRunning = 1;

    playerX = 15;
    playerY = 30;

    cpuX = 630-15;
    cpuY = 30;

    ballX = 630/2;
    ballY = 410/2;

    playerSpeed = 10;

    trajectoryX = 8;
    trajectoryY = 1;


    __asm__ __volatile__ ("cli");
    __asm__ __volatile__ ("sti");

    mainLoop();
}