#ifndef TASK_H
#define TASK_H

#include <QtConcurrentRun>
#include <QTimer>

template <typename T>
concept classTypename = std::is_class_v<T>;

template <typename RetType, typename... Args>
class Task
{
public:
    explicit Task(std::function<RetType(Args...)> fn)
    :m_task(fn){}
    explicit Task(RetType(*fn)(Args...))
    :m_task(fn){}
    template<classTypename Class>
    Task(Class *instance, RetType (Class::*fn)(Args...))
    { m_task = [instance, fn](Args... args)-> RetType { return (instance->*fn)(std::forward<Args>(args)...); };}
    template<classTypename Class>
    Task(Class *instance, RetType (Class::*fn)(Args...) const)
    { m_task = [instance, fn](Args... args)-> RetType { return (instance->*fn)(std::forward<Args>(args)...); };}

    [[nodiscard]]
    bool Is_EverCalled() const { return mb_called; }
    QFuture<RetType> AsyncCall(Args&&... args);
    QFuture<RetType> operator()(Args&&... args);
    RetType Call(Args&&... args);

private:
    std::function<RetType(Args...)> m_task;
    bool mb_called = false;
};

template <typename RetType, typename ... Args>
QFuture<RetType> Task<RetType, Args...>::AsyncCall(Args&&... args)
{
    mb_called = true;
    return QtConcurrent::run(m_task, std::forward<Args>(args)...);
}

template <typename RetType, typename ... Args>
QFuture<RetType> Task<RetType, Args...>::operator()(Args&&... args)
{
    return this->AsyncCall(std::forward<Args>(args)...);
}

template <typename RetType, typename ... Args>
RetType Task<RetType, Args...>::Call(Args&&... args)
{
    mb_called = true;
    return m_task(std::forward<Args>(args)...);
}

#endif // TASK_H
