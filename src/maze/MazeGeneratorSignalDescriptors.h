/**
 * MazeGeneratorSignalDescriptors.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <tuple>

#include <types/CommonMazeAttributes.h>
#include <types/Observer.h>

namespace spelunker::maze {
    // TODO:PLACEHOLDER
    struct MazeGeneratorSignalDescriptors {
        enum { PassageCarved, CellAdded };
        using SignalTable = std::tuple<
            types::Observer<void(const Cell &c1, const Cell &c2)>,
            types::Observer<void(const Cell &c)>
        >;
    };
}
