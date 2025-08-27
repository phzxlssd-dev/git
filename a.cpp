#include <iostream>
#include <stdexcept>
using namespace std;

class TemperatureSensor {
private:
    double tempC_;
    double offset_;

public:
    TemperatureSensor(double startC = 25.0, double offset = 0.0)
        : tempC_(startC), offset_(0.0) {
        setOffset(offset);
    }

    double readC() const {
        return tempC_ + offset_;
    }

    void setOffset(double off) {
        if (off < -2.0 || off > 2.0) {
            throw invalid_argument("Offset fora do intervalo [-2, 2]");
        }
        offset_ = off;
    }

    double offset() const {
        return offset_;
    }

    void drift(double delta) {
        tempC_ += delta;
    }

    void clamp(double minC, double maxC) {
        if (tempC_ < minC) tempC_ = minC;
        else if (tempC_ > maxC) tempC_ = maxC;
    }
};

class Fan {
private:
    bool on_;
    double coolingRateCps_;

public:
    Fan(double coolingRateCps = 0.3) : on_(false) {
        if (coolingRateCps <= 0) {
            throw invalid_argument("Cooling rate deve ser maior que 0");
        }
        coolingRateCps_ = coolingRateCps;
    }

    void turnOn() {
        on_ = true;
        cout << "Ventilador ligado." << endl;
    }

    void turnOff() {
        on_ = false;
        cout << "Ventilador desligado." << endl;
    }

    bool isOn() const {
        return on_;
    }

    double coolingEffect() const {
        return on_ ? -coolingRateCps_ : 0.0;
    }
};

int main() {
    try {
        TemperatureSensor sensor;
        Fan fan;

        cout << "Temperatura inicial: " << sensor.readC() << " °C" << endl;
        fan.turnOn();
        cout << "Cooling effect: " << fan.coolingEffect() << " °C/s" << endl;

    } catch (const invalid_argument& e) {
        cerr << "Erro: " << e.what() << endl;
    }

    return 0;
}
