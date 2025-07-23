Pickable System - Unreal Engine C++
This project is a fully modular and optimized interaction and item placement system, implemented entirely in C++ within Unreal Engine 5. The main goal is to create a powerful foundation for managing player interactions with world objects, designed to be easily extensible for larger projects.

This system was developed as a test project to solve issues and improve the interaction architecture from the original Evil Chef project.

✨ Key Features
C++ Based Architecture: All core logic is implemented in C++ to provide the highest level of performance and control.

Modular Interaction System: Using base classes (AMasterInteract, AMasterPickable) and an interface (IInteractInterface), adding new types of interactable objects is simple and clean.

Three-Stage Interaction Logic:

Pickup Item: First press of the interact key (E).

Activate Placement Mode: A second press on a valid surface activates a preview mode.

Confirm Placement: A third press confirms and places the item.

Smart Preview System (Ghost Mesh):

A dynamic, translucent preview of the held item is displayed to show the exact placement location.

The preview mesh material dynamically changes to Green (valid location) or Red (invalid location).

Precise Placement with SphereTrace: The system uses SweepSingleByChannel to accurately trace against surfaces and find the optimal placement spot.

Stacking Prevention: The trace logic is designed to prevent placing items on top of other items by checking for a specific placement collision component.

Data-Driven Design: Item properties (like name) are managed through Data Assets to allow for easy modification and addition of new items without changing code.

🚀 How to Use & Setup
Create Child Blueprints: Create new Blueprints inheriting from the following C++ classes:

AMasterInteract -> BP_MasterInteract (for simple interactables)

AMasterPickable -> BP_YourItem (for pickable items like BP_CoffeeCup)

AWorkTable -> BP_WorkTable (for surfaces that can have items placed on them)

Configure the Placement Surface (BP_WorkTable):

Assign a Static Mesh to the TableMesh component.

Select the PlacementCollision component and adjust its size and location to perfectly cover the top surface of the table. This defines the valid placement area.

Assign your materials to the Valid Placement Material (translucent green) and Invalid Placement Material (translucent red) properties.

Tweak the Placement Height Offset variable to fine-tune the item's final height and prevent it from clipping into the surface.

Configure the Pickable Item (BP_YourItem):

Add a StaticMeshComponent to it.

Create a Data Asset (parented from UMasterDataAsset) for the item and assign it to the ItemData property.

Configure the Character:

Ensure your character Blueprint inherits from APickableSystemCharacter.

In the character Blueprint, position the AttachPoint component in front of the camera where you want items to be held.

🏛️ Key C++ Classes
AMasterInteract: The parent class for all interactable objects. Manages the name widget and player overlap detection.

AMasterPickable: Inherits from AMasterInteract and implements the logic for being picked up and attached to the character.

AWorkTable: Inherits from AMasterInteract and manages all the advanced logic for tracing, displaying the Ghost Mesh, and placing an item.

APickableSystemCharacter: The player character class that handles the state management (holding an item, in placement mode) and sends interaction commands.

IInteractInterface: The interface that defines the interaction contract between the character and various objects in the world.
