# **SS Projekat - Asembler**

## Struktura Projekta(Za Windows):
---------------------------------
*  **ROOT_FOLDER**
    * **Asembler**
        * **reflex**      : biblioteka od koje zavisimo
            * **bin**     : sadrzi exe fajl koji generise lexer.cpp/lexer.h fajlove
            * **include** : _sadrzi header fajlove koji su potrebni za lexercpp
            * **lib**     : sadrzi source fajlove koji su potrebni za lexer.cpp
            * **unicode** : sadrzi source fajlove koji su potrebni za lexer.cpp
        * **spec**        : sadrzi opis lexer generatora
        * **src**         : sadrzi source fajlove projekta
        * **h**           : sadrzi header fajlove projekta

## Konfigurisanje projekta
--------------------------

### **_Konfigurisanje projekta unutar Visual Studio od nule_**:

1. Desni klik na **Solution**(Asembler) i odaberi opciju Properties
2. Pri vrhu u opciji **Configurations** selektovati _All Configurations_ i u opciji **Platform** selektovati _All Platforms_
3. Odabrati **C/C++**->**General** i kliknuti na **Additional Include Directories**. Tu dodati ove dve putanje:
    * $(ProjectDir)h
    * $(ProjectDir)reflex\include
4. Dodati **sve** *.cpp fajlove iz foldera _src_, _reflex\lib_ i _reflex\unicode_ u projekat
5. Odabrati **Pre-Build Event** i u opciji **Command Line** dodati sledecu liniju:
    * ```$(ProjectDir)reflex\bin\win64\reflex.exe -o $(ProjectDir)src\lexer.cpp --header-file=$(ProjectDir)h\lexer.h $(ProjectDir)spec\test.l```
6. Pritisnuti OK i to je to :) 



### **_Konfigurisanje projekta za Linux_**

1. Kopirati ceo **_root_** folder
2. Otvoriti terminal i pozicionirati se da bude u **_root_** folderu
3. Pokrenuti komandu *__make__*


## Pokretanje programa

Program se pokrece tako sto se navede izlazni fajl i ulazni fajl.

**Primer:** ````assembler -o output.txt input.s```