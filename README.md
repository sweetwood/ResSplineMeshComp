# Resizable Spline Mesh Component (RSplineMeshComp)

This component use a "brute force" method to create a Spline Mesh at runtime.

Source code of the component is in Components/RSplineMeshComponent.cpp/h

![alt text](https://user-images.githubusercontent.com/484208/27290853-c52c2a38-550e-11e7-94ee-2bcbe8af6b7f.gif)

The component have two CreateSplineMesh functions, one that use vectors for start and end positions, the second instead use actors.

![alt text](https://user-images.githubusercontent.com/484208/27292751-b28b46e2-5514-11e7-8a39-116fa7f7907f.PNG)

The ChunkLength parameter is the distance between two spline points, that means the sub-SplineMeshComponent will have that length.

![alt text](https://user-images.githubusercontent.com/484208/27292765-b8548052-5514-11e7-8960-000a4952ffe2.png)

The ZOffset parameter will rise up/down the Spline on Z axis

![alt text](https://user-images.githubusercontent.com/484208/27292839-e04bd114-5514-11e7-805e-3393c59330b3.png)

The IncludeLastChunk boolean will allow to add (or not) a SplineMeshComponent at the end of the Spline, this is useful in case you have some stuff attached at the end position and you don't want to overlap the SplineMesh with it (like the red circle in the first picture).

![alt text](https://user-images.githubusercontent.com/484208/27292778-bd055c48-5514-11e7-9938-e2a4cf629236.png)

![alt text](https://user-images.githubusercontent.com/484208/27292791-c32e02aa-5514-11e7-954d-cbd9bf6b1645.png)

Here an example use of the component, as you can see the CreateSplineMeshV function is called on Tick, because when you call it the function destroy and recreate the Spline it looks like we are adding/removing chunks at runtime, BE CAREFUL and DON'T use a low ChunkLength value to prevent performance issue! With a value off 50.0f we have a god effect with no fps drops.

![alt text](https://user-images.githubusercontent.com/484208/27293255-26b3be54-5516-11e7-8fca-e5ad187dc267.PNG)


