#include  "whisperm.h"

int main(const int argc, char** argv)
{
    const whisperm app(argc, argv);
    return app.run();
}

whisperm::whisperm(int argc, char** argv):app(argc, argv)
{
    QApplication::setApplicationName("Whisperm");

    this->client = new Client;
}

whisperm::~whisperm()
{
    delete client;
}

int whisperm::run() const
{
    try
    {
        LoginDialog loginDialog{this->client};
        if ( loginDialog.exec())
        {
            MainWindow window(this->client);
            window.show();
            window.UpdateContentSize();
            return QApplication::exec();
        }
    }
    catch (...)
    {
        const auto& e = std::current_exception();
    }

    QApplication::closeAllWindows();
    QApplication::quit();
    return 0;
}