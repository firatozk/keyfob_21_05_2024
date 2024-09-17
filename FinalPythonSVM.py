import numpy as np
from sklearn import svm
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import accuracy_score

# Define truck cabin dimensions
TRUCK_X = 235
TRUCK_Y = 211
TRUCK_Z = 212

# Function to generate structured data within the given range
def generate_data(num_samples, x_range, y_range, z_range):
    X = np.random.uniform(low=[x_range[0], y_range[0], z_range[0]], 
                          high=[x_range[1], y_range[1], z_range[1]], 
                          size=(num_samples, 3))
    # Generate labels based on whether points are within the truck cabin dimensions
    y = np.all((X >= [0, 0, 0]) & (X <= [TRUCK_X, TRUCK_Y, TRUCK_Z]), axis=1).astype(int)
    return X, y

# Save the SVM model details to a text file
def export_svm_model_to_txt(svm_model, scaler):
    with open('svm_model_data.txt', 'w') as f:
        f.write("Support Vectors:\n")
        np.savetxt(f, svm_model.support_vectors_)
        f.write("\nDual Coefficients:\n")
        np.savetxt(f, svm_model.dual_coef_)
        f.write("\nIntercept:\n")
        np.savetxt(f, svm_model.intercept_)
        f.write("\nScaling Mean:\n")
        np.savetxt(f, scaler.mean_)
        f.write("\nScaling Scale:\n")
        np.savetxt(f, scaler.scale_)

# Parameters
num_samples = 2000
x_range = (-30, 265)
y_range = (-30, 241)
z_range = (-25, 232)

# Generate structured data
X, y = generate_data(num_samples, x_range, y_range, z_range)

# Add some points outside the range for classification
outliers = np.random.uniform(low=[x_range[0]-50, y_range[0]-50, z_range[0]-50], 
                             high=[x_range[1]+50, y_range[1]+50, z_range[1]+50], 
                             size=(50, 3))
X = np.vstack((X, outliers))
y = np.hstack((y, np.zeros(50)))  # Add 0 labels for outliers

# Scale the features
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

# Train SVM model
svm_model = svm.SVC(kernel='rbf', C=1.0, gamma='scale')  # You can tweak C and gamma for tuning
svm_model.fit(X_scaled, y)

# Export model data to a text file
export_svm_model_to_txt(svm_model, scaler)
