[//]: # (/******************************************************************************)
[//]: # (filename:	README.md)
[//]: # (author:	Jolyn Wong Kaiyi, wong.k@digipen.edu)
[//]: # (Project:	CS396 Assignment 01)
[//]: # (Description:)
[//]: # (	Readme file for instruction on how to build the project)
[//]: # (******************************************************************************/)

# CS396 Building a ECS
ECS built for Assignment 01 of CS396, Digipen SG.

## To build
1. Go to the build directory 
2. Click the generate_project.bat batch file
3. Wait untill it says that you can press any key to continue
4. Go to project directory in same directory
5. There should be a solution and project generated, double click to open it and
   build the project. The project should be compilable and able to run.
   
## To use
There is an "Example.cpp" file under the project folder. There are example cases
of how to use the ECS in that file.

## Dependencies
- [freeglut](https://github.com/FreeGLUTProject/freeglut.git)

### Note
.lib and .dll files of freeglut are included due to freeglut being built using
cmake. There is no other ways to provide computers with no cmake with the 
needed files.
