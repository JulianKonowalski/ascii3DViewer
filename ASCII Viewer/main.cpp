#include <cmath>
#include <iostream>
#include <thread>

const int screenWidth = 70;
const int screenHeight = 50;
const int screenSize = screenWidth * screenHeight;

const float thetaSpacing = 0.07f;
const float phiSpacing = 0.02f;

const float R1 = 0.5f;
const float R2 = 1.0f;
const float K2 = 60.0f;
const float K1 = screenWidth * K2 * 3.0 / (8.0 * (R1 + R2));

int main(void) {
	float A = 0;
	float B = 0;

	char frameBuffer[screenSize];
	char zBuffer[screenSize];


	while (true) {
		
		//clear buffers
		memset(frameBuffer, ' ', screenSize);
		memset(zBuffer, 0, screenSize);

		float sinA = sin(A), cosA = cos(A);
		float sinB = sin(B), cosB = cos(B);

		for (float theta = 0; theta < 2 * 3.1415; theta += thetaSpacing) {
			float cosTheta = cos(theta), sinTheta = sin(theta);

			for (float phi = 0; phi < 2 * 3.1415; phi += phiSpacing) {
				float cosPhi = cos(phi), sinPhi = sin(phi);

				//construct the torus
				float circleX = R2 + R1 * cosTheta;
				float circleY = R1 * sinTheta;

				//get world space 3D coordinates
				float x = circleX * (cosB * cosPhi + sinA * sinB * sinPhi) - circleY * cosA * sinB;
				float y = circleX * (sinB * cosPhi - sinA * cosB * sinPhi) + circleY * cosA * sinB;
				float z = K2 + cosA * circleX * sinPhi + circleY * sinA;
				float ooz = 1 / z;

				//projection to screen space 2D coordinates
				int xProjected = (int)(screenWidth / 2 + K1 * ooz * x);
				int yProjected = (int)(screenHeight / 2 - K1 * ooz * y);

				//ligthing
				float luminance = cosPhi * cosTheta * sinB - cosA * cosTheta * sinPhi - sinA * sinTheta + cosB * (cosA * sinTheta - cosTheta * sinA * sinPhi);
				int index = xProjected + yProjected * screenWidth;
				if (luminance > 0 && ooz > zBuffer[index]) {
					zBuffer[index] = ooz;
					int luminanceIndex = luminance * 8;
					frameBuffer[index] = ".,-~:;=!*#$@"[luminanceIndex];
				}
			}
		}

		for (int i = 0; i < screenSize; ++i) { //drawing
			putchar(i % screenWidth ? frameBuffer[i] : '\n');
		}

		A += 0.04;
		B += 0.02;

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
		system("cls");
	}

}