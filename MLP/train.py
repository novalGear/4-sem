"""Training pipeline for MLP on synthetic regression task."""

import numpy as np
from typing import List, Tuple

from model import MLP
from loss import MSELoss


def generate_dataset(
    n_samples: int = 10000,
    test_ratio: float = 0.2,
    random_seed: int = 42
) -> Tuple[np.ndarray, np.ndarray, np.ndarray, np.ndarray]:
    """
    Generate dataset according to function from assignment.

    Function: f(x1, x2, x3, x4) = (x1², 3x2, 5x4 - x3, 3)ᵀ

    Args:
        n_samples: Total number of samples to generate.
        test_ratio: Proportion of samples to use for testing.
        random_seed: Seed for reproducible random generation.

    Returns:
        Tuple of (X_train, y_train, X_test, y_test) arrays.
    """
    np.random.seed(random_seed)

    # Generate inputs from standard normal distribution
    X = np.random.randn(n_samples, 4)

    # Compute outputs using formula (2) from assignment
    y = np.zeros((n_samples, 4))
    y[:, 0] = X[:, 0] ** 2      # x1²
    y[:, 1] = 3 * X[:, 1]        # 3x2
    y[:, 2] = 5 * X[:, 3] - X[:, 2]  # 5x4 - x3
    y[:, 3] = 3.0                # constant

    # Split into train/test
    n_test = int(n_samples * test_ratio)
    indices = np.random.permutation(n_samples)

    X_train = X[indices[n_test:]]
    y_train = y[indices[n_test:]]
    X_test = X[indices[:n_test]]
    y_test = y[indices[:n_test]]

    return X_train, y_train, X_test, y_test


def create_batches(
    X: np.ndarray,
    y: np.ndarray,
    batch_size: int,
    shuffle: bool = True
) -> List[Tuple[np.ndarray, np.ndarray]]:
    """
    Split data into batches for mini-batch training.

    Args:
        X: Features array of shape (n_samples, input_dim).
        y: Targets array of shape (n_samples, output_dim).
        batch_size: Number of samples per batch.
        shuffle: If True, shuffle data before batching.

    Returns:
        List of (X_batch, y_batch) tuples.
    """
    n_samples = X.shape[0]

    if shuffle:
        indices = np.random.permutation(n_samples)
        X = X[indices]
        y = y[indices]

    batches = []
    for i in range(0, n_samples, batch_size):
        X_batch = X[i:i + batch_size]
        y_batch = y[i:i + batch_size]
        batches.append((X_batch, y_batch))

    return batches


def clip_gradients(model: MLP, threshold: float = 1.0) -> None:
    """
    Clip gradients by norm to prevent exploding gradients.

    Args:
        model: MLP model whose gradients to clip.
        threshold: Maximum allowed L2 norm for gradients.
    """
    for layer in model.layers:
        if hasattr(layer, 'dW'):
            # Clip weights gradient
            norm = np.linalg.norm(layer.dW)
            if norm > threshold:
                layer.dW *= threshold / norm

            # Clip bias gradient
            norm = np.linalg.norm(layer.db)
            if norm > threshold:
                layer.db *= threshold / norm


def add_l2_regularization(model: MLP, lambda_reg: float = 0.0001) -> None:
    """
    Add L2 regularization penalty to gradients.

    Modifies gradients: dW += λ * W

    Args:
        model: MLP model whose gradients to modify.
        lambda_reg: Regularization strength (λ parameter).
    """
    for layer in model.layers:
        if hasattr(layer, 'dW'):
            layer.dW += lambda_reg * layer.W


