#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "raymath.h"

Vector2 pointToLineSegment(Vector2 p, Vector2 v1, Vector2 v2) {
	Vector2 v12 = Vector2Subtract(v2, v1);
	Vector2 v1p = Vector2Subtract(p, v1);
	float dot = Vector2DotProduct(v1p, v12);
	//printf("Dots: %f\n", dot);
	if ((dot / Vector2Length(v1p)) <= 0) return v1;
	float lenSqr12 = Vector2LengthSqr(v12);
	if (dot >= lenSqr12) return v2;
	return Vector2Add(v1, Vector2Scale(v12, dot / lenSqr12));
}

Vector2 findTarget(Vector2 p, Vector2 A, Vector2 B, Vector2 C) {
	Vector2 r = pointToLineSegment(p, A, B);
	Vector2 r2 = pointToLineSegment(p, A, C);
	if (Vector2Distance(r, p) > Vector2Distance(r2, p)) r = r2;
	Vector2 r3 = pointToLineSegment(p, B, C);
	if (Vector2Distance(r, p) > Vector2Distance(r3, p)) r = r3;
	return r;
}

Vector2 find90Vector(Vector2 v) {
	Vector2 r = {-v.y, v.x};
	return r;
}

int main() {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 700, "Distance to triangle");
	SetTargetFPS(24);

	Vector2 A = {300.0f, 100.f};
	Vector2 B = {220.0f, 390.f};
	Vector2 C = {500.0f, 400.f};

	bool displayRegion = false;
	bool displayAllPoints = false;

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(GetColor(0x1e1e1eff));
		if (IsKeyPressed(KEY_D)) {
			displayRegion = !displayRegion;
		}
		if (IsKeyPressed(KEY_T)) {
			displayAllPoints = !displayAllPoints;
		}
		DrawTriangle(A, B, C, GRAY);

		Vector2 start = GetMousePosition();
		DrawText("A", A.x, A.y, 1, WHITE);
		DrawText("B", B.x, B.y, 1, WHITE);
		DrawText("C", C.x, C.y, 1, WHITE);

		if (displayRegion) {
			DrawLineV(A, Vector2Add(A, find90Vector(Vector2Subtract(B, A))), ORANGE);
			DrawLineV(A, Vector2Add(A, find90Vector(Vector2Subtract(A, C))), ORANGE);

			DrawLineV(B, Vector2Add(B, find90Vector(Vector2Subtract(C, B))), ORANGE);
			DrawLineV(B, Vector2Add(B, find90Vector(Vector2Subtract(B, A))), ORANGE);

			DrawLineV(C, Vector2Add(C, find90Vector(Vector2Subtract(A, C))), ORANGE);
			DrawLineV(C, Vector2Add(C, find90Vector(Vector2Subtract(C, B))), ORANGE);
		}
		if (displayAllPoints) {
			DrawLineV(start, pointToLineSegment(start, A, B), RED);
			DrawLineV(start, pointToLineSegment(start, B, C), GREEN);
			DrawLineV(start, pointToLineSegment(start, A, C), BLUE);
		}

		Vector2 H = findTarget(start, A, B, C);

		char s[100];
		sprintf(s, "Distance: %f", Vector2Distance(start, H));
		DrawText(s, 0, 0, 20, WHITE);
		sprintf(s, "A(%f,%f)", A.x, A.y);
		DrawText(s, 0, 20, 20, WHITE);
		sprintf(s, "B(%f,%f)", B.x, B.y);
		DrawText(s, 0, 40, 20, WHITE);
		sprintf(s, "C(%f,%f)", C.x, C.y);
		DrawText(s, 0, 60, 20, WHITE);
		sprintf(s, "H(%f,%f)", H.x, H.y);
		DrawText(s, 0, 80, 20, WHITE);

		DrawCircleV(H, 3, RED);
		DrawLineV(start, H, WHITE);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
