#include <stdio.h>
#include <math.h>

#define TRUCK_LENGTH 235
#define TRUCK_WIDTH 212
#define TRUCK_HEIGHT 205

typedef struct {
    double x, y, z;
} Point;

// Function to calculate the norm of a point
double norm(Point p) {
    return sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
}

// Function to calculate the dot product of two points
double dot(Point p1, Point p2) {
    return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}

// Function to calculate distance between two points in 3D space
double calculateDistance(double x1, double y1, double z1, double x2, double y2, double z2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2));
}

// Function to calculate the cross product of two points
Point cross(Point p1, Point p2) {
    Point result;
    result.x = p1.y * p2.z - p1.z * p2.y;
    result.y = p1.z * p2.x - p1.x * p2.z;
    result.z = p1.x * p2.y - p1.y * p2.x;
    return result;
}

// Function prototype for trilaterate
void trilaterate(Point P1, Point P2, Point P3, double r1, double r2, double r3, Point *p_12_a, Point *p_12_b);

// Function to calculate the total distance error between a point and the sphere surfaces
double calculateError(Point p, Point P1, Point P2, Point P3, double r1, double r2, double r3) {
    double error = fabs(sqrt(pow(p.x - P1.x, 2) + pow(p.y - P1.y, 2) + pow(p.z - P1.z, 2)) - r1)
                 + fabs(sqrt(pow(p.x - P2.x, 2) + pow(p.y - P2.y, 2) + pow(p.z - P2.z, 2)) - r2)
                 + fabs(sqrt(pow(p.x - P3.x, 2) + pow(p.y - P3.y, 2) + pow(p.z - P3.z, 2)) - r3);
    return error;
}

// Function to find intersection points of three spheres and print them
void findIntersectionAndPrint(Point P1, Point P2, Point P3, double r1, double r2, double r3) {
    Point p_12_a, p_12_b;
    trilaterate(P1, P2, P3, r1, r2, r3, &p_12_a, &p_12_b);

    // Calculate the errors for each of the points
    double error_a = calculateError(p_12_a, P1, P2, P3, r1, r2, r3);
    double error_b = calculateError(p_12_b, P1, P2, P3, r1, r2, r3);

    //printf("error a is:%f\n",error_a);
    //printf("error b is:%f\n",error_b);
    
    // Decide which point is more likely to be correct based on the error
    //if (error_a < error_b) {
       // printf("p_12_a is more likely the correct solution with less error.\n");
        printf("p_12_a: (%f, %f, %f)\n", p_12_a.x, p_12_a.y, p_12_a.z);
    //} else {
       // printf("p_12_b is more likely the correct solution with less error.\n");
        printf("p_12_b: (%f, %f, %f)\n", p_12_b.x, p_12_b.y, p_12_b.z);
    //}

    // Check if the best point is inside the truck
    Point best_point = (error_a < error_b) ? p_12_a : p_12_b;
    if ((0 <= best_point.x && best_point.x <= TRUCK_WIDTH) && 
        (0 <= best_point.y && best_point.y <= TRUCK_LENGTH) && 
        (0 <= best_point.z && best_point.z <= TRUCK_HEIGHT)) {
        printf("The best point is inside the truck.\n");
    } else {
        printf("The best point is outside the truck.\n");
    }
}

// Function to find the intersection of three spheres
void trilaterate(Point P1, Point P2, Point P3, double r1, double r2, double r3, Point *p_12_a, Point *p_12_b) {
    Point temp1, temp2, temp3, e_x, e_y, e_z;
    double d, i, j, x, y, z, temp4;

    temp1.x = P2.x - P1.x;
    temp1.y = P2.y - P1.y;
    temp1.z = P2.z - P1.z;
    d = norm(temp1);
    e_x.x = temp1.x / d;
    e_x.y = temp1.y / d;
    e_x.z = temp1.z / d;

    temp2.x = P3.x - P1.x;
    temp2.y = P3.y - P1.y;
    temp2.z = P3.z - P1.z;
    i = dot(e_x, temp2);
    temp3.x = temp2.x - i * e_x.x;
    temp3.y = temp2.y - i * e_x.y;
    temp3.z = temp2.z - i * e_x.z;
    j = norm(temp3);
    e_y.x = temp3.x / j;
    e_y.y = temp3.y / j;
    e_y.z = temp3.z / j;

    e_z = cross(e_x, e_y);

    x = (r1 * r1 - r2 * r2 + d * d) / (2 * d);
    y = (r1 * r1 - r3 * r3 - 2 * i * x + i * i + j * j) / (2 * j);

    temp4 = r1 * r1 - x * x - y * y;
    if (temp4 < 0) {
        printf("The three spheres do not intersect!\n");
        return;
    }
    z = sqrt(temp4);

    p_12_a->x = P1.x + x * e_x.x + y * e_y.x + z * e_z.x;
    p_12_a->y = P1.y + x * e_x.y + y * e_y.y + z * e_z.y;
    p_12_a->z = P1.z + x * e_x.z + y * e_y.z + z * e_z.z;

    p_12_b->x = P1.x + x * e_x.x + y * e_y.x - z * e_z.x;
    p_12_b->y = P1.y + x * e_x.y + y * e_y.y - z * e_z.y;
    p_12_b->z = P1.z + x * e_x.z + y * e_y.z - z * e_z.z;
}

int main() {
    Point P1 = {57.0, 120.0, 36.0}, P2 = {56.0, 128.0, 212.0}, P3 = {205.0, 118.0, 61.0};
       // Coordinates of the three antennas
    double Antenna_x1 = 57.0, Antenna_y1 = 120.0, Antenna_z1 = 36.0;  // Antenna 1
    double Antenna_x2 = 56.0, Antenna_y2 = 128.0, Antenna_z2 = 212.0;  // Antenna 2
    double Antenna_x3 = 205.0, Antenna_y3 = 118.0, Antenna_z3 = 61.0;  // Antenna 3

    double xp = 200, yp = 150 , zp = 50; // Coordinates of the point
    double r1,r2,r3;

    for(int i=-100;i<=100;i++)
    {
        if(i % 10 == 0)
        {
                // Calculate distances
                r1 = calculateDistance(Antenna_x1, Antenna_y1, Antenna_z1, xp+i, yp+i, zp+i);
                r2 = calculateDistance(Antenna_x2, Antenna_y1, Antenna_z2, xp+i, yp+i, zp+i);
                r3 = calculateDistance(Antenna_x3, Antenna_y3, Antenna_z3, xp+i, yp+i, zp+i);

                findIntersectionAndPrint(P1, P2, P3, r1, r2, r3);
                printf("coordinates of the point is equal to = %.4f,%.4f,%.4f\n",xp+i,yp+i,zp+i);
        }
    }    
    return 0;
}
