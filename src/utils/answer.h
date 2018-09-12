#ifndef ANSWER_H
#define ANSWER_H

#include <string>
#include <vector>
#include <cmath>

struct Answer
{

private:
    bool checkVector(const std::vector<double> &a) {
        for (const auto d : a) {
            float f = static_cast<float>(d);
            if (std::isnan(f)) {
                error_msg = "faced with NaN, change configs";
                return false;
            }
            if (std::isinf(f)) {
                error_msg = "faced with INF, change configs";
                return false;
            }
        }
        return true;
    }

    void trimNaN(std::vector<double> &a) {
        for (size_t i = 0; i != a.size(); ++i) {
            float f = static_cast<float>(a[i]);
            if (std::isnan(f) || std::isinf(f)) {
                a.resize(i);
                break;
            }
        }
    }
public:
    std::vector<double> t;
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
    std::string error_msg;
    Answer(const std::vector<double> &t, const std::vector<double> &x, const std::vector<double> &y,
           const std::vector<double> &z)
        : t(t), x(x), y(y), z(z)
    {
    }
    Answer(std::vector<double> &&t, std::vector<double> &&x, std::vector<double> &&y,
           std::vector<double> &&z)
        : t(std::move(t)), x(std::move(x)), y(std::move(y)), z(std::move(z))
    {
    }

    Answer(const std::string &error_msg) : error_msg(error_msg) {}
    Answer(std::string &&error_msg) : error_msg(std::move(error_msg)) {}

    bool solvedCorrect()
    {
        checkVector(t);
        checkVector(x);
        checkVector(y);
        checkVector(z);
        return static_cast<const Answer *>(this)->solvedCorrect();
    }

    bool solvedCorrect() const
    {
        return (!static_cast<bool>(error_msg.size()));
    }

    void trimNansAndInfs() {
        trimNaN(x);
        trimNaN(y);
        trimNaN(z);
    }

};

#endif // ANSWER_H
