#include <iostream>
#include <stdexcept>
#include <iomanip>


// ---------------- TemperatureSensor ----------------
class TemperatureSensor {
private:
    double tempC_;   // temperatura real
    double offset_;  // desvio de calibração

public:
    // Construtores
    TemperatureSensor(double startC, double offset = 0.0)
        : tempC_(startC), offset_(offset) {}

    TemperatureSensor() : tempC_(25.0), offset_(0.0) {}

    // Retorna a leitura do sensor (real + offset)
    double readC() const {
        return tempC_ + offset_;
    }

    // Define o offset (válido apenas entre -2.0 e +2.0)
    void setOffset(double off) {
        if (off < -2.0 || off > 2.0) {
            throw std::out_of_range("Offset deve estar entre -2.0 e +2.0");
        }
        offset_ = off;
    }

    double offset() const {
        return offset_;
    }

    // verifica a  temperatura
    void drift(double delta) {
        tempC_ += delta;
    }

    // Garante limites físicos
    void clamp(double minC, double maxC) {
        if (tempC_ < minC) tempC_ = minC;
        if (tempC_ > maxC) tempC_ = maxC;
    }
};

// ---------------- Fan ----------------
class Fan {
private:
    bool on_;                // estado atual
    double coolingRateCps_;  // taxa de resfriamento

public:
    // Construtor com taxa customizada
    Fan(double coolingRateCps) : on_(false) {
        if (coolingRateCps <= 0.0) {
            throw std::invalid_argument("coolingRateCps deve ser > 0");
        }
        coolingRateCps_ = coolingRateCps;
    }

    // Construtor default
    Fan() : on_(false), coolingRateCps_(0.3) {}

    void turnOn() { on_ = true; }
    void turnOff() { on_ = false; }
    bool isOn() const { return on_; }

    // Retorna o efeito de resfriamento
    double coolingEffect() const {
        return on_ ? -coolingRateCps_ : 0.0;
    }
};

// ---------------- OnOffController ----------------
class OnOffController {
private:
    double minC_;  // desliga abaixo
    double maxC_;  // liga acima

public:
    OnOffController(double minC, double maxC) : minC_(minC), maxC_(maxC) {
        if (minC >= maxC) {
            throw std::invalid_argument("minC deve ser menor que maxC");
        }
    }

    OnOffController() : minC_(24.0), maxC_(26.0) {}

    void control(const TemperatureSensor& sensor, Fan& fan) const {
        double t = sensor.readC();
        if (t > maxC_) {
            fan.turnOn();
        } else if (t < minC_) {
            fan.turnOff();
        }
        // Dentro da faixa, mantém o estado
    }
};

// ---------------- MAIN ----------------
int main() {
    try {
        // Configuração inicial da simulação
        TemperatureSensor sensor(27.0, 0.0); // começa levemente quente
        Fan fan(0.4);                        // resfriamento de 0.4 °C/s quando ON
        OnOffController ctrl(24.0, 26.0);    // histerese: desliga <24, liga >26

        std::cout << std::fixed << std::setprecision(2);

        // Simula 30 "segundos"
        for (int t = 0; t < 30; ++t) {
            // Tendência natural do gabinete: aquece +0.10 °C/s
            sensor.drift(+0.10);

            // Controlador decide ligar/desligar
            ctrl.control(sensor, fan);

            // Se o ventilador estiver ligado, aplica resfriamento
            sensor.drift(fan.coolingEffect());

            // Mantém dentro de limites físicos
            sensor.clamp(0.0, 60.0);

            // Saída
            std::cout << "t=" << t << "s"
                      << " | T=" << sensor.readC() << " °C"
                      << " | Fan=" << (fan.isOn() ? "ON" : "OFF")
                      << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << "\n";
        return 1;
    }
    return 0;
}


