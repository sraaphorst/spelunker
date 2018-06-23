/**
 * Observer.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Largely taken from:
 * https://thehermeticvault.com/software-development/making-boost-signals2-more-oop-friendly
 */

#pragma once

#include <boost/signals2.hpp>

#include "Observable.h"

namespace spelunker::types {
    /**
     * A convenience wrapper for boost::signal2::signal. Used by classes interested in observing an Observable
     * to register a slot. For example, if the Observable had a WindowShowing signal, we could include an interested
     * class by invoking:
     *   observable.connect<SignalDescriptor::signalID>(function - likely lambda - matching the signal signature).
     * @tparam Signature the signature of the signal this class is managing
     */
    template<typename Signature>
    class Observer {
    public:
        Observer(const Observer &) = delete;

        Observer &operator=(const Observer &) = delete;

        Observer() = default;

    private:
        template<typename SignalDescriptors> friend
        class Observable;

        using Signal = boost::signals2::signal<Signature>;
        using SignalResult = typename Signal::result_type;

        // The boost::signals2::signal wrapped up in this observer.
        Signal signal;
    };
}