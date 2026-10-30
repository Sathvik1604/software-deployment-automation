# ==============================================================================
# Multi-Stage Dockerfile for C++ Calculator Application
# Academic Project: C++ Software Deployment Automation with Workflow Reporting
# ==============================================================================

# ------------------------------------------------------------------------------
# Stage 1: Build Environment
# ------------------------------------------------------------------------------
FROM ubuntu:22.04 AS builder

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install C++ compiler (GCC 12) and CMake build tool
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Set working directory inside container
WORKDIR /app

# Copy CMake configuration and source code
COPY CMakeLists.txt ./
COPY src/ ./src/
COPY tests/ ./tests/

# Configure and compile in Release mode
RUN cmake -B build -S . -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --config Release

# Verify test suite passes during the container build stage as a sanity check
RUN ctest --test-dir build --output-on-failure

# ------------------------------------------------------------------------------
# Stage 2: Minimal Runtime Environment
# ------------------------------------------------------------------------------
FROM ubuntu:22.04 AS runtime

# Install runtime C++ standard library dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    libstdc++6 \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Create a non-privileged user for secure container execution
RUN useradd -m -u 1001 appuser
USER appuser
WORKDIR /home/appuser

# Copy only the compiled production executable from the builder stage
COPY --from=builder /app/build/calculator /usr/local/bin/calculator

# Default container entrypoint
# When run without arguments, executes the deterministic demo suite.
# When arguments are passed (e.g. docker run img 10 + 5), passes them to calculator.
ENTRYPOINT ["/usr/local/bin/calculator"]
