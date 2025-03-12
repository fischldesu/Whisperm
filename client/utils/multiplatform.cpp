#include "tools/multiplatform.hpp"
#include "ui/Window.h"

#if defined(Q_OS_WINDOWS) && !defined(_MSC_VER)
HMODULE dwmapi = LoadLibrary(L"dwmapi.dll");
const PDWMFN DwmExtendFrameIntoClientArea = reinterpret_cast<PDWMFN>(GetProcAddress(dwmapi, "DwmExtendFrameIntoClientArea"));
const PDWMSETATTR DwmSetWindowAttribute = reinterpret_cast<PDWMSETATTR>(GetProcAddress(dwmapi, "DwmSetWindowAttribute"));
#endif

#include <QApplication>
#include <QWidget>
#include <QEvent>
#include <QPushButton>
#include <QStyle>

#include "../header/tools/AppLog.h"


System::Platform System::GetPlatform()
{
    Platform platform = Platform::Other;
#if defined(Q_OS_WINDOWS)
    platform = Platform::Windows;
#elif defined(Q_OS_LINUX)
    platform = Platform::Linux;
#elif defined(Q_OS_MACOS)
    platform = Platform::MacOS;
#endif
    return platform;
}

System::Compiler System::GetCompiler()
{
    Compiler compiler = Compiler::OtherCompiler;
#if defined(_MSC_VER)
    compiler = Compiler::MSVC;
#elif defined(__GNUC__) || defined(__GNUG__)
    compiler = Compiler::Gcc_Gxx;
#elif defined(__clang__)
    compiler = Compiler::Clang;
#endif
    return  compiler;
}

QString System::GetOSName()
{
    QString name;
#if defined(Q_OS_WINDOWS)
    name = "Microsoft Windows";
#elif defined(Q_OS_LINUX)
    name = "Linux";
#elif defined(Q_OS_MACOS)
    name = "MacOS";
#elif defined(Q_OS_IOS)
    name = "iOS";
#elif defined(Q_OS_ANDROID)
    name = "Android";
#elif defined(Q_OS_FREEBSD)
    name = "FreeBSD";
#elif defined(Q_OS_NETBSD)
    name = "NetBSD";
#elif defined(Q_OS_OPENBSD)
    name = "OpenBSD";
#elif defined(Q_OS_UNIX)
    name = "Unix";
#else
    static_assert(false, "UnknownOS");
#endif
#if defined(Q_PROCESSOR_X86_64) || defined(_WIN64) || defined(__x86_64__) || defined(__amd64)
    name += " x64";
#elif defined(Q_PROCESSOR_X86_32) || defined(_WIN32) || defined(__i386) || defined(__i386__)
    name += " x86";
#elif defined(Q_PROCESSOR_ARM_64)
    name += " ARM64";
#elif defined(Q_PROCESSOR_ARM_32)
    name += " ARM32";
#else
    static_assert(false, "UnknownArchitecture");
#endif
    return name;
}

QString System::GetCompilerName()
{
    QString name;
#if defined(Q_CC_MSVC)
    name = QString("MSVC %1").arg(Q_CC_MSVC);
#elif defined(Q_CC_MINGW)
    name = "MinGW";
#ifdef _WIN64
    name += QString("-W64 %1").arg(__GNUC__);
#else
    name += QString(" %1").arg(__GNUC__);
#endif
#elif defined(Q_CC_GNU)
    name = QString("GCC %1.%2.%3").arg(
        __GNUC__).arg(__GNUC_MINOR__).arg(__GNUC_PATCHLEVEL__);
#elif defined(Q_CC_CLANG)
    name = QString("Clang %1.%2.%3").arg(
        __clang_major__).arg(__clang_minor__).arg(__clang_patchlevel__);
#elif defined(Q_CC_INTEL)
    name = "Intel C++";
#elif defined(Q_CC_APPLE_CLANG)
    name = QString("Apple Clang %1.%2.%3").arg(
        __clang_major__).arg(__clang_minor__).arg(__clang_patchlevel__);
#else
    static_assert(false, "UnknownCompiler");
#endif
    return name;
}

WindowHelper::WindowHelper(Window *target, QObject *parent) : QObject(parent), m_target(target), m_padding(8)
{
    if (m_target)
    {
        m_target->winId();
    }
}

