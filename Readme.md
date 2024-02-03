# Sudoku Solver with OpenCV

This project combines algorithms and computer vision to create a robust Sudoku-solving solution. Using Python, OpenCV , neural networks, and the Dancing Links algorithm, the goal is to efficiently solve Sudoku puzzles. 

The incorporation of computer vision is a significant advancement, allowing the algorithm to "see" and interpret Sudoku grids within images. This process involves extracting puzzle grids from images and optimizing them for algorithmic analysis. 

The synergy between computer vision and algorithmic optimization enhances digit recognition accuracy, translating visual data seamlessly for efficient solving. Overall, this approach aims to provide a more intuitive and user-friendly experience for Sudoku enthusiasts.

## Example

| Captured Image         | Solved Puzzle          |
| ---------------------- | ---------------------- |
| ![image](https://github.com/ManujaDewmina/Sudoku-solver-with-OpenCV/blob/0e1a2182f8dba2cb8793df21980d147aae3421d3/Images/Solved/capture.jpg) | ![image](https://github.com/ManujaDewmina/Sudoku-solver-with-OpenCV/blob/0e1a2182f8dba2cb8793df21980d147aae3421d3/Images/Solved/answer.jpg) |

## How to run

1. Clone the Github repository : https://github.com/ManujaDewmina/Sudoku-solver-with-OpenCV
2. For 9X9 Sudoku puzzles : `python sudoku_vision_9by9.py` 
3. For 16X16 Sudoku puzzles : `python sudoku_vision_16by16.py` 

Note: Sometimes, errors may occur in the number detection part (OCR) of a 16x16 Sudoku puzzle.

## Demonstration

![video](https://github.com/ManujaDewmina/Sudoku-solver-with-OpenCV/blob/0e1a2182f8dba2cb8793df21980d147aae3421d3/Demonstration/Video_demonstration.mp4)