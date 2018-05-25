#ifndef PRIVILEGES_H
#define PRIVILEGES_H

class Privileges {
public:
    Privileges() {}
    void SetRoot() {}
    void SetUser() {}
    void SetRealUser() {}
    void Whoami() {}
    void SaveUserEnv(pid_t) {}
    QString GetUserEnvDbusSession()
    {
        return QString::null;
    }
    QString GetUserEnvLogname()
    {
        return QString::null;
    }
};

#endif // PRIVILEGES_H
