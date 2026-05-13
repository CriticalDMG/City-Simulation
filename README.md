
# City Simulation

A terminal-based city simulation where you manage a grid of buildings, residents, and their daily lives. You can generate entire cities, add and remove citizens, run the simulation forward (or backward) in time, and query statistics — all from a simple command-line interface.

---

## What It Does

The simulation models a city laid out on an **n × m grid**. Each cell can hold a building of a certain type (residential, communal, etc.), and each building can house a number of citizens. Every resident has a name, a profession, some money, a happiness score, and life points that all change as the simulation advances day by day.

Professions currently supported:

- **Teacher** — steady salary (1200–1300 €/month), happiness grows over time
- **Programmer** — higher salary (2000–5000 €/month), but happiness slowly declines
- **Miner** — solid pay (1000–3000 €/month), but life points decrease each month
- **Unemployed** — no income, both life points and happiness drop each month
- **Student** — no income, happiness declines; the only residents allowed in communal buildings

Each day residents pay rent (on the 1st of the month), buy food (50 €/day), and have their stats updated according to their profession's rules. Citizens who run out of money or food suffer penalties to happiness and life points. Anyone who hits 0 life points is removed from the simulation.

---

## Commands

| Command | What it does |
|---|---|
| `generate <n> <m>` | Creates a new city of size n × m with random buildings and residents |
| `add <n> <m> <name> <job> <happiness> <money> <life>` | Adds a citizen to location (n, m) |
| `remove <n> <m> <name>` | Removes a citizen from location (n, m) |
| `step` | Advances the simulation by one day |
| `step <n>` | Advances (or rewinds, if n is negative) by n days |
| `info` | Prints full structured info for the entire simulation |
| `info <x> <y>` | Shows residents and building info at location (x, y) |
| `info <x> <y> <name>` | Shows detailed history for a specific resident |
| `stat <option>` | Prints statistics for `happiness`, `money`, `life`, or `profession` |
| `stat buildings` | Shows building distribution by type and location |
| `save <name>` | Saves the current simulation state to a file |
| `load <name>` | Loads a previously saved simulation |
| `exit` | Exits the program (prompts to save if there are unsaved changes) |

Long outputs are paginated — the program will ask whether to continue after each screen's worth of content.

---

## Project Structure

```
project/
├── .vscode/    # VS Code build & debug config
├── incl/       # Header files (.h / .hpp)
└── src/        # Source files (.cpp)
```

---

## How to Compile

The project is written in **C++11**. Make sure you have `g++` or `clang++` installed.

### g++
```bash
g++ -I incl src/*.cpp -o main
```

### clang++
```bash
clang++ -I incl src/*.cpp -o main
```

### Run
```bash
./main
```

On Windows:
```bash
.\main.exe
```

---

## Data Persistence

Use `save <name>` to write the simulation to disk and `load <name>` to restore it later. The save format is binary for performance — recommended for large cities. On startup, if a previously active simulation is detected, it will be loaded automatically and validated before you start issuing commands.

---

## Notes

- All inputs are validated — invalid coordinates, negative stats, unknown professions, and duplicate residents at the same location will all produce a helpful error message rather than crashing.
- Stepping backward in time is supported (e.g. `step -5`), but you can't go earlier than the simulation's start date.
- The system is designed to scale, so not all building/resident data needs to be in memory at once — characteristics are stored efficiently across files.
