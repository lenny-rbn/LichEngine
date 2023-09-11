<html>
<link href="style.css" rel="stylesheet">

<h1 align="center"> Lich Engine Project</h1>

https://youtu.be/pDCPkfVftnc

<h2> Introduction : </h2>
The Lich Engine is game engine project couple with a engine editor. This project has been made in four months.The engine is conceive to be use on Window and use OpenGL 4.5 has graphics library. The main goal of this project was to create an engine for the creation of horror games. To fulfill this goal, we concentrate on the renderer feature to give lot of tools to create immersive environement. The groups member are :
<div class= name>
<b>
<ul>
    <li> Matéo Erbisti
    <li> Lenny Rabinne
    <li> Antoine Mordant
    <li> Bryan Bachelet
</ul>
</b>
To launch the project on Visual Studio move every file of the "Builds" folder in the LichEngine/x64/Debug folder.
</div>
<hr>
<h2> Engine Features : </h2>

<h3><b> - Render :</b></h3> 
Our engine has a lot of render features in order to facilitate the creation of amazing environment for the game created in the engine. The main feature is the Physically Based Rendering that allow our engine to have a realistic look. The second important render feature is the Deferred Rendering, that allow to have lot of lights in ours scenes. We have also others effect like Shadow Mapping, SSAO or Normal Mapping.

<p>
<h3><b> - Physics :</b></h3>
 Our engine has realistic physics available for the user. For this feature, we use the PhysX library from Nvidia that compute all calculs of the physics. Our engine can be link to Nvidia Physics Debugger software in order to help users when they work on physics problems and need to debug. 
 <p>
<div class="Part"> 
    <div class = "One">
        <h3><b> - Animation :</b></h3>
        Our engine can display classic animation by using skeleton and animation file. An object can have multiples animations and transition from one to another smoothly.
        <p>
    </div>
    <div class = "Two">
        <h3><b> - Scripting :</b></h3>
        The Lich engine allows users to create there's own script by using the reflection library of the engine. Scripts are in C++. There also components allowing them to show variables in editor's inspector.  
        <p>
    </div>
</div>
<h3><b> - UI :</b></h3>
The Lich Engine have basics UI display, we can show Text on screen and Image. All UI elements are limited to the screen to be positioned. For the text element, user can use multiples fonts and change theirs size. The image extension handle are .png and .jpg. 
 <p>
 <h2> Libraries Uses : </h2>
 For this project, we use multiple libraries to handle functionnalities. Let's list them :
 <ul>
    <li> <b>OpenGL 4.5 </b> - This is the graphics API, we use in our engine
    <li> <b>GLFW </b> - For the software window
    <li> <b>PhysX</b> - That the library that manage our engine physicx. This library is from Nvidia
    <li> <b>FreeType</b> - This library is use to handle font file and show text on screen.
    <li> <b>RapidObj & Assimp </b> - These libraries manage the loading of game models in our engine
    <li> <b>STBImage</b> - Load image and texture
    <li> <b>Refureku</b> - Library that allow to the reflection on our engine
    <li> <b>ImGui</b> - Editor UI interface library
</ul>
