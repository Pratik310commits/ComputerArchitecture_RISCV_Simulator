#include <iostream>
#include <string>
#include <vector>
#include "system.h"

int main() {
    System mySystem;
    SystemConfig config;
    config.name = "My Config";
    config._cpuConfg.name = "CPU0";
    config._cpuConfg.programSizeInBytes = 148;
    config._cpuConfg.stackMemorySizeInBytes = 256;
    config._mainMemoryConfg.name = "System RAM";
    config._mainMemoryConfg.sizeInBytes = 5120;
    config._mainMemoryConfg.programSpaceInBytes = 512;
    config._mainMemoryConfg.programStartPosition = 0;
    config._mainMemoryConfg.bufferAfterStack = 256;
    mySystem.configureSystem(config);
    mySystem.run();
    return 0;
}



