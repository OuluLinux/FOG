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
You can use U++ TheIDE or just:

```
make
```

##### In Windows using MSYS2 Clang64 #####
- Install msys2 using Getting started tutorial [here](https://www.msys2.org/), but set installation folder to "C:\msys" directory (or modify makefile).
- Open C:\msys\clang64.exe
- ```pacman -S --needed base-devel mingw-w64-clang-x86_64-toolchain```
- ```cd /c/git/FOG``` or wherever you cloned the repository.
- ```make```
- Copy manually ```libc++.dll``` and ```libunwind.dll``` from ```C:\msys\clang64\bin``` to the bin directory.
