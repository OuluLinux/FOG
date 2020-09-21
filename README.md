# FOG - Flexible Object Generator #

The Flexible Object Generator is a meta-compiler for a super-set C++ language. FOG offers preprocessing facilities using meta-object concepts that integrate with the C++ language.

FOG renders the C preprocessor obsolete, eliminates substantial lexical redundancy from C++ and generalises C++ so that patterns can be realised by using meta-functions. 

This is the program from [http://www.computing.surrey.ac.uk/research/dsrg/fog/](http://www.computing.surrey.ac.uk/research/dsrg/fog/).


### Compiling FOG ###

#### FreeBSD ####
```
git clone https://github.com/UltimateScript/Fog FOG
sudo pkg install upp
umk FOG/src Fog CLANG ~/fog
~/fog
```

#### Others ####
You need the Ultimate++ to compile it:

1. Clone this repository: `git clone https://github.com/UltimateScript/FOG.git`
2. Download and install U++ from https://www.ultimatepp.org/
3. Open the project in the U++ TheIDE
	1. Create new assembly in the U++ TheIDE startup dialog with the folder C:\FOG\src
	2. At the lower left corner, select "All packages of first nest"
	3. Select Fog, open it, compile it

