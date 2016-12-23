# README #

### What is this repository for? ###

SynAssember is an application to create software syntesizer written in C# for the UI and C++ for the AudioEngine

### How do I get set up? ###

Steps to setup the project:

1. Install VisualStudio 2015 Community
2. Clone SynAssembler repository
3. Open from Visual Studio the solution: AudioEngine\AudioEngine.sln and compile it
4. Open from Visual Studio the solution: SynAssembler\SynAssember.sln and compile it
Now you can run SynAssembler from IDE

### Contribution guidelines ###

* Writing tests
* Code review
* Other guidelines

### Who do I talk to? ###

* For any question contact kernel255@gmail.com

### Appendix ###

Files organization

Some ElaborationUnits requires some files to work like PCMWaveform and MIDISequencer.
These files need to reside in dedicated folders under <main_execultable_folder>\Resources\.
During build this folder is created and files in Waves folder are copied.