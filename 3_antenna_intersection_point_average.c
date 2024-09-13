#include <stdio.h>
#include <math.h>

#define TRUCK_LENGTH 235
#define TRUCK_WIDTH 211
#define TRUCK_HEIGHT 212
    
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

// Function to check if a point is inside the truck cabin
int is_inside_truck(Point p) {
    return (0 <= p.x && p.x <= TRUCK_WIDTH) &&
           (0 <= p.y && p.y <= TRUCK_LENGTH) &&
           (0 <= p.z && p.z <= TRUCK_HEIGHT);
}

// Function to calculate the average of two points
Point average_point(Point p1, Point p2) {
    Point avg;
    avg.x = (p1.x + p2.x) / 2.0;
    avg.y = (p1.y + p2.y) / 2.0;
    avg.z = (p1.z + p2.z) / 2.0;
    return avg;
}

// Function to find intersection points of three spheres and print them
void findIntersectionAndPrint(Point P1, Point P2, Point P3, double r1, double r2, double r3) {
    Point p_12_a, p_12_b;
    trilaterate(P1, P2, P3, r1, r2, r3, &p_12_a, &p_12_b);
    printf("Intersection points: (%f, %f, %f) and (%f, %f, %f)\n", p_12_a.x, p_12_a.y, p_12_a.z, p_12_b.x, p_12_b.y, p_12_b.z);

    int inside_a = is_inside_truck(p_12_a);
    int inside_b = is_inside_truck(p_12_b);

    if (inside_a && inside_b) {
        printf("Both intersection points are inside the truck cabin.\n");
    } else if (!inside_a && !inside_b) {
        printf("Both intersection points are outside the truck cabin.\n");
    } else if (inside_a && !inside_b) {
        printf("One intersection point is inside the truck cabin, the other is outside.\n");
        Point avg_point = average_point(p_12_a, p_12_b);
        printf("Average point: (%f, %f, %f)\n", avg_point.x, avg_point.y, avg_point.z);
        if (is_inside_truck(avg_point)) {
            printf("The average point is inside the truck cabin.\n");
        } else {
            printf("The average point is outside the truck cabin.\n");
        }
    } else {
        printf("One intersection point is outside the truck cabin, the other is inside.\n");
        Point avg_point = average_point(p_12_a, p_12_b);
        printf("Average point: (%f, %f, %f)\n", avg_point.x, avg_point.y, avg_point.z);
        if (is_inside_truck(avg_point)) {
            printf("The average point is inside the truck cabin.\n");
        } else {
            printf("The average point is outside the truck cabin.\n");
        }
    }
}

// Function to find the intersection of three spheres
// P1,P2,P3 are the centers, r1,r2,r3 are the radii
// Implementation based on Wikipedia Trilateration article.
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
    // Example positions of the antennas (center points)
    Point P1 = {57, 120, 36};
    Point P2 = {56, 128, 200};
    Point P3 = {205, 118, 61};

    // Calculate distances from key fob position to antennas
    Point key_fob_position = {140,170,190}; // Example key fob position
    double r1 = norm((Point){key_fob_position.x - P1.x, key_fob_position.y - P1.y, key_fob_position.z - P1.z});
    double r2 = norm((Point){key_fob_position.x - P2.x, key_fob_position.y - P2.y, key_fob_position.z - P2.z});
    double r3 = norm((Point){key_fob_position.x - P3.x, key_fob_position.y - P3.y, key_fob_position.z - P3.z});

    findIntersectionAndPrint(P1, P2, P3, r1, r2, r3);
    return 0;
}
