/**
 * Observable.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Largely taken from:
 * https://thehermeticvault.com/software-development/making-boost-signals2-more-oop-friendly
 */

#pragma once

#include <boost/signals2.hpp>

namespace spelunker::types {
    /**
     * The superclass for something that can be observed. It must be accompanied by an Observers type.
     *
     * The SignalDescriptors parameter describes what can be observed. It must consist of:
     *   1. An enum of the IDs of signals available to be omitted; and
     *   2. The SignalTable, which is an std::tuple of Observer (with parameter the signature of the signal),
     *      each corresponding to a signal ID in the enum.
     * The order / value of the enums comprise lookups into the SignalTable, e.g. if we have WindowShowing with
     * value 0 in our enum, then the entry at position 0in the std::tuple SignalTable must correspond to this signal.
     *
     * For an example, @see{maze::MazeGeneratorSignalDescriptors}.
     * @tparam SignalDescriptors the description of what can be observed, as above
     */
    template<typename SignalDescriptors>
    class Observable {
    private:
        using SignalTable = typename SignalDescriptors::SignalTable;

    public:
        /**
         * Register an observer with this Observable. The Boost.Signals2 connection object is returned, which can
         * be used to disconnect.
         * @tparam signalId the ID of the signal as per the enum described in the class description (e.g. WindowShowing)
         * @tparam F the signature of the slot to connect
         * @param f the slot to connect, which will receive the signal notification
         * @return the connection object representing the connection
         */
        template<size_t SignalId, typename F>
        boost::signals2::connection
        connect(F &&f) {
            // Get the signal from the std::tuple corresponding to the signal type in position signalId.
            // The actual management of multiple connections is handled by Boost.Signals2.
            return std::get<SignalId>(signals).signal.connect(std::forward<F>(f));
        }

    protected:
        Observable() = default;

        /**
         * Emit a signal notifying registered observers.
         * Note that std::tuple_element provides compile-time indexed access to the types of the elements of the tuple,
         *   to indicate the return type of the call to notify should be, i.e. as SignalResult based on the Signal.
         * @tparam signalId the ID of the signal as per the enum described in the class description (e.g. WindowShowing)
         * @tparam Args types of arguments to pass along with the signal
         * @param args actual arguments to pass along with the signal
         * @return an optional of what was returned by the most recently registered Observer, if any exist
         */
        template<size_t SignalId, typename... Args>
        typename std::tuple_element<SignalId, SignalTable>::type::SignalResult
        notify(Args &&... args) const {
            return std::get<SignalId>(signals).signal(std::forward<Args>(args)...);
        }

    private:
        SignalTable signals;
    };
}