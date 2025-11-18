# ESP32-S3 development environment using ESP-IDF
FROM espressif/idf:release-v5.2

# Install additional helpful tools
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        git \
        wget \
        nano \
        python3-pip && \
    rm -rf /var/lib/apt/lists/*

# Default workdir where the ESP-IDF project will be mounted
WORKDIR /workspace
