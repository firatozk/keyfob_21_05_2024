#include <stdio.h>
#include <math.h>

struct Point {
    double x;
    double y;
    double z;
};

double calculateDistance(struct Point point1, struct Point point2) {
    return sqrt(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2) + pow(point1.z - point2.z, 2));
}

int main() {
    struct Point antenna1 = {57, 120, 36};
    struct Point antenna2 = {56, 128, 212};
    struct Point antenna3 = {205, 118, 61};

    struct Point targetPoint = {200, 200, 200};

    double distance_antenna1 = calculateDistance(targetPoint, antenna1);
    double distance_antenna2 = calculateDistance(targetPoint, antenna2);
    double distance_antenna3 = calculateDistance(targetPoint, antenna3);

    printf("Distance to antenna 1: %.2f\n", distance_antenna1);
    printf("Distance to antenna 2: %.2f\n", distance_antenna2);
    printf("Distance to antenna 3: %.2f\n", distance_antenna3);

    return 0;
}
