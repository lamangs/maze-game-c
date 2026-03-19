# Police Chase Game (C)

A 2D grid-based console game written in C where the player controls a thief trying to escape while being chased by a police AI.

## Overview
This project focuses on implementing game logic, movement systems, and basic AI behavior using low-level programming in C.

## Features
- 16x16 grid-based game system
- Player-controlled movement (W/A/S/D)
- Two AI difficulty modes:
  - Easy → random movement
  - Hard → heuristic-based chasing (Manhattan distance)
- Obstacle handling (walls)
- Win/lose conditions
- Replay system

## AI Logic
In hard mode, the police uses Manhattan distance to evaluate all possible moves and selects the optimal path toward the player. If multiple optimal moves exist, one is chosen randomly.

## Technologies
- C
- Standard libraries (`stdio.h`, `stdlib.h`, `time.h`, etc.)

## How to Run
Compile:
```bash
gcc mazegame.c -o game
