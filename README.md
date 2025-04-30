# Automated Rubik's Cube Solving & Scrambling Robot with LEGO EV3

This project explores the intersection of robotics, programming, and puzzle solving by automating the solution of a Rubik's Cube. It serves as a hands-on learning experience in mechanical design, sensor integration, and software coordination using LEGO EV3.

An automated system that uses LEGO Mindstorms EV3 hardware and RobotC/Python software to scramble and solve any 3×3 Rubik's Cube configuration. The robot detects cube placement, scrambles the cube, reads cube state, computes a solution via a web-based solver, and executes moves to solve the puzzle, timing the entire process.

## Features

- Detects cube presence via ultrasonic sensor before operation.
- Random scrambling using EV3 buttons (Left/Right) and file-based move sequences.
- Cube state acquisition via stored scramble sequence (fallback for color sensor limitations).
- Solving algorithm integration using a Selenium-based web solver for consistent solution times (~1 min).
- Precise manipulation with three motors: gripping arm, rotating base, and color-sensor actuator.
- User interface via EV3 buttons: Left/Right to scramble, Enter to sense/solve, Down to exit.
- Time-to-solve display on EV3 console.

## Demo

You can find the project demonstration and final report here:

- [Project Demo (PDF)](docs/Project%20Demo.pdf)
- [Final Report (PDF)](docs/Robot%20Project%20Final%20Report.pdf)

1. Place the cube on the base (white on top, green front).
2. Press Left/Right button to scramble; robot randomizes cube and writes moves to `scramble.txt`.
3. Press Enter to sense (via file) and compute solve sequence (`output.txt`).
4. Press Enter again to execute solve; time taken is displayed.
5. Press Down to end the program.

## Hardware Requirements

- LEGO Mindstorms EV3 Brick
- 3 EV3 Large Motors (arm flip, base rotation, sensor actuator)
- EV3 Ultrasonic Sensor
- EV3 Color Sensor (optional; fallback implemented)
- Standard EV3 buttons for control input
- EV3 structural parts for chassis, arm, and base

## Software Requirements

- RobotC for EV3 (firmware v1.x)
- Python 3.x with Selenium
- ChromeDriver (matching local Chrome version)
- Online Rubik's Cube Solver access

## Repository Structure

```
├── main.py                 # Python script to fetch solution via Selenium
├── runRobot.c              # RobotC code controlling EV3 robot
├── readandwritefile.c      # EV3 file I/O utilities
├── scramble.txt            # Generated scramble move list
├── output.txt              # Retrieved solve move list
└── README.md               # Project overview and instructions
```

## Setup & Installation

1. Clone this repository:
   ```bash
git clone https://github.com/s7pan/Rubik-s-Cube-Solver-.git
cd Rubik-s-Cube-Solver-
```
2. Install Python dependencies:
   ```bash
pip install selenium
```
3. Download ChromeDriver and ensure it's on your PATH.
4. Load `runRobot.c` and `readandwritefile.c` into your EV3 RobotC environment and compile to the brick.

## Usage

1. Power on the EV3 brick and select the uploaded program.
2. Ensure the Python environment is running and `main.py` is configured with correct WebDriver path.
3. Interact via EV3 buttons as described in the [Demo](#demo) section.

## Project Workflow

- Mechanical redesign of EV3 chassis and gripping arm for stability and precision citeturn0file1
- Modular RobotC functions: `ultrasonic()`, `flip()`, `base()`, `turnLayer()`, `readAndTurn()` citeturn0file0
- Python Selenium integration to retrieve solve sequences from online solver citeturn0file1

## Testing & Validation

| Test Case           | Component        | Outcome                      |
|---------------------|------------------|------------------------------|
| Ultrasonic check    | `ultrasonic()`   | Pass: Robot waits for cube   |
| Scramble moves      | `randomScramble` | Pass: Valid sequences in file|
| Solve execution     | `readAndTurn`    | Pass: Cube solved consistently|
| Color sensing       | `colourSensor()` | Fail: fallback used           | citeturn0file1

## Future Improvements

- Integrate vision-based color detection for direct cube scanning
- Implement Kociemba's two-phase algorithm on-brick for faster solves
- Add real-time EV3 display of move sequence
- Enhance mechanical grip friction and stability

## License

MIT License

Copyright (c) 2025 s7pan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

