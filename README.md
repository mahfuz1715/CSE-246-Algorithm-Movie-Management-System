# Smart Movie Recommendations — Movie Management System

A C-based **Movie Management and Smart Recommendation** project developed for the **CSE 246: Algorithm** course. The project demonstrates the practical application of three fundamental algorithms—**Longest Common Subsequence (LCS)**, **0/1 Knapsack**, and **Topological Sorting**—to manage, select, compare, and organize a large collection of movies.

The system is designed to work with movie datasets of up to **10,000 entries** and includes an automatic input generator for creating large test datasets.

---

## 📌 Project Overview

The main goal of this project is to build an efficient movie management system that can:

- Generate a large set of movie data automatically.
- Measure genre-based similarity between movies using **LCS**.
- Select the maximum possible number of movies within a specified time limit using **0/1 Knapsack**.
- Arrange selected movies according to their **franchise order** using **Topological Sorting**.
- Measure and save the execution time of the implemented algorithms.

The project is implemented in **C** with file-based input and output.

---

## 🎯 Objectives

The project focuses on applying algorithmic problem-solving techniques to a practical movie-management scenario.

### Main objectives

1. Compare movies based on their genre sequences.
2. Select the maximum number of movies under a given viewing-time constraint.
3. Maintain the correct viewing order for movies belonging to the same franchise.
4. Test the algorithms with large datasets.
5. Analyze the execution time of the algorithms.

---

## 🧠 Algorithms Used

### 1. Longest Common Subsequence (LCS)

LCS is used to calculate the similarity between the genre sequences of two movies.

For example:

```text
Movie A → Action, Comedy, Drama
Movie B → Action, Drama, Horror
```

The common subsequence is:

```text
Action, Drama
```

So, the LCS length represents the number of common genres appearing in the same relative order.

In this project, LCS is computed for movie pairs to produce a genre-similarity matrix.

**Time Complexity:** `O(m × n)`

Since the maximum number of genres per movie is fixed at 10, the comparison between two movies is effectively constant-sized, and computing similarity for all movie pairs is treated as `O(n²)`.

---

### 2. 0/1 Knapsack

The 0/1 Knapsack algorithm is used to select the **maximum number of movies** that can be watched within a specified time limit.

Each movie is treated as:

- **Weight:** Movie duration
- **Value:** `1`
- **Capacity:** Available viewing time

The algorithm determines which movies should be selected while keeping the total duration within the time limit.

In the provided input generator, the default time limit written to the input file is **500 minutes**.

**Time Complexity:** `O(n × W)`

Here, `W` is the time limit. With a fixed limit of 500 minutes, this behaves as `O(n)` with respect to the number of movies.

---

### 3. Topological Sorting

Topological Sorting is used to arrange the selected movies in their proper **franchise sequence**.

For movies belonging to the same franchise, an earlier franchise order creates a dependency on a later order.

For example:

```text
Movie 1 → FranchiseA, Order 1
Movie 2 → FranchiseA, Order 2
Movie 3 → FranchiseA, Order 3
```

The resulting order becomes:

```text
Movie 1
Movie 2
Movie 3
```

The implementation uses **Kahn's Algorithm** for topological sorting.

**Time Complexity:** `O(V + E)`

where:

- `V` = number of selected movies
- `E` = number of dependency edges

---

## 🏗️ Project Workflow

The complete project follows this workflow:

```text
                 auto_input_generator.c
                           │
                           ▼
                       input.txt
                           │
                           ▼
                  movie_management.c
                           │
            ┌──────────────┼──────────────┐
            ▼              ▼              ▼
           LCS         0/1 Knapsack    Topological
       Similarity       Selection         Sort
            │              │              │
            └──────────────┼──────────────┘
                           ▼
                      output.txt
                           │
                           └───► time_results.txt
```

### Step 1 — Generate Input

`auto_input_generator.c` creates movie records automatically and saves them into:

```text
input.txt
```

The generator asks for the number of movies to generate, between **100 and 10,000**.

Each generated movie contains:

```text
Movie Name
Duration
Number of Genres
Genres
Franchise
Franchise Order
```

---

### Step 2 — Read Movie Data

`movie_management.c` opens `input.txt` and loads all movie information into memory.

Each movie is represented using a `Movie` structure containing:

```text
name
duration
genres
genre_count
franchise
franchise_order
```

---

### Step 3 — Calculate Genre Similarity

The program runs LCS for movie pairs and stores the resulting similarity values in a similarity matrix.

This provides an algorithmic measurement of how closely the genre sequences of different movies match.

---

### Step 4 — Select Movies

The 0/1 Knapsack algorithm selects the maximum number of movies that fit within the available time limit.

