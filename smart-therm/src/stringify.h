String stringify(float number) {
    int rounded = round(number * 10);
    return String(rounded / 10) + "," + (rounded % 10);
}