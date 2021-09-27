The System-Monitor program
<image>
This is a relatively simply System Monitor Program created as an examination assignment  in the Object Oriented Programming Course of the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). 


The program works from the Linux terminal.

It might need some refinements and further development, but it was made for educational purposes mainly, and as for now the further development is witheld, as spend most of my time in continuous programming learning curve.


![System Monitor](images/System_monitor.jpg)

## ncurses
[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output.

For running it might be necessary to install ncurses within your own Linux environment: `sudo apt install libncurses5-dev libncursesw5-dev` (although is highly likely that it is already installed on your Linux machine).

## Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## Instructions

1. Clone the project repository: `git clone https://github.com/udacity/CppND-System-Monitor-Project-Updated.git`

2. Build the project: `make build`

3. Run the resulting executable: `./build/monitor`
![Starting System Monitor](images/starting_monitor.png)

4. Follow along with the lesson.

5. Implement the `System`, `Process`, and `Processor` classes, as well as functions within the `LinuxParser` namespace.

6. Submit!