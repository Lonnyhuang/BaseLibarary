#ifndef GLOBAL_H
#define GLOBAL_H
#include <vector>
#include <memory>
#include <functional>
namespace HCommon {

template<typename T>
constexpr inline const T &qMin(const T &a, const T &b) { return (a < b) ? a : b; }
template<typename T>
constexpr inline const T &qMax(const T &a, const T &b) { return (a < b) ? b : a; }

template<typename T>
inline T* qGetPtrHelper(T *ptr) { return ptr; }
template<typename Ptr>
inline auto qGetPtrHelper(Ptr &ptr) -> decltype(ptr.get()) { return ptr.get(); }

#define DECLARE_PRIVATE(Class) \
    inline Class##Private * d_func(){ \
        return reinterpret_cast<Class##Private *>(qGetPtrHelper(d_ptr)); \
    } \
    inline const Class##Private * d_func() const { \
        return reinterpret_cast<Class##Private *>(qGetPtrHelper(d_ptr)); \
    } \
    friend class Class##Private;

#define DISABLE_COPY(Class) \
    Class(const Class &) = delete; \
    Class &operator=(const Class &) = delete;

#define D_PTR(Class) Class##Private *const d = d_func();
#define SCHED_IDLE 5

#define LOGGERINIT(confFile, argc, argv) \
    el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);\
    el::Configurations conf(confFile);\
    el::Loggers::reconfigureAllLoggers(conf);\
    START_EASYLOGGINGPP(argc, argv);

#ifdef SIGNAL_SLOT
namespace HCommon {

#define emit
#define slots
#define signals public
#define connect(sender, signal, slot) ((sender)->signal.bind(slot))

template<typename... Args>
class Slot
{
public:
    using OnFunc = std::function<void(Args&&...)>;

    Slot(const OnFunc& func)
        : m_func(func)
    {
        // Do nothing
    }

    void exec(Args&&... args)
    {
        m_func(std::forward<Args>(args)...);
    }

private:
    OnFunc m_func = nullptr;
};

template<typename... Args>
class Signal
{
public:
    using SlotPtr = std::shared_ptr<Slot<Args&&...>>;
    using OnFunc = std::function<void(Args&&...)>;

    void bind(const OnFunc& func)
    {
        m_slotVec.push_back(SlotPtr(new Slot<Args&&...>(func)));
    }

    void operator()(Args&&... args)
    {
        for (auto& iter : m_slotVec)
        {
            iter->exec(std::forward<Args>(args)...);
        }
    }

private:
    std::vector<SlotPtr> m_slotVec;
};

#endif
}
#endif // GLOBAL_H
