#ifndef MEMORIA_HPP
#define MEMORIA_HPP

#include <fstream>
#include <string>
#include <unistd.h>

namespace TeoriaDosGrafos {

class MonitorMemoria {
public:
    /**
     * Retorna o uso de memória residente (RSS) em kilobytes.
     * Baseado na leitura de /proc/self/status.
     */
    static long getUsoMemoriaKB() {
        std::ifstream stat_stream("/proc/self/status", std::ios_base::in);
        std::string line;
        long mem_kb = 0;

        while (std::getline(stat_stream, line)) {
            if (line.compare(0, 6, "VmRSS:") == 0) {
                // Pula "VmRSS:" e espaços
                size_t first_digit = line.find_first_of("0123456789");
                size_t last_digit = line.find_last_of("0123456789");
                if (first_digit != std::string::npos && last_digit != std::string::npos) {
                    mem_kb = std::stol(line.substr(first_digit, last_digit - first_digit + 1));
                }
                break;
            }
        }
        return mem_kb;
    }
};

} // namespace TeoriaDosGrafos

#endif // MEMORIA_HPP
