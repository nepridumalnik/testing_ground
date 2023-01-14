#include <iostream>
#include <functional>
#include <map>

class Signal
{
public:
    Signal() : id_{} {};

    ~Signal()
    {
        for (auto &[key, val] : slots_)
        {
            val.first = nullptr;
        }
    }

    template <typename T>
    int connect(T *obj, void (T::*func)(int))
    {
        slots_.insert({id_, std::make_pair(obj, std::bind(func, obj, std::placeholders::_1))});
        return id_++;
    }

    void disconnect(int id)
    {
        slots_.erase(id);
    }

    void emit(int i)
    {
        for (const auto &slotData : slots_)
        {
            if (slotData.second.first)
            {
                slotData.second.second(i);
            }
        }
    }

private:
    std::map<int, std::pair<void *, std::function<void(int)>>> slots_;
    size_t id_ = 0;
};

class Slot
{
public:
    Slot() : signal_{} {}

    ~Slot()
    {
        signal_->disconnect(id_);
    }

    void setSignal(Signal *signal)
    {
        signal_ = signal;
        id_ = signal->connect(this, &Slot::receive);
    }

    void receive(int i)
    {
        std::cout << "Slot received: " << i << std::endl;
    }

private:
    Signal *signal_;
    size_t id_;
};

int main()
{
    Signal signal;

    {
        Slot slot;
        slot.setSignal(&signal);
        signal.emit(5);
    }

    signal.emit(5);

    return 0;
}
