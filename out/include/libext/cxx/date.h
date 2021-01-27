#pragma once

namespace ext {

enum class date_part {
    day,
    week,
    hour,
    minute,
    second,
    millisecond
};

class date {
private:
    double _time;
    int _year;
    int _month;
    int _date;
    int _day;
    int _hour;
    int _minute;
    int _second;
    int _millisecond;
public:
    date()
        : _time(0)
        , _year(1970)
        , _month(1)
        , _date(1)
        , _day(4)
        , _hour(0)
        , _minute(0)
        , _second(0)
        , _millisecond(0) {}
    date(ext::date const& date)
        : _time(date._time)
        , _year(date._year)
        , _month(date._month)
        , _date(date._date)
        , _day(date._day)
        , _hour(date._hour)
        , _minute(date._minute)
        , _second(date._second)
        , _millisecond(date._millisecond) {}
    ext::date& operator=(ext::date const& date) {
        _time = date._time;
        _year = date._year;
        _month = date._month;
        _date = date._date;
        _day = date._day;
        _hour = date._hour;
        _minute = date._minute;
        _second = date._second;
        _millisecond = date._millisecond;
        return *this;
    }
    bool operator==(ext::date const& date) {
        return (_time == date._time);
    }
    bool operator!=(ext::date const& date) {
        return !operator==(date);
    }
    double time() const {
        return _time;
    }
    int year() const {
        return _year;
    }
    int month() const {
        return _month;
    }
    int date() const {
        return _date;
    }
    int day() const {
        return _day;
    }
    int hour() const {
        return _hour;
    }
    int minute() const {
        return _minute;
    }
    int second() const {
        return _second;
    }
    int millisecond() const {
        return _millisecond;
    }
    ext::date add(int num, ext::date_part part) const {
        return ext::date();
    }
    ext::string ISO_datetime() const {
        return ext::string::format("%d-%02d-%02dT%02d:%02d:%02d", _year, _month, _date, _hour, _minute, _second);
    }
    ext::string ISO_date() const {
        return ext::string::format("%d-%02d-%02d", _year, _month, _date);
    }
    static ext::date now() {
        return ext::date();
    }
    static ext::date from_time(double time) {
        return ext::date();
    }
};

} /* namespace ext */