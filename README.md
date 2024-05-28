<a name="readme-top"></a>
<br />
<h1 align="center">RUSH (Rapid UNIX Shell)</h1>

  <p align="center">
    A simple Linux kernel built in C
  </p>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->
## About The Project
The Rapid Unix SHell (RUSH) is a simple and efficient Unix shell designed to serve as a command line interpreter. When a user inputs a command, rush creates a child process to execute it, then waits for the process to complete before prompting for more input. This loop continues until the user types the built-in command exit. Rush handles commands by searching specified directories, managed through the built-in path command. It supports additional built-in commands like cd and path, output redirection using the > symbol, and parallel command execution using the & operator. Errors are uniformly reported with a specific error message, and the shell ensures robust handling of various whitespace variations in user input.

### Built With
* C



<!-- GETTING STARTED -->
## Getting Started
To start using the Rapid Unix SHell (rush), follow these steps:
1) Download the Code: Clone or download the repository containing the rush source code.
2) Compile the Code: Navigate to the directory containing the code and run the following command in your terminal:
  ```sh
  make
  ```
3) Run the Shell: After compilation, you can start the shell by executing:
   ```sh
   ./rush
   ```
4) Start Using Rush: Once the shell is running, you will see the rush> prompt. You can now enter commands, which rush will execute in its simple, efficient manner.
   
To exit the shell, simply type:
```sh
exit
```
