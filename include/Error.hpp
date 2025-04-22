/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Error
*/

#pragma once

#include <exception>
#include <sstream>
#include <string>

/**
 * @brief generic Error class
 * @details throw exception::Error("raytracer::Core", "failed to open ", <path>, " because: ", dlerror());
 * @return void
 */
namespace raytracer::exception {
class Error : public std::exception
{
    public:
        /**
         * @brief Error constructor template
         * @details takes (what) (where) and (...) it works like va_list
         * @return Error
         */
        template<typename... Args>
        constexpr explicit Error(const std::string &where, Args &&...args)
            : _where(where), _what(concatStrings(std::forward<Args>(args)...))
        {
            /* empty */
        }

        const char *what() const noexcept override;
        const char *where() const noexcept;

    private:
        const std::string _where;
        const std::string _what;

        /**
         * @brief Error::concatStrings
         * @details concat strings in va_list ...
         * @return std::string
         */
        template<typename... Args>
        static std::string concatStrings(Args &&...args)
        {
            std::ostringstream oss;

            (oss << ... << args);
            return oss.str();
        }
};
}// namespace raytracer::exception
