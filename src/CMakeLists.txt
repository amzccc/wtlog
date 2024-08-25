cmake_minimum_required(VERSION 3.20)

target_sources(Log_Object PUBLIC
    wtlog.cpp
    LogManager.cpp
    LogScheduler.cpp
    LogBuffer.cpp    
)