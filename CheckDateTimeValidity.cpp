#define CHECK_LOW_RANGE(value, range, msg)  \
    if (value < range) {                    \
        throw domain_error(msg);            \
    }

#define CHECK_HIGH_RANGE(value, range, msg) \
    if (value > range) {                    \
        throw domain_error(msg);            \
    }

#define CHECK_RANGE(obj, field, min, max)                       \
    CHECK_LOW_RANGE(obj.field, min, #field " is too small"s)    \
    CHECK_HIGH_RANGE(obj.field, max, #field " is too big"s)

void CheckDateTimeValidity(const DateTime& dt) {
    CHECK_RANGE(dt, year, 1, 9999)
    CHECK_RANGE(dt, month, 1, 12)

    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const array month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    CHECK_RANGE(dt, day, 1, month_lengths[dt.month -1])

    CHECK_RANGE(dt, hour, 1, 23)
    CHECK_RANGE(dt, minute, 1, 59)
    CHECK_RANGE(dt, second, 1, 59)
}
