#!/usr/bin/env python3

# From Wikiupedia page https://en.wikipedia.org/wiki/Sunrise_equation
# I have renamed som identifiers I do not like

import logging
from datetime import datetime, timedelta, timezone, tzinfo
from math import acos, asin, ceil, cos, degrees, fmod, radians, sin, sqrt
from time import time

log = logging.getLogger()


def ts2human(ts: int | float, debugtz: tzinfo | None) -> str:
    return str(datetime.fromtimestamp(ts, debugtz))


def j2ts(j: float | int) -> float:
    return (j - 2440587.5) * 86400


def ts2j(ts: float | int) -> float:
    return ts / 86400.0 + 2440587.5


def j2human(j: float | int, debugtz: tzinfo | None) -> str:
    ts = j2ts(j)
    return f"{ts} = {ts2human(ts, debugtz)}"


def deg2human(deg: float | int) -> str:
    x = int(deg * 3600.0)
    num = f"∠{deg:.3f}°"
    rad = f"∠{radians(deg):.3f}rad"
    human = f"∠{x // 3600}°{x // 60 % 60}′{x % 60}″"  # N.B. not correct for negative x
    return f"{rad} = {human} = {num}"


def calc(
    current_timestamp: float,
    lat: float,
    lon: float,
    elevation: float = 0.0,
    *,
    debugtz: tzinfo | None = None,
) -> tuple[float, float, None] | tuple[None, None, bool]:
    log.debug(f"Latitude               lat     = {deg2human(lat)}")
    log.debug(f"Longitude              lon     = {deg2human(lon)}")
    log.debug(
        f"Now                    ts      = {ts2human(current_timestamp, debugtz)}"
    )

    J_date = ts2j(current_timestamp)
    log.debug(f"Julian date            j_date  = {J_date:.3f} days")

    # Julian day
    # TODO: ceil ?
    J_day = ceil(J_date - (2451545.0 + 0.0009) + 69.184 / 86400.0)
    log.debug(f"Julian day             j_day   = {J_day:.3f} days")

    # Mean solar time
    mst = J_day + 0.0009 - lon / 360.0
    log.debug(f"Mean solar time        mst     = {mst:.9f} days")

    # Solar mean anomaly
    # M_degrees = 357.5291 + 0.98560028 * mst  # Same, but looks ugly
    M_degrees = fmod(357.5291 + 0.98560028 * mst, 360)
    M_radians = radians(M_degrees)
    log.debug(f"Solar mean anomaly     M_xxx   = {deg2human(M_degrees)}")

    # Equation of the center
    C_degrees = 1.9148 * sin(M_radians) + 0.02 * sin(2 * M_radians) + 0.0003 * sin(3 * M_radians)
    # The difference for final program result is few milliseconds
    # https://www.astrouw.edu.pl/~jskowron/pracownia/praca/sunspot_answerbook_expl/expl-4.html
    # e = 0.01671
    # C_degrees = \
    #     degrees(2 * e - (1 / 4) * e ** 3 + (5 / 96) * e ** 5) * sin(M_radians) \
    #     + degrees(5 / 4 * e ** 2 - (11 / 24) * e ** 4 + (17 / 192) * e ** 6) * sin(2 * M_radians) \
    #     + degrees(13 / 12 * e ** 3 - (43 / 64) * e ** 5) * sin(3 * M_radians) \
    #     + degrees((103 / 96) * e ** 4 - (451 / 480) * e ** 6) * sin(4 * M_radians) \
    #     + degrees((1097 / 960) * e ** 5) * sin(5 * M_radians) \
    #     + degrees((1223 / 960) * e ** 6) * sin(6 * M_radians)

    log.debug(f"Equation of the center C_deg   = {deg2human(C_degrees)}")

    # Ecliptic longitude
    # L_degrees = M_degrees + C_degrees + 180.0 + 102.9372  # Same, but looks ugly
    L_degrees = fmod(M_degrees + C_degrees + 180.0 + 102.9372, 360)
    log.debug(f"Ecliptic longitude     L_deg   = {deg2human(L_degrees)}")

    L_radians = radians(L_degrees)

    # Solar transit (Julian date)
    J_transit = (2451545.0 + mst + 0.0053 * sin(M_radians) - 0.0069 * sin(2 * L_radians))
    log.debug(f"Solar transit time     J_trans = {j2human(J_transit, debugtz)}")
    log.debug(f"Solar transit time     J_trans = {J_transit}")

    # Declination of the Sun
    sin_d = sin(L_radians) * sin(radians(23.4397))
    # cos_d = sqrt(1-sin_d**2) # exactly the same precision, but 1.5 times slower
    cos_d = cos(asin(sin_d))

    # Hour angle
    some_cos = (sin(radians(-0.833 - 2.076 * sqrt(elevation) / 60.0)) - sin(radians(lat)) * sin_d) / (cos(radians(lat)) * cos_d)
    try:
        ha_radians = acos(some_cos)
    except ValueError:
        return None, None, some_cos > 0.0
    ha_degrees = degrees(ha_radians)  # 0...180

    log.debug(f"Hour angle             ha_xxx   = {deg2human(ha_degrees)}")

    j_rise = J_transit - ha_degrees / 360
    j_set = J_transit + ha_degrees / 360

    log.debug(f"Sunrise                j_rise  = {j2human(j_rise, debugtz)}")
    log.debug(f"Sunset                 j_set   = {j2human(j_set, debugtz)}")
    log.debug(f"Day length                       {ha_degrees / (180 / 24):.3f} hours")

    return j2ts(j_rise), j2ts(j_set), None


def main():
    logging.basicConfig(level=logging.DEBUG)
    latitude = 48.269655
    longitude = 14.311495
    elevation = 0
    ts = time();
    #ts = 946681200
    print(calc(ts, latitude, longitude, elevation, debugtz=timezone(timedelta(hours=1), "fake-zone")))


if __name__ == "__main__":
    main()
