#ifndef LOGGER_H
#define LOGGER_H

#include <QString>

/*
 * @class Logger
 * @brief Singleton для логирования событий сервера в файл и консоль.
 */
class Logger {
public:
    /*
     * @brief Получить единственный экземпляр Logger
     * @return Ссылка на Logger
     */
    static Logger& instance();
    /*
     * @brief Записать сообщение в лог
     * @param message Текст сообщения
     */
    void log(const QString& message);

private:
    Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};

#endif // LOGGER_H
