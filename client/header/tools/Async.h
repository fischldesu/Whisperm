#ifndef ASYNC_H
#define ASYNC_H

#include <QtConcurrentRun>
#include <QTimer>

template <typename T>
concept classTypename = std::is_class_v<T>;

template <typename RetType, classTypename Class, typename... Args>
class AsyncTask
{
public:
    AsyncTask(Class *instance, RetType (Class::*fn)(Args...))
    { m_task = [instance, fn](Args... args)-> RetType { return (instance->*fn)(std::forward<Args>(args)...); };}

    AsyncTask(Class *instance, RetType (Class::*fn)(Args...) const)
    { m_task = [instance, fn](Args... args)-> RetType { return (instance->*fn)(std::forward<Args>(args)...); };}

    ~AsyncTask()
    { if (!m_future.isFinished()) m_future.waitForFinished();}

    void Async_Run(Args&&... args, uint delayMsec = 0)
    {
        if (delayMsec > 0)
            return QTimer::singleShot(delayMsec, [this, args...]{ this->Start(std::forward<Args>(args)...); });
        this->Start(std::forward<Args>(args)...);
    }

    [[nodiscard("Promise::run returns void")]]
    QFuture<RetType> then(Args&&... args)
    { this->Async_Run(std::forward<Args>(args)...); return m_future; }

private:
    void Start(Args&&... args)
    { m_future = QtConcurrent::run(m_task, std::forward<Args>(args)...); m_futureWatcher.setFuture(m_future); }

    std::function<RetType(Args...)> m_task;
    QFuture<RetType> m_future;
    QFutureWatcher<RetType> m_futureWatcher;
};


//AsyncTask factory
template <typename RetType, classTypename Class, typename... Args>
std::unique_ptr<AsyncTask<RetType, Class, Args...>> MakeAsyncTask(Class *instance, RetType (Class::*fn)(Args...))
{ return std::make_unique<AsyncTask<RetType, Class, Args...>>(instance, fn); }
template <typename RetType, classTypename Class, typename... Args>
std::unique_ptr<AsyncTask<RetType, Class, Args...>> MakeAsyncTask(Class *instance, RetType (Class::*fn)(Args...) const)
{ return std::make_unique<AsyncTask<RetType, Class, Args...>>(instance, fn); }

#endif // ASYNC_H
