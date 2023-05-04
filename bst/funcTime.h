#pragma once

#include <chrono>
#include <iostream>

// 计算函数运行时间
class FuncTime {
    std::chrono::steady_clock::time_point start, end;

  public:
    FuncTime(){};

    void funcStart() {
        start = std::chrono::steady_clock::now();
    }

    void funcEnd() {
        end = std::chrono::steady_clock::now();
    }

    double funcRunTime() {
        return std::chrono::duration<double, std::milli>(end - start).count();
    }

    void funRunTimeStr() {
        double t = funcRunTime();
        std::cout << "运行时间: " << t << "ms" << std::endl;
    }
};