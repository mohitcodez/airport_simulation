#include "utils.h"

int calculateTravelTime(int dep_h, int dep_m, int arr_h, int arr_m) {
    int dep = dep_h*60 + dep_m;
    int arr = arr_h*60 + arr_m;
    if (arr < dep) arr += 24*60; // next day
    return arr - dep;
}
