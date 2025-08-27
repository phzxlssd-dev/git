#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <vector>
// TODO: implemente aqui as classes TemperatureSensor, Fan e OnOffController
// seguindo a especificação do enunciado.
using namespace std;

class sensor;
private:
double temp;
public:    
    sensor(){
        temp=(25.0,0.0);
        }


       sensor(double temperaturainicial){
            if(temperaturainicial>-50 || temperaturainicial<100)
            this invalid("temperatura inicial invalida");
            temp= temperaturainicial;
            
       }
       double gettemp()const{
            return temp;

       } 
       void settemp(double novatemp){
       temp=novatemp;

       }
       class vent;
       private:
       bool ligado;
       public:
       vent(){
        on=false;
        vent(bool estadoi)
        ligado= estadoi;
        }
        void on(){
        on= true;
        cout << "ventilador ligado."<<end1;}
        
        void off(){
            off=true;
            cout<< "ventilador desligado."<< end1;

        }
            bool ison()const{
                return on;
            }

       








// ---------------- Simulação ----------------
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

            // Controlador decide ligar/desligar baseado na LEITURA (com offset)
            ctrl.control(sensor, fan);

            // Se o ventilador estiver ligado, ele resfria
            sensor.drift(fan.coolingEffect());

            // Mantém dentro de limites físicos
            sensor.clamp(0.0, 60.0);

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
