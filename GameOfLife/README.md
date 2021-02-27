# Divya's Game of Life
CS3210: Programming Languages and Tools - Programming with C++ - May 2020
<br>
Author: Divya Thomas
<br><br>
[Run this program on Repl.it!](https://repl.it/@divthomas/Divyas-Game-of-Life)

<br>

## File Contents
[Project Description/Requirements](#desc)

[Ecosystem Rules](#ecosystem-rules)

[Technologies Used](#tech)

[Included Project Documents/Additional Links](#links)

<br><br>
## Project Description and Requirements <a name="desc" />

The goal of the course project is to solidify the foundational C++ knowledge obtained in this course by desinging and implementing a functional, larger-scope software system. The project topic has been chosen to necessitate the use of a wide array of language concepts and features we studied/will study, including user-defined types, inheritance, run-time polymorphism, exceptions, separate compilation, smart pointers, containers and algorithms, file input/output, and other standard library facilities.

The project objective is to **build an interactive command line program that (crudely) simulates a small terrestrial ecosystem** according to the rules below. 

The core notion of the simulator is that of an iteration: a single "step" of the simulation during which the ecosystem transforms itself to its next state according to its [rules](#ecosystem-rules) (think [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)).

An ecosystem is defined by two inputs:
  - a rectangular [map of the area](Area-Map.md) that describes the (initial) location of the geographic features and the living organisms (plants + animals) that populate the area;
  - a [list of species](Species.md) populating the ecosystem, together with their essential characteristics and inter-relationships. 

The ecosystem map and the list of species are loaded from the corresponding files provided to the program as command line arguments. 

Runing the simulator starts a new simulation session. Within a single simulation session, the user can select to run a single iteration, or a "batch" of several iterations at once (e.g. 10 or 100 iterations). After each run, the simulator prints the session's current ecosystem state to the console. The user can then choose to continue the simulation session (by doing another run), save the current ecosystem state to a file, or exit. If a user who ran at least one iteration chooses to exit, they are asked whether they want to save the current ecosystem state to a file before exiting.

Saving the current ecosystem state to a file simply means saving the current state of the map to a file. This means that things like the animals' current energy levels and the plants' regrowth states will be reset to their initial values when the user saves and then "restores" the state by using the saved map to start a new simulation.

## Ecosystem rules

1. During each iteration every organism in the ecosystem gets a chance to perform its living functions (move, grow, consume). Plants get their turn first, then the herbivores, then the omnivores, then the cycle repeats at the next iteration. 
2. Animals move freely around the ecosystem to find food, but they cannot move over the geographic obstacles, plants or other animals (unless they are consuming them). All animals move with the same speed (one block per iteration).
3. Plants remain static and don't spread into new areas.
4. Each living organism has an associated number of "energy points" it gives to the organism that consumes it.
5. Plants don't eat other organisms; herbivores eat only plants; omnivores eat plants and other animals.
6. Plants that have been eaten regrow in the same location according to their "regrowth coefficient"; for example, a plant with a regrowth coefficient of 2 regrows in two iterations. A plant cannot regrow if its location is currently occupied by an animal.
7. Animals that have been eaten are removed from the similation.
8. Each animal has a current energy level as well as a "max" energy level that limits the number of energy points they can absorb. All animals start the simulation with a current energy level equal to their "max" level. Each move subtracts one point from their current energy level. An animal with zero energy points dies and is removed from the simulation. 
9. An animal can consume another organism only if it can add a greater-than-zero number of the consumed organism's energy points to its energy level without going over its "max" limit. To consume another organism, the consumer has to move to the organism's position.
10. Two adjacent animals of the same species with the current energy level that is more than a half of their "max" can produce an offspring (given that there is space next to either of them to place the offspring).
11. Animals act in their own interest: e.g. if an animal is in danger of being eaten, it'll try to escape, if it's running out of energy, it'll seek to replenish it, etc.


## Technologies Used <a name="tech" />

  C++

## Included Project Documents / Additional Links <a name="links" />

[Source Code](GameOfLife_SourceCode.cpp)

[Map Specifics](Area-Map.md)

[Species List Specifics](Species.md)

[Sample Input Files](https://github.com/divthomas22/DivyaThomasPortfolio/tree/main/GameOfLife/input)

[Run this program on Repl.it!](https://repl.it/@divthomas/Divyas-Game-of-Life)
