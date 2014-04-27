#ifndef CONCATENATED_STREAM_PROVIDER_H
#define CONCATENATED_STREAM_PROVIDER_H

#include "StreamProvider.h"

#include <list>

template<typename T>
class ConcatenatedStreamProvider : public StreamProvider<T> {

public:
    template<typename Iterator>
    ConcatenatedStreamProvider(Iterator begin, Iterator end)
        : source_(begin, end) {}

    ConcatenatedStreamProvider(StreamProviderPtr<T> first,
                               StreamProviderPtr<T> second) {
        source_.push_back(std::move(first));
        source_.push_back(std::move(second));
    }

    std::shared_ptr<T> get() override {
        return current_;
    }

    bool advance() override {
        while(!source_.empty()) {
            auto& provider = source_.front();
            if(provider->advance()) {
                current_ = provider->get();
                return true;
            }
            source_.pop_front();
        }
        current_.reset();
        return false;
    }

private:
    std::list<StreamProviderPtr<T>> source_;
    std::shared_ptr<T> current_;

};

#endif
