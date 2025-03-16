#ifndef MULTIPLATFORM_HPP
#define MULTIPLATFORM_HPP

#include <QWidget>
class Window;

#if defined (Q_OS_WINDOWS)
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <uxtheme.h>
#ifdef _MSC_VER
#include <dwmapi.h>
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "dwmapi.lib")
#else
typedef HRESULT (WINAPI *PDWMFN)(HWND, const MARGINS*);
typedef HRESULT (WINAPI *PDWMSETATTR)(HWND, DWORD, LPCVOID, DWORD);
const extern PDWMFN DwmExtendFrameIntoClientArea;
const extern PDWMSETATTR DwmSetWindowAttribute;
#endif
#else
#define HTLEFT 10
#define HTRIGHT 11
#define HTTOP 12
#define HTTOPLEFT 13
#define HTTOPRIGHT 14
#define HTBOTTOM 15
#define HTBOTTOMLEFT 16
#define HTBOTTOMRIGHT 17
#if defined(Q_OS_LINUX)
#include <xcb/xcb.h>
#elif defined(Q_OS_DARWIN)

#endif
#endif

namespace System
{
    enum Platform
    {
        Windows,
        Linux,
        MacOS,
        Other
    };

    enum Compiler
    {
        MSVC,
        Gcc_Gxx,
        Clang,
        OtherCompiler
    };

    Platform GetPlatform();
    Compiler GetCompiler();

    QString GetOSName();
    QString GetCompilerName();

};

namespace Reflex
{
    template <typename E>
    concept EnumTypename = std::is_enum_v<E>;

    template <typename T>
    constexpr QString TemplateName()
    {
#if defined(__GNUC__) || defined(__clang__)
        const QString prettyFunction = __PRETTY_FUNCTION__;
        const auto start = prettyFunction.indexOf("T = ") + 4;
        const auto end = prettyFunction.lastIndexOf(']');
        if (start > 4 && end > start)
            { return prettyFunction.mid(start, end - start);}
#elif defined(_MSC_VER)
        const QString funcSig = __FUNCSIG__;
        const auto begin = funcSig.indexOf('<') + 1;
        const auto end = funcSig.lastIndexOf('>');
        if (begin > 1 && end > begin) 
            { return funcSig.mid(begin, end - begin); }
#else
        static_assert(false, "UnsupportedCompiler");
#endif
        return {};
    }
}

class NativeWindowHelper final : public QObject
{
    Q_OBJECT
public:
    enum class Theme {
        Dark,
        Light
    };
    explicit NativeWindowHelper(Window *target, QObject* parent = nullptr);
    ~NativeWindowHelper() override;

    void InitWindowStyle(int useSpecialBackdrop = 0);
    void SetWindowDarkMode(bool dark) const;
    bool EventHandler_NativeEvent(const QByteArray &eventType, void *message, qintptr *result);

    [[nodiscard]]
    int Border(QPoint pos) const;
    [[nodiscard]]
    bool Caption(QPoint pos) const;

    void set_padding(const int padding)
    { if (padding > 0) m_padding = padding; }
    [[nodiscard]] int get_padding() const
    { return m_padding; }

    void set_titlebar(QWidget* titlebar)
    { m_titlebar = titlebar; }
    [[nodiscard]] QWidget*  get_titlebar() const
    { return m_titlebar; }

    [[nodiscard]] QColor get_bgColor() const
    { return m_bgColor; }

    static Theme GetSystemTheme();

private:
    Window* m_target = nullptr;
    QWidget* m_titlebar = nullptr;
    QColor m_bgColor;
    int m_padding = 0;
    bool mb_Initialized = false;
#if defined(Q_OS_LINUX)

    #endif
};

#endif // MULTIPLATFORM_HPP