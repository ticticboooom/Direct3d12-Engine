# How to Contribute effectively and efficently
The below areas will be covered:
- [Reporting Bugs / Issues](#Reporting-Bugs-/-Issues)
- [Requesting to Add Functionality](#Requesting-to-Add-Functionality)
- [Communicating](#Communicating)

## Reporting Bugs / Issues
Be sure to create **1 bug per issue**, this means that if you have discovered multiple bugs in the code you should make them all into seperate issues in GitHub.

#### Look with your eyes
Before adding any issue you **must first look in the issues database** to check if there is a matching issue. 
This will speed up fix time and will make sure we have no angry developers. 
It also ensures that there is no confusion with whether a bug / feature has already been fixed, still exists or is just a copy of an old one.

#### Send over the deets
Whenever raising an issue, make sure you give the required information so that a developer can fix it as soon as possible without having to do hours of testing just to discover that you were an old version (or something like that).
These include:
- GPU Model (and the corresponding driver if you know it)
- Engine version (the one you are using)
- Issue description (in as much detail as is physically possible)
- Possible reproduction steps (how you experienced the issue in the first place)
- Expected Result (what should happen)
- Actual result (what actually happened)
- Any error messages or warnings that were recieved

You do not have to include the operating system as it is assumed you are using Windows 10, as this is the only platform that This engine will run on will run on.

## Adding Functionality
If you want to add a new ability / functionality to the engine you must follow these steps:
- Make sure the new functionality is actaully wanted
- Make sure you have agreed with other developers what is the best way to impliment it

#### Stability
Ensure that any commits made to the repository are stable, for example, only commit your work after it has been tested and runs smoothly.

#### Make them Readable
Ensure that every commit message is fully readable and consists of fully explained fixes and / or fullt described features.
Do not write simple messages like:
```
I made a change, and it fixed things
```
They should be more along the lines of:
```
When using the sausages component, it would move the object,
the component was attatched to, to the left not the right (like the sausage component should do)

The sausage component has had the "Move Like a sausage" method fixed with some tweaking 
to the vector addition section.
```
Obviously you would change the sausage scenario to the issue or feature you are fixing or adding.

#### Documentation IMPORTANT
Everything that is added (code wise) ****must**** be thourouly documented as this repository is for helping learn Direct3D 12 along with producing an Engine off the back of it.
Code should be documented with these rules:
- Class and constructor must have a description of the object and what it does (in detail explaining what it does and how it does it)
- **Every** method / function must be fully described and comments added inside the code (relatively high level) to explain what the code is doing
- And remember you are helping the future game developers. Documentation is important to learn and understand.


## Communicating
There is a developer discord chat which you can use to send messages to everyone. It is located here:

