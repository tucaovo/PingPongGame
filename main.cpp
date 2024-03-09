#include <GLFW/glfw3.h>
#include <iostream>

const int Width = 800;
const int Height = 600;
const float PaddleWidth = 0.02f;
const float PaddleHeight = 0.15f;
const float PaddleSpeed = 0.0005f;
const float BallSize = 0.02f;

int Player1Score = 0;
int Player2Score = 0;

float BallPosX = 0.0f; 
float BallPosY = 0.0f; 
float BallDirX = 0.00025f; 
float BallDirY = 0.00025f; 

float Player1PosY = 0.0f;
float Player2PosY = 0.0f;


void DrawPaddle(float x, float y) {
    glBegin(GL_QUADS);
    glVertex2f(x - PaddleWidth, y - PaddleHeight);
    glVertex2f(x + PaddleWidth, y - PaddleHeight);
    glVertex2f(x + PaddleWidth, y + PaddleHeight);
    glVertex2f(x - PaddleWidth, y + PaddleHeight);
    glEnd();
}

void DrawBall(float x, float y) {
    glColor3f(1.0f, 0.0f, 0.0f); 
    glBegin(GL_QUADS);
    glVertex2f(x - BallSize, y - BallSize);
    glVertex2f(x + BallSize, y - BallSize);
    glVertex2f(x + BallSize, y + BallSize);
    glVertex2f(x - BallSize, y + BallSize);
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f); 
}

void UpdatePlayerPaddles(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && Player1PosY < 1.0f - PaddleHeight) {
        Player1PosY += PaddleSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && Player1PosY > -1.0f + PaddleHeight) {
        Player1PosY -= PaddleSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && Player2PosY < 1.0f - PaddleHeight) {
        Player2PosY += PaddleSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && Player2PosY > -1.0f + PaddleHeight) {
        Player2PosY -= PaddleSpeed;
    }
}

void UpdateBall(GLFWwindow* window) {
    BallPosX += BallDirX;
    BallPosY += BallDirY;

    if (BallPosY > 1.0f - BallSize || BallPosY < -1.0f + BallSize) {
        BallDirY = -BallDirY;
    }

    if ((BallPosX < -1.0f + PaddleWidth + BallSize && BallPosY < Player1PosY + PaddleHeight && BallPosY > Player1PosY - PaddleHeight) ||
        (BallPosX > 1.0f - PaddleWidth - BallSize * 2 && BallPosY < Player2PosY + PaddleHeight && BallPosY > Player2PosY - PaddleHeight)) {
        BallDirX = -BallDirX;
    }

    if (BallPosX < -1.0f) {
        Player2Score++;
    }
    else if (BallPosX > 1.0f) {
        Player1Score++;
    }

    if (Player1Score == 3 || Player2Score == 3) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (BallPosX < -1.0f || BallPosX > 1.0f) {
        BallPosX = 0.0f;
        BallPosY = 0.0f;
        BallDirX = (BallDirX > 0) ? -0.00025f : 0.00025f;
        BallDirY = (BallDirY > 0) ? -0.00025f : 0.00025f;
    }
}

int main() {
    GLFWwindow* Window;

    if (!glfwInit()) {
        return -1;
    }

    Window = glfwCreateWindow(Width, Height, "Simple Ping Pong", NULL, NULL);
    if (!Window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(Window);

    while (!glfwWindowShouldClose(Window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        UpdatePlayerPaddles(Window);
        UpdateBall(Window);

        DrawPaddle(-1.0f + PaddleWidth, Player1PosY);
        DrawPaddle(1.0f - PaddleWidth * 3, Player2PosY);
        DrawBall(BallPosX, BallPosY);

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    int Winner; 
    if (Player1Score == 3) {
        Winner = 1;
    }
    else {
        Winner = 2;
    }
    std::cout << "Player " << Winner << " wins!" << std::endl;

    glfwTerminate();
    return 0;
}