"""Multi-Layer Perceptron implementation."""

from typing import List, Union
from layers import Linear, ReLU

import numpy as np

class MLP:
    """
    Multi-Layer Perceptron with alternating Linear and ReLU layers.

    The network is built as a sequence of layers:
    Linear -> ReLU -> Linear -> ReLU -> ... -> Linear

    Example:
        >>> mlp = MLP([4, 64, 32, 4])
        >>> output = mlp.forward(np.random.randn(32, 4))
        >>> print(output.shape)
        (32, 4)
    """

    def __init__(self, layer_sizes: List[int]):
        """
        Initialize MLP with specified architecture.

        Args:
            layer_sizes: List of neuron counts for each layer.
                First element is input dimension, last is output dimension.
                Example: [4, 64, 32, 4] creates:
                    - Linear(4 -> 64)
                    - ReLU()
                    - Linear(64 -> 32)
                    - ReLU()
                    - Linear(32 -> 4)

        Raises:
            ValueError: If layer_sizes has fewer than 2 elements.
        """
        if len(layer_sizes) < 2:
            raise ValueError("At least input and output layers required")

        self.layers = []

        for i in range(len(layer_sizes) - 1):
            # Add linear layer
            self.layers.append(Linear(layer_sizes[i], layer_sizes[i + 1]))

            # Add ReLU after all layers except the last
            if i < len(layer_sizes) - 2:
                self.layers.append(ReLU())

    def forward(self, x: np.ndarray) -> np.ndarray:
        """
        Forward pass through all layers.

        Args:
            x: Input array of shape (batch_size, input_dim).

        Returns:
            Output predictions of shape (batch_size, output_dim).
        """
        for layer in self.layers:
            x = layer.forward(x)
        return x

    def backward(self, grad_output: np.ndarray) -> np.ndarray:
        """
        Backward pass through all layers in reverse order.

        Args:
            grad_output: Gradient from loss function or next layer,
                shape (batch_size, output_dim).

        Returns:
            Gradient with respect to network input,
                shape (batch_size, input_dim).
        """
        for layer in reversed(self.layers):
            grad_output = layer.backward(grad_output)
        return grad_output

    def update(self, learning_rate: float) -> None:
        """
        Update all trainable layers using accumulated gradients.

        Args:
            learning_rate: Step size for gradient descent.
        """
        for layer in self.layers:
            layer.update(learning_rate)

    def zero_grad(self) -> None:
        """
        Reset accumulated gradients for all trainable layers.

        Should be called after each weight update or before gradient accumulation.
        """
        for layer in self.layers:
            if hasattr(layer, 'dW'):
                layer.dW.fill(0)
                layer.db.fill(0)
