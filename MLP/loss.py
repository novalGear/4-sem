"""Loss function implementations."""

import numpy as np
from typing import Optional


class MSELoss:
    """
    Mean Squared Error loss for regression.

    Computes: L = (1/N) * Σ(y_pred - y_true)²

    Attributes:
        pred: Cached predictions from forward pass.
        target: Cached targets from forward pass.
    """

    def __init__(self):
        """Initialize MSE loss."""
        self.pred: Optional[np.ndarray] = None
        self.target: Optional[np.ndarray] = None

    def forward(self, prediction: np.ndarray, target: np.ndarray) -> float:
        """
        Compute MSE loss value.

        Args:
            prediction: Model predictions, shape (batch_size, output_dim).
            target: Ground truth values, shape (batch_size, output_dim).

        Returns:
            Scalar loss value averaged over all elements.

        Raises:
            ValueError: If input shapes do not match.
        """
        if prediction.shape != target.shape:
            raise ValueError(
                f"Shape mismatch: prediction {prediction.shape} vs target {target.shape}"
            )

        self.pred = prediction
        self.target = target

        squared_errors = (prediction - target) ** 2
        return float(np.mean(squared_errors))

    def backward(self) -> np.ndarray:
        """
        Compute gradient of MSE loss w.r.t predictions.

        Formula: ∂L/∂pred = (2/N) * (pred - target)
        where N = batch_size * output_dim.

        Returns:
            Gradient array of same shape as prediction.

        Raises:
            RuntimeError: If forward() has not been called first.
        """
        if self.pred is None or self.target is None:
            raise RuntimeError("Must call forward() before backward()")

        n = self.pred.size
        return 2 * (self.pred - self.target) / n
