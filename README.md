# Philosophers

The Philosophers project involves solving the dining philosophers problem using threads and synchronization mechanisms. The aim is to find a solution where the philosophers alternate between eating, thinking, and sleeping while adhering to certain constraints to avoid resource conflicts and ensure fairness in their actions.

## Installation

To compile and execute the Philosophers program, follow these steps:

1. Clone the repository
    ```bash
    git clone https://github.com/Pradene/philo
    ```

2. Build the project:
    ```bash
    make
    ```

3. Run the program:
    ```bash
    ./philo philo_number time_to_die time_to_eat time_to_sleep [optional: repetition]
    ```

Replace number_of_philosophers, time_to_die, time_to_eat, and time_to_sleep with appropriate numerical values for the simulation parameters. The last parameter is optional and represents the number of times each philosopher must eat before the program stops.