def train_epoch(
    model: MLP,
    loss_fn: MSELoss,
    batches: List[Tuple[np.ndarray, np.ndarray]],
    learning_rate: float,
    clip_threshold: float = 1.0,
    lambda_reg: float = 0.0001
) -> float:
    """
    Train model for one epoch with gradient accumulation over batches.

    Args:
        model: MLP model to train.
        loss_fn: Loss function.
        batches: List of (X_batch, y_batch) tuples.
        learning_rate: Step size for gradient descent.
        clip_threshold: Gradient clipping threshold (0 to disable).
        lambda_reg: L2 regularization strength (0 to disable).

    Returns:
        Average loss over all batches in this epoch.
    """
    total_loss = 0.0

    for X_batch, y_batch in batches:
        batch_loss = 0.0

        # Accumulate gradients over all samples in batch
        for i in range(X_batch.shape[0]):
            x = X_batch[i:i + 1]
            y = y_batch[i:i + 1]

            # Forward pass
            prediction = model.forward(x)

            # Compute loss
            loss = loss_fn.forward(prediction, y)
            batch_loss += loss

            # Backward pass (accumulates gradients)
            grad = loss_fn.backward()
            model.backward(grad)

        # Average loss for this batch
        batch_loss /= X_batch.shape[0]
        total_loss += batch_loss

        # Apply optimization techniques
        if clip_threshold > 0:
            clip_gradients(model, clip_threshold)
        if lambda_reg > 0:
            add_l2_regularization(model, lambda_reg)

        # Update weights with accumulated gradients
        model.update(learning_rate)
        model.zero_grad()

    return total_loss / len(batches)


def evaluate(
    model: MLP,
    loss_fn: MSELoss,
    batches: List[Tuple[np.ndarray, np.ndarray]]
) -> float:
    """
    Evaluate model on given batches without gradient computation.

    Args:
        model: MLP model to evaluate.
        loss_fn: Loss function.
        batches: List of (X_batch, y_batch) tuples.

    Returns:
        Average loss over all samples.
    """
    total_loss = 0.0
    total_samples = 0

    for X_batch, y_batch in batches:
        prediction = model.forward(X_batch)
        loss = loss_fn.forward(prediction, y_batch)
        total_loss += loss * X_batch.shape[0]
        total_samples += X_batch.shape[0]

    return total_loss / total_samples


def main() -> None:
    """Run complete training pipeline."""
    # Hyperparameters
    BATCH_SIZE = 32
    EPOCHS = 100
    LEARNING_RATE = 0.001
    CLIP_THRESHOLD = 1.0
    L2_REGULARIZATION = 0.0001
    VERBOSE_INTERVAL = 10

    # Generate data
    print("Generating dataset...")
    X_train, y_train, X_test, y_test = generate_dataset()

    # Create batches
    train_batches = create_batches(X_train, y_train, BATCH_SIZE, shuffle=True)
    test_batches = create_batches(X_test, y_test, BATCH_SIZE, shuffle=False)

    print(f"Train samples: {X_train.shape[0]}, Test samples: {X_test.shape[0]}")
    print(f"Train batches: {len(train_batches)}, Test batches: {len(test_batches)}")

    # Initialize model
    model = MLP([4, 128, 64, 4])
    loss_fn = MSELoss()

    print(f"Model architecture: 4 -> 64 -> 32 -> 4")
    print(f"Total parameters: {sum(l.W.size + l.b.size for l in model.layers if hasattr(l, 'W'))}")

    # Training loop
    print(f"\nTraining for {EPOCHS} epochs...")
    print("-" * 60)

    for epoch in range(EPOCHS):
        train_loss = train_epoch(
            model, loss_fn, train_batches,
            LEARNING_RATE, CLIP_THRESHOLD, L2_REGULARIZATION
        )
        test_loss = evaluate(model, loss_fn, test_batches)

        if (epoch + 1) % VERBOSE_INTERVAL == 0 or epoch == 0:
            print(f"Epoch {epoch+1:3d}/{EPOCHS} | Train Loss: {train_loss:.6f} | Test Loss: {test_loss:.6f}")

    # Final results
    print("-" * 60)
    print("\nFinal Results:")
    print(f"  Test Loss: {test_loss:.6f}")

    # Sample predictions
    print("\nSample predictions (first 5 test samples):")
    print("-" * 60)
    for i in range(min(5, X_test.shape[0])):
        x = X_test[i:i+1]
        y_true = y_test[i]
        y_pred = model.forward(x).flatten()
        error = np.abs(y_true - y_pred)

        print(f"\nSample {i+1}:")
        print(f"  Input:  {X_test[i]}")
        print(f"  True:   {y_true}")
        print(f"  Pred:   {y_pred}")
        print(f"  Error:  {error}")
        print(f"  Max error: {error.max():.4f}")


if __name__ == "__main__":
    main()
