# Build a Docker image
```
docker build -t sklenik:latest ./
```

# Run a Docker container
```
docker run --rm --mount type=bind,source=$(pwd),target=/work -ti sklenik:latest bash
```

# Compile project
```
docker run --rm --mount type=bind,source=$(pwd),target=/work -ti sklenik:latest arduino-cli compile --fqbn arduino:avr:nano /work/sklenik.ino
```