# FOG - Flexible Object Generator #

The Flexible Object Generator is a meta-compiler for a super-set C++ language. FOG offers preprocessing facilities using meta-object concepts that integrate with the C++ language.

FOG renders the C preprocessor obsolete, eliminates substantial lexical redundancy from C++ and generalises C++ so that patterns can be realised by using meta-functions. 

This is the program from [http://www.computing.surrey.ac.uk/research/dsrg/fog/](http://www.computing.surrey.ac.uk/research/dsrg/fog/).

### Example ###
If you like C++, then you **really** should read the [Meta-Compilation for C++](https://github.com/UltimateScript/FOG/blob/master/doc/FogThesis.pdf).
The FOG is much more complex language than C++ and it requires a bit of boring reading of a scientific paper.

Compiling following with FOG
```
auto declaration BuildInterface() {
        auto for (iterator k = $variables(); k; ++k) {
                !inline $k->derive_type() get_${k->name()} () {return $k->name();}
                !inline void set_${k->name()} (const $k->derive_type()& v) {$k->name() = v;}
        }
}

struct Person {
        int age;
        double height;

        $BuildInterface();

};
```
produces header:
```
struct Person {
    int age;
    double height;
    
    int get_age();
    double get_height();
    void set_age(const int& v);
    void set_height(const double& v);
};
```
and implementation:
```
int Person::get_age() {
    return age;
};

double Person::get_height() {
    return height;
};

void Person::set_age(const int& v) {
    age = v;
};

void Person::set_height(const double& v) {
    height = v;
};
```

### Getting started ###
Before starting, you should learn how to compile programs with the [Ultimate++ IDE](https://www.ultimatepp.org/). Use their documentation.

![Debugging the FOG application in TheIDE in Ubuntu Linux](https://raw.githubusercontent.com/UltimateScript/FOG/master/doc/fog_ide.jpg)

Compile the latest fog executable with the IDE and add it's directory to your PATH environment variable.
Then create new assembly in the IDE's startup dialog. Set the directory of the assembly as the example directory.
Open the Main project. Compile and run it by pressing F5 key.

### Compiling FOG ###

#### FreeBSD ####
```
git clone https://github.com/UltimateScript/Fog FOG
sudo pkg install upp
umk FOG/src Fog CLANG +CLANG ~/fog
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

