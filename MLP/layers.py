"""Neural network layer implementations."""

import numpy as np
from typing import Optional, Tuple


class Linear:
    """
    Fully connected linear layer: y = x @ W + b.

    Attributes:
        W: Weight matrix of shape (input_dim, output_dim)
        b: Bias vector of shape (output_dim,)
        dW: Accumulated gradient for weights
        db: Accumulated gradient for bias
        x: Input cache from forward pass
    """

    def __init__(self, input_dim: int, output_dim: int):
        """
        Initialize linear layer with Xavier initialization.

        Args:
            input_dim: Number of input features.
            output_dim: Number of output neurons.
        """
        scale = np.sqrt(2.0 / input_dim)
        self.W = np.random.randn(input_dim, output_dim) * scale
        self.b = np.zeros(output_dim)
        self.dW = np.zeros_like(self.W)
        self.db = np.zeros_like(self.b)
        self.x: Optional[np.ndarray] = None

    def forward(self, x: np.ndarray) -> np.ndarray:
        """
        Forward pass: y = x @ W + b.

        Args:
            x: Input array of shape (batch_size, input_dim).

        Returns:
            Output array of shape (batch_size, output_dim).
        """
        self.x = x
        return x @ self.W + self.b

    def backward(self, grad_output: np.ndarray) -> np.ndarray:
        """
        Backward pass computing gradients.

        Args:
            grad_output: Gradient from next layer, shape (batch_size, output_dim).

        Returns:
            Gradient with respect to input, shape (batch_size, input_dim).
        """
        # Gradient for input (propagate backward)
        grad_input = grad_output @ self.W.T

        # Accumulate gradients for parameters
        self.dW += self.x.T @ grad_output
        self.db += grad_output.sum(axis=0)

        return grad_input

    def update(self, learning_rate: float) -> None:
        """
        Update weights using accumulated gradients.

        Args:
            learning_rate: Step size for gradient descent.
        """
        self.W -= learning_rate * self.dW
        self.b -= learning_rate * self.db
        self.dW.fill(0)
        self.db.fill(0)


class ReLU:
    """
    Rectified Linear Unit activation: f(x) = max(0, x).

    Attributes:
        x: Input cache from forward pass.
    """

    def __init__(self):
        """Initialize ReLU activation layer."""
        self.x: Optional[np.ndarray] = None

    def forward(self, x: np.ndarray) -> np.ndarray:
        """
        Forward pass: apply max(0, x) element-wise.

        Args:
            x: Input array of any shape.

        Returns:
            Output array with same shape, negative values set to zero.
        """
        self.x = x
        return np.maximum(0, x)

    def backward(self, grad_output: np.ndarray) -> np.ndarray:
        """
        Backward pass: zero out gradients where input was negative.

        Args:
            grad_output: Gradient from next layer, same shape as input.

        Returns:
            Gradient with respect to input.
        """
        return grad_output * (self.x > 0)

    def update(self, learning_rate: float) -> None:
        """
        Update method for API consistency. ReLU has no trainable parameters.

        Args:
            learning_rate: Not used.
        """
        pass
