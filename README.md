# ESP32-S3 Docker ESP-IDF Environment

This project provides a Dockerized ESP32-S3 development environment based on the ESP-IDF. All tools, toolchains, and dependencies are contained within the container, allowing you to build stable firmware on any machine.

## Structure

```
.
├── Dockerfile              # Defines the ESP-IDF image
├── docker-compose.yml      # Configures the development service
└── firmware                # Sample ESP-IDF source code
    ├── CMakeLists.txt
    ├── sdkconfig.defaults
    └── main
        ├── CMakeLists.txt
        └── main.c
```

## Prerequisites
- Docker
- Docker Compose v2
- Access to the `/dev/ttyUSB0` device (or update it in `docker-compose.yml`).

## How to Use

1. **Build the image**
   ```bash
   docker compose build
   ```
2. **Open a shell in the container**
   ```bash
   docker compose run --rm esp32-dev bash
   ```
3. **Build the firmware** (inside the container)
   ```bash
   idf.py set-target esp32s3
   idf.py build
   ```
4. **Flash and monitor** (with the board connected at `/dev/ttyUSB0`)
   ```bash
   idf.py -p /dev/ttyUSB0 flash monitor
   ```

The sample firmware (`firmware/main/main.c`) will print chip information and a periodic message to ensure that the ESP-IDF is running correctly inside the container.
