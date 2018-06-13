/**
 * ThickMazeTypeclasses.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <sstream>
#include <string>

#include "typeclasses/Show.h"
#include "ThickMazeAttributes.h"
#include "ThickMaze.h"
#include "StringThickMazeRenderer.h"

namespace spelunker::typeclasses {
    template<>
    struct Show<thickmaze::ThickMaze> {
        static std::string show(const thickmaze::ThickMaze &tm) {
            std::ostringstream out;
            thickmaze::StringThickMazeRenderer r(out);
            r.render(tm);
            return out.str();
        }
    };
}