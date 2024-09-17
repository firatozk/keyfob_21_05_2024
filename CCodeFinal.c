#include <stdio.h>
#include <math.h>

#define NUM_FEATURES 3
#define MAX_SUPPORT_VECTORS 746  // Adjust if necessary

// Function to scale input data
void scale_data(double point[], double scaling_mean[], double scaling_scale[], int num_features) {
    for (int i = 0; i < num_features; i++) {
        point[i] = (point[i] - scaling_mean[i]) / scaling_scale[i];
    }
}

// Function to compute the Euclidean distance squared
double euclidean_distance_squared(double point[], double support_vector[], int num_features) {
    double sum = 0.0;
    for (int i = 0; i < num_features; i++) {
        double diff = point[i] - support_vector[i];
        sum += diff * diff;
    }
    return sum;
}

// Function to classify a point using the loaded SVM model
int classify_point(double point[], double support_vectors[][NUM_FEATURES], double dual_coef[], double intercept, int num_support_vectors) {
    double decision_value = intercept;  // Start with the intercept term
    double gamma = 1.0;  // Gamma for RBF kernel, adjust if necessary

    // Loop over all support vectors to compute the decision function
    for (int i = 0; i < num_support_vectors; i++) {
        double dist_sq = euclidean_distance_squared(point, support_vectors[i], NUM_FEATURES);
        double kernel_value = exp(-gamma * dist_sq);  // RBF kernel
        decision_value += dual_coef[i] * kernel_value;
    }

    // If decision_value >= 0, it's classified as inside the cabin
    return decision_value >= 0 ? 1 : 0;  // 1 for inside, 0 for outside
}

int main() {
    // Example: the point to classify (100, 65, -30)
    double point[NUM_FEATURES] = {100, 65, -30};

    // Load your SVM model here, including support_vectors, dual_coef, intercept, scaling_mean, and scaling_scale
    // In practice, these should be loaded from your model file
    double support_vectors[MAX_SUPPORT_VECTORS][NUM_FEATURES] = { /* Loaded from the file */ };
    double dual_coef[MAX_SUPPORT_VECTORS] = { /* Loaded from the file */ };
    double intercept = -3.576245;  // Loaded from the file
    double scaling_mean[NUM_FEATURES] = {119.772094, 104.726506, 104.034932};
    double scaling_scale[NUM_FEATURES] = {86.566185, 78.831587, 75.253412};
    int num_support_vectors = 746;  // Loaded from the file

    // Scale the input point
    scale_data(point, scaling_mean, scaling_scale, NUM_FEATURES);

    // Classify the scaled point
    int classification = classify_point(point, support_vectors, dual_coef, intercept, num_support_vectors);

    if (classification == 1) {
        printf("Point (100, 65, -30) is classified as INSIDE the truck cabin.\n");
    } else {
        printf("Point (100, 65, -30) is classified as OUTSIDE the truck cabin.\n");
    }

    return 0;
}