WindowHelper::~WindowHelper() = default;

void WindowHelper::InitWindowStyle(const int useSpecialBackdrop)
{
    if (!m_target) return;
    if (!mb_Initialized)
    {
        m_target->setWindowFlags( Qt::FramelessWindowHint | Qt::Window);
        mb_Initialized = true;
    }
#if defined(Q_OS_WINDOWS)
    constexpr auto margins = MARGINS{-1, -1, -1, -1};
    const auto hwnd = reinterpret_cast<HWND>(m_target->winId());
    const auto style = ::GetWindowLong(hwnd, GWL_STYLE);
    ::SetWindowLong(hwnd, GWL_STYLE, style | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_CAPTION );
    DwmExtendFrameIntoClientArea(hwnd, &margins);
    if (useSpecialBackdrop)
    {
        DwmSetWindowAttribute(hwnd, 38, &useSpecialBackdrop, sizeof(useSpecialBackdrop));
    }
    m_bgColor = QColor{Qt::transparent};
#endif
}

void WindowHelper::SetWindowDarkMode(const bool dark) const
{

}

bool WindowHelper::NativeEventHandler(const QByteArray &eventType, void *message, qintptr *result)
{
    if (!m_target)
        return false;
#if defined(Q_OS_WINDOWS)
    if (!(eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG") || !m_target->isVisible())
        return false;
    if (const auto msg = static_cast<::MSG *>(message); msg->hwnd == reinterpret_cast<HWND>(m_target->winId()))
        switch (msg->message)
        {
        case WM_NCHITTEST:
        {
            const auto local = m_target->mapFromGlobal(QCursor::pos());
            if (const int hit = this->Border(local))
            {
                *result = hit;
                return true;
            }
            if (this->Caption(local))
            {
                *result = HTCAPTION;
                return true;
            }

        }
        break;
        case WM_NCCALCSIZE:
            *result = 0;
            if (IsZoomed(msg->hwnd)) m_target->set_Margins({7, 7, 7, 7});
            else m_target->set_Margins({0, 0, 0, 0});
            return true;
        default:
            break;
        }
#elif defined(Q_OS_LINUX)
#endif
    return m_target->QWidget::nativeEvent(eventType, message, result);
}

int WindowHelper::Border(const QPoint pos) const
{
    const bool l = pos.x() < m_padding;
    const bool t = pos.y() < m_padding;
    const bool r = pos.x() > m_target->width() - m_padding;
    const bool b = pos.y() > m_target->height() - m_padding;
    return (r && b)   ? HTBOTTOMRIGHT
           : (l && b) ? HTBOTTOMLEFT
           : (r && t) ? HTTOPRIGHT
           : (l && t) ? HTTOPLEFT
           : b        ? HTBOTTOM
           : r        ? HTRIGHT
           : l        ? HTLEFT
           : t        ? HTTOP
                      : 0;
}

bool WindowHelper::Caption(const QPoint pos) const
{
    bool result = false;
    if (m_titlebar)
    {
        if (m_titlebar->geometry().contains(pos))
            result = true;
        for (const auto childObject : m_titlebar->children())
        {
            if (const auto button = qobject_cast<QPushButton *>(childObject); button && button->geometry().contains(pos))
            {
                result = false;
                break;
            }
        }
    }
    return result;
}

int WindowHelper::get_padding() const
{
    return m_padding;
}

QWidget *WindowHelper::get_titlebar() const
{
    return m_titlebar;
}

QColor WindowHelper::bgColor() const
{
    return m_bgColor;
}

void WindowHelper::set_titlebar(QWidget *titlebar)
{
    m_titlebar = titlebar;
}

WindowHelper::PreferedTheme WindowHelper::GetSystemTheme()
{
    const auto windowColor = QApplication::palette().color(QPalette::Window);
    return ((windowColor.red() * 299 + windowColor.green() * 587 + windowColor.blue() * 114) / 1000) < 128
               ? PreferedTheme::Dark
               : PreferedTheme::Light;
}

void WindowHelper::set_padding(const int padding)
{
    if (padding > 0)
        m_padding = padding;
    else
        Log("(WindowHelper)padding set invalid.", AppLogger::LogLevel::Warning);
}
