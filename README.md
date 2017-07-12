
# MTL/LTL C++ Implementation
---
## Structure Example
![Alt text](/README/structure.bmp?raw=true "Object Connection Structure")
---
## Description :
### Configuration: 
#### In main file "MTL.cpp":
#### 1. Assign the sensor number, simulation time stamps;
#### 2. Link the sensor to the .log file;
#### 3. Write your MTL formula as a string. For MTL format, see Notes below;
### How to run:
#### 1. run setup.sh to compile the project
#### 2. type ./Debug/MTL to running MTL

---
## Notes:
### Meaning the MTL String:
### Relationship with TACAS14--->
#### Algorithm 1: "!" = "NOT{}"
#### Algorithm 2: "G[2]" = "KEP[2]{}"
#### Algorithm 3: "&" = "AND{,}"
#### Algorithm 4: "G[2,5]" = "ALW[2,5]{}"
#### Algorithm 5: "F[1,2]" = "UNT[1,2]{,}"
