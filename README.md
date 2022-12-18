# VR Game with STEAM perspective

A VR game that is made to empower females into the thinking of STEAM by a gamification experience.

STEAM: Science, Technology, Engineering, Artistic, Mathematics
Developed with Unreal Engine 5

## Table Of Content
- [The Idea](#the-idea)
- [Key Bindings](#keybindings)
  - [Gameplay Bindings](#gameplay-bindings)
  - [MainMenu Bindings](#mainmenu-bindings)
- [How to contribute](#how-to-contribute)
- [Deployment](#deployment)
- [Assets Used](#assets-used)
- [Technical Document](technical-document.docx)
- [Post mortem](post-mortem.docx)

## The Idea
Our general idea is an Sandbox city game where you control the world in VR. The world is a big grid where you can place buildings and solve problems for a specific part of STEAM. The world is living and you as the gamer has both the power to build but also go down to the "Ground" and walk around (With teleportation or similar movement) to explore the world you have created. You can see how npcs reacts to your creations and use this information to care about the persons! The game style is low poly and is created to learn the aspects of STEAM.

## KeyBindings

### Gameplay Bindings
| Key | Used | Description |
|-----|------|---------------|
|  X  | YES  | Dialog        |
|  Y  | NO   |           |
|  A  | YES  | Switch View   |
|  B  | YES  | Pause Menu    |
| Left JoyStick | YES |Look around (Snap turn, 22.5 deg) |
| Right JoyStick | YES |Walk Around |
| Left Grab | NO | |
| Right Grab | NO | |
| Left Trigger | YES | Click Ingame Menu |
| Right Trigger | YES | Click Ingame Menu, Place houses |

### MainMenu Bindings
| Key | Used | Description |
|-----|------|---------------|
|  X  | NO  |         |
|  Y  | NO   |           |
|  A  | NO  |  |
|  B  | NO  |     |
| Left JoyStick | NO | |
| Right JoyStick | NO | |
| Left Grab | NO | |
| Right Grab | NO | |
| Left Trigger | YES | Click Menu |
| Right Trigger | YES | Click Menu |

## How to contribute
To contribute to the system you will need the following: 
1. An Oculus Quest device
2. Unreal Engine 5.0.3 installed
3. MetaXR plugin for Unreal Engine 5
4. Android Studio 4 installed
5. Install the Unreal Engine 5 NDK for Android
6. Download the project and set it up

## Deployment
When you want to deploy the project you can do the following:

- Use Oculus Link or AirLink.  
This uses the Oculus Link cable or Airlink to use the computers power to run the project. This is used for a fast and easy debugging process but be aware that this is using the computers processing power to run the project.
- Use Unreal Engine 5 build system  
Unreal Engine has a building system that finds your device if it is connected to the computer and then releases it to the device directly. This is good to see the efficiency on the Oculus device
- Use the Unreal Engine 5 project package system for android and run the install file from that. 

## Assets Used
We have only used an low poly asset called "Assetville Town" with the Unreal Engine 5 VR Template.
