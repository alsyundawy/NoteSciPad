/*
 * This file is part of NoteSciPad.
 * Copyright 2026 Harry DS Alsyundawy (NoteSciPad Maintainer)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QString>

class MainWindow;
class ScintillaNext;
class ApplicationSettings;

class ThemeManager : public QObject
{
    Q_OBJECT

public:
    enum class ThemeMode {
        Auto = 0,
        Light = 1,
        Dark = 2
    };
    Q_ENUM(ThemeMode)

    static ThemeManager *instance();

    void initialize(ApplicationSettings *settings);

    ThemeMode themeMode() const { return m_themeMode; }
    void setThemeMode(ThemeMode mode, MainWindow *window = nullptr);
    void toggleTheme(MainWindow *window = nullptr);

    bool isDarkActive() const;

    void applyTheme(MainWindow *window);
    void applyThemeToEditor(ScintillaNext *editor);
    QString getThemeStyleSheet() const;

signals:
    void themeChanged(bool isDark);

private slots:
    void onSystemColorSchemeChanged();

private:
    explicit ThemeManager(QObject *parent = nullptr);
    ~ThemeManager() override;

    ThemeMode m_themeMode = ThemeMode::Auto;
    ApplicationSettings *m_settings = nullptr;
    MainWindow *m_activeWindow = nullptr;
};

#endif // THEMEMANAGER_H
