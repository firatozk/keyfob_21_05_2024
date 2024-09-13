#include <stdio.h>
#include <math.h>

// Function to calculate distance between two points in 3D space
double calculateDistance(double x1, double y1, double z1, double x2, double y2, double z2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2));
}

int main() {
    // Coordinates of the three antennas
    double x1 = 57.0, y1 = 120.0, z1 = 36.0;  // Antenna 1
    double x2 = 56.0, y2 = 128.0, z2 = 212.0;  // Antenna 2
    double x3 = 205.0, y3 = 118.0, z3 = 61.0;  // Antenna 3

    // Coordinates of the point
    double xp = 200, yp = 150 , zp = 50;

    // Calculate distances
    double d1 = calculateDistance(x1, y1, z1, xp, yp, zp);
    double d2 = calculateDistance(x2, y2, z2, xp, yp, zp);
    double d3 = calculateDistance(x3, y3, z3, xp, yp, zp);

    // Output the distances
   /* printf("Distance to Antenna 1: %.3lf\n", d1);
    printf("Distance to Antenna 2: %.3lf\n", d2);
    printf("Distance to Antenna 3: %.3lf\n", d3);*/

    printf("r1 = %.4lf, r2 = %.4lf, r3 = %.4lf", d1, d2, d3);
    return 0;
}
