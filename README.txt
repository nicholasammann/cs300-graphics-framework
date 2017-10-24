/* -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: README.txt
Purpose: ReadMe file for CS300 Assignment 2
Language: English
Platform: Most likely cross platform.
Project: nicholas.ammann_CS300_2
Author: Nicholas Ammann, nicholas.ammann, 180002915
Creation date: 10/24/17
--------------------------------------------------------*/



1) User Interface Instructions:

	Under the object header (ex. Cube): 
		Under Shader:
			Select the shader being used on the object from the drop down.
	
		Under Textures:
			Select the texture mapping being used from the drop down.
			Toggle Display Textures to turn texturing on and off.
			
		Under Material:
			Change material properties.
			
	
	Under Lights:
		Under Constants:
			Constant values are under the appropriate headers.
			
		Under Directional Lights:
			If there are directional lights in the scene, they will display here.
		
		Under Spot Lights:
			If there are spot lights in the scene, they will display here.
		
		Under Point Lights:
			If there are point lights in the scene, they will display here.
		
		
	Under Scene:
		
		Change the current scene in the drop down "Current".
		
		Toggle camera rotation with the checkbox "Toggle Camera Rotation".
		
		Toggle light rotation with the checkbox "Toggle Light Rotation".
		
		Change the type of lights being used in the drop down "Light Type". 
		(Only applies to Scene 1 b/c Scene 2 is mixed, and Scene 3 is custom.
		
		Change the number of the lights being used in the scene using the slider "Light Count".
		If you decrease then increase the number, the lights added will be default pink lights.
		Switch scenes to reset to the original configuration.
		
	
	Under Actions:
	
		Enter the name of an obj file into the text box, then click Load Model.
		(ex. cube.obj, teapot.obj, sphere.obj, etc.)  If the text box is empty or
		contains an incorrect file name, the program will crash when the button is
		pressed.
		
		Click Reload Shaders to reload all shaders from file.
		
		Click Clear Console to clear the debug console of text.
		
	
		
2) Critical Errors:

	If the text box for loading models is empty or contains an incorrect file name,
	the program will crash.

	
3) Completed Parts of Assignment:
	
	All required parts of the assignment have been completed.
	The extra credits (rotating camera) has also been completed.

	
4) Missing Parts of Assignment:
	
	To the best of my knowledge, there are no missing parts of the assignment.

	
5) Source Code Location: Specify file path, file name, function name

	All relevant code is under OpenGLFramework/
	
	Scene Setup:  Source/Core/Scene.hpp  -  One function to create lights for each scene.
	
	Phong Model Terms: 
		- Assets/Shaders/phong_lighting.vert
		- Assets/Shaders/phong_shading.frag
		- Assets/Shaders/blinn.frag
		
		Separate functions for computing color for each light type.
		Function for computing fog attenuation.

	Material / Texture Support:  Source/Graphics/Texture.hpp, Source/Graphics/Texture.cpp
		
		Texture loaded from file and generated in the constructor.
		
		Texture bound in the Bind function.
		Sampler2D uniform set by calling SetUniform and passing in the sampler2D name and slot.
		Texture unbound in the Unbind function.
		
		
	Material Emissive Component:
	
		- Assets/Shaders/phong_lighting.vert
		- Assets/Shaders/phong_shading.frag
		- Assets/Shaders/blinn.frag
		
		Emissive added to color in the main function of each shader.
		

6) Test Machine:

	DigiPen desktop in Edison, Seat Y-10.


7) Time Spent on Assignment: 75 hours

	I built my own framework after turning in assignment 1, and I had no real experience 
	with OpenGL or any other graphics API before building this framework, so I had to learn OpenGL.


8) Build Information:

	Make sure to build as 32 bit, not 64 bit!




























