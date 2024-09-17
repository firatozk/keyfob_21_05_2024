import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
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

# Function to export SVM model details to a text file
def export_svm_model_to_txt(svm_model, scaler, filename='svm_model_data.txt'):
    with open(filename, 'w') as f:
        # Number of support vectors
        num_support_vectors = svm_model.support_vectors_.shape[0]
        f.write(f"{num_support_vectors}\n")

        # Support vectors
        for vector in svm_model.support_vectors_:
            f.write(' '.join(map(str, vector)) + '\n')

        # Dual coefficients
        for coef in svm_model.dual_coef_.flatten():
            f.write(f"{coef}\n")

        # Intercept
        f.write(f"{svm_model.intercept_[0]}\n")

        # Scaling mean
        f.write(' '.join(map(str, scaler.mean_)) + '\n')

        # Scaling scale
        f.write(' '.join(map(str, scaler.scale_)) + '\n')

# Parameters
num_samples = 20000
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

# Predict
y_pred = svm_model.predict(X_scaled)

# Evaluate the model
accuracy = accuracy_score(y, y_pred)
print(f"Model Accuracy: {accuracy * 100:.2f}%")

# Count samples inside and outside
inside_count = np.sum(y == 1)
outside_count = np.sum(y == 0)
predicted_inside_count = np.sum(y_pred == 1)
predicted_outside_count = np.sum(y_pred == 0)

print("Actual Inside Samples:", inside_count)
print("Actual Outside Samples:", outside_count)
print("Predicted Inside Samples:", predicted_inside_count)
print("Predicted Outside Samples:", predicted_outside_count)

# Test prediction for a specific point (e.g., (100, 65,-30))
point = np.array([[100, 65, -30]])
point_scaled = scaler.transform(point)
point_prediction = svm_model.predict(point_scaled)
print(f"Prediction for point (100, 65, -30): {'Inside' if point_prediction == 1 else 'Outside'}")

# Plotting
fig = plt.figure(figsize=(15, 5))

# Inside points plot
ax1 = fig.add_subplot(131, projection='3d')
ax1.scatter(X[y == 1, 0], X[y == 1, 1], X[y == 1, 2], c='blue', marker='o', label='Inside Cabin')
ax1.set_xlabel('X')
ax1.set_ylabel('Y')
ax1.set_zlabel('Z')
ax1.set_title('Points Inside Cabin')
ax1.legend()

# Outside points plot
ax2 = fig.add_subplot(132, projection='3d')
ax2.scatter(X[y == 0, 0], X[y == 0, 1], X[y == 0, 2], c='red', marker='o', label='Outside Cabin')
ax2.set_xlabel('X')
ax2.set_ylabel('Y')
ax2.set_zlabel('Z')
ax2.set_title('Points Outside Cabin')
ax2.legend()

# Prediction plot
ax3 = fig.add_subplot(133, projection='3d')
ax3.scatter(X[:, 0], X[:, 1], X[:, 2], c=y_pred, marker='o', cmap='coolwarm', label='Prediction')
ax3.set_xlabel('X')
ax3.set_ylabel('Y')
ax3.set_zlabel('Z')
ax3.set_title('SVM Predictions')
ax3.legend()

plt.tight_layout()
plt.show()

# Export model to text file
export_svm_model_to_txt(svm_model, scaler)