Selected movies are marked using the `selected[]` array.

---

### Step 5 — Arrange Selected Movies

The selected movies are used to build a dependency graph based on franchise and franchise order.

Kahn's Topological Sorting algorithm then produces the final franchise-aware order.

---

### Step 6 — Generate Results

The program writes the selected movies and their topological order to:

```text
output.txt
```

Execution times for the three algorithms are written to:

```text
time_results.txt
```

---

## 📂 Project Structure

```text
Movie-Management/
│
├── auto_input_generator.c   # Generates large movie datasets
├── movie_management.c       # Main project implementation
├── input.txt                # Generated/input movie data
├── output.txt               # Selected and sorted movie results
├── time_results.txt         # Algorithm execution times
└── README.md
```

> `input.txt`, `output.txt`, and `time_results.txt` are runtime/data files. The two `.c` files contain the source-code implementation.

---

## ⚙️ Requirements

To compile and run the project, you need:

- A C compiler such as **GCC**
- A terminal / command prompt
- A system capable of running standard C programs

---

## ▶️ How to Run

### 1. Compile the input generator

```bash
gcc auto_input_generator.c -o generator
```

### 2. Run the generator

```bash
./generator
```

Enter a number between `100` and `10000`.

Example:

```text
Enter the number of movies to generate (100 to 10000): 100
```

This creates:

```text
input.txt
```

---

### 3. Compile the main program

```bash
gcc movie_management.c -o movie_management
```

### 4. Run the main program

```bash
./movie_management
```

After successful execution, the program creates:

```text
output.txt
time_results.txt
```

You can then open these files to inspect the selected movies, their franchise-based order, and the execution times.

---

## 📄 Input Format

The generated input file starts with the viewing-time limit:

```text
500
```

Each following line contains information about one movie:

```text
MovieName Duration GenreCount Genre1 Genre2 ... Franchise FranchiseOrder
```

Example:

```text
Movie1 120 3 Action Comedy Drama FranchiseA 2
```

---

## 📤 Output

The generated `output.txt` contains two main sections:

```text
Selected Movies:
Movie...
Movie...
Movie...

Topological Order of Selected Movies:
Movie...
Movie...
Movie...
```

The `time_results.txt` file contains:

```text
LCS Time: ...
Knapsack Time: ...
Topological Sort Time: ...
```

These results can be used to evaluate and compare the computational cost of the implemented algorithms.

---

## ⏱️ Overall Time Complexity

Based on the project implementation and the analysis in the report:

| Algorithm | Time Complexity |
|-----------|-----------------|
| LCS for all movie pairs | `O(n²)` |
| 0/1 Knapsack | `O(n × W)` |
| Topological Sort | `O(V + E)` |
| **Overall** | **`O(n²)`** |

The overall complexity is dominated by the pairwise LCS computation.

---

## ⚠️ Challenges Considered

The project also highlights several algorithmic challenges:

- LCS requires comparison across movie pairs, which becomes expensive for large datasets.
- The Knapsack model assigns the same value (`1`) to every movie, although real-world movie preferences may differ.
- Topological ordering depends on consistent franchise information and franchise-order data.

For a dataset of up to 10,000 movies, these considerations become important when evaluating performance.

---

## 🚀 Possible Future Improvements

The current system can be extended in several ways:

- Introduce more meaningful movie-value scores instead of assigning every movie a value of `1`.
- Improve the movie similarity model beyond simple genre-sequence LCS.
- Add machine-learning-based recommendation techniques.
- Develop a graphical user interface for easier interaction.
- Add richer movie metadata such as ratings, actors, directors, and release years.
- Improve scalability for very large datasets.

---

## 👥 Project Information

**Course:** CSE 246 — Algorithm  
**Project:** Smart Movie Recommendations / Movie Management  
**Semester:** Fall 2024  
**Institution:** Department of Computer Science and Engineering, East West University

---

## 📚 Key Concepts Demonstrated

This project provides practical implementation of:

- Dynamic Programming
- Longest Common Subsequence
- 0/1 Knapsack
- Graph Representation
- In-Degree Calculation
- Kahn's Topological Sorting Algorithm
- File Handling in C
- Dynamic Memory Allocation
- Execution Time Measurement
- Large Dataset Generation

---

## ✅ Conclusion

The Smart Movie Recommendations project demonstrates how classical algorithms can be combined to solve different parts of a practical movie-management problem.

**LCS** handles genre-based similarity, **0/1 Knapsack** finds the maximum number of movies within a time constraint, and **Topological Sorting** organizes selected movies according to franchise dependencies.

Together, these components form a compact algorithm-focused system for experimenting with movie selection, similarity analysis, ordering, and performance measurement.
