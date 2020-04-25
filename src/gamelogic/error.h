#pragma once

#include <string>

struct InvalidPlayError {
    std::string error;
};

struct SetupError {
    std::string error;
};

struct InternalError {
    std::string error;
};
