[//]: # (/******************************************************************************)
[//]: # (filename:	README.md)
[//]: # (author:	   Jolyn Wong Kaiyi, wong.k@digipen.edu)
[//]: # (Project:	   CS396 Assignment 01)
[//]: # (Description:)
[//]: # (	         Readme file for instruction on how to build the project)
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
- [freeglut_lib_bin](https://github.com/hotaru08/freeglut_lib_bin.git) (Contains .lib and .dll files)


## Things to notes
Most of the ECS is following the same principles that xecs follows. 
Some differences to note would be:

1) Archetype is not exposed to the user of the ecs
   This is because as a user, they may not be familiar which how a ecs works
   in genernal. Hence, the idea is to provide a more user friendly interface
   for the user to use. Eg. they only need to know how to create, delete, find 
   for entities.

   This interface is wrapped as ECSManager, and is provided to the user via the 
   game instance class in Example.cpp.


2) Fixed bug where the bits comparision returns the wrong state.
   In xecs, due to this the archetype is always created instead of getting an 
   existing archetype.

3) (Attempted)

   There were thoughts to replace the array in the Pool to a vector, to dynamically expand the pool size based on the archetype needs to save space.
   Some archetypes do not require 32 components or even it might require more than 32. But after much consideration, 
   speed is what I want this ECS for and used an array so that the data can be more packed to each other. This reduced 
   the chance of cache misses, as vector contains a pointer to pointer to the exact location and array just have the data at that place.

4) EntityManager was created to manage the entity and its archetypes instead so the creation process is different,
   where xecs requires the user to getArchetype to create entities in the archetype, my ECS only require the user 
   to create entity either by passing in the components they want or through a function.

   Some functions names are changed to be more direct in their meaning.
   eg. getOrCreateArchetype -> Retrieved as it does not matter at a glance what happens under the hood

5) ForEach takes query directly instead of needed to call search on the query.
   This follows up what was mentioned earlier about the archetype being hidden
   from the user. So instead of eg. Search(Query), the query is taken in directly
   which under the hood would handle the search for archetype for the user.