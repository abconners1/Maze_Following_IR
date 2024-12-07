# Maze Navigation Robot

## Project Overview
This project implements a closed-loop proportional control system for a mobile robot designed to explore and solve a simple maze. The robot uses:

- **Infrared (IR) Sensors**: To detect walls and measure distances.
- **Break Beam Encoders**: To track wheel rotations and estimate position.
- **Closed-Loop Control**: To ensure accurate movement and navigation.
- **Algorithmic Optimization**: Using Tremaux's algorithm to identify and eliminate blind alleys, the robot optimizes its path through the maze.

The robot employs the left-hand rule to navigate the maze on its initial run and stores movements (forward, left, and right) in an array. This data is used to optimize navigation in subsequent runs.

---

## Features
1. **Wall Detection**: Front and left-facing IR sensors determine the proximity of walls.
2. **Closed-Loop Motor Control**: Proportional control adjusts motor speeds for accurate distance traversal.
3. **Maze Optimization**: Tremaux's algorithm removes blind alleys to reduce navigation time.
4. **Encoder-Based Position Tracking**: Encoders mounted on the motor shafts provide precise positional feedback.

---

## Project Setup

### Hardware Requirements
- **Microcontroller**: Arduino-compatible board.
- **Sensors**:
  - Sharp GP2D120 IR Sensors (1 front-facing, 1 left-facing).
  - Omron EE-SG3/EE-SG3-B Break Beam Encoders (2, one for each wheel).
- **Motors**: Two DC motors with PWM and direction control.
- **Power Supply**: Sufficient to run the microcontroller, sensors, and motors.
- **Chassis**: Mobile robot base for mounting sensors and motors.

### Software Requirements
- Arduino IDE (version 1.8 or later).
- PinChangeInt library for handling interrupts.

### Circuit Setup
1. **Encoders**: Connect the left and right encoders to digital pins 8 and 7, respectively.
2. **IR Sensors**:
   - Left IR Sensor to analog pin A1.
   - Front IR Sensor to analog pin A0.
3. **Motors**:
   - Left Motor: PWM to pin 11, direction to pin 13.
   - Right Motor: PWM to pin 3, direction to pin 12.

### Software Setup
1. Install the `PinChangeInt` library in the Arduino IDE.
2. Upload the provided source code to the microcontroller.

---

## Usage Instructions
1. Power on the robot.
2. Place the robot at the maze's starting position.
3. The robot will begin exploring the maze, following the left-hand rule.
4. Movements are recorded as an array of commands (`F` for forward, `L` for left, `R` for right).
5. On the second run, the robot will use the stored commands to navigate an optimized path, bypassing blind alleys.

---

## Code Details
The main functions include:
- `setup()`: Initializes pins, encoders, and interrupts.
- `loop()`: Continuously reads IR sensor data and explores the maze.
- `drive(float distance)`: Drives the robot forward for a specified distance.
- `motor(int m, int pwm)`: Controls motor speed and direction.
- `exploreMaze(int leftIR, int forwardIR)`: Implements the maze exploration logic.
- `storeToArray(int turn)`: Records navigation commands into an array.
- `goForward()`: Moves the robot forward while monitoring sensors.
- `turnObstacleHandling(int forwardIR)`: Handles obstacle avoidance during turns.
- Interrupt handlers (`indexLeftEncoderCount()` and `indexRightEncoderCount()`) update encoder counts.

---

## Testing and Results
### Key Observations
- The robot successfully navigates the maze in its initial run and optimizes its path in subsequent runs.
- Average times for the first run were approximately 2:50 minutes, while optimized runs reduced this to around 2:14 minutes.

### Challenges
- Proportional gain settings required fine-tuning for consistent performance.
- Non-quadrature encoders limited precision, affecting overall control accuracy.

---

## Potential Improvements
1. Implementing full PID control for enhanced accuracy.
2. Adding more robust encoders (e.g., quadrature encoders) for precise position tracking.
3. Using a servo-mounted IR sensor for detecting both left and right walls simultaneously.

---

## References
1. "Maze solving algorithm." Wikipedia. Wikimedia Foundation.
2. "Navigating a Simple Maze." ROBOTC API Guide.

