#include <stdio.h>
#include <math.h>
#include <gsl/gsl_multiroots.h>

// Function to calculate the distance between two points in 3D space
double calculateDistance(double x1, double y1, double z1, double x2, double y2, double z2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2));
}

// Function to check if the point is inside the cabin
int isInsideCabin(double px, double py, double pz, double cabinXMin, double cabinXMax, double cabinYMin, double cabinYMax, double cabinZMin, double cabinZMax) {
    return (px >= cabinXMin && px <= cabinXMax &&
            py >= cabinYMin && py <= cabinYMax &&
            pz >= cabinZMin && pz <= cabinZMax);
}

// Define the system of equations for trilateration
int trilateration(const gsl_vector *x, void *params, gsl_vector *f) {
    double *p = (double *)params;
    double x1 = p[0], y1 = p[1], z1 = p[2];
    double d1 = p[3];
    double x2 = p[4], y2 = p[5], z2 = p[6];
    double d2 = p[7];
    double x3 = p[8], y3 = p[9], z3 = p[10];
    double d3 = p[11];

    double xi = gsl_vector_get(x, 0);
    double yi = gsl_vector_get(x, 1);
    double zi = gsl_vector_get(x, 2);

    gsl_vector_set(f, 0, calculateDistance(xi, yi, zi, x1, y1, z1) - d1);
    gsl_vector_set(f, 1, calculateDistance(xi, yi, zi, x2, y2, z2) - d2);
    gsl_vector_set(f, 2, calculateDistance(xi, yi, zi, x3, y3, z3) - d3);

    return GSL_SUCCESS;
}

int main() {
    // Coordinates of antennas
    double x1 = 57, y1 = 120, z1 = 36;  // Antenna 1
    double x2 = 56, y2 = 128, z2 = 212;  // Antenna 2
    double x3 = 205, y3 = 118, z3 = 61;  // Antenna 3

    // Distances from the point to each antenna
    double d1, d2, d3;

    // Cabin dimensions
    double cabinXMin = 0.0, cabinXMax = 235.0;
    double cabinYMin = 0.0, cabinYMax = 211.0;
    double cabinZMin = 0.0, cabinZMax = 212.0;

    // Input the distances from the key fob to each antenna
    printf("Enter the distances from the key fob to each antenna (d1, d2, d3): ");
    scanf("%lf %lf %lf", &d1, &d2, &d3);

    // Initialize GSL vectors and parameters
    gsl_vector *x = gsl_vector_alloc(3);
    gsl_vector *f = gsl_vector_alloc(3);
    gsl_matrix *J = gsl_matrix_alloc(3, 3);
    gsl_multiroot_function fcn = {&trilateration, 3, (double[]){x1, y1, z1, d1, x2, y2, z2, d2, x3, y3, z3, d3}};
    gsl_multiroot_fsolver *solver = gsl_multiroot_fsolver_alloc(gsl_multiroot_fsolver_hybrids, 3);
    gsl_multiroot_fsolver_set(solver, &fcn, x);

    // Set initial guess
    gsl_vector_set(x, 0, (x1 + x2 + x3) / 3.0);
    gsl_vector_set(x, 1, (y1 + y2 + y3) / 3.0);
    gsl_vector_set(x, 2, (z1 + z2 + z3) / 3.0);

    // Solve the system of equations
    int status;
    do {
        status = gsl_multiroot_fsolver_iterate(solver);
        if (status) break;
        status = gsl_multiroot_test_residual(solver->f, 1e-6);
    } while (status == GSL_CONTINUE);

    // Extract the solution
    double x_fob = gsl_vector_get(solver->x, 0);
    double y_fob = gsl_vector_get(solver->x, 1);
    double z_fob = gsl_vector_get(solver->x, 2);

    printf("Estimated position of key fob: (%.2lf, %.2lf, %.2lf)\n", x_fob, y_fob, z_fob);

    // Check if the point is inside the cabin
    if (isInsideCabin(x_fob, y_fob, z_fob, cabinXMin, cabinXMax, cabinYMin, cabinYMax, cabinZMin, cabinZMax)) {
        printf("The key fob is inside the truck cabin.\n");
    } else {
        printf("The key fob is outside the truck cabin.\n");
    }

    // Free GSL memory
    gsl_vector_free(x);
    gsl_vector_free(f);
    gsl_matrix_free(J);
    gsl_multiroot_fsolver_free(solver);

    return 0;
}
