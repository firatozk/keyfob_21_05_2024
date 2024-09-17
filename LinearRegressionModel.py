import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import StandardScaler

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

# Parameters
num_samples = 10000
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

# Train linear regression model
model = LinearRegression()
model.fit(X_scaled, y)

# Extract coefficients and intercept
coefficients = model.coef_
intercept = model.intercept_

# Print coefficients and intercept
print("Coefficients:", coefficients)
print("Intercept:", intercept)

# Predict and visualize
y_pred = model.predict(X_scaled)
y_pred_binary = np.round(y_pred)  # Convert predictions to binary (0 or 1)

# Count samples inside and outside
inside_count = np.sum(y == 1)
outside_count = np.sum(y == 0)
predicted_inside_count = np.sum(y_pred_binary == 1)
predicted_outside_count = np.sum(y_pred_binary == 0)

print("Actual Inside Samples:", inside_count)
print("Actual Outside Samples:", outside_count)
print("Predicted Inside Samples:", predicted_inside_count)
print("Predicted Outside Samples:", predicted_outside_count)

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
ax3.scatter(X[:, 0], X[:, 1], X[:, 2], c=y_pred_binary, marker='o', cmap='coolwarm', label='Prediction')
ax3.set_xlabel('X')
ax3.set_ylabel('Y')
ax3.set_zlabel('Z')
ax3.set_title('Linear Regression Predictions')
ax3.legend()

plt.tight_layout()
plt.show()
